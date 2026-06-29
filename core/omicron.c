#define _GNU_SOURCE
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "omicron.h"

#define OMI_FS 0x1cu
#define OMI_GS 0x1du
#define OMI_RS 0x1eu
#define OMI_US 0x1fu

#define STATE_CANONICAL 0x01u
#define STATE_ALIST     0x02u
#define RING_SIZE 5040u
#define RING_FILE "/tmp/omi_receipt_ring.bin"
#define MAX_RECEIPT 4096
#define MEMORY_SIZE 65536u
#define STACK_SIZE 512u
#define OMI_FRAME 0x30000020u

typedef enum { NODE_ATOM, NODE_STRING, NODE_LIST, NODE_PAIR } NodeKind;

typedef struct Node Node;
struct Node {
    NodeKind kind;
    char *text;
    Node **items;
    size_t count, cap;
    Node *car, *cdr;
};

typedef struct { char *data; size_t len, cap; } Buffer;

typedef struct { unsigned char lane; char *value; } Event;
typedef struct { Event *items; size_t len, cap; } EventList;

typedef struct {
    const char *src;
    size_t len, pos;
    int line, col;
    char error[256];
} Parser;

typedef struct {
    Node *root;
    char *normalized;
    char shape[16];
    unsigned char state;
    EventList events;
    Buffer tape;
    uint64_t source_hash, normalized_hash, tape_hash;
} Compilation;

typedef struct {
    uint16_t s0, s1, s2, s3;
    uint16_t s4, s5, s6, s7;
    uint32_t payload, mask, car, cdr;
} OmiInst;

typedef struct {
    uint32_t pc, car_reg, cdr_reg, payload, mask, flags;
    int halted;
    uint64_t epoch;
    uint32_t memory[MEMORY_SIZE];
    uint32_t stack[STACK_SIZE];
    int stack_ptr;
} CpuState;

typedef struct {
    uint64_t cycle;
    char receipt[MAX_RECEIPT];
    uint64_t hash;
    uint16_t result;
} RingSlot;

typedef enum {
    METATRON_SURFACE_CONS = 0,
    METATRON_SURFACE_OMI_LISP,
    METATRON_SURFACE_GEOMETRY,
    METATRON_SURFACE_BARCODE,
    METATRON_SURFACE_DOM,
    METATRON_SURFACE_GPIO,
    METATRON_SURFACE_SYMBOLIC,
    METATRON_SURFACE_PROJECTIVE,
    METATRON_SURFACE_UNKNOWN
} MetatronSurfaceKind;

typedef struct {
    MetatronSurfaceKind surface;
    uint8_t accepted, scribable, reserved0, reserved1;
    uint64_t cycle, hash;
    uint32_t slot5040;
    uint32_t frame_type, fiber_q, fiber_phase, fano7, role3, local240;
    char notation[512];
} MetatronScribeRecord;

MetatronSurfaceKind metatron_surface_parse(const char *name);
int metatron_scribe_receipt(const RingSlot *slot, MetatronSurfaceKind kind, MetatronScribeRecord *out);

static RingSlot ring[RING_SIZE];
static uint64_t g_cycle = 0;
static volatile int g_running = 1;

static int omicron_dialect_valid(OmicronDialect d) {
    unsigned v = (unsigned)d;
    return v >= 0xf0u && v <= 0xffu;
}

void omicron_config_init(OmicronConfig *cfg) {
    if(!cfg)return;
    memset(cfg,0,sizeof(*cfg));
    cfg->mode=OMICRON_MODE_CLI;
    cfg->command=OMICRON_COMMAND_DEFAULT;
    cfg->dialect=OMICRON_DIALECT_OMICRON;
}

const char *omicron_mode_name(OmicronMode m) {
    switch(m){
        case OMICRON_MODE_CLI:return "cli";
        case OMICRON_MODE_SOFTWARE_BOOT:return "software-boot";
        case OMICRON_MODE_HARDWARE_BOOT:return "hardware-boot";
        case OMICRON_MODE_EMBEDDED:return "embedded";
        case OMICRON_MODE_MODULE:return "module";
        case OMICRON_MODE_BITBOARD:return "bitboard";
        default:return "unknown";
    }
}

const char *omicron_command_name(OmicronCommand c) {
    switch(c){
        case OMICRON_COMMAND_DEFAULT:return "default";
        case OMICRON_COMMAND_EVAL:return "eval";
        case OMICRON_COMMAND_REPL:return "repl";
        case OMICRON_COMMAND_RING:return "ring";
        case OMICRON_COMMAND_BOOT:return "boot";
        case OMICRON_COMMAND_AUTO:return "auto";
        case OMICRON_COMMAND_RUN:return "run";
        case OMICRON_COMMAND_CPU:return "cpu";
        case OMICRON_COMMAND_GEOM:return "geom";
        case OMICRON_COMMAND_RENDER_FRAME:return "render-frame";
        case OMICRON_COMMAND_RENDER_PPM:return "render-ppm";
        case OMICRON_COMMAND_SCRIBE:return "scribe";
        case OMICRON_COMMAND_RENDER_OBJ:return "render-obj";
        case OMICRON_COMMAND_RENDER_GLTF:return "render-gltf";
        case OMICRON_COMMAND_SMITH:return "smith";
        case OMICRON_COMMAND_TWIN:return "twin";
        case OMICRON_COMMAND_SEED:return "seed";
        case OMICRON_COMMAND_SERVE:return "serve";
        case OMICRON_COMMAND_CHECK:return "check";
        case OMICRON_COMMAND_HELP:return "help";
        case OMICRON_COMMAND_WATCH:return "watch";
        case OMICRON_COMMAND_EXPR:return "expr";
        default:return "unknown";
    }
}

const char *omicron_dialect_name(OmicronDialect d) {
    if(d==OMICRON_DIALECT_OMICRON)return "omicron";
    if(d==OMICRON_DIALECT_CANONICAL)return "canonical";
    return omicron_dialect_valid(d) ? "reserved" : "invalid";
}

int omicron_stage_preheader(OmicronDialect d, uint8_t out[OMICRON_PREHEADER_LEN]) {
    if(!out||!omicron_dialect_valid(d))return 0;
    out[0]=(uint8_t)d; out[1]=0x00u; out[2]=OMI_FS; out[3]=OMI_GS;
    out[4]=OMI_RS; out[5]=OMI_US; out[6]=0x20u; out[7]=(uint8_t)d;
    return 1;
}

static int omicron_object_pair_ok(const void *p, size_t n) {
    return (p&&n>0)||(!p&&n==0);
}

int omicron_load_system_objects(const OmicronConfig *cfg) {
    if(!cfg)return 0;
    if(!omicron_object_pair_ok(cfg->omi_object,cfg->omi_object_len))return 0;
    if(!omicron_object_pair_ok(cfg->imo_object,cfg->imo_object_len))return 0;
    if(!omicron_object_pair_ok(cfg->tetragrammatron_object,cfg->tetragrammatron_object_len))return 0;
    if(!omicron_object_pair_ok(cfg->metatron_object,cfg->metatron_object_len))return 0;
    return 1;
}

int omicron_induce_omi_lisp(OmicronConfig *cfg) {
    if(!cfg)return 0;
    if(!omicron_dialect_valid(cfg->dialect))return 0;
    if((cfg->flags&OMICRON_FLAG_PREHEADER_STAGED)==0)return 0;
    if(cfg->preheader[0]!=(uint8_t)cfg->dialect)return 0;
    if(cfg->preheader[1]!=0x00u)return 0;
    if(cfg->preheader[2]!=OMI_FS||cfg->preheader[3]!=OMI_GS)return 0;
    if(cfg->preheader[4]!=OMI_RS||cfg->preheader[5]!=OMI_US)return 0;
    if(cfg->preheader[6]!=OMICRON_READABLE_BOUNDARY)return 0;
    if(cfg->preheader[7]!=(uint8_t)cfg->dialect)return 0;
    cfg->readable_boundary=OMICRON_READABLE_BOUNDARY;
    cfg->dot_operator=OMICRON_DOT_OPERATOR;
    return 1;
}

int omicron_config_from_cli(OmicronConfig *cfg, int argc, char **argv) {
    if(!cfg)return 0;
    cfg->argc=argc;
    cfg->argv=argv;
    cfg->command_arg=(argc>2&&argv)?argv[2]:NULL;
    cfg->command_arg2=(argc>3&&argv)?argv[3]:NULL;
    cfg->command=OMICRON_COMMAND_DEFAULT;
    cfg->mode=OMICRON_MODE_CLI;
    if(argc>1&&argv&&argv[1]){
        const char *a=argv[1];
        if(strcmp(a,"--eval")==0&&argc>2)cfg->command=OMICRON_COMMAND_EVAL;
        else if(strcmp(a,"--repl")==0)cfg->command=OMICRON_COMMAND_REPL;
        else if(strcmp(a,"--ring")==0)cfg->command=OMICRON_COMMAND_RING;
        else if(strcmp(a,"--boot")==0){cfg->command=OMICRON_COMMAND_BOOT;cfg->mode=OMICRON_MODE_SOFTWARE_BOOT;}
        else if(strcmp(a,"--auto")==0)cfg->command=OMICRON_COMMAND_AUTO;
        else if(strcmp(a,"--run")==0&&argc>2)cfg->command=OMICRON_COMMAND_RUN;
        else if(strcmp(a,"--cpu")==0&&argc>2)cfg->command=OMICRON_COMMAND_CPU;
        else if(strcmp(a,"--geom")==0)cfg->command=OMICRON_COMMAND_GEOM;
        else if(strcmp(a,"--render-frame")==0)cfg->command=OMICRON_COMMAND_RENDER_FRAME;
        else if(strcmp(a,"--render-ppm")==0)cfg->command=OMICRON_COMMAND_RENDER_PPM;
        else if(strcmp(a,"--scribe")==0&&argc>2)cfg->command=OMICRON_COMMAND_SCRIBE;
        else if(strcmp(a,"--render-obj")==0)cfg->command=OMICRON_COMMAND_RENDER_OBJ;
        else if(strcmp(a,"--render-gltf")==0)cfg->command=OMICRON_COMMAND_RENDER_GLTF;
        else if(strcmp(a,"--smith")==0)cfg->command=OMICRON_COMMAND_SMITH;
        else if(strcmp(a,"--twin")==0)cfg->command=OMICRON_COMMAND_TWIN;
        else if(strcmp(a,"--seed")==0&&argc>2)cfg->command=OMICRON_COMMAND_SEED;
        else if(strcmp(a,"--serve")==0)cfg->command=OMICRON_COMMAND_SERVE;
        else if(strcmp(a,"--check")==0)cfg->command=OMICRON_COMMAND_CHECK;
        else if(strcmp(a,"--help")==0||strcmp(a,"-h")==0)cfg->command=OMICRON_COMMAND_HELP;
        else if(strcmp(a,"--watch")==0)cfg->command=OMICRON_COMMAND_WATCH;
        else if(strcmp(a,"--hardware-boot")==0)cfg->mode=OMICRON_MODE_HARDWARE_BOOT;
        else if(strcmp(a,"--embedded")==0)cfg->mode=OMICRON_MODE_EMBEDDED;
        else if(strcmp(a,"--module")==0)cfg->mode=OMICRON_MODE_MODULE;
        else if(strcmp(a,"--bitboard")==0)cfg->mode=OMICRON_MODE_BITBOARD;
        else {cfg->command=OMICRON_COMMAND_EXPR;cfg->command_arg=a;}
    }
    cfg->dialect=OMICRON_DIALECT_OMICRON;
    return 1;
}

int omicron_boot(OmicronConfig *cfg) {
    if(!cfg)return 1;
    cfg->flags&=~(OMICRON_FLAG_PREHEADER_STAGED|OMICRON_FLAG_PRELANGUAGE_INDUCED|OMICRON_FLAG_OBJECTS_BOUND);
    if(!omicron_stage_preheader(cfg->dialect,cfg->preheader))return 2;
    cfg->flags|=OMICRON_FLAG_PREHEADER_STAGED;
    if(!omicron_induce_omi_lisp(cfg))return 3;
    cfg->flags|=OMICRON_FLAG_PRELANGUAGE_INDUCED;
    if(!omicron_load_system_objects(cfg))return 4;
    cfg->flags|=OMICRON_FLAG_OBJECTS_BOUND;
    return 0;
}

static int omicron_hex_digit(int c) {
    if(c>='0'&&c<='9')return c-'0';
    if(c>='a'&&c<='f')return c-'a'+10;
    if(c>='A'&&c<='F')return c-'A'+10;
    return -1;
}

static int omicron_parse_u16_hex_part(const char *s, size_t n, uint16_t *out) {
    uint32_t v=0;
    if(!s||!out||n==0||n>4)return 0;
    for(size_t i=0;i<n;i++){
        int d=omicron_hex_digit((unsigned char)s[i]);
        if(d<0)return 0;
        v=(v<<4)|(uint32_t)d;
    }
    *out=(uint16_t)v;
    return 1;
}

static int omicron_parse_u32_hex_part(const char *s, size_t n, uint32_t *out) {
    uint32_t v=0;
    if(!s||!out||n==0||n>8)return 0;
    for(size_t i=0;i<n;i++){
        int d=omicron_hex_digit((unsigned char)s[i]);
        if(d<0)return 0;
        v=(v<<4)|(uint32_t)d;
    }
    *out=v;
    return 1;
}

static int omicron_parse_dec_part(const char *s, size_t n, uint16_t max, uint16_t *out) {
    uint32_t v=0;
    if(!s||!out||n==0)return 0;
    for(size_t i=0;i<n;i++){
        if(!isdigit((unsigned char)s[i]))return 0;
        v=(v*10u)+(uint32_t)(s[i]-'0');
        if(v>max)return 0;
    }
    *out=(uint16_t)v;
    return 1;
}

static int omicron_parse_hex_frame_dash(const char *s, size_t n, uint16_t out[8]) {
    size_t start=0, idx=0;
    if(!s||!out||n==0)return 0;
    for(size_t i=0;i<=n;i++){
        if(i==n||s[i]=='-'){
            if(idx>=8||!omicron_parse_u16_hex_part(s+start,i-start,&out[idx]))return 0;
            idx++;
            start=i+1;
        }
    }
    return idx==8;
}

static int omicron_parse_hex_frame_colon(const char *s, size_t n, uint16_t out[8]) {
    size_t start=0, idx=0;
    if(!s||!out||n==0)return 0;
    for(size_t i=0;i<=n;i++){
        if(i==n||s[i]==':'){
            if(idx>=8||!omicron_parse_u16_hex_part(s+start,i-start,&out[idx]))return 0;
            idx++;
            start=i+1;
        }
    }
    return idx==8;
}

static int omicron_parse_ipv4(const char *s, size_t n, uint16_t out[8]) {
    size_t start=0, idx=0;
    unsigned v[4]={0,0,0,0};
    if(!s||!out||n==0)return 0;
    for(size_t i=0;i<=n;i++){
        if(i==n||s[i]=='.'){
            uint16_t p=0;
            if(idx>=4||!omicron_parse_dec_part(s+start,i-start,255u,&p))return 0;
            v[idx++]=p;
            start=i+1;
        }
    }
    if(idx!=4)return 0;
    memset(out,0,8*sizeof(out[0]));
    out[6]=(uint16_t)((v[0]<<8)|v[1]);
    out[7]=(uint16_t)((v[2]<<8)|v[3]);
    return 1;
}

static int omicron_base36_value(const char *s, size_t n, uint64_t *out) {
    uint64_t v=0;
    if(!s||!out||n==0)return 0;
    for(size_t i=0;i<n;i++){
        unsigned d;
        if(s[i]>='0'&&s[i]<='9')d=(unsigned)(s[i]-'0');
        else if(s[i]>='A'&&s[i]<='Z')d=(unsigned)(s[i]-'A'+10);
        else return 0;
        if(v>(UINT64_MAX-d)/36u)return 0;
        v=(v*36u)+d;
    }
    *out=v;
    return 1;
}

static int omicron_b64url_digit(int c) {
    if(c>='A'&&c<='Z')return c-'A';
    if(c>='a'&&c<='z')return c-'a'+26;
    if(c>='0'&&c<='9')return c-'0'+52;
    if(c=='-')return 62;
    if(c=='_')return 63;
    return -1;
}

static int omicron_base64url_decode(const char *s, size_t n, uint8_t *out, size_t cap, size_t *out_len) {
    uint32_t buf=0;
    int bits=0;
    size_t len=0;
    if(!s||!out||!out_len||n==0||n%4u==1u)return 0;
    for(size_t i=0;i<n;i++){
        if(s[i]=='='){
            for(size_t j=i;j<n;j++)if(s[j]!='=')return 0;
            break;
        }
        int d=omicron_b64url_digit((unsigned char)s[i]);
        if(d<0)return 0;
        buf=(buf<<6)|(uint32_t)d;
        bits+=6;
        while(bits>=8){
            bits-=8;
            if(len>=cap)return 0;
            out[len++]=(uint8_t)((buf>>bits)&0xffu);
        }
    }
    *out_len=len;
    return 1;
}

static int omicron_copy_part(char dst[OMICRON_ADDRESS_SEGMENT_MAX], const char *s, size_t n) {
    if(!dst||!s||n==0||n>=OMICRON_ADDRESS_SEGMENT_MAX)return 0;
    memcpy(dst,s,n);
    dst[n]=0;
    return 1;
}

int omicron_parse_address_candidate(const char *src, OmicronAddressCandidate *out) {
    char buf[OMICRON_ADDRESS_SOURCE_MAX];
    char *body=buf, *q, *at, *slash;
    size_t n;
    int family=0;
    if(!src||!out)return 0;
    n=strlen(src);
    if(n==0||n>=OMICRON_ADDRESS_SOURCE_MAX)return 0;
    memset(out,0,sizeof(*out));
    memcpy(out->source,src,n+1);
    memcpy(buf,src,n+1);
    at=strchr(buf,'@');
    if(at){
        char *at2=strchr(at+1,'@');
        if(!at2||strchr(at2+1,'@'))return 0;
        if(!omicron_base36_value(at+1,(size_t)(at2-at-1),&out->car36_value))return 0;
        if(!omicron_base64url_decode(at2+1,strlen(at2+1),out->cdr64_bytes,OMICRON_ADDRESS_CDR64_MAX,&out->cdr64_len))return 0;
        out->has_cons_closure=1;
        *at=0;
    }
    q=strchr(buf,'?');
    if(q){
        char *q2=strchr(q+1,'?');
        char *q3=q2?strchr(q2+1,'?'):NULL;
        if(!q2||q3)return 0;
        if(!omicron_parse_u32_hex_part(q+1,(size_t)(q2-q-1),&out->payload))return 0;
        if(!omicron_parse_u32_hex_part(q2+1,strlen(q2+1),&out->mask))return 0;
        out->has_payload_mask=1;
        *q=0;
    }
    if(strcmp(buf,"omi---imo")==0){
        out->has_open=1;
        out->has_close=1;
        out->lowered_candidate=1;
        return 1;
    }
    if(strncmp(body,"ip4:",4)==0){family=4;body+=4;}
    else if(strncmp(body,"ip6::",5)==0){family=6;body+=5;}
    else if(strncmp(body,"ip6:",4)==0){family=6;body+=4;}
    else if(strncmp(body,"omi-",4)==0){family=1;out->has_open=1;body+=4;}
    else if(strncmp(body,"imo-",4)==0){family=1;out->has_close=1;body+=4;}
    else return 0;
    if(family==1){
        size_t blen=strlen(body);
        if(blen>4&&strcmp(body+blen-4,"-imo")==0){
            out->has_close=1;
            body[blen-4]=0;
        }
    }
    slash=strchr(body,'/');
    if(slash){
        char *p=slash+1;
        *slash=0;
        while(*p){
            char *next=strchr(p,'/');
            size_t pn=next?(size_t)(next-p):strlen(p);
            if(pn==0||out->path_count>=OMICRON_ADDRESS_PATH_MAX)return 0;
            if(!omicron_copy_part(out->path[out->path_count],p,pn))return 0;
            if(out->path_count==0){
                uint16_t max=(family==4)?32u:128u;
                uint16_t pref=0;
                if(omicron_parse_dec_part(p,pn,max,&pref)){
                    out->prefix=pref;
                    out->has_prefix=1;
                }
            }
            out->path_count++;
            if(!next)break;
            p=next+1;
        }
    }
    if(family==4){
        if(!omicron_parse_ipv4(body,strlen(body),out->frame))return 0;
    }else if(family==6){
        if(strchr(body,'-')){
            if(!omicron_parse_hex_frame_dash(body,strlen(body),out->frame))return 0;
        }else{
            if(!omicron_parse_hex_frame_colon(body,strlen(body),out->frame))return 0;
        }
    }else{
        if(!omicron_parse_hex_frame_dash(body,strlen(body),out->frame))return 0;
    }
    out->has_frame=1;
    memcpy(out->lowered_frame,out->frame,sizeof(out->frame));
    out->lowered_candidate=1;
    return 1;
}

static const char *omilog_skip_horizontal(const char *p) {
    while(p&&(*p==' '||*p=='\t'||*p=='\r'))p++;
    return p;
}

static const char *omilog_skip_space(const char *p) {
    while(p&&isspace((unsigned char)*p))p++;
    return p;
}

static int omilog_copy_token(char *dst, size_t cap, const char *s, size_t n) {
    if(!dst||!s||n==0||n>=cap)return 0;
    memcpy(dst,s,n);
    dst[n]=0;
    return 1;
}

static int omilog_keyword_ok(const char *s, size_t n) {
    if(!s||n==0||n>=OMICRON_OMILOG_KEYWORD_MAX)return 0;
    for(size_t i=0;i<n;i++)if(!(s[i]>='A'&&s[i]<='Z'))return 0;
    return 1;
}

static int omilog_keyword_authority_bearing(const char *s, size_t n) {
    static const char *bad[]={"VALID","ACCEPT","ACCEPTED","RECEIPT","PROOF","PROJECTION"};
    if(!s)return 1;
    for(size_t i=0;i<sizeof(bad)/sizeof(bad[0]);i++)if(strlen(bad[i])==n&&memcmp(s,bad[i],n)==0)return 1;
    return 0;
}

static int omilog_has_oexpr(const char *s, size_t n) {
    const char *open=NULL;
    if(!s||n==0)return 0;
    for(size_t i=0;i<n;i++){
        if(s[i]=='('&&!open)open=s+i;
        if(s[i]==')'&&open&&s+i>open)return 1;
    }
    return 0;
}

static int omilog_token_eq_lower(const char *s, size_t n, const char *word) {
    size_t wn=strlen(word);
    if(!s||!word||n!=wn)return 0;
    for(size_t i=0;i<n;i++)if((char)tolower((unsigned char)s[i])!=word[i])return 0;
    return 1;
}

static int omilog_block_has_authority_term(const char *s, size_t n) {
    size_t i=0;
    if(!s)return 1;
    while(i<n){
        while(i<n&&!isalpha((unsigned char)s[i]))i++;
        size_t start=i;
        while(i<n&&isalpha((unsigned char)s[i]))i++;
        if(i>start){
            size_t len=i-start;
            if(omilog_token_eq_lower(s+start,len,"valid"))return 1;
            if(omilog_token_eq_lower(s+start,len,"accept"))return 1;
            if(omilog_token_eq_lower(s+start,len,"accepted"))return 1;
            if(omilog_token_eq_lower(s+start,len,"receipt"))return 1;
            if(omilog_token_eq_lower(s+start,len,"proof"))return 1;
            if(omilog_token_eq_lower(s+start,len,"projection"))return 1;
        }
    }
    return 0;
}

int omilog_parse_candidate(const char *src, OmiLogCandidate *out) {
    const char *p, *head, *key, *assign, *line_end, *block, *close;
    char address[OMICRON_ADDRESS_SOURCE_MAX];
    size_t head_len, key_len, assign_len;
    if(!src||!out)return 0;
    memset(out,0,sizeof(*out));
    p=omilog_skip_space(src);
    if(!p||!*p)return 0;
    head=p;
    while(*p&&*p!=' '&&*p!='\t'&&*p!='\r'&&*p!='\n')p++;
    head_len=(size_t)(p-head);
    if(head_len==0||head_len>=sizeof(address))return 0;
    memcpy(address,head,head_len);
    address[head_len]=0;
    if(!omicron_parse_address_candidate(address,&out->address))return 0;
    p=omilog_skip_horizontal(p);
    key=p;
    while(*p&&*p!=' '&&*p!='\t'&&*p!='\r'&&*p!='\n')p++;
    key_len=(size_t)(p-key);
    if(!omilog_keyword_ok(key,key_len))return 0;
    if(omilog_keyword_authority_bearing(key,key_len))return 0;
    if(!omilog_copy_token(out->keyword,sizeof(out->keyword),key,key_len))return 0;
    p=omilog_skip_horizontal(p);
    assign=p;
    while(*p&&*p!='\r'&&*p!='\n')p++;
    line_end=p;
    while(line_end>assign&&(line_end[-1]==' '||line_end[-1]=='\t'))line_end--;
    assign_len=(size_t)(line_end-assign);
    if(!omilog_copy_token(out->assignment,sizeof(out->assignment),assign,assign_len))return 0;
    while(*p=='\r'||*p=='\n')p++;
    block=omilog_skip_space(p);
    if(block&&*block){
        if(strncmp(block,"omi-",4)!=0)return 0;
        close=strstr(block,"-imo");
        if(!close)return 0;
        out->source_block_start=block;
        out->source_block_len=(size_t)((close+4)-block);
        if(omilog_block_has_authority_term(out->source_block_start,out->source_block_len))return 0;
        out->has_source_block=1;
        out->has_o_expression_body=(uint8_t)omilog_has_oexpr(block,out->source_block_len);
        p=close+4;
        p=omilog_skip_space(p);
        if(*p)return 0;
    }
    out->candidate_only=1;
    return 1;
}

int omilog_format_candidate_head(const OmiLogCandidate *candidate, char *out, size_t cap) {
    int n;
    if(!candidate||!out||cap==0)return 0;
    if(!candidate->candidate_only||!candidate->address.source[0]||!candidate->keyword[0]||!candidate->assignment[0])return 0;
    n=snprintf(out,cap,"%s %s %s",candidate->address.source,candidate->keyword,candidate->assignment);
    if(n<0||(size_t)n>=cap)return 0;
    return 1;
}

static void die(const char *msg) { fprintf(stderr, "FATAL: %s\n", msg); _exit(1); }

static void *xmalloc(size_t n) { void *p = malloc(n ? n : 1); if (!p) die("malloc"); return p; }
static void *xrealloc(void *p, size_t n) { p = realloc(p, n ? n : 1); if (!p) die("realloc"); return p; }
static char *xstrdup(const char *s) { size_t n = strlen(s); char *p = (char *)xmalloc(n+1); memcpy(p,s,n+1); return p; }
static char *xstrndup(const char *s, size_t n) { char *p = (char *)xmalloc(n+1); memcpy(p,s,n); p[n]=0; return p; }

static void breserve(Buffer *b, size_t extra) {
    size_t need = b->len+extra+1;
    if (need <= b->cap) return;
    size_t cap = b->cap ? b->cap : 128;
    while (cap < need) cap *= 2;
    b->data = (char *)xrealloc(b->data, cap);
    b->cap = cap;
}
static void bputc(Buffer *b, unsigned char c) { breserve(b,1); b->data[b->len++]=(char)c; b->data[b->len]=0; }
static void bappend(Buffer *b, const char *s) { size_t n = strlen(s); breserve(b,n); memcpy(b->data+b->len,s,n); b->len+=n; b->data[b->len]=0; }
static void bappend_bytes(Buffer *b, const unsigned char *d, size_t n) { breserve(b,n); memcpy(b->data+b->len,d,n); b->len+=n; b->data[b->len]=0; }
static void bfree(Buffer *b) { free(b->data); b->data=NULL; b->len=b->cap=0; }

static uint64_t fnv1a64(const unsigned char *data, size_t len) {
    uint64_t h = 1469598103934665603ull;
    for (size_t i = 0; i < len; i++) { h ^= data[i]; h *= 1099511628211ull; }
    return h;
}

static uint16_t u16(uint32_t x) { return (uint16_t)(x & 0xffffu); }
static uint32_t rotl32(uint32_t x, int n) { n &= 31; return (x << n) | (x >> (32 - n)); }
static uint32_t rotr32(uint32_t x, int n) { n &= 31; return (x >> n) | (x << (32 - n)); }
static uint16_t rotl16(uint16_t x, uint16_t n) { n &= 15; if (!n) return x; return u16(((uint32_t)x << n) | ((uint32_t)x >> (16-n))); }
static uint16_t rotr16(uint16_t x, uint16_t n) { n &= 15; if (!n) return x; return u16(((uint32_t)x >> n) | ((uint32_t)x << (16-n))); }
static uint16_t delta16(uint16_t x, uint16_t c) { return u16(rotl16(x,1) ^ rotl16(x,3) ^ rotr16(x,2) ^ c); }

static uint32_t bqf32(uint32_t x, uint32_t y) { return (60u * x * x) + (16u * x * y) + (4u * y * y); }

static Node *nnew(NodeKind k) {
    Node *n = (Node *)xmalloc(sizeof(Node));
    n->kind = k; n->text = NULL; n->items = NULL; n->count = n->cap = 0; n->car = n->cdr = NULL;
    return n;
}
static Node *natom(const char *t) { Node *n = nnew(NODE_ATOM); n->text = xstrdup(t); return n; }
static Node *nstring(const char *t) { Node *n = nnew(NODE_STRING); n->text = xstrdup(t); return n; }
static Node *npair(Node *a, Node *d) { Node *n = nnew(NODE_PAIR); n->car = a; n->cdr = d; return n; }

static void npush(Node *l, Node *item) {
    if (l->count == l->cap) {
        size_t cap = l->cap ? l->cap*2 : 8;
        l->items = (Node **)xrealloc(l->items, cap*sizeof(Node*));
        l->cap = cap;
    }
    l->items[l->count++] = item;
}

static void nfree(Node *n) {
    if (!n) return;
    free(n->text);
    if (n->kind == NODE_LIST) { for (size_t i = 0; i < n->count; i++) nfree(n->items[i]); free(n->items); }
    else if (n->kind == NODE_PAIR) { nfree(n->car); nfree(n->cdr); }
    free(n);
}

static Node *ncopy(Node *n) {
    if (!n) return NULL;
    if (n->kind == NODE_ATOM) return natom(n->text ? n->text : "");
    if (n->kind == NODE_STRING) return nstring(n->text ? n->text : "");
    if (n->kind == NODE_LIST) { Node *l = nnew(NODE_LIST); for (size_t i=0;i<n->count;i++) npush(l,ncopy(n->items[i])); return l; }
    if (n->kind == NODE_PAIR) return npair(ncopy(n->car), ncopy(n->cdr));
    return NULL;
}

static int perr(Parser *p, const char *msg) { snprintf(p->error,sizeof(p->error),"%s at line %d, col %d",msg,p->line,p->col); return 0; }
static int pdone(Parser *p) { return p->pos >= p->len; }
static char ppeek(Parser *p) { return pdone(p) ? '\0' : p->src[p->pos]; }
static char pnext(Parser *p) { char c = pdone(p)?'\0':p->src[p->pos++]; if(c=='\n'){p->line++;p->col=1;}else p->col++; return c; }
static void pskip(Parser *p) { while(!pdone(p)){char c=ppeek(p);if(c==' '||c=='\t'||c=='\r'||c=='\n'){pnext(p);}else if(c==';'){while(!pdone(p)&&ppeek(p)!='\n')pnext(p);}else break;} }
static int isatomdelim(char c) { return c=='\0'||c=='('||c==')'||c=='"'||c==';'||isspace((unsigned char)c); }
static int isdot(Parser *p) { if(ppeek(p)!='.') return 0; char n=(p->pos+1<p->len)?p->src[p->pos+1]:'\0'; return isatomdelim(n); }

static Node *pexpr(Parser *p);
static Node *pdotted(Node *l, Node *cdr) { Node *t=cdr; while(l->count>0) t=npair(l->items[--l->count],t); free(l->items); free(l); return t; }

static Node *plist(Parser *p) {
    pnext(p); Node *l=nnew(NODE_LIST);
    while(!pdone(p)){pskip(p);if(ppeek(p)==')'){pnext(p);return l;}if(isdot(p)){if(l->count==0){nfree(l);perr(p,"dotted car missing");return NULL;}pnext(p);Node *cdr=pexpr(p);if(!cdr){nfree(l);return NULL;}pskip(p);if(ppeek(p)!=')'){nfree(l);nfree(cdr);perr(p,"dotted missing )");return NULL;}pnext(p);return pdotted(l,cdr);}Node *item=pexpr(p);if(!item){nfree(l);return NULL;}npush(l,item);}
    nfree(l); perr(p,"unterminated list"); return NULL;
}

static Node *pstring(Parser *p) {
    Buffer out={0}; pnext(p);
    while(!pdone(p)&&ppeek(p)!='"'){char c=pnext(p);if(c=='\\'&&!pdone(p)){char e=pnext(p);if(e=='n')bputc(&out,'\n');else if(e=='t')bputc(&out,'\t');else if(e=='r')bputc(&out,'\r');else bputc(&out,(unsigned char)e);}else bputc(&out,(unsigned char)c);}
    if(ppeek(p)!='"'){bfree(&out);perr(p,"unterminated string");return NULL;}pnext(p);Node *n=nstring(out.data?out.data:"");bfree(&out);return n;
}

static Node *patom(Parser *p) {
    size_t start=p->pos; while(!pdone(p)&&!isatomdelim(ppeek(p))) pnext(p);
    if(p->pos==start){perr(p,"empty atom");return NULL;}char *t=xstrndup(p->src+start,p->pos-start);Node *n=natom(t);free(t);return n;
}

static Node *pexpr(Parser *p) {
    pskip(p); if(pdone(p)){perr(p,"missing expr");return NULL;}
    if(ppeek(p)=='(') return plist(p); if(ppeek(p)==')'){perr(p,"unexpected )");return NULL;}
    if(ppeek(p)=='"') return pstring(p); return patom(p);
}

static Node *parse_sexpr(const char *src, char *err, size_t errlen) {
    Parser p; p.src=src; p.len=strlen(src); p.pos=0; p.line=1; p.col=1; p.error[0]=0;
    Node *root=pexpr(&p); if(!root){snprintf(err,errlen,"%s",p.error);return NULL;} pskip(&p);
    if(!pdone(&p)){nfree(root);perr(&p,"trailing");snprintf(err,errlen,"%s",p.error);return NULL;} return root;
}

static void jesc(Buffer *b, const char *s) {
    for(const unsigned char *p=(const unsigned char*)s;*p;p++){unsigned char c=*p;if(c=='"'||c=='\\'){bputc(b,'\\');bputc(b,c);}else if(c=='\n')bappend(b,"\\n");else if(c=='\r')bappend(b,"\\r");else if(c=='\t')bappend(b,"\\t");else if(c<0x20){char tmp[8];snprintf(tmp,sizeof(tmp),"\\u%04x",c);bappend(b,tmp);}else bputc(b,c);}
}

static void ncanon(Node *n, Buffer *b) {
    if(!n){bappend(b,"nil");return;}
    if(n->kind==NODE_ATOM){bappend(b,n->text?n->text:"");}
    else if(n->kind==NODE_STRING){bputc(b,'"');jesc(b,n->text?n->text:"");bputc(b,'"');}
    else if(n->kind==NODE_LIST){bputc(b,'(');for(size_t i=0;i<n->count;i++){if(i)bputc(b,' ');ncanon(n->items[i],b);}bputc(b,')');}
    else if(n->kind==NODE_PAIR){bputc(b,'(');ncanon(n->car,b);bappend(b," . ");ncanon(n->cdr,b);bputc(b,')');}
}

static int isatom(Node *n, const char *t) { return n && n->kind==NODE_ATOM && n->text && strcmp(n->text,t)==0; }

static int lane_from_atom(Node *n, unsigned char *lane) {
    if(!n||n->kind!=NODE_ATOM||!n->text) return 0;
    if(strcmp(n->text,"FS")==0){*lane=OMI_FS;return 1;} if(strcmp(n->text,"GS")==0){*lane=OMI_GS;return 1;}
    if(strcmp(n->text,"RS")==0){*lane=OMI_RS;return 1;} if(strcmp(n->text,"US")==0){*lane=OMI_US;return 1;} return 0;
}

static void epush(EventList *e, unsigned char lane, const char *val) {
    if(e->len==e->cap){size_t cap=e->cap?e->cap*2:16;e->items=(Event*)xrealloc(e->items,cap*sizeof(Event));e->cap=cap;}
    e->items[e->len].lane=lane; e->items[e->len].value=xstrdup(val); e->len++;
}
static void efree(EventList *e){for(size_t i=0;i<e->len;i++)free(e->items[i].value);free(e->items);e->items=NULL;e->len=e->cap=0;}

static int is_canonical_root(Node *root) { return root && root->kind==NODE_LIST && root->count>0 && isatom(root->items[0],"omi"); }
static int has_lane_pair(Node *n) { if(!n) return 0; if(n->kind==NODE_PAIR){unsigned char l;if(lane_from_atom(n->car,&l))return 1;return has_lane_pair(n->car)||has_lane_pair(n->cdr);} if(n->kind==NODE_LIST){for(size_t i=0;i<n->count;i++)if(has_lane_pair(n->items[i]))return 1;} return 0; }

static void collect_canon(Node *n, EventList *e, int depth) {
    if(!n)return; if(n->kind==NODE_LIST){if(n->count>0&&n->items[0]->kind==NODE_ATOM){if(depth>=2)epush(e,OMI_RS,n->items[0]->text);for(size_t i=1;i<n->count;i++)collect_canon(n->items[i],e,depth+1);return;}for(size_t i=0;i<n->count;i++)collect_canon(n->items[i],e,depth+1);}
    else if(n->kind==NODE_PAIR){collect_canon(n->car,e,depth+1);collect_canon(n->cdr,e,depth+1);}else if(depth>=2){Buffer b={0};ncanon(n,&b);epush(e,OMI_US,b.data?b.data:"");bfree(&b);}
}
static void collect_canon_root(Node *root, EventList *e){epush(e,OMI_FS,"omi");for(size_t i=1;i<root->count;i++){Node *sec=root->items[i];if(sec&&sec->kind==NODE_LIST&&sec->count>0&&sec->items[0]->kind==NODE_ATOM){epush(e,OMI_GS,sec->items[0]->text);for(size_t j=1;j<sec->count;j++)collect_canon(sec->items[j],e,2);}else collect_canon(sec,e,1);}}
static int try_alist_pair(Node *n, EventList *e) { unsigned char l; if(!n||n->kind!=NODE_PAIR)return 0; if(lane_from_atom(n->car,&l)){Buffer b={0};ncanon(n->cdr,&b);epush(e,l,b.data?b.data:"");bfree(&b);return 1;} return 0; }
static void collect_alist(Node *n, EventList *e){if(!n)return;if(n->kind==NODE_PAIR){if(!try_alist_pair(n,e)){collect_alist(n->car,e);collect_alist(n->cdr,e);}}else if(n->kind==NODE_LIST){for(size_t i=0;i<n->count;i++)collect_alist(n->items[i],e);}}

static void build_tape(Compilation *c, const char *src) {
    unsigned char hdr[8]={0x00,0x1b,OMI_FS,OMI_GS,OMI_RS,OMI_US,src&&src[0]?(unsigned char)src[0]:0x00,c->state};
    bappend_bytes(&c->tape,hdr,8);
    for(size_t i=0;i<c->events.len;i++){const char *val=c->events.items[i].value;size_t n=strlen(val);if(n>65535u)die("record too large");bputc(&c->tape,c->events.items[i].lane);unsigned char lenb[2]={(unsigned char)((n>>8)&0xff),(unsigned char)(n&0xff)};bappend_bytes(&c->tape,lenb,2);bappend_bytes(&c->tape,(const unsigned char*)val,n);}
}

static int compile_omi(const char *src, Compilation *c, char *err, size_t errlen) {
    Buffer norm={0}; memset(c,0,sizeof(*c));
    c->root=parse_sexpr(src,err,errlen); if(!c->root) return 0;
    ncanon(c->root,&norm); c->normalized=norm.data?norm.data:xstrdup("");
    if(is_canonical_root(c->root)){snprintf(c->shape,sizeof(c->shape),"canonical");c->state=STATE_CANONICAL;collect_canon_root(c->root,&c->events);}
    else if(has_lane_pair(c->root)){snprintf(c->shape,sizeof(c->shape),"alist");c->state=STATE_ALIST;collect_alist(c->root,&c->events);}
    else{snprintf(err,errlen,"unsupported shape");return 0;}
    if(c->events.len==0){snprintf(err,errlen,"no lane records");return 0;}
    build_tape(c,src); c->source_hash=fnv1a64((const unsigned char*)src,strlen(src));
    c->normalized_hash=fnv1a64((const unsigned char*)c->normalized,strlen(c->normalized));
    c->tape_hash=fnv1a64((const unsigned char*)c->tape.data,c->tape.len); return 1;
}
static void cfree(Compilation*c){nfree(c->root);free(c->normalized);efree(&c->events);bfree(&c->tape);memset(c,0,sizeof(*c));}

static Node *reduce(Node *expr) {
    if(!expr) return NULL;
    if(expr->kind==NODE_ATOM) return natom(expr->text?expr->text:"");
    if(expr->kind==NODE_STRING) return nstring(expr->text?expr->text:"");
    if(expr->kind==NODE_PAIR){return npair(reduce(expr->car),reduce(expr->cdr));}
    if(expr->kind==NODE_LIST){
        if(expr->count==0) return nnew(NODE_LIST);
        Node *op=reduce(expr->items[0]); if(!op) return NULL;
        if(op->kind==NODE_ATOM&&op->text){
            if(strcmp(op->text,"cons")==0){Node*a=(expr->count>1)?reduce(expr->items[1]):natom("nil");Node*b=(expr->count>2)?reduce(expr->items[2]):natom("nil");nfree(op);return npair(a?a:natom("nil"),b?b:natom("nil"));}
            if(strcmp(op->text,"car")==0){Node*arg=(expr->count>1)?reduce(expr->items[1]):NULL;nfree(op);if(!arg)return natom("nil");if(arg->kind==NODE_PAIR){Node*r=arg->car;arg->car=NULL;nfree(arg);return r;}if(arg->kind==NODE_LIST&&arg->count>0){Node*r=arg->items[0];arg->items[0]=NULL;nfree(arg);return r;}nfree(arg);return natom("nil");}
            if(strcmp(op->text,"cdr")==0){Node*arg=(expr->count>1)?reduce(expr->items[1]):NULL;nfree(op);if(!arg)return nnew(NODE_LIST);if(arg->kind==NODE_PAIR){Node*r=arg->cdr;arg->cdr=NULL;nfree(arg);return r;}if(arg->kind==NODE_LIST){if(arg->count<=1){nfree(arg);return nnew(NODE_LIST);}Node*r=nnew(NODE_LIST);for(size_t i=1;i<arg->count;i++)npush(r,arg->items[i]);arg->count=0;nfree(arg);return r;}nfree(arg);return nnew(NODE_LIST);}
            if(strcmp(op->text,"xor")==0){Node*a=(expr->count>1)?reduce(expr->items[1]):NULL;Node*b=(expr->count>2)?reduce(expr->items[2]):NULL;nfree(op);uint64_t va=0,vb=0;if(a&&a->text)va=strtoull(a->text,NULL,0);if(b&&b->text)vb=strtoull(b->text,NULL,0);nfree(a);nfree(b);char tmp[32];snprintf(tmp,sizeof(tmp),"0x%016llx",(unsigned long long)(va^vb));return natom(tmp);}
            if(strcmp(op->text,"rot")==0){Node*x=(expr->count>1)?reduce(expr->items[1]):NULL;Node*n=(expr->count>2)?reduce(expr->items[2]):NULL;nfree(op);uint64_t vx=0,vn=0;if(x&&x->text)vx=strtoull(x->text,NULL,0);if(n&&n->text)vn=strtoull(n->text,NULL,0);vn&=63;uint64_t vr=(vx<<vn)|(vx>>(64-vn));nfree(x);nfree(n);char tmp[32];snprintf(tmp,sizeof(tmp),"0x%016llx",(unsigned long long)vr);return natom(tmp);}
            if(strcmp(op->text,"mask")==0){Node*x=(expr->count>1)?reduce(expr->items[1]):NULL;Node*m=(expr->count>2)?reduce(expr->items[2]):NULL;nfree(op);uint64_t vx=0,vm=0;if(x&&x->text)vx=strtoull(x->text,NULL,0);if(m&&m->text)vm=strtoull(m->text,NULL,0);nfree(x);nfree(m);char tmp[32];snprintf(tmp,sizeof(tmp),"0x%016llx",(unsigned long long)(vx&vm));return natom(tmp);}
            if(strcmp(op->text,"hash")==0){Node*arg=(expr->count>1)?reduce(expr->items[1]):NULL;nfree(op);Buffer b={0};ncanon(arg,&b);uint64_t h=fnv1a64((const unsigned char*)(b.data?b.data:""),b.len);nfree(arg);bfree(&b);char tmp[32];snprintf(tmp,sizeof(tmp),"0x%016llx",(unsigned long long)h);return natom(tmp);}
            if(strcmp(op->text,"quote")==0){Node*arg=(expr->count>1)?expr->items[1]:NULL;nfree(op);return arg?ncopy(arg):natom("nil");}
            if(strcmp(op->text,"normalize")==0){Node*arg=(expr->count>1)?reduce(expr->items[1]):NULL;nfree(op);Buffer b={0};ncanon(arg,&b);nfree(arg);Node*r=nstring(b.data?b.data:"");bfree(&b);return r;}
            if(strcmp(op->text,"eval")==0||strcmp(op->text,"reduce")==0){Node*arg=(expr->count>1)?reduce(expr->items[1]):NULL;nfree(op);Node*r=reduce(arg);nfree(arg);return r;}
            if(strcmp(op->text,"receipt")==0){Node*arg=(expr->count>1)?expr->items[1]:NULL;nfree(op);if(!arg)return nstring("{}");Buffer b={0};ncanon(arg,&b);Compilation c;char err[256];Buffer rj={0};if(compile_omi(b.data,&c,err,sizeof(err))){bappend(&rj,"{\"shape\":\"");bappend(&rj,c.shape);bappend(&rj,"\",\"events\":");char tmp[32];snprintf(tmp,sizeof(tmp),"%zu",c.events.len);bappend(&rj,tmp);bappend(&rj,"}");cfree(&c);}else{bappend(&rj,"{\"error\":\"");bappend(&rj,err);bappend(&rj,"\"}");}bfree(&b);Node*r=nstring(rj.data?rj.data:"{}");bfree(&rj);return r;}
            if(strcmp(op->text,"list")==0){nfree(op);Node*r=nnew(NODE_LIST);for(size_t i=1;i<expr->count;i++){Node*ev=reduce(expr->items[i]);if(ev)npush(r,ev);}return r;}
            {Node*r=nnew(NODE_LIST);npush(r,op);for(size_t i=1;i<expr->count;i++){Node*ev=reduce(expr->items[i]);if(ev)npush(r,ev);}return r;}
        }
        Node*r=nnew(NODE_LIST);npush(r,op);for(size_t i=1;i<expr->count;i++){Node*ev=reduce(expr->items[i]);if(ev)npush(r,ev);}return r;
    }
    return NULL;
}

static char *generate_receipt(const char *src, const char *result, uint64_t cycle) {
    Buffer b={0}; bappend(&b,"{\"cy\":"); char tmp[32]; snprintf(tmp,sizeof(tmp),"%llu",(unsigned long long)cycle); bappend(&b,tmp);
    bappend(&b,",\"s\":\""); jesc(&b,src?src:""); bappend(&b,"\",\"r\":\""); jesc(&b,result?result:"");
    bappend(&b,"\",\"h\":\""); uint64_t h=fnv1a64((const unsigned char*)(result?result:""),result?strlen(result):0);
    snprintf(tmp,sizeof(tmp),"0x%016llx",(unsigned long long)h); bappend(&b,tmp); bappend(&b,"\"}");
    char *r=b.data; b.data=NULL; bfree(&b); return r;
}

static void ring_load(void) {
    int fd=open(RING_FILE,O_RDONLY); if(fd>=0){ssize_t rv=read(fd,ring,sizeof(ring));(void)rv;close(fd);}
}
static void ring_save(void) {
    int fd=open(RING_FILE,O_WRONLY|O_CREAT,0666); if(fd>=0){ssize_t wv=write(fd,ring,sizeof(ring));(void)wv;close(fd);}
}
static size_t ring_idx(void) { return g_cycle % RING_SIZE; }
static void ring_store(uint64_t h, const char *receipt_str) {
    size_t idx=ring_idx(); ring[idx].cycle=g_cycle; ring[idx].hash=h;
    memset(ring[idx].receipt,0,MAX_RECEIPT); strncpy(ring[idx].receipt,receipt_str,MAX_RECEIPT-1);
    ring_save();
}
static void ring_dump(void) {
    for(size_t i=0;i<RING_SIZE;i++){if(ring[i].hash!=0||ring[i].receipt[0])printf("RING[%zu] cyc=%llu h=0x%016llx %s\n",i,(unsigned long long)ring[i].cycle,(unsigned long long)ring[i].hash,ring[i].receipt);}
}

static uint16_t ring_xor_fold(void) {
    uint16_t fold=0; for(size_t i=0;i<RING_SIZE;i++) fold=u16(fold ^ (uint16_t)(ring[i].hash & 0xFFFF)); return fold;
}
static uint16_t ring_sum_fold(void) {
    uint16_t fold=0; for(size_t i=0;i<RING_SIZE;i++) fold=u16(fold + (uint16_t)(ring[i].hash & 0xFFFF)); return fold;
}
static uint16_t ring_rot_fold(void) {
    uint16_t fold=0; for(size_t i=0;i<RING_SIZE;i++) fold=u16(fold ^ rotl16((uint16_t)(ring[i].hash&0xFFFF),(uint16_t)(i&15))); return fold;
}

static int ring_has_receipt(uint16_t val) {
    for(size_t i=0;i<RING_SIZE;i++) if((uint16_t)(ring[i].hash&0xFFFF)==val && ring[i].hash!=0) return 1; return 0;
}

static int parse_hex_field(const char *text, size_t len, uint32_t *out) {
    uint32_t v=0; if(len<3||text[0]!='0'||text[1]!='x') return 0;
    for(size_t i=2;i<len;i++){unsigned char c=(unsigned char)text[i];uint32_t d;if(c>='0'&&c<='9')d=(uint32_t)(c-'0');else if(c>='a'&&c<='f')d=10u+(uint32_t)(c-'a');else if(c>='A'&&c<='F')d=10u+(uint32_t)(c-'A');else return 0;v=(v<<4)|d;}
    *out=v; return 1;
}

static int take_hex(const char **p, char delim, uint32_t *out) {
    const char *start=*p; while(**p&&**p!=delim) (*p)++;
    if(!parse_hex_field(start,(size_t)(*p-start),out)) return 0;
    if(delim&&**p!=delim) return 0; if(delim)(*p)++; return 1;
}

static int parse_omi_addr(const char *line, OmiInst *inst) {
    const char *p=line; uint32_t v; while(isspace((unsigned char)*p))p++;
    if(!take_hex(&p,'-',&v))return 0; inst->s0=u16(v);
    if(!take_hex(&p,'-',&v))return 0; inst->s1=u16(v);
    if(!take_hex(&p,'-',&v))return 0; inst->s2=u16(v);
    if(!take_hex(&p,'/',&v))return 0; inst->s3=u16(v);
    if(!take_hex(&p,'/',&v))return 0; inst->s4=u16(v);
    if(!take_hex(&p,'/',&v))return 0; inst->s5=u16(v);
    if(!take_hex(&p,'/',&v))return 0; inst->s6=u16(v);
    if(!take_hex(&p,'?',&v))return 0; inst->s7=u16(v);
    if(!take_hex(&p,'?',&v))return 0; inst->payload=v;
    if(!take_hex(&p,'@',&v))return 0; inst->mask=v;
    if(!take_hex(&p,'@',&v))return 0; inst->car=v;
    {const char *start=p; while(*p&&!isspace((unsigned char)*p))p++; if(!parse_hex_field(start,(size_t)(p-start),&v))return 0; inst->cdr=v;}
    while(isspace((unsigned char)*p))p++; return *p=='\0';
}

static uint16_t execute_omi_op(const OmiInst *inst) {
    switch(inst->s3){
    case 0x0000: return 0;
    case 0x0001: return u16(inst->s4 ^ inst->s5);
    case 0x0002: return u16(~(inst->s4 ^ inst->s5));
    case 0x0003: return u16(~(inst->s4 & inst->s5));
    case 0x0004: return u16(inst->s4 & inst->s5);
    case 0x0005: return u16(inst->s4 | inst->s5);
    case 0x0006: return rotl16(inst->s4, inst->s5);
    case 0x0007: return rotr16(inst->s4, inst->s5);
    case 0x0008: return delta16(inst->s4, inst->s5);
    case 0x0009: return u16(bqf32((inst->s4>>8)&0xffu, inst->s5&0xffu));
    case 0x000a: return u16(inst->s4);
    case 0x000b: return u16(inst->s4);
    case 0x000c: return u16(inst->s4);
    case 0x000d: return u16(inst->s4 ^ inst->s5 ^ inst->s6);
    case 0x000e: return u16(inst->car ^ inst->cdr);
    case 0x000f: return 0;
    default: return u16(inst->s0^inst->s1^inst->s2^inst->s3^inst->s4^inst->s5^inst->s6^inst->s7^inst->payload^inst->mask^inst->car^inst->cdr);
    }
}

static void cpu_exec(CpuState *cpu, uint8_t op) {
    switch(op&0xF){
    case 0x0: break;
    case 0x1: cpu->car_reg=cpu->cdr_reg; cpu->cdr_reg=cpu->payload; break;
    case 0x2: cpu->payload=cpu->car_reg; break;
    case 0x3: cpu->payload=cpu->cdr_reg; break;
    case 0x4: cpu->payload^=cpu->mask; break;
    case 0x5: cpu->payload|=cpu->mask; break;
    case 0x6: cpu->payload&=cpu->mask; break;
    case 0x7: cpu->payload=rotl32(cpu->payload,1); break;
    case 0x8: if(cpu->mask<MEMORY_SIZE)cpu->payload=cpu->memory[cpu->mask]; break;
    case 0x9: if(cpu->mask<MEMORY_SIZE)cpu->memory[cpu->mask]=cpu->payload; break;
    case 0xA: cpu->pc=cpu->mask&0xFFFF; break;
    case 0xB: cpu->cdr_reg=cpu->pc; cpu->pc=cpu->mask&0xFFFF; break;
    case 0xC: if(cpu->stack_ptr<STACK_SIZE)cpu->stack[cpu->stack_ptr++]=cpu->payload; break;
    case 0xD: if(cpu->stack_ptr>0)cpu->payload=cpu->stack[--cpu->stack_ptr]; break;
    case 0xE: cpu->flags=(cpu->payload==cpu->mask)?0xFFFFFFFF:0; break;
    case 0xF: cpu->halted=1; break;
    }
    cpu->epoch++;
}

static void extract_nibbles(const OmiInst *inst, uint8_t *nibbles, int *count) {
    *count=0; uint16_t segs[]={inst->s0,inst->s1,inst->s2,inst->s3};
    for(int i=0;i<4;i++){nibbles[(*count)++]=(segs[i]>>12)&0xF;nibbles[(*count)++]=(segs[i]>>8)&0xF;nibbles[(*count)++]=(segs[i]>>4)&0xF;nibbles[(*count)++]=(segs[i]>>0)&0xF;}
    uint16_t segs2[]={inst->s4,inst->s5,inst->s6,inst->s7};
    for(int i=0;i<4;i++){nibbles[(*count)++]=(segs2[i]>>12)&0xF;nibbles[(*count)++]=(segs2[i]>>8)&0xF;nibbles[(*count)++]=(segs2[i]>>4)&0xF;nibbles[(*count)++]=(segs2[i]>>0)&0xF;}
}

static void cpu_run(CpuState *cpu, const OmiInst *inst) {
    uint8_t nibbles[32]; int count=0; extract_nibbles(inst,nibbles,&count);
    cpu->payload=inst->payload; cpu->mask=inst->mask; cpu->car_reg=inst->car; cpu->cdr_reg=inst->cdr;
    for(int i=0;i<count&&!cpu->halted;i++) cpu_exec(cpu,nibbles[i]);
}

static void cpu_init(CpuState *cpu) { memset(cpu,0,sizeof(*cpu)); }

/* ─── Polybius 5x5 + QuQuart Geometry ─── */

static int polybius_origin_row(void) { return 1; }
static int polybius_origin_col(void) { return 1; }
static int polybius_get_low_ququart(int idx, int *row, int *col) { if(idx<0||idx>3)return 0; *row=1; *col=idx+2; return 1; }
static int polybius_get_high_ququart(int idx, int *row, int *col) { if(idx<0||idx>3)return 0; *row=idx+2; *col=1; return 1; }
static int polybius_get_interior(int x, int y, int *row, int *col) { if(x<0||x>3||y<0||y>3)return 0; *row=y+2; *col=x+2; return 1; }
static int polybius_is_rail(int row, int col) { if(row<1||row>5||col<1||col>5)return 0; if(row==1&&col==1)return 0; return row==1||col==1; }
static int polybius_is_interior(int row, int col) { if(row<1||row>5||col<1||col>5)return 0; if(row==1&&col==1)return 0; return row>1&&col>1; }

/* ─── Fano Plane incidence hypergraph (7 points, 7 lines) ─── */
static const uint8_t FANO_LINES[7][3] = {
    {0,1,2},{0,3,4},{0,5,6},{1,3,5},{1,4,6},{2,3,6},{2,4,5}
};
/* Dual: for each point, which 3 lines contain it */
static const uint8_t FANO_PT_LINES[7][3] = {
    {0,1,2},{0,3,4},{0,5,6},{1,3,5},{1,4,6},{2,3,6},{2,4,5}
};

/* ConfigMatrix: incidence from Fano point to shape family base in SHAPE_DB */
static const uint8_t CONFIG_MATRIX[7] = {0, 3, 6, 9, 12, 15, 18};
#define CONFIG_FAMILY_SIZE 3

/* ─── QuQuart Phase Map ─── */
static const unsigned char QUQUART_PHASE[4] = {OMI_US, OMI_GS, OMI_RS, OMI_FS};

/* ─── 5040 ring slot computation ─── */
static uint32_t compute_slot5040(int fano7, int role3, int local240) {
    return (uint32_t)(((fano7%7)*720u) + ((role3%3)*240u) + (uint32_t)(local240%240));
}

/* ─── Digital Twin Geometry: full Hopf/BQF/Fano/Polybius/Solid pipeline ─── */

/* Derive a named constant from tetrahedron projective point traversal */
static double tetra_derive_constant(const char *name, int steps) {
    /* Placeholder: returns standard floating-point approximations.
       TODO: derive exact sexagesimal values from tetrahedron centroid + edge/face
       traversals at precise fractions (0;20, 0;30, etc.) */
    (void)steps;
    if (strcmp(name, "phi") == 0) return 1.61803398874989484820;
    if (strcmp(name, "pi") == 0)  return 3.14159265358979323846;
    return 1.0;
}

/* ─── Chiral diagonal resolution: π as D+ vs D- competition for 0x1E ───
   D+ = {0,5,A,F} — posting/forward
   D- = {3,6,9,C} — pulling/inverse
   Both XOR→0, both SUM→0x1E (30).
   Direction = which diagonal reaches 0x1E first under bounded digestion. */
typedef enum { CHIRAL_BALANCED=0, CHIRAL_POSTING=1, CHIRAL_PULLING=-1, CHIRAL_INCOMPLETE=-2 } ChiralDir;

static ChiralDir tetra_chiral_pi(uint64_t hash) {
    int dplus = 0, dminus = 0, dp = -1, dm = -1;
    for (int i = 0; i < 16; i++) {
        int n = (int)((hash >> (60 - 4*i)) & 0xF);
        switch (n) {
            case 0x0: dplus += 0;  if (dp < 0 && dplus >= 30) dp = i; break;
            case 0x5: dplus += 5;  if (dp < 0 && dplus >= 30) dp = i; break;
            case 0xA: dplus += 10; if (dp < 0 && dplus >= 30) dp = i; break;
            case 0xF: dplus += 15; if (dp < 0 && dplus >= 30) dp = i; break;
            case 0x3: dminus += 3; if (dm < 0 && dminus >= 30) dm = i; break;
            case 0x6: dminus += 6; if (dm < 0 && dminus >= 30) dm = i; break;
            case 0x9: dminus += 9; if (dm < 0 && dminus >= 30) dm = i; break;
            case 0xC: dminus += 12; if (dm < 0 && dminus >= 30) dm = i; break;
        }
    }
    if (dp >= 0 && (dm < 0 || dp < dm)) return CHIRAL_POSTING;
    if (dm >= 0 && (dp < 0 || dm < dp)) return CHIRAL_PULLING;
    if (dp >= 0 && dm >= 0 && dp == dm) return CHIRAL_BALANCED;
    return CHIRAL_INCOMPLETE;
}

/* ─── Tetrahedron Seed Crystals ───
   All Platonic, Archimedean, Catalan, and 4D shapes derive from the
   tetrahedron and its stellation (stellated octahedron).
   Tetrahedron -> Stellated Octahedron -> Octahedron -> Cube -> ... */

typedef struct { double x, y, z; } V3;
typedef struct { int v[3]; } F3;
typedef struct { double x, y, z, w; } V4;
typedef uint16_t Citation;
typedef struct { Citation a, b; } OmiRelEdge;

/* ─── Tetrahedron: seed of all shapes (4 vertices, 6 edges, 4 faces) ─── */
static const OmiRelEdge SEED_TETRA_E[] = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
static const F3 SEED_TETRA_F[] = {{0,1,2},{0,1,3},{0,2,3},{1,2,3}};
#define SEED_TETRA_N 4
#define SEED_TETRA_EN 6
#define SEED_TETRA_FN 4

/* ─── Stellated Tetrahedron: tetra ∪ -tetra (8 vertices, 12 edges, 8 faces) ─── */
static const OmiRelEdge SEED_STELLATED_E[] = {
    {0,1},{0,2},{0,3},{1,2},{1,3},{2,3},
    {4,5},{4,6},{4,7},{5,6},{5,7},{6,7}
};
#define SEED_STELLATED_N 8
#define SEED_STELLATED_EN 12

/* ─── Octahedron from stellated tetrahedron (6 vertices) ─── */
static const OmiRelEdge SEED_OCTA_E[] = {
    {0,2},{0,3},{0,4},{0,5},{1,2},{1,3},{1,4},{1,5},{2,4},{2,5},{3,4},{3,5}
};
#define SEED_OCTA_N 6
#define SEED_OCTA_EN 12

/* ─── Cube (hexahedron): dual of octahedron ─── */
static const OmiRelEdge SEED_CUBE_E[] = {
    {0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},
    {0,4},{1,5},{2,6},{3,7}
};
#define SEED_CUBE_N 8
#define SEED_CUBE_EN 12

/* ─── Icosahedron (12 vertices) via tetrahedral stellation ─── */
static const OmiRelEdge SEED_ICOSA_E[] = {
    {0,1},{0,5},{0,7},{0,10},{0,11},
    {1,5},{1,7},{1,8},{1,9},
    {2,3},{2,4},{2,6},{2,10},{2,11},
    {3,4},{3,6},{3,8},{3,9},
    {4,5},{4,9},{4,11},
    {5,9},{5,11},
    {6,7},{6,8},{6,10},
    {7,8},{7,10},
    {8,9},{10,11}
};
#define SEED_ICOSA_N 12
#define SEED_ICOSA_EN 30

/* ─── Dodecahedron (20 vertices): dual of icosahedron ─── */
static const OmiRelEdge SEED_DODEC_E[] = {
    {0,8},{0,12},{0,16},{1,8},{1,14},{1,18},
    {2,9},{2,15},{2,18},{3,9},{3,13},{3,16},
    {4,10},{4,12},{4,17},{5,10},{5,14},{5,19},
    {6,11},{6,15},{6,19},{7,11},{7,13},{7,17},
    {8,9},{10,11},{12,14},{13,15},{16,17},{18,19}
};
#define SEED_DODEC_N 20
#define SEED_DODEC_EN 30

/* ─── Cuboctahedron (12 vertices, 24 edges) — Archimedean ─── */
static const OmiRelEdge SEED_CUBOCT_E[] = {
    {0,4},{0,5},{0,8},{0,9},{1,4},{1,5},{1,10},{1,11},
    {2,6},{2,7},{2,8},{2,9},{3,6},{3,7},{3,10},{3,11},
    {4,8},{4,10},{5,9},{5,11},{6,8},{6,10},{7,9},{7,11}
};
#define SEED_CUBOCT_N 12
#define SEED_CUBOCT_EN 24

/* ─── Truncated Octahedron (24 vertices, 36 edges) — Archimedean ─── */
static const OmiRelEdge SEED_TRUNCOCT_E[] = {
    {0,4},{0,8},{0,10},{1,6},{1,9},{1,11},
    {2,5},{2,8},{2,10},{3,7},{3,9},{3,11},
    {4,12},{4,14},{5,13},{5,15},{6,12},{6,14},
    {7,13},{7,15},{8,16},{9,18},{10,17},{11,19},
    {12,20},{13,22},{14,21},{15,23},{16,20},{16,22},
    {17,21},{17,23},{18,20},{18,22},{19,21},{19,23}
};
#define SEED_TRUNCOCT_N 24
#define SEED_TRUNCOCT_EN 36

/* ════════════════════════════════════════════ */
/* Seed shapes: generated from Platonic truncation/rectification */
/* ════════════════════════════════════════════ */

/* Truncated Cube (24v, 36e, 0f) */
static const OmiRelEdge SEED_TRUNCCUBE_E[] = {
    {0,1},{0,2},{0,11},{1,2},{1,5},{2,13},{3,4},{3,5},{3,8},{4,5},{4,17},{6,7},{6,8},{6,10},{7,8},{7,18},{9,10},{9,11},{9,22},{10,11},{12,13},{12,14},{12,23},{13,14},{14,16},{15,16},{15,17},{15,20},{16,17},{18,19},{18,20},{19,20},{19,21},{21,22},{21,23},{22,23},
};
#define SEED_TRUNCCUBE_N 24
#define SEED_TRUNCCUBE_EN 36

/* Rhombic Dodecahedron (14v, 24e, 0f) */
static const OmiRelEdge SEED_RHOMBICDODEC_E[] = {
    {0,1},{0,2},{0,4},{0,5},{1,3},{1,12},{2,3},{2,13},{3,7},{3,9},{4,6},{4,12},{5,6},{5,13},{6,10},{6,11},{7,8},{7,12},{8,9},{8,10},{8,11},{9,13},{10,12},{11,13},
};
#define SEED_RHOMBICDODEC_N 14
#define SEED_RHOMBICDODEC_EN 24

/* Icosidodecahedron (30v, 60e, 0f) */
static const OmiRelEdge SEED_ICOSIDODEC_E[] = {
    {0,1},{0,2},{0,5},{0,6},{1,4},{1,5},{1,22},{2,3},{2,6},{2,27},{3,4},{3,27},{3,29},{4,22},{4,29},{5,8},{5,21},{6,7},{6,26},{7,8},{7,26},{7,28},{8,21},{8,28},{9,10},{9,11},{9,14},{9,15},{10,13},{10,14},{10,20},{11,12},{11,15},{11,25},{12,13},{12,25},{12,29},{13,20},{13,29},{14,17},{14,19},{15,16},{15,24},{16,17},{16,24},{16,28},{17,19},{17,28},{18,19},{18,20},{18,21},{18,22},{19,21},{20,22},{23,24},{23,25},{23,26},{23,27},{24,26},{25,27},
};
#define SEED_ICOSIDODEC_N 30
#define SEED_ICOSIDODEC_EN 60

/* ════════════════════════════════════════════ */
/* Auto-generated Archimedean + Catalan solids  */
/* ════════════════════════════════════════════ */

/* Truncated Icosahedron (60v, 90e, 32f) */
static const OmiRelEdge SEED_TRUNCICOSA_E[] = {
    {0,1},{0,4},{0,53},{1,2},{1,58},{2,3},{2,29},{3,4},{3,8},{4,38},{5,6},{5,9},{5,48},{6,7},{6,44},{7,8},{7,39},{8,9},{9,28},{10,11},{10,14},{10,31},{11,12},{11,16},{12,13},{12,20},{13,14},{13,55},{14,51},{15,16},{15,19},{15,21},{16,17},{17,18},{17,30},{18,19},{18,41},{19,46},{20,21},{20,24},{21,22},{22,23},{22,45},{23,24},{23,26},{24,56},{25,26},{25,29},{25,57},{26,27},{27,28},{27,49},{28,29},{30,31},{30,34},{31,32},{32,33},{32,50},{33,34},{33,36},{34,42},{35,36},{35,39},{35,43},{36,37},{37,38},{37,54},{38,39},{40,41},{40,44},{40,47},{41,42},{42,43},{43,44},{45,46},{45,49},{46,47},{47,48},{48,49},{50,51},{50,54},{51,52},{52,53},{52,59},{53,54},{55,56},{55,59},{56,57},{57,58},{58,59},
};
#define SEED_TRUNCICOSA_N 60
#define SEED_TRUNCICOSA_EN 90

/* Truncated Dodecahedron (60v, 90e, 32f) */
static const OmiRelEdge SEED_TRUNCDODEC_E[] = {
    {0,1},{0,2},{0,49},{1,2},{1,26},{2,38},{3,4},{3,5},{3,25},{4,5},{4,56},{5,42},{6,7},{6,8},{6,46},{7,8},{7,54},{8,28},{9,10},{9,11},{9,40},{10,11},{10,27},{11,48},{12,13},{12,14},{12,31},{13,14},{13,53},{14,37},{15,16},{15,17},{15,58},{16,17},{16,32},{17,43},{18,19},{18,20},{18,45},{19,20},{19,33},{20,57},{21,22},{21,23},{21,41},{22,23},{22,51},{23,34},{24,25},{24,26},{24,29},{25,26},{27,28},{27,29},{28,29},{30,31},{30,32},{30,35},{31,32},{33,34},{33,35},{34,35},{36,37},{36,38},{36,44},{37,38},{39,40},{39,41},{39,47},{40,41},{42,43},{42,44},{43,44},{45,46},{45,47},{46,47},{48,49},{48,50},{49,50},{50,52},{51,52},{51,53},{52,53},{54,55},{54,56},{55,56},{55,59},{57,58},{57,59},{58,59},
};
#define SEED_TRUNCDODEC_N 60
#define SEED_TRUNCDODEC_EN 90

/* Truncated Icosidodecahedron (120v, 180e, 62f) */
static const OmiRelEdge SEED_TRUNCICOSIDODEC_E[] = {
    {0,1},{0,3},{0,27},{1,2},{1,10},{2,3},{2,7},{3,22},{4,5},{4,7},{4,18},{5,6},{5,90},{6,7},{6,23},{8,9},{8,11},{8,111},{9,10},{9,15},{10,11},{11,26},{12,13},{12,15},{12,110},{13,14},{13,119},{14,15},{14,19},{16,17},{16,19},{16,118},{17,18},{17,91},{18,19},{20,21},{20,23},{20,87},{21,22},{21,35},{22,23},{24,25},{24,27},{24,30},{25,26},{25,106},{26,27},{28,29},{28,31},{28,114},{29,30},{29,107},{30,31},{31,34},{32,33},{32,35},{32,86},{33,34},{33,115},{34,35},{36,37},{36,39},{36,45},{37,38},{37,60},{38,39},{38,57},{39,40},{40,41},{40,43},{41,42},{41,56},{42,43},{42,81},{43,53},{44,45},{44,47},{44,61},{45,46},{46,47},{46,48},{47,100},{48,49},{48,51},{49,50},{49,52},{50,51},{50,116},{51,101},{52,53},{52,55},{53,54},{54,55},{54,80},{55,117},{56,57},{56,59},{57,58},{58,59},{58,68},{59,76},{60,61},{60,63},{61,62},{62,63},{62,97},{63,65},{64,65},{64,67},{64,69},{65,66},{66,67},{66,96},{67,113},{68,69},{68,71},{69,70},{70,71},{70,112},{71,77},{72,73},{72,75},{72,82},{73,74},{73,79},{74,75},{74,84},{75,89},{76,77},{76,79},{77,78},{78,79},{78,85},{80,81},{80,83},{81,82},{82,83},{83,88},{84,85},{84,87},{85,86},{86,87},{88,89},{88,91},{89,90},{90,91},{92,93},{92,95},{92,98},{93,94},{93,103},{94,95},{94,108},{95,105},{96,97},{96,99},{97,98},{98,99},{99,104},{100,101},{100,103},{101,102},{102,103},{102,109},{104,105},{104,107},{105,106},{106,107},{108,109},{108,111},{109,110},{110,111},{112,113},{112,115},{113,114},{114,115},{116,117},{116,119},{117,118},{118,119},
};
#define SEED_TRUNCICOSIDODEC_N 120
#define SEED_TRUNCICOSIDODEC_EN 180

/* Tetrakis Hexahedron (14v, 36e, 0f) */
static const OmiRelEdge SEED_TETRAKISHEX_E[] = {
    {0,1},{0,2},{0,3},{0,4},{1,2},{1,3},{1,8},{1,10},{1,13},{2,4},{2,9},{2,11},{2,13},{3,4},{3,6},{3,10},{3,12},{4,7},{4,11},{4,12},{5,6},{5,7},{5,8},{5,9},{6,7},{6,8},{6,10},{6,12},{7,9},{7,11},{7,12},{8,9},{8,10},{8,13},{9,11},{9,13},
};
#define SEED_TETRAKISHEX_N 14
#define SEED_TETRAKISHEX_EN 36

/* Triakis Octahedron (14v, 36e, 0f) */
static const OmiRelEdge SEED_TRIAKISOCTA_E[] = {
    {0,1},{0,2},{0,3},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7},{1,8},{2,3},{2,7},{2,8},{2,9},{2,10},{2,13},{3,4},{3,5},{3,9},{3,10},{3,11},{4,5},{5,6},{5,7},{5,10},{5,11},{5,12},{6,7},{7,8},{7,10},{7,12},{7,13},{9,10},{10,11},{10,12},{10,13},
};
#define SEED_TRIAKISOCTA_N 14
#define SEED_TRIAKISOCTA_EN 36

/* Pentakis Dodecahedron (32v, 90e, 0f) */
static const OmiRelEdge SEED_PENTAKISDODEC_E[] = {
    {0,1},{0,2},{0,3},{0,4},{0,5},{1,2},{1,3},{1,15},{1,30},{1,31},{2,5},{2,25},{2,27},{2,30},{3,4},{3,22},{3,23},{3,31},{4,5},{4,6},{4,8},{4,23},{5,6},{5,9},{5,27},{6,7},{6,8},{6,9},{7,8},{7,9},{7,19},{7,28},{7,29},{8,21},{8,23},{8,29},{9,26},{9,27},{9,28},{10,11},{10,12},{10,13},{10,14},{10,15},{11,12},{11,13},{11,16},{11,18},{11,24},{12,15},{12,24},{12,25},{12,30},{13,14},{13,16},{13,17},{13,20},{14,15},{14,20},{14,22},{14,31},{15,30},{15,31},{16,17},{16,18},{16,19},{17,19},{17,20},{17,21},{17,29},{18,19},{18,24},{18,26},{18,28},{19,28},{19,29},{20,21},{20,22},{21,22},{21,23},{21,29},{22,23},{22,31},{24,25},{24,26},{25,26},{25,27},{25,30},{26,27},{26,28},
};
#define SEED_PENTAKISDODEC_N 32
#define SEED_PENTAKISDODEC_EN 90

/* Triakis Icosahedron (32v, 90e, 0f) */
static const OmiRelEdge SEED_TRIAKISICOSA_E[] = {
    {0,1},{0,2},{0,3},{1,2},{1,3},{1,5},{1,9},{1,10},{1,11},{1,20},{1,21},{1,28},{2,3},{2,11},{2,12},{2,13},{2,14},{2,24},{2,28},{2,29},{3,4},{3,5},{3,6},{3,14},{3,20},{3,24},{3,26},{4,5},{4,6},{5,6},{5,7},{5,8},{5,9},{5,20},{5,21},{5,30},{6,8},{6,14},{6,15},{6,16},{6,26},{6,30},{6,31},{7,8},{7,9},{8,9},{8,16},{8,17},{8,18},{8,27},{8,30},{8,31},{9,10},{9,11},{9,18},{9,21},{9,25},{9,27},{10,11},{11,13},{11,18},{11,19},{11,25},{11,28},{11,29},{12,13},{12,14},{13,14},{13,16},{13,18},{13,19},{13,22},{13,23},{13,29},{14,15},{14,16},{14,22},{14,24},{14,26},{15,16},{16,17},{16,18},{16,22},{16,23},{16,31},{17,18},{18,19},{18,23},{18,25},{18,27},
};
#define SEED_TRIAKISICOSA_N 32
#define SEED_TRIAKISICOSA_EN 90

/* Disdyakis Triacontahedron (62v, 180e, 0f) */
static const OmiRelEdge SEED_DISDYAKIS_E[] = {
    {0,1},{0,2},{0,3},{0,4},{1,2},{1,3},{1,8},{1,10},{1,18},{2,4},{2,16},{2,17},{2,18},{2,19},{2,20},{2,22},{2,23},{3,4},{3,5},{3,6},{3,8},{3,9},{3,11},{3,13},{3,14},{4,5},{4,7},{4,16},{5,6},{5,7},{6,7},{6,14},{6,15},{6,52},{7,16},{7,17},{7,46},{7,47},{7,48},{7,51},{7,52},{8,9},{8,10},{9,10},{9,11},{9,12},{9,59},{10,18},{10,19},{10,53},{10,54},{10,55},{10,58},{10,59},{11,12},{11,13},{12,13},{12,35},{12,36},{12,37},{12,55},{12,57},{12,59},{12,61},{13,14},{13,15},{13,61},{14,15},{15,32},{15,37},{15,38},{15,47},{15,50},{15,52},{15,61},{16,17},{17,23},{17,24},{17,51},{18,19},{19,20},{19,21},{19,58},{20,21},{20,22},{21,22},{21,42},{21,43},{21,44},{21,54},{21,56},{21,58},{21,60},{22,23},{22,24},{22,60},{23,24},{24,40},{24,44},{24,45},{24,48},{24,49},{24,51},{24,60},{25,26},{25,27},{25,28},{25,29},{26,27},{26,28},{26,33},{26,34},{26,41},{27,29},{27,30},{27,32},{27,33},{27,35},{27,36},{27,37},{27,38},{28,29},{28,39},{28,40},{28,41},{28,42},{28,43},{28,44},{28,45},{29,30},{29,31},{29,39},{30,31},{30,32},{31,32},{31,39},{31,40},{31,46},{31,47},{31,48},{31,49},{31,50},{32,38},{32,50},{33,34},{33,35},{34,35},{34,41},{34,42},{34,53},{34,54},{34,55},{34,56},{34,57},{35,36},{35,57},{36,37},{37,38},{37,61},{39,40},{40,45},{40,49},{41,42},{42,43},{42,56},{43,44},{44,45},{44,60},{46,47},{46,48},{47,50},{47,52},{48,49},{48,51},{53,54},{53,55},{54,56},{54,58},{55,57},{55,59},
};
#define SEED_DISDYAKIS_N 62
#define SEED_DISDYAKIS_EN 180

/* ─── Solid family: each Fano point roots a family tree ───
   Fano 0: Tetrahedron family (A₄ symmetry)
   Fano 1: Octahedron family (B₃ symmetry)
   Fano 2: Cube family (B₃ symmetry)
   Fano 3: Cuboctahedron family (B₃ symmetry)
   Fano 4: Icosahedron family (H₃ symmetry)
   Fano 5: Dodecahedron family (H₃ symmetry)
   Fano 6: Icosidodecahedron family (H₃ symmetry)
   Each family has a base shape + truncation + rectification + dual */

typedef struct {
    const char *name;        /* solid name */
    int fano_root;           /* 0-6: originating Fano point */
    int family_seq;          /* sequence within family */
    int nverts, nedges, nfaces;
    const OmiRelEdge *edges;
    int schlafli_p, schlafli_q;  /* {p,q} Schläfli symbol */
} ShapeDef;

/* Compact shape database: seed shapes + key derivatives */
static const ShapeDef SHAPE_DB[] = {
    /* Fano 0: Tetrahedron family */
    {"Tetrahedron", 0,0, 4,6,4, SEED_TETRA_E, 3,3},
    {"Stellated Tetrahedron", 0,1, 8,12,8, SEED_STELLATED_E, 3,3},
    {"Octahedron", 0,2, 6,12,8, SEED_OCTA_E, 3,4},
    /* Fano 1: Octahedron family */
    {"Octahedron", 1,0, 6,12,8, SEED_OCTA_E, 3,4},
    {"Truncated Octahedron", 1,1, 24,36,14, SEED_TRUNCOCT_E, 4,6},
    {"Tetrakis Hexahedron", 1,2, 14,36,24, SEED_TETRAKISHEX_E, 4,6},
    /* Fano 2: Cube family */
    {"Cube", 2,0, 8,12,6, SEED_CUBE_E, 4,3},
    {"Truncated Cube", 2,1, 24,36,14, SEED_TRUNCCUBE_E, 3,8},
    {"Triakis Octahedron", 2,2, 14,36,24, SEED_TRIAKISOCTA_E, 3,8},
    /* Fano 3: Cuboctahedron family */
    {"Cuboctahedron", 3,0, 12,24,14, SEED_CUBOCT_E, 3,4},
    {"Cuboctahedron", 3,1, 12,24,14, SEED_CUBOCT_E, 3,4},
    {"Rhombic Dodecahedron", 3,2, 14,24,12, SEED_RHOMBICDODEC_E, 3,4},
    /* Fano 4: Icosahedron family */
    {"Icosahedron", 4,0, 12,30,20, SEED_ICOSA_E, 3,5},
    {"Truncated Icosahedron", 4,1, 60,90,32, SEED_TRUNCICOSA_E, 5,3},
    {"Pentakis Dodecahedron", 4,2, 32,90,60, SEED_PENTAKISDODEC_E, 5,3},
    /* Fano 5: Dodecahedron family */
    {"Dodecahedron", 5,0, 20,30,12, SEED_DODEC_E, 5,3},
    {"Truncated Dodecahedron", 5,1, 60,90,32, SEED_TRUNCDODEC_E, 3,5},
    {"Triakis Icosahedron", 5,2, 32,90,60, SEED_TRIAKISICOSA_E, 3,5},
    /* Fano 6: Icosidodecahedron family */
    {"Icosidodecahedron", 6,0, 30,60,32, SEED_ICOSIDODEC_E, 3,5},
    {"Truncated Icosidodecahedron", 6,1, 120,180,62, SEED_TRUNCICOSIDODEC_E, 4,6},
    {"Disdyakis Triacontahedron", 6,2, 62,180,120, SEED_DISDYAKIS_E, 4,6},
};
#define SHAPE_DB_N (sizeof(SHAPE_DB)/sizeof(SHAPE_DB[0]))

/* ─── Renderer vertex pool: parallel to SHAPE_DB, projection-boundary only ─── */
/* ─── Three-tetrahedron projective kernel (int8_t coordinates, 36 values) ─── */
static const int8_t T0[4][3] = {{1,1,1},{1,-1,-1},{-1,1,-1},{-1,-1,1}};
static const int8_t T1[4][3] = {{-1,-1,-1},{-1,1,1},{1,-1,1},{1,1,-1}};
static const int8_t OCTA[6][3] = {{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}};
static const int8_t CUBOCT[12][3] = {
    {1,1,0},{1,-1,0},{-1,1,0},{-1,-1,0},
    {1,0,1},{1,0,-1},{-1,0,1},{-1,0,-1},
    {0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1}
};

/* ─── Golden ratio from {3,5}/{5,3} incidence field recurrence ───
   φ = 1 + 1/φ is the algebraic identity of icosahedral (H₃) symmetry,
   derived from the continued fraction of the Schläfli pentagonal ratio.
   No sqrt. Pure rational iteration, 48 steps → double precision. */
static double tetra_golden_ratio(void) {
    double x = 2.0;
    for (int i = 0; i < 48; i++) x = 1.0 + 1.0 / x;
    return x;
}

/* ─── Resolve vertex coordinates from incidence (projection boundary only) ─── */
static void resolve_vertex(int shape_idx, int cite, double *x, double *y, double *z) {
    double phi = tetra_golden_ratio(), psi = 1.0 / phi;
    switch (shape_idx) {
        case 0: { const int8_t (*v)[3] = &T0[cite]; *x=(*v)[0];*y=(*v)[1];*z=(*v)[2]; return; }
        case 1: { const int8_t (*v)[3] = cite<4?&T0[cite]:&T1[cite-4]; *x=(*v)[0];*y=(*v)[1];*z=(*v)[2]; return; }
        case 2: case 3: { const int8_t (*v)[3]=&OCTA[cite]; *x=(*v)[0];*y=(*v)[1];*z=(*v)[2]; return; }
        case 4: { static const int8_t d[24][3]={{0,1,2},{0,1,-2},{0,-1,2},{0,-1,-2},{0,2,1},{0,-2,1},{0,2,-1},{0,-2,-1},{1,0,2},{1,0,-2},{-1,0,2},{-1,0,-2},{1,2,0},{1,-2,0},{-1,2,0},{-1,-2,0},{2,0,1},{-2,0,1},{2,0,-1},{-2,0,-1},{2,1,0},{-2,1,0},{2,-1,0},{-2,-1,0}}; *x=d[cite][0];*y=d[cite][1];*z=d[cite][2]; return; }
        case 5: { static const int8_t d[14][3]={{1,0,0},{1,1,-1},{1,-1,-1},{1,1,1},{1,-1,1},{-1,0,0},{-1,1,1},{-1,-1,1},{-1,1,-1},{-1,-1,-1},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}}; const int8_t (*v)[3]=&d[cite]; double s=1.0/sqrt(3.0); int i; for(i=0;i<3;i++)if((*v)[i]==0)s=1.0; *x=(*v)[0]*s;*y=(*v)[1]*s;*z=(*v)[2]*s; return; }
        case 6: { static const int8_t d[8][3]={{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1},{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1}}; *x=d[cite][0];*y=d[cite][1];*z=d[cite][2]; return; }
        case 7: { static const double d[24][3]={{-1,-1.0/3,-1},{-1.0/3,-1,-1},{-1,-1,-1.0/3},{1,-1.0/3,-1},{1,-1,-1.0/3},{1.0/3,-1,-1},{1.0/3,1,-1},{1,1,-1.0/3},{1,1.0/3,-1},{-1,1,-1.0/3},{-1.0/3,1,-1},{-1,1.0/3,-1},{-1,-1.0/3,1},{-1,-1,1.0/3},{-1.0/3,-1,1},{1,-1.0/3,1},{1.0/3,-1,1},{1,-1,1.0/3},{1,1,1.0/3},{1.0/3,1,1},{1,1.0/3,1},{-1.0/3,1,1},{-1,1,1.0/3},{-1,1.0/3,1}}; *x=d[cite][0];*y=d[cite][1];*z=d[cite][2]; return; }
        case 8: { static const int8_t d[14][3]={{-1,-1,-1},{0,0,-1},{-1,0,0},{0,-1,0},{1,-1,-1},{1,0,0},{1,1,-1},{0,1,0},{-1,1,-1},{-1,-1,1},{0,0,1},{1,-1,1},{1,1,1},{-1,1,1}}; const int8_t (*v)[3]=&d[cite]; double s=1.0/sqrt(3.0); int i; for(i=0;i<3;i++)if((*v)[i]==0)s=1.0; *x=(*v)[0]*s;*y=(*v)[1]*s;*z=(*v)[2]*s; return; }
        case 9: case 10: *x=CUBOCT[cite][0];*y=CUBOCT[cite][1];*z=CUBOCT[cite][2]; return;
        case 11: { static const int8_t d[14][3]={{1,0,0},{1,1,1},{1,1,-1},{0,1,0},{1,-1,1},{1,-1,-1},{0,-1,0},{-1,1,1},{-1,0,0},{-1,1,-1},{-1,-1,1},{-1,-1,-1},{0,0,1},{0,0,-1}}; const int8_t (*v)[3]=&d[cite]; double s=1.0/sqrt(3.0); int i; for(i=0;i<3;i++)if((*v)[i]==0)s=1.0; *x=(*v)[0]*s;*y=(*v)[1]*s;*z=(*v)[2]*s; return; }
        case 12: { const double d[12][3]={{0,-phi,1},{0,-phi,-1},{0,phi,1},{0,phi,-1},{-phi,1,0},{-phi,-1,0},{phi,1,0},{phi,-1,0},{1,0,-phi},{-1,0,-phi},{1,0,phi},{-1,0,phi}}; *x=d[cite][0];*y=d[cite][1];*z=d[cite][2]; return; }
        case 13: { static const double e[60][3]={
{0.333333333333,-1.078689325833,1.206011329583},{-0.333333333333,-1.078689325833,1.206011329583},{-0.539344662917,-1.412022659167,0.666666666667},{0,-1.61803398875,0.333333333333},{0.539344662917,-1.412022659167,0.666666666667},{-0.333333333333,-1.078689325833,-1.206011329583},{0.333333333333,-1.078689325833,-1.206011329583},{0.539344662917,-1.412022659167,-0.666666666667},{0,-1.61803398875,-0.333333333333},{-0.539344662917,-1.412022659167,-0.666666666667},{0.539344662917,1.412022659167,0.666666666667},{0,1.61803398875,0.333333333333},{-0.539344662917,1.412022659167,0.666666666667},{-0.333333333333,1.078689325833,1.206011329583},{0.333333333333,1.078689325833,1.206011329583},{-0.539344662917,1.412022659167,-0.666666666667},{0,1.61803398875,-0.333333333333},{0.539344662917,1.412022659167,-0.666666666667},{0.333333333333,1.078689325833,-1.206011329583},{-0.333333333333,1.078689325833,-1.206011329583},{-1.078689325833,1.206011329583,0.333333333333},{-1.078689325833,1.206011329583,-0.333333333333},{-1.412022659167,0.666666666667,-0.539344662917},{-1.61803398875,0.333333333333,0},{-1.412022659167,0.666666666667,0.539344662917},{-1.412022659167,-0.666666666667,0.539344662917},{-1.61803398875,-0.333333333333,0},{-1.412022659167,-0.666666666667,-0.539344662917},{-1.078689325833,-1.206011329583,-0.333333333333},{-1.078689325833,-1.206011329583,0.333333333333},{1.078689325833,1.206011329583,-0.333333333333},{1.078689325833,1.206011329583,0.333333333333},{1.412022659167,0.666666666667,0.539344662917},{1.61803398875,0.333333333333,0},{1.412022659167,0.666666666667,-0.539344662917},{1.412022659167,-0.666666666667,-0.539344662917},{1.61803398875,-0.333333333333,0},{1.412022659167,-0.666666666667,0.539344662917},{1.078689325833,-1.206011329583,0.333333333333},{1.078689325833,-1.206011329583,-0.333333333333},{0.333333333333,0,-1.61803398875},{0.666666666667,0.539344662917,-1.412022659167},{1.206011329583,0.333333333333,-1.078689325833},{1.206011329583,-0.333333333333,-1.078689325833},{0.666666666667,-0.539344662917,-1.412022659167},{-1.206011329583,0.333333333333,-1.078689325833},{-0.666666666667,0.539344662917,-1.412022659167},{-0.333333333333,0,-1.61803398875},{-0.666666666667,-0.539344662917,-1.412022659167},{-1.206011329583,-0.333333333333,-1.078689325833},{1.206011329583,0.333333333333,1.078689325833},{0.666666666667,0.539344662917,1.412022659167},{0.333333333333,0,1.61803398875},{0.666666666667,-0.539344662917,1.412022659167},{1.206011329583,-0.333333333333,1.078689325833},{-0.666666666667,0.539344662917,1.412022659167},{-1.206011329583,0.333333333333,1.078689325833},{-1.206011329583,-0.333333333333,1.078689325833},{-0.666666666667,-0.539344662917,1.412022659167},{-0.333333333333,0,1.61803398875}}; *x=e[cite][0];*y=e[cite][1];*z=e[cite][2]; return; }
        case 14: { static const double d[32][3]={
{0,-0.850650808352,0.525731112119},{0,-0.356822089773,0.934172358963},{0.57735026919,-0.57735026919,0.57735026919},{-0.57735026919,-0.57735026919,0.57735026919},{-0.356822089773,-0.934172358963,0},{0.356822089773,-0.934172358963,0},{0,-0.850650808352,-0.525731112119},{0,-0.356822089773,-0.934172358963},{-0.57735026919,-0.57735026919,-0.57735026919},{0.57735026919,-0.57735026919,-0.57735026919},{0,0.850650808352,0.525731112119},{0.356822089773,0.934172358963,0},{0.57735026919,0.57735026919,0.57735026919},{-0.356822089773,0.934172358963,0},{-0.57735026919,0.57735026919,0.57735026919},{0,0.356822089773,0.934172358963},{0,0.850650808352,-0.525731112119},{-0.57735026919,0.57735026919,-0.57735026919},{0.57735026919,0.57735026919,-0.57735026919},{0,0.356822089773,-0.934172358963},{-0.850650808352,0.525731112119,0},{-0.934172358963,0,-0.356822089773},{-0.934172358963,0,0.356822089773},{-0.850650808352,-0.525731112119,0},{0.850650808352,0.525731112119,0},{0.934172358963,0,0.356822089773},{0.934172358963,0,-0.356822089773},{0.850650808352,-0.525731112119,0},{0.525731112119,0,-0.850650808352},{-0.525731112119,0,-0.850650808352},{0.525731112119,0,0.850650808352},{-0.525731112119,0,0.850650808352}}; *x=d[cite][0];*y=d[cite][1];*z=d[cite][2]; return; }
        case 15: { const double d[20][3]={{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1},{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1},{0,-psi,-phi},{0,psi,-phi},{0,-psi,phi},{0,psi,phi},{-psi,-phi,0},{-psi,phi,0},{psi,-phi,0},{psi,phi,0},{-phi,0,-psi},{-phi,0,psi},{phi,0,-psi},{phi,0,psi}}; *x=d[cite][0];*y=d[cite][1];*z=d[cite][2]; return; }
        case 16: { static const double d[60][3]={
{-1.206011329583,-0.666666666667,-0.87267799625},{-0.666666666667,-0.87267799625,-1.206011329583},{-0.87267799625,-1.206011329583,-0.666666666667},{0.666666666667,-0.87267799625,-1.206011329583},{1.206011329583,-0.666666666667,-0.87267799625},{0.87267799625,-1.206011329583,-0.666666666667},{0.87267799625,1.206011329583,-0.666666666667},{1.206011329583,0.666666666667,-0.87267799625},{0.666666666667,0.87267799625,-1.206011329583},{-0.87267799625,1.206011329583,-0.666666666667},{-0.666666666667,0.87267799625,-1.206011329583},{-1.206011329583,0.666666666667,-0.87267799625},{-0.666666666667,-0.87267799625,1.206011329583},{-1.206011329583,-0.666666666667,0.87267799625},{-0.87267799625,-1.206011329583,0.666666666667},{1.206011329583,-0.666666666667,0.87267799625},{0.666666666667,-0.87267799625,1.206011329583},{0.87267799625,-1.206011329583,0.666666666667},{0.87267799625,1.206011329583,0.666666666667},{0.666666666667,0.87267799625,1.206011329583},{1.206011329583,0.666666666667,0.87267799625},{-0.87267799625,1.206011329583,0.666666666667},{-1.206011329583,0.666666666667,0.87267799625},{-0.666666666667,0.87267799625,1.206011329583},{0,-0.206011329583,-1.61803398875},{0.333333333333,-0.7453559925,-1.412022659167},{-0.333333333333,-0.7453559925,-1.412022659167},{-0.333333333333,0.7453559925,-1.412022659167},{0.333333333333,0.7453559925,-1.412022659167},{0,0.206011329583,-1.61803398875},{0,-0.206011329583,1.61803398875},{-0.333333333333,-0.7453559925,1.412022659167},{0.333333333333,-0.7453559925,1.412022659167},{0.333333333333,0.7453559925,1.412022659167},{-0.333333333333,0.7453559925,1.412022659167},{0,0.206011329583,1.61803398875},{-0.206011329583,-1.61803398875,0},{-0.7453559925,-1.412022659167,0.333333333333},{-0.7453559925,-1.412022659167,-0.333333333333},{-0.206011329583,1.61803398875,0},{-0.7453559925,1.412022659167,-0.333333333333},{-0.7453559925,1.412022659167,0.333333333333},{0.7453559925,-1.412022659167,-0.333333333333},{0.7453559925,-1.412022659167,0.333333333333},{0.206011329583,-1.61803398875,0},{0.7453559925,1.412022659167,0.333333333333},{0.7453559925,1.412022659167,-0.333333333333},{0.206011329583,1.61803398875,0},{-1.412022659167,0.333333333333,-0.7453559925},{-1.412022659167,-0.333333333333,-0.7453559925},{-1.61803398875,0,-0.206011329583},{-1.412022659167,0.333333333333,0.7453559925},{-1.61803398875,0,0.206011329583},{-1.412022659167,-0.333333333333,0.7453559925},{1.412022659167,0.333333333333,-0.7453559925},{1.61803398875,0,-0.206011329583},{1.412022659167,-0.333333333333,-0.7453559925},{1.412022659167,0.333333333333,0.7453559925},{1.412022659167,-0.333333333333,0.7453559925},{1.61803398875,0,0.206011329583}}; *x=d[cite][0];*y=d[cite][1];*z=d[cite][2]; return; }
        case 17: { static const double d[32][3]={
{-0.57735026919,-0.57735026919,-0.57735026919},{-0.525731112119,0,-0.850650808352},{-0.850650808352,-0.525731112119,0},{0,-0.850650808352,-0.525731112119},{0.57735026919,-0.57735026919,-0.57735026919},{0.525731112119,0,-0.850650808352},{0.850650808352,-0.525731112119,0},{0.57735026919,0.57735026919,-0.57735026919},{0.850650808352,0.525731112119,0},{0,0.850650808352,-0.525731112119},{-0.57735026919,0.57735026919,-0.57735026919},{-0.850650808352,0.525731112119,0},{-0.57735026919,-0.57735026919,0.57735026919},{-0.525731112119,0,0.850650808352},{0,-0.850650808352,0.525731112119},{0.57735026919,-0.57735026919,0.57735026919},{0.525731112119,0,0.850650808352},{0.57735026919,0.57735026919,0.57735026919},{0,0.850650808352,0.525731112119},{-0.57735026919,0.57735026919,0.57735026919},{0,-0.356822089773,-0.934172358963},{0,0.356822089773,-0.934172358963},{0,-0.356822089773,0.934172358963},{0,0.356822089773,0.934172358963},{-0.356822089773,-0.934172358963,0},{-0.356822089773,0.934172358963,0},{0.356822089773,-0.934172358963,0},{0.356822089773,0.934172358963,0},{-0.934172358963,0,-0.356822089773},{-0.934172358963,0,0.356822089773},{0.934172358963,0,-0.356822089773},{0.934172358963,0,0.356822089773}}; *x=d[cite][0];*y=d[cite][1];*z=d[cite][2]; return; }
        case 18: { static const double d[30][3]={
{0,-1.61803398875,0},{-0.809016994375,-1.309016994375,0.5},{0.809016994375,-1.309016994375,0.5},{0.5,-0.809016994375,1.309016994375},{-0.5,-0.809016994375,1.309016994375},{-0.809016994375,-1.309016994375,-0.5},{0.809016994375,-1.309016994375,-0.5},{0.5,-0.809016994375,-1.309016994375},{-0.5,-0.809016994375,-1.309016994375},{0,1.61803398875,0},{-0.809016994375,1.309016994375,0.5},{0.809016994375,1.309016994375,0.5},{0.5,0.809016994375,1.309016994375},{-0.5,0.809016994375,1.309016994375},{-0.809016994375,1.309016994375,-0.5},{0.809016994375,1.309016994375,-0.5},{0.5,0.809016994375,-1.309016994375},{-0.5,0.809016994375,-1.309016994375},{-1.61803398875,0,0},{-1.309016994375,0.5,-0.809016994375},{-1.309016994375,0.5,0.809016994375},{-1.309016994375,-0.5,-0.809016994375},{-1.309016994375,-0.5,0.809016994375},{1.61803398875,0,0},{1.309016994375,0.5,-0.809016994375},{1.309016994375,0.5,0.809016994375},{1.309016994375,-0.5,-0.809016994375},{1.309016994375,-0.5,0.809016994375},{0,0,-1.61803398875},{0,0,1.61803398875}}; *x=d[cite][0];*y=d[cite][1];*z=d[cite][2]; return; }
        case 19: { static const double d[120][3]={
{0.269672331458,-1.515028323958,-0.166666666667},{0.269672331458,-1.515028323958,0.166666666667},{-0.269672331458,-1.515028323958,0.166666666667},{-0.269672331458,-1.515028323958,-0.166666666667},{-0.706011329583,-1.142350327708,0.769672331458},{-0.975683661042,-1.039344662917,0.603005664792},{-0.809016994375,-1.309016994375,0.166666666667},{-0.539344662917,-1.412022659167,0.333333333333},{0.975683661042,-1.039344662917,0.603005664792},{0.706011329583,-1.142350327708,0.769672331458},{0.539344662917,-1.412022659167,0.333333333333},{0.809016994375,-1.309016994375,0.166666666667},{0.769672331458,-0.706011329583,1.142350327708},{0.333333333333,-0.539344662917,1.412022659167},{0.166666666667,-0.809016994375,1.309016994375},{0.603005664792,-0.975683661042,1.039344662917},{-0.333333333333,-0.539344662917,1.412022659167},{-0.769672331458,-0.706011329583,1.142350327708},{-0.603005664792,-0.975683661042,1.039344662917},{-0.166666666667,-0.809016994375,1.309016994375},{-0.975683661042,-1.039344662917,-0.603005664792},{-0.706011329583,-1.142350327708,-0.769672331458},{-0.539344662917,-1.412022659167,-0.333333333333},{-0.809016994375,-1.309016994375,-0.166666666667},{0.706011329583,-1.142350327708,-0.769672331458},{0.975683661042,-1.039344662917,-0.603005664792},{0.809016994375,-1.309016994375,-0.166666666667},{0.539344662917,-1.412022659167,-0.333333333333},{0.333333333333,-0.539344662917,-1.412022659167},{0.769672331458,-0.706011329583,-1.142350327708},{0.603005664792,-0.975683661042,-1.039344662917},{0.166666666667,-0.809016994375,-1.309016994375},{-0.769672331458,-0.706011329583,-1.142350327708},{-0.333333333333,-0.539344662917,-1.412022659167},{-0.166666666667,-0.809016994375,-1.309016994375},{-0.603005664792,-0.975683661042,-1.039344662917},{0.269672331458,1.515028323958,0.166666666667},{0.269672331458,1.515028323958,-0.166666666667},{-0.269672331458,1.515028323958,-0.166666666667},{-0.269672331458,1.515028323958,0.166666666667},{-0.539344662917,1.412022659167,0.333333333333},{-0.809016994375,1.309016994375,0.166666666667},{-0.975683661042,1.039344662917,0.603005664792},{-0.706011329583,1.142350327708,0.769672331458},{0.809016994375,1.309016994375,0.166666666667},{0.539344662917,1.412022659167,0.333333333333},{0.706011329583,1.142350327708,0.769672331458},{0.975683661042,1.039344662917,0.603005664792},{0.603005664792,0.975683661042,1.039344662917},{0.166666666667,0.809016994375,1.309016994375},{0.333333333333,0.539344662917,1.412022659167},{0.769672331458,0.706011329583,1.142350327708},{-0.166666666667,0.809016994375,1.309016994375},{-0.603005664792,0.975683661042,1.039344662917},{-0.769672331458,0.706011329583,1.142350327708},{-0.333333333333,0.539344662917,1.412022659167},{-0.809016994375,1.309016994375,-0.166666666667},{-0.539344662917,1.412022659167,-0.333333333333},{-0.706011329583,1.142350327708,-0.769672331458},{-0.975683661042,1.039344662917,-0.603005664792},{0.539344662917,1.412022659167,-0.333333333333},{0.809016994375,1.309016994375,-0.166666666667},{0.975683661042,1.039344662917,-0.603005664792},{0.706011329583,1.142350327708,-0.769672331458},{0.166666666667,0.809016994375,-1.309016994375},{0.603005664792,0.975683661042,-1.039344662917},{0.769672331458,0.706011329583,-1.142350327708},{0.333333333333,0.539344662917,-1.412022659167},{-0.603005664792,0.975683661042,-1.039344662917},{-0.166666666667,0.809016994375,-1.309016994375},{-0.333333333333,0.539344662917,-1.412022659167},{-0.769672331458,0.706011329583,-1.142350327708},{-1.515028323958,0.166666666667,0.269672331458},{-1.515028323958,0.166666666667,-0.269672331458},{-1.515028323958,-0.166666666667,-0.269672331458},{-1.515028323958,-0.166666666667,0.269672331458},{-1.142350327708,0.769672331458,-0.706011329583},{-1.039344662917,0.603005664792,-0.975683661042},{-1.309016994375,0.166666666667,-0.809016994375},{-1.412022659167,0.333333333333,-0.539344662917},{-1.039344662917,0.603005664792,0.975683661042},{-1.142350327708,0.769672331458,0.706011329583},{-1.412022659167,0.333333333333,0.539344662917},{-1.309016994375,0.166666666667,0.809016994375},{-1.412022659167,-0.333333333333,-0.539344662917},{-1.309016994375,-0.166666666667,-0.809016994375},{-1.039344662917,-0.603005664792,-0.975683661042},{-1.142350327708,-0.769672331458,-0.706011329583},{-1.309016994375,-0.166666666667,0.809016994375},{-1.412022659167,-0.333333333333,0.539344662917},{-1.142350327708,-0.769672331458,0.706011329583},{-1.039344662917,-0.603005664792,0.975683661042},{1.515028323958,0.166666666667,-0.269672331458},{1.515028323958,0.166666666667,0.269672331458},{1.515028323958,-0.166666666667,0.269672331458},{1.515028323958,-0.166666666667,-0.269672331458},{1.039344662917,0.603005664792,-0.975683661042},{1.142350327708,0.769672331458,-0.706011329583},{1.412022659167,0.333333333333,-0.539344662917},{1.309016994375,0.166666666667,-0.809016994375},{1.142350327708,0.769672331458,0.706011329583},{1.039344662917,0.603005664792,0.975683661042},{1.309016994375,0.166666666667,0.809016994375},{1.412022659167,0.333333333333,0.539344662917},{1.309016994375,-0.166666666667,-0.809016994375},{1.412022659167,-0.333333333333,-0.539344662917},{1.142350327708,-0.769672331458,-0.706011329583},{1.039344662917,-0.603005664792,-0.975683661042},{1.412022659167,-0.333333333333,0.539344662917},{1.309016994375,-0.166666666667,0.809016994375},{1.039344662917,-0.603005664792,0.975683661042},{1.142350327708,-0.769672331458,0.706011329583},{-0.166666666667,0.269672331458,-1.515028323958},{0.166666666667,0.269672331458,-1.515028323958},{0.166666666667,-0.269672331458,-1.515028323958},{-0.166666666667,-0.269672331458,-1.515028323958},{0.166666666667,0.269672331458,1.515028323958},{-0.166666666667,0.269672331458,1.515028323958},{-0.166666666667,-0.269672331458,1.515028323958},{0.166666666667,-0.269672331458,1.515028323958}}; *x=d[cite][0];*y=d[cite][1];*z=d[cite][2]; return; }
        case 20: { static const double d[62][3]={
{0,-1,0},{0.356822089773,-0.934172358963,0},{0,-0.850650808352,-0.525731112119},{0,-0.850650808352,0.525731112119},{-0.356822089773,-0.934172358963,0},{-0.5,-0.809016994375,0.309016994375},{-0.57735026919,-0.57735026919,0.57735026919},{-0.850650808352,-0.525731112119,0},{0.5,-0.809016994375,0.309016994375},{0.57735026919,-0.57735026919,0.57735026919},{0.850650808352,-0.525731112119,0},{0.309016994375,-0.5,0.809016994375},{0.525731112119,0,0.850650808352},{0,-0.356822089773,0.934172358963},{-0.309016994375,-0.5,0.809016994375},{-0.525731112119,0,0.850650808352},{-0.5,-0.809016994375,-0.309016994375},{-0.57735026919,-0.57735026919,-0.57735026919},{0.5,-0.809016994375,-0.309016994375},{0.57735026919,-0.57735026919,-0.57735026919},{0.309016994375,-0.5,-0.809016994375},{0.525731112119,0,-0.850650808352},{0,-0.356822089773,-0.934172358963},{-0.309016994375,-0.5,-0.809016994375},{-0.525731112119,0,-0.850650808352},{0,1,0},{0.356822089773,0.934172358963,0},{0,0.850650808352,0.525731112119},{0,0.850650808352,-0.525731112119},{-0.356822089773,0.934172358963,0},{-0.5,0.809016994375,0.309016994375},{-0.850650808352,0.525731112119,0},{-0.57735026919,0.57735026919,0.57735026919},{0.5,0.809016994375,0.309016994375},{0.850650808352,0.525731112119,0},{0.57735026919,0.57735026919,0.57735026919},{0.309016994375,0.5,0.809016994375},{0,0.356822089773,0.934172358963},{-0.309016994375,0.5,0.809016994375},{-0.5,0.809016994375,-0.309016994375},{-0.57735026919,0.57735026919,-0.57735026919},{0.5,0.809016994375,-0.309016994375},{0.57735026919,0.57735026919,-0.57735026919},{0.309016994375,0.5,-0.809016994375},{0,0.356822089773,-0.934172358963},{-0.309016994375,0.5,-0.809016994375},{-1,0,0},{-0.934172358963,0,0.356822089773},{-0.934172358963,0,-0.356822089773},{-0.809016994375,0.309016994375,-0.5},{-0.809016994375,0.309016994375,0.5},{-0.809016994375,-0.309016994375,-0.5},{-0.809016994375,-0.309016994375,0.5},{1,0,0},{0.934172358963,0,-0.356822089773},{0.934172358963,0,0.356822089773},{0.809016994375,0.309016994375,-0.5},{0.809016994375,0.309016994375,0.5},{0.809016994375,-0.309016994375,-0.5},{0.809016994375,-0.309016994375,0.5},{0,0,-1},{0,0,1}}; *x=d[cite][0];*y=d[cite][1];*z=d[cite][2]; return; }
    }
}

/* Citation → sphere: projection-boundary only. Delegates to resolve_vertex. */
static void cite_to_sphere(int shape_idx, int cite, double *x, double *y, double *z) {
    resolve_vertex(shape_idx, cite, x, y, z);
}

/* ─── Deterministic solid selection from computation state ─── */
typedef struct {
    int solid_idx;           /* 0-20: which shape def */
    const ShapeDef *shape;
    int highlight_vertex;    /* BQF → vertex index */
    int highlight_edge;      /* BQF → edge index */
    double rotate_x, rotate_y, rotate_z;
    double scale;
} SolidRenderState;

static const ShapeDef *solid_lookup(int fano7, int role3) {
    int base = CONFIG_MATRIX[fano7 % 7];
    int idx = base + (role3 % CONFIG_FAMILY_SIZE);
    return (idx < (int)SHAPE_DB_N) ? &SHAPE_DB[idx] : &SHAPE_DB[0];
}

static SolidRenderState resolve_solid_geometry(uint16_t xf, uint16_t sf, uint16_t rf, int fano7, int role3, int local240, uint64_t hash) {
    SolidRenderState s;
    memset(&s, 0, sizeof(s));

    s.solid_idx = (int)((xf ^ sf ^ rf) % (int)SHAPE_DB_N);
    s.shape = solid_lookup(fano7, role3);

    if (s.shape && s.shape->nverts > 0)
        s.highlight_vertex = local240 % s.shape->nverts;
    if (s.shape && s.shape->nedges > 0)
        s.highlight_edge = (local240 / 3) % s.shape->nedges;

    ChiralDir chi = tetra_chiral_pi(hash);
    double chirality = (chi == CHIRAL_PULLING) ? -1.0 : 1.0;
    s.rotate_x = chirality * (double)(xf % 360) * M_PI / 180.0;
    s.rotate_y = chirality * (double)(sf % 360) * M_PI / 180.0;
    s.rotate_z = chirality * (double)(rf % 360) * M_PI / 180.0;
    s.scale = 0.8;
    return s;
}

typedef struct {
    int chart11, baseQ, fiberQ, fano7, role3;
    uint32_t qxy;
    int local240, slot5040;
    int a, b, c;            /* thrust direction (Hopf S3→S2 projection) */
    double fiberPhase;
    double qw, qx, qy, qz;  /* quaternion candidate */
    int polybius_row, polybius_col;
    int frame_type;          /* 0=US, 1=GS, 2=RS, 3=FS */
    uint64_t cycle;
    uint16_t result;
    uint16_t xf, sf, rf;
    uint16_t opcode;
    SolidRenderState solid;  /* solid geometry */
} TwinGeometry;

static int mod_pos(int x, int m) { int r = x % m; return r < 0 ? r + m : r; }
static double clamp_d(double n, double lo, double hi) { return n < lo ? lo : (n > hi ? hi : n); }

static uint32_t fnv1a32_buf(const unsigned char *data, size_t len) {
    uint32_t h = 0x811c9dc5;
    for (size_t i = 0; i < len; i++) { h ^= data[i]; h *= 0x01000193; }
    return h;
}

static uint32_t fnv1a32_str(const char *s) {
    return fnv1a32_buf((const unsigned char *)s, strlen(s));
}

static TwinGeometry resolve_hopf_ququart_route(int chart11, int baseQ, int fiberQ, int fano7, int role3, uint64_t hash) {
    TwinGeometry g;
    memset(&g, 0, sizeof(g));
    g.chart11 = mod_pos(chart11, 11);
    g.baseQ = mod_pos(baseQ, 4);
    g.fiberQ = mod_pos(fiberQ, 4);
    g.fano7 = mod_pos(fano7, 7);
    g.role3 = mod_pos(role3, 3);
    int x = g.baseQ, y = g.fiberQ;
    g.qxy = bqf32((uint32_t)x, (uint32_t)y);
    g.local240 = (int)(g.qxy % 240);
    g.slot5040 = g.fano7 * 720 + g.role3 * 240 + g.local240;

    ChiralDir chi = tetra_chiral_pi(hash);
    double chirality = (chi == CHIRAL_PULLING) ? -1.0 : 1.0;
    double theta = ((x + 0.5) / 4.0) * M_PI;
    double phi = ((y + 0.5) / 4.0) * 2.0 * M_PI * chirality;
    double halfTheta = theta / 2.0;
    g.qw = cos(halfTheta);
    g.qx = sin(halfTheta) * cos(phi);
    g.qy = sin(halfTheta) * sin(phi);
    g.qz = 0.0;
    g.fiberPhase = phi;

    double w = g.qw, qx = g.qx, qy = g.qy, qz = g.qz;
    g.a = (int)round(2.0 * (qx * qz + w * qy));
    g.b = (int)round(2.0 * (qy * qz - w * qx));
    g.c = (int)round(1.0 - 2.0 * (qx * qx + qy * qy));

    int local16 = g.local240 & 0x0f;
    int px = local16 & 3;
    int py = (local16 >> 2) & 3;
    polybius_get_interior(px, py, &g.polybius_row, &g.polybius_col);

    g.frame_type = baseQ & 3;
    g.solid = resolve_solid_geometry((uint16_t)g.chart11, (uint16_t)g.baseQ,
        (uint16_t)g.fiberQ, g.fano7, g.role3, g.local240, hash);
    return g;
}

static int count_filled_ring_slots(void) {
    int n = 0;
    for (size_t i = 0; i < RING_SIZE; i++) { if (ring[i].hash != 0 || ring[i].receipt[0]) n++; }
    return n;
}

/* Derive twin geometry from autonomous computation state */
static TwinGeometry tetragrammatron_geometry_route(uint64_t cycle, uint16_t xf, uint16_t sf, uint16_t rf, uint16_t opcode, uint16_t result) {
    int opcode_idx = -1;
    static const uint16_t opcodes[] = {0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,0x0009,0x000d,0x000e};
    for (int i = 0; i < 11; i++) { if (opcode == opcodes[i]) { opcode_idx = i; break; } }
    int channel = (opcode_idx >= 0) ? (opcode_idx % 4) : 0;
    int baseQ = channel;

    int relationCount = count_filled_ring_slots();
    double stability = relationCount < 2 ? 0.0 : clamp_d((double)(rf ^ xf) / 65535.0, 0.0, 1.0);

    char seed_buf[256];
    snprintf(seed_buf, sizeof(seed_buf), "omi.tetragrammatron.geometry.fiber.v0|%llu|%d|%d|%d|%d",
        (unsigned long long)cycle, relationCount, (int)xf, (int)sf, (int)rf);
    uint32_t fiberSeed = fnv1a32_str(seed_buf);
    int fiberQ = (int)((fiberSeed + (unsigned)(stability * 60.0) + (unsigned)relationCount) % 4);

    snprintf(seed_buf, sizeof(seed_buf), "omi.tetragrammatron.geometry.chart.v0|%llu|%d|%d|%d",
        (unsigned long long)cycle, (unsigned)result, channel, (int)xf);
    uint32_t chartSeed = fnv1a32_str(seed_buf);
    int chart11 = (int)(chartSeed % 11);

    snprintf(seed_buf, sizeof(seed_buf), "omi.tetragrammatron.geometry.role.v0|%llu|%d|%d|%d",
        (unsigned long long)cycle, baseQ, fiberQ, chart11);
    uint32_t roleSeed = fnv1a32_str(seed_buf);
    int role3 = (int)(roleSeed % 3);
    int fano7 = (int)((roleSeed + (unsigned)relationCount) % 7);

    TwinGeometry g = resolve_hopf_ququart_route(chart11, baseQ, fiberQ, fano7, role3, ring[cycle % RING_SIZE].hash);
    g.cycle = cycle;
    g.result = result;
    g.xf = xf; g.sf = sf; g.rf = rf;
    g.opcode = opcode;
    return g;
}

/* ─── Smith Chart projection: maps 5040-slot ring to reflection coefficient Γ ─── */
typedef struct {
    double gr, gi;   /* gamma real, imag */
    double zr, zi;   /* normalized impedance real, imag */
    double yr, yi;   /* normalized admittance real, imag */
    double rho, theta;
} SmithState;

static SmithState resolve_smith(uint32_t slot5040, uint64_t hash) {
    SmithState s;
    memset(&s, 0, sizeof(s));
    s.rho = (double)(hash & 0xFFFF) / 65535.0;
    s.theta = 2.0 * M_PI * (double)(slot5040 % 5040) / 5040.0;
    double gr = s.rho * cos(s.theta);
    double gi = s.rho * sin(s.theta);
    double denom = (1.0 - gr) * (1.0 - gr) + gi * gi;
    if (denom < 1e-12) denom = 1e-12;
    double zr = ((1.0 + gr) * (1.0 - gr) + gi * gi) / denom;
    double zi = (2.0 * gi) / denom;
    double zmag2 = zr * zr + zi * zi;
    if (zmag2 < 1e-12) zmag2 = 1e-12;
    s.gr = gr; s.gi = gi;
    s.zr = zr; s.zi = zi;
    s.yr = zr / zmag2; s.yi = -zi / zmag2;
    return s;
}

static void print_twin_geometry(const TwinGeometry *g) {
    printf("TWIN cyc=%llu res=0x%04x op=0x%04x",
        (unsigned long long)g->cycle, g->result, g->opcode);
    printf(" frame=%d chart11=%d baseQ=%d fiberQ=%d fano7=%d role3=%d",
        g->frame_type, g->chart11, g->baseQ, g->fiberQ, g->fano7, g->role3);
    printf(" bqf=%u local240=%d slot5040=%d",
        (unsigned)g->qxy, g->local240, g->slot5040);
    printf(" hopf=(%d,%d,%d) phase=%.4f", g->a, g->b, g->c, g->fiberPhase);
    printf(" polybius=(%d,%d)", g->polybius_row, g->polybius_col);
    printf(" quat=(%.4f,%.4f,%.4f,%.4f)", g->qw, g->qx, g->qy, g->qz);
    if (g->solid.shape)
        printf(" solid=%s[%d] v=%d e=%d",
            g->solid.shape->name, g->solid.solid_idx,
            g->solid.highlight_vertex, g->solid.highlight_edge);
    printf("\n");
}

/* ─── Digital Twin Renderers ─── */

static void render_frame_json(void) {
    uint16_t xf = ring_xor_fold(), sf = ring_sum_fold(), rf = ring_rot_fold();
    int filled = count_filled_ring_slots();
    int frame_counts[4] = {0};
    int hopf_counts[3] = {0};

    printf("{\"twin\":{\"ring\":{\"size\":%u,\"filled\":%d,"
           "\"xor\":\"0x%04x\",\"sum\":\"0x%04x\",\"rot\":\"0x%04x\"},"
           "\"frames\":[",
        (unsigned)RING_SIZE, filled, xf, sf, rf);
    for (int f = 0; f < 4; f++) {
        if (f) printf(",");
        const char *names[] = {"US","GS","RS","FS"};
        printf("{\"id\":%d,\"name\":\"%s\",\"count\":0}", f, names[f]);
    }

    printf("],\"receipts\":[");
    int first = 1;
    for (size_t i = 0; i < RING_SIZE; i++) {
        if (!ring[i].hash && !ring[i].receipt[0]) continue;
        TwinGeometry g = resolve_hopf_ququart_route(
            (int)(ring[i].cycle % 11),
            (int)(ring[i].hash % 4),
            (int)((ring[i].hash >> 8) % 4),
            (int)((ring[i].cycle + ring[i].hash) % 7),
            (int)(ring[i].hash % 3),
            ring[i].hash);
        if (g.frame_type >= 0 && g.frame_type < 4) {
            frame_counts[g.frame_type]++;
        }
        hopf_counts[0] += g.a;
        hopf_counts[1] += g.b;
        hopf_counts[2] += g.c;

        if (!first) printf(",");
        first = 0;
        const char *fnames[] = {"US","GS","RS","FS"};
        const char *sname = g.solid.shape ? g.solid.shape->name : "none";
        SmithState sm = resolve_smith((uint32_t)g.slot5040, ring[i].hash);
        printf("{\"cy\":%llu,\"h\":\"0x%016llx\","
               "\"twin\":{\"chart\":%d,\"base\":%d,\"fiber\":%d,"
               "\"fano\":%d,\"role\":%d,\"bqf\":%u,\"local\":%d,"
               "\"slot\":%d,\"hopf\":[%d,%d,%d],\"phase\":%.4f,"
               "\"cell\":[%d,%d],\"quat\":[%.4f,%.4f,%.4f,%.4f],"
               "\"frame\":\"%s\","
               "\"solid\":{\"id\":%d,\"name\":\"%s\","
               "\"verts\":%d,\"edges\":%d,\"faces\":%d,"
               "\"hv\":%d,\"he\":%d},"
               "\"smith\":{\"gamma\":[%.4f,%.4f],"
               "\"z\":[%.4f,%.4f],\"y\":[%.4f,%.4f],"
               "\"rho\":%.4f,\"theta\":%.4f}}}",
            (unsigned long long)ring[i].cycle,
            (unsigned long long)ring[i].hash,
            g.chart11, g.baseQ, g.fiberQ,
            g.fano7, g.role3, (unsigned)g.qxy, g.local240,
            g.slot5040, g.a, g.b, g.c, g.fiberPhase,
            g.polybius_row, g.polybius_col,
            g.qw, g.qx, g.qy, g.qz,
            fnames[g.frame_type & 3],
            g.solid.solid_idx, sname,
            g.solid.shape ? g.solid.shape->nverts : 0,
            g.solid.shape ? g.solid.shape->nedges : 0,
            g.solid.shape ? g.solid.shape->nfaces : 0,
            g.solid.highlight_vertex, g.solid.highlight_edge,
            sm.gr, sm.gi, sm.zr, sm.zi, sm.yr, sm.yi, sm.rho, sm.theta);
    }

    printf("],\"summary\":{\"frame_counts\":[");
    for (int f = 0; f < 4; f++) {
        if (f) printf(",");
        const char *names[] = {"US","GS","RS","FS"};
        printf("{\"name\":\"%s\",\"count\":%d}", names[f], frame_counts[f]);
    }
    printf("],\"hopf_flux\":[%d,%d,%d],\"filled\":%d}}}\n",
        hopf_counts[0], hopf_counts[1], hopf_counts[2], filled);
}

/* ─── Smith Chart SVG Renderer (--smith) ─── */
static void render_smith_svg(void) {
    int filled = count_filled_ring_slots();
    uint16_t xf = ring_xor_fold(), sf = ring_sum_fold(), rf = ring_rot_fold();
    const double S = 300.0;
    double rs[] = {0, 0.5, 1, 2, 5};
    int frame_counts[4] = {0};
    const char *fcolors[] = {"#ff4444","#44ff44","#4444ff","#44ffff"};
    const char *fnames[] = {"US","GS","RS","FS"};

    printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    printf("<svg viewBox=\"-400 -400 800 800\" xmlns=\"http://www.w3.org/2000/svg\">\n");
    printf("<defs><clipPath id=\"uc\"><circle cx=\"0\" cy=\"0\" r=\"%.0f\"/></clipPath></defs>\n", S);
    printf("<rect x=\"-400\" y=\"-400\" width=\"800\" height=\"800\" fill=\"#0a0a14\"/>\n");
    printf("<circle cx=\"0\" cy=\"0\" r=\"%.0f\" fill=\"none\" stroke=\"#444466\" stroke-width=\"2\"/>\n", S);
    printf("<g clip-path=\"url(#uc)\" stroke=\"#333355\" stroke-width=\"0.5\" fill=\"none\">\n");
    for (int i = 1; i < 5; i++) {
        double cx = rs[i] / (1.0 + rs[i]);
        double r = 1.0 / (1.0 + rs[i]);
        printf("<circle cx=\"%.3f\" cy=\"0\" r=\"%.3f\"/>\n", cx * S, r * S);
    }
    double xs[] = {0.5, 1, 2, 5};
    for (int i = 0; i < 4; i++) {
        double cy = 1.0 / xs[i];
        double cr = fabs(cy);
        printf("<circle cx=\"%.3f\" cy=\"%.3f\" r=\"%.3f\"/>\n", S, cy * S, cr * S);
        printf("<circle cx=\"%.3f\" cy=\"%.3f\" r=\"%.3f\"/>\n", S, -cy * S, cr * S);
    }
    printf("</g>\n");
    printf("<circle cx=\"0\" cy=\"0\" r=\"4\" fill=\"#ffffff\"/>\n");
    printf("<line x1=\"-6\" y1=\"0\" x2=\"6\" y2=\"0\" stroke=\"#ffffff\" stroke-width=\"1\"/>\n");
    printf("<line x1=\"0\" y1=\"-6\" x2=\"0\" y2=\"6\" stroke=\"#ffffff\" stroke-width=\"1\"/>\n");

    for (size_t i = 0; i < RING_SIZE; i++) {
        if (!ring[i].hash && !ring[i].receipt[0]) continue;
        TwinGeometry g = resolve_hopf_ququart_route(
            (int)(ring[i].cycle % 11), (int)(ring[i].hash % 4),
            (int)((ring[i].hash >> 8) % 4),
            (int)((ring[i].cycle + ring[i].hash) % 7),
            (int)(ring[i].hash % 3),
            ring[i].hash);
        SmithState sm = resolve_smith((uint32_t)g.slot5040, ring[i].hash);
        double px = sm.gr * S;
        double py = -sm.gi * S;
        int fi = g.frame_type & 3;
        if (fi < 4) frame_counts[fi]++;
        printf("<circle cx=\"%.3f\" cy=\"%.3f\" r=\"2.5\" fill=\"%s\" opacity=\"0.7\"/>\n",
            px, py, fcolors[fi]);
    }

    printf("<g transform=\"translate(-380, 340)\">\n");
    for (int f = 0; f < 4; f++) {
        printf("<rect x=\"0\" y=\"%d\" width=\"10\" height=\"10\" fill=\"%s\"/>\n", f*20, fcolors[f]);
        printf("<text x=\"16\" y=\"%d\" fill=\"#aaaacc\" font-size=\"12\" font-family=\"monospace\">%s: %d</text>\n", f*20+9, fnames[f], frame_counts[f]);
    }
    printf("</g>\n");
    printf("<text x=\"0\" y=\"-360\" fill=\"#aaaacc\" font-size=\"16\" font-family=\"monospace\" text-anchor=\"middle\">OMI Smith Chart — %d receipts  xor=0x%04x  sum=0x%04x  rot=0x%04x</text>\n", filled, xf, sf, rf);
    printf("</svg>\n");
}

static void render_ppm(void) {
    int W = 420, H = 480;
    unsigned char *fb = (unsigned char *)calloc((size_t)W * H * 3, 1);
    if (!fb) { fprintf(stderr, "ppm: alloc fail\n"); return; }

#define PPM_PTR(r,c) (&fb[((size_t)(r)*(size_t)W+(size_t)(c))*3])
#define PPM_R(r,c) (*PPM_PTR(r,c))
#define PPM_G(r,c) (*(PPM_PTR(r,c)+1))
#define PPM_B(r,c) (*(PPM_PTR(r,c)+2))

    /* background */
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            { PPM_R(y,x) = 15; PPM_G(y,x) = 15; PPM_B(y,x) = 20; }

    /* Polybius 5x5 grid: 5 cells of 50px + 6 gaps of 5px = 280px */
    int grid_x = 70, grid_y = 30, cell = 50, gap = 5, grid_px = cell * 5 + gap * 6;

    /* Draw grid lines */
    for (int i = 0; i <= 5; i++) {
        int pos = i * (cell + gap);
        for (int d = 0; d < gap; d++) {
            int px = grid_x + pos + d;
            for (int y = grid_y; y < grid_y + grid_px; y++)
                if (px < W && y < H) { PPM_R(y,px) = 40; PPM_G(y,px) = 40; PPM_B(y,px) = 50; }
            int py = grid_y + pos + d;
            for (int x = grid_x; x < grid_x + grid_px; x++)
                if (x < W && py < H) { PPM_R(py,x) = 40; PPM_G(py,x) = 40; PPM_B(py,x) = 50; }
        }
    }

    /* Fill cells from ring receipts */
    for (size_t i = 0; i < RING_SIZE; i++) {
        if (!ring[i].hash && !ring[i].receipt[0]) continue;
        TwinGeometry g = resolve_hopf_ququart_route(
            (int)(ring[i].cycle % 11),
            (int)(ring[i].hash % 4),
            (int)((ring[i].hash >> 8) % 4),
            (int)((ring[i].cycle + ring[i].hash) % 7),
            (int)(ring[i].hash % 3),
            ring[i].hash);
        int col = g.polybius_col; /* 2..5 */
        int row = g.polybius_row;
        if (row < 1 || row > 5 || col < 1 || col > 5) continue;

        int cx = grid_x + (col - 1) * (cell + gap);
        int cy = grid_y + (row - 1) * (cell + gap);
        unsigned char r, gv, b;
        switch (g.frame_type) {
            case 0: r = 200; gv = 60;  b = 60;  break; /* US = red */
            case 1: r = 60;  gv = 200; b = 60;  break; /* GS = green */
            case 2: r = 60;  gv = 60;  b = 200; break; /* RS = blue */
            case 3: r = 60;  gv = 200; b = 200; break; /* FS = cyan */
            default: r = 100; gv = 100; b = 100;
        }
        for (int dy = 2; dy < cell - 2; dy++)
            for (int dx = 2; dx < cell - 2; dx++) {
                int px = cx + dx, py = cy + dy;
                if (px < W && py < H) {
                    unsigned char *p = fb + ((size_t)py * W + (size_t)px) * 3;
                    /* blend */
                    p[0] = (unsigned char)(((int)p[0] + (int)r) / 2);
                    p[1] = (unsigned char)(((int)p[1] + (int)gv) / 2);
                    p[2] = (unsigned char)(((int)p[2] + (int)b) / 2);
                }
            }
    }

    /* Mark origin (1,1) in white */
    int ox = grid_x + 0 * (cell + gap);
    int oy = grid_y + 0 * (cell + gap);
    for (int dy = 0; dy < cell; dy++)
        for (int dx = 0; dx < cell; dx++) {
            int px = ox + dx, py = oy + dy;
            if (px < W && py < H) { PPM_R(py,px) = 220; PPM_G(py,px) = 220; PPM_B(py,px) = 200; }
        }

    /* Ring occupancy bar at bottom */
    int bar_y = 380, bar_h = 20;
    for (size_t i = 0; i < RING_SIZE; i++) {
        int bx = (int)((size_t)i * (W - 20) / RING_SIZE) + 10;
        int filled = (ring[i].hash != 0 || ring[i].receipt[0]);
        for (int dy = 0; dy < bar_h; dy++) {
            int py = bar_y + dy;
            if (py < H && bx < W) {
                if (filled) { PPM_R(py,bx) = 100; PPM_G(py,bx) = 180; PPM_B(py,bx) = 255; }
                else { PPM_R(py,bx) = 30; PPM_G(py,bx) = 30; PPM_B(py,bx) = 40; }
            }
        }
    }

    /* Frame type color legend */
    unsigned char colors[4][3] = {{200,60,60},{60,200,60},{60,60,200},{60,200,200}};
    int legend_y = 420;
    for (int f = 0; f < 4; f++) {
        int lx = 30 + f * 100;
        for (int dy = 0; dy < 12; dy++)
            for (int dx = 0; dx < 12; dx++) {
                int px = lx + dx, py = legend_y + dy;
                if (px < W && py < H) {
                    PPM_R(py,px) = colors[f][0];
                    PPM_G(py,px) = colors[f][1];
                    PPM_B(py,px) = colors[f][2];
                }
            }
    }

    /* Output PPM */
    printf("P6\n%d %d\n255\n", W, H);
    fwrite(fb, 1, (size_t)W * H * 3, stdout);
    free(fb);
#undef PPM_PTR
#undef PPM_R
#undef PPM_G
#undef PPM_B
}

/* ─── Helper: quaternion to 3x3 rotation matrix ─── */
static void quat_to_rot(double qw, double qx, double qy, double qz, double rot[3][3]) {
    double xx = qx*qx, yy = qy*qy, zz = qz*qz;
    double xy = qx*qy, xz = qx*qz, xw = qx*qw;
    double yz = qy*qz, yw = qy*qw, zw = qz*qw;
    rot[0][0] = 1.0 - 2.0*(yy + zz); rot[0][1] = 2.0*(xy - zw);       rot[0][2] = 2.0*(xz + yw);
    rot[1][0] = 2.0*(xy + zw);       rot[1][1] = 1.0 - 2.0*(xx + zz); rot[1][2] = 2.0*(yz - xw);
    rot[2][0] = 2.0*(xz - yw);       rot[2][1] = 2.0*(yz + xw);       rot[2][2] = 1.0 - 2.0*(xx + yy);
}

static const ShapeDef *find_solid_with_data(int fano7, int role3) {
    int base = CONFIG_MATRIX[fano7 % 7];
    int idx = base + (role3 % CONFIG_FAMILY_SIZE);
    if (idx < (int)SHAPE_DB_N && SHAPE_DB[idx].edges && SHAPE_DB[idx].nverts > 0)
        return &SHAPE_DB[idx];
    return NULL;
}

/* Draw a line between two 3D projected points in a framebuffer */
static void ppm_draw_line(unsigned char *fb, int W, int H, int x0, int y0, int x1, int y1,
                          unsigned char r, unsigned char g, unsigned char b) {
    int dx = abs(x1 - x0), dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    for (;;) {
        if (x0 >= 0 && x0 < W && y0 >= 0 && y0 < H) {
            unsigned char *p = fb + ((size_t)y0 * W + (size_t)x0) * 3;
            p[0] = r; p[1] = g; p[2] = b;
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

/* ─── OBJ renderer: outputs solid wireframe as Wavefront OBJ ─── */
static void render_obj(const TwinGeometry *g) {
    const ShapeDef *shape = g->solid.shape;
    if (!shape || !shape->edges || shape->nverts == 0) {
        printf("# no solid data\n");
        return;
    }
    int si = (int)(shape - SHAPE_DB);
    if (si < 0 || si >= (int)SHAPE_DB_N) { printf("# invalid shape\n"); return; }
    int nv = shape->nverts, ne = shape->nedges;
    double rot[3][3];
    quat_to_rot(g->qw, g->qx, g->qy, g->qz, rot);

    printf("# OBJ wireframe: %s  v=%d e=%d\n", shape->name, nv, ne);
    for (int i = 0; i < nv; i++) {
        double x, y, z;
        cite_to_sphere(si, i, &x, &y, &z);
        double rx = rot[0][0]*x + rot[0][1]*y + rot[0][2]*z;
        double ry = rot[1][0]*x + rot[1][1]*y + rot[1][2]*z;
        double rz = rot[2][0]*x + rot[2][1]*y + rot[2][2]*z;
        printf("v %.6f %.6f %.6f\n", rx, ry, rz);
    }
    for (int i = 0; i < ne; i++) {
        printf("l %d %d\n", shape->edges[i].a + 1, shape->edges[i].b + 1);
    }
}

/* ─── base64 encoder ─── */
static void b64_encode(const unsigned char *in, size_t len, char *out) {
    static const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t i;
    for (i = 0; i < len; i += 3) {
        unsigned v = (unsigned)in[i] << 16;
        if (i+1 < len) v |= (unsigned)in[i+1] << 8;
        if (i+2 < len) v |= (unsigned)in[i+2];
        *out++ = chars[(v >> 18) & 0x3f];
        *out++ = chars[(v >> 12) & 0x3f];
        *out++ = i+1 < len ? chars[(v >> 6) & 0x3f] : '=';
        *out++ = i+2 < len ? chars[v & 0x3f] : '=';
    }
    *out = 0;
}

/* ─── SHA-1 (FIPS 180-4) for WebSocket handshake ─── */
static void sha1(const unsigned char *msg, size_t len, unsigned char out[20]) {
    uint32_t h[5] = {0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476,0xC3D2E1F0};
    size_t blen = ((len + 9 + 63) / 64) * 64;
    unsigned char *buf = (unsigned char *)xmalloc(blen);
    if (!buf) return;
    memset(buf, 0, blen); memcpy(buf, msg, len);
    buf[len] = 0x80;
    uint64_t bits = (uint64_t)len * 8;
    for (int i = 0; i < 8; i++) buf[blen - 8 + i] = (unsigned char)(bits >> (56 - i * 8));
    for (size_t off = 0; off < blen; off += 64) {
        uint32_t w[80];
        for (int i = 0; i < 16; i++)
            w[i] = ((uint32_t)buf[off+i*4]<<24)|((uint32_t)buf[off+i*4+1]<<16)|((uint32_t)buf[off+i*4+2]<<8)|buf[off+i*4+3];
        for (int i = 16; i < 80; i++) {
            uint32_t t = w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16];
            w[i] = (t << 1) | (t >> 31);
        }
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
        for (int i = 0; i < 80; i++) {
            uint32_t f, k;
            if (i < 20) { f = (b & c) | ((~b) & d); k = 0x5A827999; }
            else if (i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
            else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
            else { f = b ^ c ^ d; k = 0xCA62C1D6; }
            uint32_t temp = ((a << 5) | (a >> 27)) + f + e + k + w[i];
            e = d; d = c; c = (b << 30) | (b >> 2); b = a; a = temp;
        }
        h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e;
    }
    for (int i = 0; i < 5; i++) { out[i*4] = (unsigned char)(h[i]>>24); out[i*4+1]=(unsigned char)(h[i]>>16); out[i*4+2]=(unsigned char)(h[i]>>8); out[i*4+3]=(unsigned char)h[i]; }
    free(buf);
}

/* ─── glTF 2.0 wireframe renderer ─── */
static void render_gltf(void) {
    uint16_t xf=ring_xor_fold(), sf=ring_sum_fold(), rf=ring_rot_fold();
    uint64_t rh = ring[g_cycle % RING_SIZE].hash;
    TwinGeometry tg = resolve_hopf_ququart_route(
        (int)(g_cycle%11), (int)(xf%4), (int)(rf%4),
        (int)((xf^rf)%7), (int)(sf%3), rh);
    const ShapeDef *shape = tg.solid.shape;
    if (!shape || !shape->edges || shape->nverts == 0) { printf("{}"); return; }
    int si = (int)(shape - SHAPE_DB);
    int nv = shape->nverts, ne = shape->nedges;
    double rot[3][3];
    quat_to_rot(tg.qw, tg.qx, tg.qy, tg.qz, rot);

    int vlen = nv * 12, elen = ne * 4, blen = vlen + elen;
    unsigned char *bin = (unsigned char *)xmalloc((size_t)blen);
    uint16_t *ei = (uint16_t *)(bin + vlen);

    double minv[3] = {1e9,1e9,1e9}, maxv[3] = {-1e9,-1e9,-1e9};
    for (int i = 0; i < nv; i++) {
        double x, y, z;
        resolve_vertex(si, i, &x, &y, &z);
        double rx = rot[0][0]*x + rot[0][1]*y + rot[0][2]*z;
        double ry = rot[1][0]*x + rot[1][1]*y + rot[1][2]*z;
        double rz = rot[2][0]*x + rot[2][1]*y + rot[2][2]*z;
        float fv[3] = {(float)rx, (float)ry, (float)rz};
        memcpy(bin + i*12, fv, 12);
        for (int j = 0; j < 3; j++) {
            if (fv[j] < minv[j]) minv[j] = fv[j];
            if (fv[j] > maxv[j]) maxv[j] = fv[j];
        }
    }
    for (int i = 0; i < ne; i++) {
        ei[i*2] = (uint16_t)shape->edges[i].a;
        ei[i*2+1] = (uint16_t)shape->edges[i].b;
    }

    size_t b64len = ((size_t)blen + 2) / 3 * 4 + 1;
    char *b64 = (char *)xmalloc(b64len);
    b64_encode(bin, (size_t)blen, b64);
    free(bin);

    printf("{\"asset\":{\"version\":\"2.0\",\"generator\":\"omicron\"},"
           "\"scene\":0,\"scenes\":[{\"nodes\":[0]}],"
           "\"nodes\":[{\"mesh\":0}],"
           "\"meshes\":[{\"primitives\":[{\"mode\":1,"
           "\"attributes\":{\"POSITION\":0},\"indices\":1}]}],"
           "\"accessors\":["
           "{\"bufferView\":0,\"componentType\":5126,\"count\":%d,"
           "\"type\":\"VEC3\",\"min\":[%.6f,%.6f,%.6f],\"max\":[%.6f,%.6f,%.6f]},"
           "{\"bufferView\":1,\"componentType\":5123,\"count\":%d,"
           "\"type\":\"SCALAR\",\"min\":[0],\"max\":[%d]}"
           "],"
           "\"bufferViews\":["
           "{\"buffer\":0,\"byteOffset\":0,\"byteLength\":%d},"
           "{\"buffer\":0,\"byteOffset\":%d,\"byteLength\":%d}"
           "],"
           "\"buffers\":[{\"byteLength\":%d,\"uri\":\"data:application/octet-stream;base64,%s\"}]}\n",
           nv, minv[0], minv[1], minv[2], maxv[0], maxv[1], maxv[2],
           ne*2, nv-1, vlen, vlen, elen, blen, b64);
    free(b64);
}

/* ─── Boot ROM ─── */
static const char *BOOT_ROM[] = {
    "0x0000-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x0003-0x0003-0x0003-0x0003/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x039F-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x03BF-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x5555-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x55AA-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0xAAAA-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0xAA55-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x30000020-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x7C00-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0xAA55?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x5A3C-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x001C-0x001D-0x001E-0x001F/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x00F0-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x13B0-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x0000-0x0000-0x0000-0x0010/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x0000-0x0000-0x0000-0x0011/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x0000-0x0000-0x0000-0x0012/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x0000-0x0000-0x0000-0x0020/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x0000-0x0000-0x0000-0x0021/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0x0000-0x0000-0x0000-0x0022/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
    "0xFFFF-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
};
static const int BOOT_COUNT = sizeof(BOOT_ROM)/sizeof(BOOT_ROM[0]);

/* ─── Process S-expression ─── */
typedef struct { char *receipt; char *result_canon; char *normalized; char shape[16]; size_t events; int accepted; } SxResult;
static void sx_free(SxResult *pr){free(pr->receipt);free(pr->result_canon);free(pr->normalized);memset(pr,0,sizeof(*pr));}

static SxResult process_sexpr(const char *line, uint64_t cycle) {
    SxResult pr; memset(&pr,0,sizeof(pr)); char err[256]; Compilation c; Node *result=NULL; Buffer rb={0};
    int has_omi=compile_omi(line,&c,err,sizeof(err));
    if(has_omi){pr.normalized=xstrdup(c.normalized);snprintf(pr.shape,sizeof(pr.shape),"%s",c.shape);pr.events=c.events.len;}
    if(!has_omi){Node *ast=parse_sexpr(line,err,sizeof(err));if(!ast){Buffer eb={0};bappend(&eb,"{\"e\":\"");jesc(&eb,err);bappend(&eb,"\"}");pr.receipt=generate_receipt(line,eb.data,cycle);pr.result_canon=xstrdup(eb.data?eb.data:"");bfree(&eb);return pr;}result=reduce(ast);ncanon(result,&rb);snprintf(pr.shape,sizeof(pr.shape),"sexpr");nfree(ast);}
    else{result=reduce(c.root);ncanon(result,&rb);}
    pr.accepted=1; pr.receipt=generate_receipt(line,rb.data,cycle); pr.result_canon=rb.data?xstrdup(rb.data):xstrdup(""); bfree(&rb); nfree(result); if(has_omi)cfree(&c);
    uint64_t rh=fnv1a64((const unsigned char*)pr.result_canon,strlen(pr.result_canon)); ring_store(rh,pr.receipt); return pr;
}

static void handle_signal(int sig) { (void)sig; ring_save(); g_running=0; }

/* ─── Write ring in OMIRING1 format ─── */
static int write_ring_omi(const char *path) {
    FILE *f=fopen(path,"wb"); if(!f)return 0;
    fwrite("OMIRING1",1,8,f);
    for(size_t i=0;i<8;i++){unsigned char z=0;fwrite(&z,1,1,f);}
    for(size_t i=0;i<RING_SIZE;i++){uint64_t v=ring[i].hash;fwrite(&v,8,1,f);}
    fclose(f); return 1;
}

/* ─── HTTP Server (--serve) ─── */

#define SERVE_PORT 8080
#define BUFLEN 65536

static const char *mime_type(const char *path) {
    const char *ext = strrchr(path, '.');
    if (!ext) return "application/octet-stream";
    if (strcmp(ext, ".html") == 0) return "text/html;charset=utf-8";
    if (strcmp(ext, ".js") == 0)   return "application/javascript;charset=utf-8";
    if (strcmp(ext, ".css") == 0)  return "text/css;charset=utf-8";
    if (strcmp(ext, ".json") == 0) return "application/json;charset=utf-8";
    if (strcmp(ext, ".png") == 0)  return "image/png";
    if (strcmp(ext, ".ico") == 0)  return "image/x-icon";
    return "application/octet-stream";
}

static void http_ok(int fd, const char *ctype, const char *body, size_t blen) {
    char hdr[512];
    int hlen = snprintf(hdr, sizeof(hdr),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Connection: close\r\n"
        "\r\n", ctype, blen);
    write(fd, hdr, (size_t)hlen);
    if (blen) write(fd, body, blen);
}

static void http_404(int fd) {
    const char *msg = "<h1>404 Not Found</h1>";
    http_ok(fd, "text/html;charset=utf-8", msg, strlen(msg));
}

static void serve_file(int fd, const char *path) {
    /* strip leading / */
    const char *rel = path;
    while (*rel == '/') rel++;
    if (!*rel) rel = "index.html";

    /* safety: only serve from built portal output */
    char full[512];
    snprintf(full, sizeof(full), "portal/dist/%s", rel);

    FILE *f = fopen(full, "rb");
    if (!f) { http_404(fd); return; }
    fseek(f, 0, SEEK_END);
    long flen = ftell(f);
    if (flen < 0) { fclose(f); http_404(fd); return; }
    rewind(f);

    char *buf = (char *)xmalloc((size_t)flen + 1);
    size_t nread = fread(buf, 1, (size_t)flen, f);
    fclose(f);
    buf[nread] = 0;

    http_ok(fd, mime_type(rel), buf, nread);
    free(buf);
}

static void serve_frame_json(int fd) {
    /* Capture render_frame_json output via temp file (avoids pipe deadlock) */
    static const char *tmp = "/tmp/omi_frame.json";
    int saved = dup(STDOUT_FILENO);
    int tf = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (tf < 0) { close(saved); http_404(fd); return; }
    dup2(tf, STDOUT_FILENO);
    close(tf);
    render_frame_json();
    fflush(stdout);
    dup2(saved, STDOUT_FILENO);
    close(saved);

    int rf = open(tmp, O_RDONLY);
    if (rf < 0) { http_404(fd); return; }
    off_t flen = lseek(rf, 0, SEEK_END);
    lseek(rf, 0, SEEK_SET);
    if (flen <= 0) { close(rf); http_404(fd); return; }
    char *buf = (char *)xmalloc((size_t)flen + 1);
    ssize_t n = read(rf, buf, (size_t)flen);
    close(rf);
    if (n <= 0) { free(buf); http_404(fd); return; }
    buf[n] = 0;
    http_ok(fd, "application/json;charset=utf-8", buf, (size_t)n);
    free(buf);
}

static void serve_ring_json(int fd) {
    Buffer b = {0};
    bappend(&b, "{\"ring\":[");
    int first = 1;
    for (size_t i = 0; i < RING_SIZE; i++) {
        if (!ring[i].hash && !ring[i].receipt[0]) continue;
        if (!first) bappend(&b, ",");
        first = 0;
        char tmp[256];
        snprintf(tmp, sizeof(tmp),
            "{\"slot\":%zu,\"cy\":%llu,\"h\":\"0x%016llx\"",
            i, (unsigned long long)ring[i].cycle,
            (unsigned long long)ring[i].hash);
        bappend(&b, tmp);
        /* escape receipt */
        bappend(&b, ",\"r\":\"");
        jesc(&b, ring[i].receipt);
        bappend(&b, "\"}");
    }
    bappend(&b, "]}");
    http_ok(fd, "application/json;charset=utf-8", b.data, b.len);
    bfree(&b);
}

/* ─── Stream client management (SSE + WebSocket) ─── */
#define MAX_STREAM_CLIENTS 16
typedef struct { int fd; int type; } StreamClient;  /* type 0=SSE, 1=WS */
static StreamClient stream_clients[MAX_STREAM_CLIENTS];
static int stream_client_count = 0;

static void add_stream_client(int fd, int type) {
    if (stream_client_count >= MAX_STREAM_CLIENTS) { close(fd); return; }
    stream_clients[stream_client_count].fd = fd;
    stream_clients[stream_client_count].type = type;
    stream_client_count++;
}

static void remove_stream_client(int idx) {
    close(stream_clients[idx].fd);
    stream_clients[idx] = stream_clients[--stream_client_count];
}

static void push_frame_to_clients(void) {
    if (stream_client_count == 0) return;
    static const char *tmp = "/tmp/omi_frame.json";
    int saved = dup(STDOUT_FILENO);
    int tf = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (tf < 0) { close(saved); return; }
    dup2(tf, STDOUT_FILENO); close(tf);
    render_frame_json();
    fflush(stdout);
    dup2(saved, STDOUT_FILENO); close(saved);
    int rf = open(tmp, O_RDONLY);
    if (rf < 0) return;
    off_t flen = lseek(rf, 0, SEEK_END); lseek(rf, 0, SEEK_SET);
    if (flen <= 0) { close(rf); return; }
    char *buf = (char *)xmalloc((size_t)flen + 1);
    ssize_t n = read(rf, buf, (size_t)flen); close(rf);
    if (n <= 0) { free(buf); return; }
    buf[n] = 0;
    for (int i = stream_client_count - 1; i >= 0; i--) {
        int fd = stream_clients[i].fd;
        if (stream_clients[i].type == 0) {
            char sse_buf[65536];
            int slen = snprintf(sse_buf, sizeof(sse_buf), "data: %s\n\n", buf);
            if (write(fd, sse_buf, (size_t)slen) < 0) remove_stream_client(i);
        } else {
            size_t plen = (size_t)n;
            uint8_t ws_hdr[10]; size_t hdr_len;
            if (plen < 126) {
                ws_hdr[0] = 0x81; ws_hdr[1] = (uint8_t)plen; hdr_len = 2;
            } else if (plen < 65536) {
                ws_hdr[0] = 0x81; ws_hdr[1] = 126;
                ws_hdr[2] = (uint8_t)(plen >> 8); ws_hdr[3] = (uint8_t)(plen & 0xFF); hdr_len = 4;
            } else {
                ws_hdr[0] = 0x81; ws_hdr[1] = 127; uint64_t pl64 = (uint64_t)plen;
                for (int j = 0; j < 8; j++) ws_hdr[2+j] = (uint8_t)(pl64 >> (56 - j*8));
                hdr_len = 10;
            }
            if (write(fd, ws_hdr, hdr_len) < 0 || write(fd, buf, plen) < 0) remove_stream_client(i);
        }
    }
    free(buf);
}

/* ─── /solid endpoint: returns vertex/edge arrays for a SHAPE_DB entry ─── */
static void serve_solid_json(int fd, const char *path) {
    int id = 0;
    const char *q = strchr(path, '=');
    if (q) id = atoi(q + 1);
    if (id < 0 || id >= (int)SHAPE_DB_N) { http_404(fd); return; }
    const ShapeDef *s = &SHAPE_DB[id];
    Buffer b = {0}; char tmp[128];
    snprintf(tmp, sizeof(tmp), "{\"id\":%d,\"name\":\"", id);
    bappend(&b, tmp); jesc(&b, s->name);
    snprintf(tmp, sizeof(tmp), "\",\"nverts\":%d,\"nedges\":%d,\"nfaces\":%d,\"verts\":[", s->nverts, s->nedges, s->nfaces);
    bappend(&b, tmp);
    for (int i = 0; i < s->nverts; i++) {
        if (i) bappend(&b, ",");
        double x, y, z; resolve_vertex(id, i, &x, &y, &z);
        snprintf(tmp, sizeof(tmp), "%.10f,%.10f,%.10f", x, y, z);
        bappend(&b, tmp);
    }
    bappend(&b, "],\"edges\":[");
    for (int i = 0; i < s->nedges; i++) {
        if (i) bappend(&b, ",");
        snprintf(tmp, sizeof(tmp), "%u,%u", (unsigned)s->edges[i].a, (unsigned)s->edges[i].b);
        bappend(&b, tmp);
    }
    snprintf(tmp, sizeof(tmp), "],\"schlafli\":[%d,%d]}", s->schlafli_p, s->schlafli_q);
    bappend(&b, tmp);
    http_ok(fd, "application/json;charset=utf-8", b.data, b.len);
    bfree(&b);
}

/* ─── Incidence metadata tables ─── */

/* Wythoff symbols for each SHAPE_DB entry (empty string if unknown) */
static const char *INCIDENCE_WYTHOFF[21] = {
    "3 | 2 3",        /* 0 Tetrahedron */
    "3/2 | 2 3",      /* 1 Stellated Tetrahedron */
    "4 | 2 3",        /* 2 Octahedron */
    "4 | 2 3",        /* 3 Octahedron */
    "2 4 | 3",        /* 4 Truncated Octahedron */
    "4 | 2 3",        /* 5 Tetrakis Hexahedron */
    "3 | 2 4",        /* 6 Cube */
    "2 3 | 4",        /* 7 Truncated Cube */
    "4 | 2 3",        /* 8 Triakis Octahedron */
    "2 | 3 4",        /* 9 Cuboctahedron */
    "2 | 3 4",        /* 10 Cuboctahedron */
    "2 | 3 4",        /* 11 Rhombic Dodecahedron */
    "5 | 2 3",        /* 12 Icosahedron */
    "2 5 | 3",        /* 13 Truncated Icosahedron */
    "5 | 2 3",        /* 14 Pentakis Dodecahedron */
    "3 | 2 5",        /* 15 Dodecahedron */
    "2 3 | 5",        /* 16 Truncated Dodecahedron */
    "5 | 2 3",        /* 17 Triakis Icosahedron */
    "2 | 3 5",        /* 18 Icosidodecahedron */
    "2 3 5 |",        /* 19 Truncated Icosidodecahedron */
    "2 3 | 5",        /* 20 Disdyakis Triacontahedron */
};

/* Coxeter core notation (seed → truncation → rectification → dual) */
static const char *INCIDENCE_CORE[21] = {
    "{3,3}",          /* 0 Tetrahedron */
    "*{3,3}",         /* 1 Stellated Tetrahedron (compound) */
    "{3,4}",          /* 2 Octahedron */
    "{3,4}",          /* 3 Octahedron */
    "t{3,4}",         /* 4 Truncated Octahedron */
    "k{4,3}",         /* 5 Tetrakis Hexahedron */
    "{4,3}",          /* 6 Cube */
    "t{4,3}",         /* 7 Truncated Cube */
    "k{3,4}",         /* 8 Triakis Octahedron */
    "r{3,4}",         /* 9 Cuboctahedron */
    "r{3,4}",         /* 10 Cuboctahedron */
    "j{3,4}",         /* 11 Rhombic Dodecahedron */
    "{3,5}",          /* 12 Icosahedron */
    "t{3,5}",         /* 13 Truncated Icosahedron */
    "k{5,3}",         /* 14 Pentakis Dodecahedron */
    "{5,3}",          /* 15 Dodecahedron */
    "t{5,3}",         /* 16 Truncated Dodecahedron */
    "k{3,5}",         /* 17 Triakis Icosahedron */
    "r{3,5}",         /* 18 Icosidodecahedron */
    "tr{3,5}",        /* 19 Truncated Icosidodecahedron */
    "m{3,5}",         /* 20 Disdyakis Triacontahedron */
};

/* BQF branch terms indexed by Fano root (0-6): select monomial from 60x² + 16xy + 4y² */
static const int BRANCH_TERM[7] = {4, 4, 16, 16, 60, 60, 60};
static const char *BQF_BRANCH[7] = {"4y^2","4y^2","16xy","16xy","60x^2","60x^2","60x^2"};

/* ─── /incidence endpoint: deterministic incidence data, no projection geometry ─── */
static void serve_incidence_json(int fd, int id) {
    if (id < 0 || id >= (int)SHAPE_DB_N) { http_404(fd); return; }
    const ShapeDef *s = &SHAPE_DB[id];
    Buffer b = {0}; char tmp[512];
    int chi = s->nverts - s->nedges + s->nfaces;
    int graph_b1 = s->nedges - s->nverts + 1;
    int surface_b1 = (chi == 2) ? 0 : (chi > 2 ? 0 : 2 - chi);
    const char *wyth = (id < 21) ? INCIDENCE_WYTHOFF[id] : "";
    const char *core = (id < 21) ? INCIDENCE_CORE[id] : "";
    int fr = s->fano_root;
    if (fr < 0) fr = 0; if (fr > 6) fr = 0;
    int bt = BRANCH_TERM[fr];
    const char *bqf = BQF_BRANCH[fr];
    const char *topo = (id == 1) ? "projected_compound" : "convex_sphere";
    int valid = (s->nverts > 0 && s->nedges > 0 && s->edges != NULL) ? 1 : 0;

    snprintf(tmp, sizeof(tmp),
        "{\"id\":%d,\"name\":\"", id);
    bappend(&b, tmp); jesc(&b, s->name);
    snprintf(tmp, sizeof(tmp),
        "\",\"schlafli\":[%d,%d],"
        "\"wythoff\":\"%s\",\"core\":\"%s\","
        "\"incidence_product\":\"%dx%dy\","
        "\"bqf_branch\":\"%s\",\"branch_term\":%d,"
        "\"topology_model\":\"%s\","
        "\"fano\":%d,\"role\":%d,"
        "\"verts\":%d,\"edges\":%d,\"faces\":%d,"
        "\"graph_beta\":[1,%d],"
        "\"surface_beta\":[1,%d,1],"
        "\"valid\":%s}",
        s->schlafli_p, s->schlafli_q,
        wyth, core,
        s->schlafli_p, s->schlafli_q,
        bqf, bt, topo,
        fr, s->family_seq,
        s->nverts, s->nedges, s->nfaces,
        graph_b1, surface_b1,
        valid ? "true" : "false");
    bappend(&b, tmp);
    http_ok(fd, "application/json;charset=utf-8", b.data, b.len);
    bfree(&b);
}

/* ─── /receipt endpoint: deterministic hashable receipt over incidence ─── */
static void serve_receipt_json(int fd, int id) {
    if (id < 0 || id >= (int)SHAPE_DB_N) { http_404(fd); return; }
    const ShapeDef *s = &SHAPE_DB[id];
    char tmp[512];
    const char *wyth = (id < 21) ? INCIDENCE_WYTHOFF[id] : "";
    const char *core = (id < 21) ? INCIDENCE_CORE[id] : "";
    int fr = s->fano_root; if (fr < 0) fr = 0; if (fr > 6) fr = 0;
    int bt = BRANCH_TERM[fr];
    const char *bqf = BQF_BRANCH[fr];
    const char *topo = (id == 1) ? "projected_compound" : "convex_sphere";
    int chi = s->nverts - s->nedges + s->nfaces;
    int graph_b1 = s->nedges - s->nverts + 1;
    int surface_b1 = (chi == 2) ? 0 : (chi > 2 ? 0 : 2 - chi);
    int valid = (s->nverts > 0 && s->nedges > 0 && s->edges != NULL) ? 1 : 0;

    char nesc[256]; int nlen = 0;
    for (const char *p = s->name; *p && nlen < 252; p++) {
        if (*p == '|' || *p == '\\') nesc[nlen++] = '\\';
        nesc[nlen++] = *p;
    }
    nesc[nlen] = 0;

    char inc_prod[32]; snprintf(inc_prod, sizeof(inc_prod), "%dx%dy", s->schlafli_p, s->schlafli_q);
    snprintf(tmp, sizeof(tmp),
        "incidence|%d|%s|%d|%d|%s|%s|%s|%s|%d|%s|%d|%d|%d|%d|%d|%d|%d|%d",
        id, nesc, s->schlafli_p, s->schlafli_q, wyth, core,
        inc_prod, bqf, bt, topo,
        fr, s->family_seq,
        s->nverts, s->nedges, s->nfaces,
        graph_b1, surface_b1, valid);
    uint64_t h = fnv1a64((const unsigned char *)tmp, strlen(tmp));

    Buffer b = {0};
    snprintf(tmp, sizeof(tmp),
        "{\"id\":%d,\"name\":\"", id);
    bappend(&b, tmp); jesc(&b, s->name);
    snprintf(tmp, sizeof(tmp),
        "\",\"receipt_type\":\"incidence\","
        "\"hash\":\"0x%016llx\","
        "\"valid\":%s,"
        "\"source\":\"/incidence?id=%d\"}",
        (unsigned long long)h,
        valid ? "true" : "false", id);
    bappend(&b, tmp);
    http_ok(fd, "application/json;charset=utf-8", b.data, b.len);
    bfree(&b);
}

/* ─── SSE endpoint: text/event-stream, periodic frame pushes ─── */
static void serve_sse(int fd) {
    const char *hdr = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/event-stream\r\n"
        "Cache-Control: no-cache\r\n"
        "Connection: keep-alive\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "\r\n";
    write(fd, hdr, strlen(hdr));
    add_stream_client(fd, 0);
}

/* ─── WebSocket endpoint: upgrade, then framed text pushes ─── */
static void serve_ws(int fd, const char *req) {
    const char *key_hdr = strstr(req, "Sec-WebSocket-Key:");
    if (!key_hdr) { http_404(fd); return; }
    key_hdr += 18;
    while (*key_hdr && (*key_hdr == ' ' || *key_hdr == '\t')) key_hdr++;
    char key[256]; int ki = 0;
    while (*key_hdr && *key_hdr != '\r' && *key_hdr != '\n' && ki < 255) key[ki++] = *key_hdr++;
    key[ki] = 0;
    char concat[320]; int clen = snprintf(concat, sizeof(concat), "%s258EAFA5-E914-47DA-95CA-5AB5DC11D7B5", key);
    unsigned char hash[20]; sha1((unsigned char*)concat, (size_t)clen, hash);
    char accept_b64[64]; b64_encode(hash, 20, accept_b64);
    char resp[512]; int rlen = snprintf(resp, sizeof(resp),
        "HTTP/1.1 101 Switching Protocols\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: %s\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "\r\n", accept_b64);
    write(fd, resp, (size_t)rlen);
    add_stream_client(fd, 1);
}

static void serve_http(int port) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return; }
    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons((uint16_t)port);
    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); close(s); return;
    }
    if (listen(s, 5) < 0) {
        perror("listen"); close(s); return;
    }
    fprintf(stderr, "OPENCORE v2 — HTTP serve on http://127.0.0.1:%d\n", port);
    fprintf(stderr, "  /frame   — deterministic JSON frame\n");
    fprintf(stderr, "  /ring    — full ring dump\n");
    fprintf(stderr, "  /solid   — solid geometry (e.g. /solid?id=5)\n");
    fprintf(stderr, "  /incidence — incidence data (e.g. /incidence?id=9)\n");
    fprintf(stderr, "  /receipt  — hashable receipt of incidence (e.g. /receipt?id=9)\n");
    fprintf(stderr, "  /events  — SSE frame stream\n");
    fprintf(stderr, "  /ws      — WebSocket frame stream\n");
    fprintf(stderr, "  /        — portal build (index.html)\n");
    fflush(stderr);

    int push_counter = 0;
    while (g_running) {
        fd_set rfds; FD_ZERO(&rfds); FD_SET(s, &rfds);
        int maxfd = s;
        for (int i = 0; i < stream_client_count; i++) {
            FD_SET(stream_clients[i].fd, &rfds);
            if (stream_clients[i].fd > maxfd) maxfd = stream_clients[i].fd;
        }
        struct timeval tv = {1, 0};
        int sel = select(maxfd + 1, &rfds, NULL, NULL, &tv);
        if (sel < 0) continue;

        for (int i = stream_client_count - 1; i >= 0; i--) {
            int fd = stream_clients[i].fd;
            if (FD_ISSET(fd, &rfds)) {
                char dummy[64];
                if (read(fd, dummy, sizeof(dummy)) <= 0) remove_stream_client(i);
            }
        }

        if (stream_client_count > 0) {
            push_counter++;
            if (push_counter >= 1) { push_frame_to_clients(); push_counter = 0; }
        }

        if (FD_ISSET(s, &rfds)) {
            struct sockaddr_in cli;
            socklen_t clen = sizeof(cli);
            int c = accept(s, (struct sockaddr*)&cli, &clen);
            if (c < 0) continue;

            char req[BUFLEN];
            ssize_t n = read(c, req, sizeof(req) - 1);
            if (n > 0) {
                req[n] = 0;
                char method[16], path[512];
                method[0] = path[0] = 0;
                sscanf(req, "%15s %511s", method, path);
                if (strcmp(method, "GET") == 0) {
                    if (strcmp(path, "/frame") == 0) {
                        serve_frame_json(c); close(c);
                    } else if (strcmp(path, "/ring") == 0) {
                        serve_ring_json(c); close(c);
                    } else if (strcmp(path, "/events") == 0) {
                        serve_sse(c); /* keeps fd open */
                    } else if (strcmp(path, "/ws") == 0) {
                        serve_ws(c, req); /* keeps fd open */
                    } else if (strncmp(path, "/receipt", 8) == 0) {
                        const char *rq = strchr(path, '=');
                        int rid = rq ? atoi(rq + 1) : 0;
                        serve_receipt_json(c, rid); close(c);
                    } else if (strncmp(path, "/incidence", 10) == 0) {
                        const char *iq = strchr(path, '=');
                        int iid = iq ? atoi(iq + 1) : 0;
                        serve_incidence_json(c, iid); close(c);
                    } else if (strncmp(path, "/solid", 6) == 0) {
                        serve_solid_json(c, path); close(c);
                    } else {
                        serve_file(c, path); close(c);
                    }
                } else close(c);
            } else close(c);
        }
    }
    close(s);
    fprintf(stderr, "serve: stopped\n");
}

/* ─── Main ─── */
/* ─── Incidence self-check ─── */
static int check_incidence(void) {
    int ok = 1;
    int errors = 0;

    /* 1. Fano plane consistency: every point appears in exactly 3 lines */
    for (int pt = 0; pt < 7; pt++) {
        int count = 0;
        int lines[3];
        for (int li = 0; li < 7; li++)
            if (FANO_LINES[li][0] == pt || FANO_LINES[li][1] == pt || FANO_LINES[li][2] == pt)
                { if (count < 3) lines[count] = li; count++; }
        if (count != 3) {
            printf("FAIL: FANO: point %d appears in %d lines (expected 3)\n", pt, count);
            ok = 0; errors++;
        } else {
            for (int i = 0; i < 3; i++)
                if (lines[i] != FANO_PT_LINES[pt][i]) {
                    printf("FAIL: FANO: point %d line mismatch: got %d expected %d\n",
                        pt, lines[i], FANO_PT_LINES[pt][i]);
                    ok = 0; errors++;
                }
        }
    }

    /* 2. CONFIG_MATRIX / SHAPE_DB consistency */
    for (int f = 0; f < 7; f++) {
        int base = CONFIG_MATRIX[f];
        if (base < 0 || base >= (int)SHAPE_DB_N) {
            printf("FAIL: CONFIG_MATRIX[%d]=%d out of range (SHAPE_DB_N=%zu)\n", f, base, SHAPE_DB_N);
            ok = 0; errors++; continue;
        }
        /* family_seq 0 at base should have this fano_root */
        if (SHAPE_DB[base].fano_root != f) {
            printf("FAIL: CONFIG_MATRIX[%d] -> SHAPE_DB[%d].fano_root=%d (expected %d)\n",
                f, base, SHAPE_DB[base].fano_root, f);
            ok = 0; errors++;
        }
        if (SHAPE_DB[base].family_seq != 0) {
            printf("FAIL: CONFIG_MATRIX[%d] -> SHAPE_DB[%d].family_seq=%d (expected 0)\n",
                f, base, SHAPE_DB[base].family_seq);
            ok = 0; errors++;
        }
        /* All 3 family members should share fano_root */
        for (int k = 0; k < CONFIG_FAMILY_SIZE; k++) {
            int idx = base + k;
            if (idx >= (int)SHAPE_DB_N) {
                printf("FAIL: CONFIG_MATRIX[%d]+%d=%d exceeds SHAPE_DB_N\n", f, k, idx);
                ok = 0; errors++; continue;
            }
            if (SHAPE_DB[idx].fano_root != f) {
                printf("FAIL: SHAPE_DB[%d].fano_root=%d (expected %d from CONFIG_MATRIX[%d])\n",
                    idx, SHAPE_DB[idx].fano_root, f, f);
                ok = 0; errors++;
            }
            if (SHAPE_DB[idx].family_seq != k) {
                printf("FAIL: SHAPE_DB[%d].family_seq=%d (expected %d)\n",
                    idx, SHAPE_DB[idx].family_seq, k);
                ok = 0; errors++;
            }
        }
    }

    /* 3. SHAPE_DB integrity: edges, vertex ranges, no duplicates */
    for (int i = 0; i < (int)SHAPE_DB_N; i++) {
        const ShapeDef *s = &SHAPE_DB[i];
        if (s->nverts <= 0) {
            printf("FAIL: SHAPE_DB[%d] %s nverts=%d (must be >0)\n", i, s->name, s->nverts);
            ok = 0; errors++;
        }
        if (s->nedges <= 0) {
            printf("FAIL: SHAPE_DB[%d] %s nedges=%d (must be >0)\n", i, s->name, s->nedges);
            ok = 0; errors++;
        }
        if (!s->edges) {
            printf("FAIL: SHAPE_DB[%d] %s edges is NULL\n", i, s->name);
            ok = 0; errors++;
            continue;
        }
        /* Check vertex indices in range */
        int dupes = 0;
        for (int e = 0; e < s->nedges; e++) {
            int a = s->edges[e].a, b = s->edges[e].b;
            if (a < 0 || a >= s->nverts || b < 0 || b >= s->nverts) {
                printf("FAIL: SHAPE_DB[%d] %s edge[%d]=(%d,%d) vertex out of range [0,%d)\n",
                    i, s->name, e, a, b, s->nverts);
                ok = 0; errors++;
            }
        }
        /* Check for duplicate edges */
        for (int e = 0; e < s->nedges; e++)
            for (int f = e+1; f < s->nedges; f++)
                if ((s->edges[e].a == s->edges[f].a && s->edges[e].b == s->edges[f].b) ||
                    (s->edges[e].a == s->edges[f].b && s->edges[e].b == s->edges[f].a)) {
                    dupes++;
                    if (dupes <= 3)
                        printf("FAIL: SHAPE_DB[%d] %s duplicate edge: (%d,%d) at [%d] and [%d]\n",
                            i, s->name, s->edges[e].a, s->edges[e].b, e, f);
                }
        if (dupes) { ok = 0; errors += dupes; }

        /* Schläfli basic validation */
        if (s->schlafli_p < 3 || s->schlafli_q < 3) {
            printf("WARN: SHAPE_DB[%d] %s unusual Schläfli {%d,%d}\n",
                i, s->name, s->schlafli_p, s->schlafli_q);
        }
    }

    /* 4. solid_lookup / find_solid_with_data reachability */
    for (int f = 0; f < 7; f++)
        for (int r = 0; r < CONFIG_FAMILY_SIZE; r++) {
            const ShapeDef *s = solid_lookup(f, r);
            if (!s) {
                printf("FAIL: solid_lookup(%d,%d) returned NULL\n", f, r);
                ok = 0; errors++;
            }
        }

    /* 5. Edge-incidence validation: handshake lemma + vertex degree = q */
    for (int i = 0; i < (int)SHAPE_DB_N; i++) {
        const ShapeDef *s = &SHAPE_DB[i];
        if (s->nverts <= 0 || s->nedges <= 0 || !s->edges) continue;
        /* Count degree per vertex */
        int *deg = (int *)calloc((size_t)s->nverts, sizeof(int));
        if (!deg) { printf("FAIL: SHAPE_DB[%d] %s calloc failed\n", i, s->name); ok = 0; errors++; continue; }
        for (int e = 0; e < s->nedges; e++) {
            Citation a = s->edges[e].a;
            Citation b = s->edges[e].b;
            if (a < s->nverts) deg[a]++;
            if (b < s->nverts) deg[b]++;
        }
        /* Handshake lemma: sum(deg) = 2 * nedges */
        int sum = 0, min_deg = s->nverts, max_deg = 0;
        for (int v = 0; v < s->nverts; v++) {
            sum += deg[v];
            if (deg[v] < min_deg) min_deg = deg[v];
            if (deg[v] > max_deg) max_deg = deg[v];
        }
        if (sum != 2 * s->nedges) {
            printf("FAIL: SHAPE_DB[%d] %s handshake sum=%d != 2*nedges=%d\n",
                i, s->name, sum, 2 * s->nedges);
            ok = 0; errors++;
        }
        /* For polyhedra: each vertex degree should match q (Schläfli) or be irregular if truncated */
        if (s->nfaces > 0) {
            /* Stellated tetrahedron is a star compound (V-E+F=4), skip Euler check */
            int is_star = (i == 1);
            /* Euler: V - E + F = 2 (spherical) */
            if (!is_star && s->nverts - s->nedges + s->nfaces != 2) {
                printf("FAIL: SHAPE_DB[%d] %s Euler V-E+F=%d (expected 2)\n",
                    i, s->name, s->nverts - s->nedges + s->nfaces);
                ok = 0; errors++;
            }
            /* Check for isolated vertices (degree 0) — indicates bug */
            if (min_deg == 0) {
                printf("FAIL: SHAPE_DB[%d] %s vertex has degree 0 (isolated vertex)\n",
                    i, s->name);
                ok = 0; errors++;
            }
            /* Platonic: regular degree = q */
            if (min_deg == max_deg && min_deg == s->schlafli_q) {
                /* regular polyhedron, ok */
            } else if (min_deg != max_deg && min_deg > 0) {
                /* truncated/Archimedean/Catalan: verify semi-regular pattern */
                printf("INFO: SHAPE_DB[%d] %s degree range [%d,%d] q=%d (semi-regular)\n",
                    i, s->name, min_deg, max_deg, s->schlafli_q);
            }
        }
        free(deg);
    }

    printf("incidence self-check: %s (%d errors)\n", ok ? "PASS" : "FAIL", errors);
    return ok ? 0 : 1;
}

static int omicron_command_needs_boot_context(OmicronCommand c) {
    return c==OMICRON_COMMAND_BOOT||c==OMICRON_COMMAND_AUTO||c==OMICRON_COMMAND_DEFAULT;
}

static int omicron_dispatch_legacy(const OmicronConfig *cfgp, int argc, char **argv) {
    const OmicronConfig cfg = cfgp ? *cfgp : (OmicronConfig){0};

    if(argc>1){
        if(cfg.command==OMICRON_COMMAND_EVAL){
            SxResult pr=process_sexpr(cfg.command_arg,g_cycle); g_cycle++;
            printf("%s\n",pr.receipt); int ok=pr.accepted; sx_free(&pr); return ok?0:1;
        }
        if(cfg.command==OMICRON_COMMAND_REPL){
            printf("OPENCORE SEED v2\n"); char line[8192];
            while(g_running&&fgets(line,sizeof(line),stdin)){size_t llen=strlen(line);if(llen>0&&line[llen-1]=='\n')line[llen-1]=0;if(strcmp(line,":quit")==0)break;if(strcmp(line,":ring")==0){ring_dump();continue;}if(line[0]==0)continue;g_cycle++;SxResult pr=process_sexpr(line,g_cycle);if(pr.accepted)printf("ACCEPT cyc=%llu shape=%s ev=%zu res=%s\n%s\n",(unsigned long long)g_cycle,pr.shape,pr.events,pr.result_canon,pr.receipt);else printf("REJECT cyc=%llu %s\n",(unsigned long long)g_cycle,pr.receipt);sx_free(&pr);}
            ring_save(); return 0;
        }
        if(cfg.command==OMICRON_COMMAND_RING){ring_dump();return 0;}
        if(cfg.command==OMICRON_COMMAND_BOOT){
            CpuState cpu; cpu_init(&cpu);
            for(int i=0;i<BOOT_COUNT;i++){OmiInst inst;if(!parse_omi_addr(BOOT_ROM[i],&inst)){printf("boot: parse fail at %d\n",i);return 1;}cpu_run(&cpu,&inst);printf("boot[%d] opcode=0x%04x pc=%u car=0x%08x cdr=0x%08x pay=0x%08x epoch=%llu\n",i,inst.s3,cpu.pc,cpu.car_reg,cpu.cdr_reg,cpu.payload,(unsigned long long)cpu.epoch);if(cpu.halted)break;}
            printf("boot done pc=%u epoch=%llu\n",cpu.pc,(unsigned long long)cpu.epoch); return 0;
        }
        if(cfg.command==OMICRON_COMMAND_AUTO){
            printf("OPENCORE v2 — autonomous ring mode\n");
            uint64_t rounds=0; uint16_t last=0; const char *stop="running";
            CpuState cpu; cpu_init(&cpu);
            for(int i=0;i<BOOT_COUNT;i++){OmiInst inst;if(!parse_omi_addr(BOOT_ROM[i],&inst)){stop="boot-fail";break;}cpu_run(&cpu,&inst);if(cpu.halted)break;}
            printf("boot: pc=%u epoch=%llu\n",cpu.pc,(unsigned long long)cpu.epoch);
            if(strcmp(stop,"boot-fail")==0){printf("boot failed\n");ring_save();return 1;}
            stop="running"; cpu.halted=0;

            /* Pre-fill ring from CPU state */
            ring[ring_idx()].hash=cpu.payload;
            ring[ring_idx()].cycle=g_cycle++;
            ring_save();

            while(g_running){
                uint16_t xf=ring_xor_fold();
                uint16_t sf=ring_sum_fold();
                uint16_t rf=ring_rot_fold();
                uint16_t base_x=u16(xf^rotl16(sf,(uint16_t)(g_cycle&15)));
                uint16_t base_y=u16(rf^delta16(sf,xf));
                /* Fano plane routing: derive point from ring folds, select opcode via incidence */
                int fano_pt = u16(xf ^ sf) % 7;
                int fano_line = 0;
                for (int fi = 0; fi < 7; fi++) {
                    if (FANO_LINES[fi][0] == fano_pt || FANO_LINES[fi][1] == fano_pt || FANO_LINES[fi][2] == fano_pt) {
                        fano_line = fi; break;
                    }
                }
                static const uint16_t opcodes[]={0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,0x0009,0x000d,0x000e};
                uint8_t opcode_idx = (uint8_t)(((unsigned)fano_line * 3u + (unsigned)(rf % 3u)) % 11u);
                uint16_t opc=opcodes[opcode_idx];
                uint16_t x=u16(base_x^rotl16(u16(g_cycle),opc));
                uint16_t y=u16(base_y^rotr16(u16(g_cycle),opc));

                OmiInst inst; memset(&inst,0,sizeof(inst));
                inst.s0=u16(g_cycle); inst.s1=u16(rounds); inst.s2=u16(g_cycle%RING_SIZE); inst.s3=opc;
                inst.s4=x; inst.s5=y; inst.s6=u16(x^y^opc); inst.s7=delta16(x,y);
                inst.payload=((uint32_t)x<<16)|y;
                inst.mask=((uint32_t)inst.s6<<16)|inst.s7;
                inst.car=((uint32_t)inst.s0<<16)|inst.s1;
                inst.cdr=((uint32_t)inst.s2<<16)|inst.s3;

                uint16_t result=execute_omi_op(&inst);

                char line_buf[256]; snprintf(line_buf,sizeof(line_buf),"0x%04x-0x%04x-0x%04x-0x%04x/0x%04x/0x%04x/0x%04x/0x%04x?0x%08x?0x%08x@0x%08x@0x%08x",
                    inst.s0,inst.s1,inst.s2,inst.s3,inst.s4,inst.s5,inst.s6,inst.s7,inst.payload,inst.mask,inst.car,inst.cdr);

                SxResult pr=process_sexpr(line_buf,g_cycle);
                TwinGeometry twin=resolve_hopf_ququart_route(
                    (int)(g_cycle%11), (int)(xf%4), (int)(rf%4),
                    (int)((xf^rf)%7), (int)(sf%3),
                    ring[g_cycle % RING_SIZE].hash);
                twin.cycle=g_cycle; twin.result=result; twin.opcode=opc;
                twin.xf=xf; twin.sf=sf; twin.rf=rf;
                printf("AUTO cyc=%llu round=%llu op=0x%04x x=0x%04x y=0x%04x res=0x%04x xf=0x%04x sf=0x%04x rf=0x%04x\n",
                    (unsigned long long)g_cycle,(unsigned long long)rounds,opc,x,y,result,xf,sf,rf);
                printf("  twin: slot5040=%d frame=%d fano=%d role=%d cell=(%d,%d) hopf=(%d,%d,%d)\n",
                    twin.slot5040, twin.frame_type, twin.fano7, twin.role3,
                    twin.polybius_row, twin.polybius_col, twin.a, twin.b, twin.c);
                printf("  %s\n",pr.receipt);
                g_cycle++; rounds++; last=result; sx_free(&pr);

                if(result==0){stop="zero";break;}
                if(ring_has_receipt(result)){stop="repeat";break;}
                if(g_cycle%RING_SIZE==0){stop="epoch";break;}

                /* Check stdin non-blocking — physical carrier interrupt */
                fd_set rfds; FD_ZERO(&rfds); FD_SET(0,&rfds);
                struct timeval tv={0,0};
                if(select(1,&rfds,NULL,NULL,&tv)>0){
                    char buf[8192]; if(fgets(buf,sizeof(buf),stdin)){
                        size_t llen=strlen(buf); if(llen>0&&buf[llen-1]=='\n')buf[llen-1]=0;
                        if(strcmp(buf,":quit")==0)break;
                        if(strcmp(buf,":ring")==0){ring_dump();continue;}
                        if(buf[0]){SxResult sp=process_sexpr(buf,g_cycle);g_cycle++;if(sp.accepted)printf("EXT cyc=%llu %s\n%s\n",(unsigned long long)g_cycle,sp.result_canon,sp.receipt);else printf("EXT REJECT %s\n",sp.receipt);sx_free(&sp);}
                    }
                }
            }
            printf("auto stop=%s rounds=%llu cycles=%llu last=0x%04x\n",stop,(unsigned long long)rounds,(unsigned long long)g_cycle,last);
            write_ring_omi("omi.auto.ring"); ring_save(); return 0;
        }
        if(cfg.command==OMICRON_COMMAND_RUN){
            FILE *f=fopen(cfg.command_arg,"rb"); if(!f){perror("open");return 1;}
            char line[8192];
            while(g_running&&fgets(line,sizeof(line),f)){char *p=line;while(isspace((unsigned char)*p))p++;if(*p==0||*p==';')continue;line[strcspn(line,"\r\n")]=0;g_cycle++;SxResult pr=process_sexpr(p,g_cycle);if(pr.accepted)printf("ACCEPT cyc=%llu shape=%s res=%s\n%s\n",(unsigned long long)g_cycle,pr.shape,pr.result_canon,pr.receipt);else printf("REJECT cyc=%llu %s\n",(unsigned long long)g_cycle,pr.receipt);sx_free(&pr);}
            fclose(f); ring_save(); return 0;
        }
        if(cfg.command==OMICRON_COMMAND_CPU){
            OmiInst inst; if(!parse_omi_addr(cfg.command_arg,&inst)){fprintf(stderr,"parse fail\n");return 1;}
            CpuState cpu; cpu_init(&cpu); cpu_run(&cpu,&inst);
            printf("pc=%u car=0x%08x cdr=0x%08x pay=0x%08x mask=0x%08x flags=0x%08x halted=%d epoch=%llu\n",
                cpu.pc,cpu.car_reg,cpu.cdr_reg,cpu.payload,cpu.mask,cpu.flags,cpu.halted,(unsigned long long)cpu.epoch);
            return 0;
        }
        if(cfg.command==OMICRON_COMMAND_GEOM){
            printf("OPENCORE v2 — Geometry Frame\n");
            printf("Polybius 5x5: origin at (1,1)\n");
            for(int i=0;i<4;i++){int r,c;polybius_get_low_ququart(i,&r,&c);printf("  low ququart[%d]=(%d,%d)\n",i,r,c);}
            for(int i=0;i<4;i++){int r,c;polybius_get_high_ququart(i,&r,&c);printf("  high ququart[%d]=(%d,%d)\n",i,r,c);}
            printf("  interior 4x4: (2..5, 2..5)\n");
            printf("Fano plane lines:\n");
            for(int i=0;i<7;i++)printf("  line %d: {%d,%d,%d}\n",i,FANO_LINES[i][0],FANO_LINES[i][1],FANO_LINES[i][2]);
            printf("BQF(1,1)=%u BQF(2,3)=%u\n",bqf32(1,1),bqf32(2,3));
            printf("Frame constant: 0x%08x\n",OMI_FRAME);
            printf("Delta(0x5A3C)=0x%04x\n",delta16(0x5A3C,0x5A3C));
            printf("QuQuart phases: US=%02x GS=%02x RS=%02x FS=%02x\n",QUQUART_PHASE[0],QUQUART_PHASE[1],QUQUART_PHASE[2],QUQUART_PHASE[3]);
            printf("5040 slot(fano=2,role=1,local=17)=%u\n",compute_slot5040(2,1,17));
            return 0;
        }
        if(cfg.command==OMICRON_COMMAND_RENDER_FRAME){render_frame_json();return 0;}
        if(cfg.command==OMICRON_COMMAND_RENDER_PPM){render_ppm();return 0;}
        if(cfg.command==OMICRON_COMMAND_SCRIBE){
            MetatronSurfaceKind k=metatron_surface_parse(cfg.command_arg);
            MetatronScribeRecord r;
            const RingSlot *best=NULL;
            if(k==METATRON_SURFACE_UNKNOWN){fprintf(stderr,"scribe: unknown surface: %s\n",cfg.command_arg);return 2;}
            for(size_t i=0;i<RING_SIZE;i++){if(!ring[i].hash||!ring[i].receipt[0])continue;if(!best||ring[i].cycle>=best->cycle)best=&ring[i];}
            metatron_scribe_receipt(best,k,&r);
            if(!r.accepted){fprintf(stderr,"scribe: no accepted receipt\n");return 3;}
            if(!r.scribable){fprintf(stderr,"scribe: failed for surface: %s\n",cfg.command_arg);return 4;}
            printf("%s\n",r.notation); return 0;
        }
        if(cfg.command==OMICRON_COMMAND_RENDER_OBJ){
            uint16_t xf=ring_xor_fold(), sf=ring_sum_fold(), rf=ring_rot_fold();
            uint64_t rh = ring[g_cycle % RING_SIZE].hash;
            TwinGeometry g=resolve_hopf_ququart_route(
                (int)(g_cycle%11), (int)(xf%4), (int)(rf%4),
                (int)((xf^rf)%7), (int)(sf%3), rh);
            g.cycle=g_cycle; g.xf=xf; g.sf=sf; g.rf=rf;
            render_obj(&g); return 0;
        }
        if(cfg.command==OMICRON_COMMAND_RENDER_GLTF){render_gltf();return 0;}
        if(cfg.command==OMICRON_COMMAND_SMITH){render_smith_svg();return 0;}
        if(cfg.command==OMICRON_COMMAND_TWIN){
            printf("OPENCORE v2 \342\200\224 digital twin universe\n");
            uint16_t xf=ring_xor_fold(), sf=ring_sum_fold(), rf=ring_rot_fold();
            printf("ring: filled=%d xor=0x%04x sum=0x%04x rot=0x%04x\n",
                count_filled_ring_slots(), xf, sf, rf);
            int frame_counts[4]={0}; uint64_t total=0;
            for(size_t i=0;i<RING_SIZE;i++){
                if(!ring[i].hash&&!ring[i].receipt[0])continue;
                total++;
                int opcode_idx=(int)(ring[i].hash%11);
                static const uint16_t opcodes[]={0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,0x0009,0x000d,0x000e};
                uint16_t opc=(opcode_idx<11)?opcodes[opcode_idx]:0;
                uint16_t result=(uint16_t)(ring[i].hash&0xFFFF);
                TwinGeometry g=resolve_hopf_ququart_route(
                    (int)(ring[i].cycle%11),
                    (int)(ring[i].hash%4),
                    (int)((ring[i].hash>>8)%4),
                    (int)((ring[i].cycle+ring[i].hash)%7),
                    (int)(ring[i].hash%3),
                    ring[i].hash);
                g.cycle=ring[i].cycle; g.result=result; g.opcode=opc;
                g.xf=xf; g.sf=sf; g.rf=rf;
                if(g.frame_type>=0&&g.frame_type<4)frame_counts[g.frame_type]++;
                if(total<=10||ring[i].cycle==g_cycle-1)print_twin_geometry(&g);
            }
            printf("twin: total=%llu frames: US=%d GS=%d RS=%d FS=%d\n",
                (unsigned long long)total,frame_counts[0],frame_counts[1],frame_counts[2],frame_counts[3]);
            printf("twin: hopf flux=(%d,%d,%d)\n",(int)(xf%7),(int)(sf%7),(int)(rf%7));
            return 0;
        }
        if(cfg.command==OMICRON_COMMAND_SEED){
            FILE *f=fopen(cfg.command_arg,"wb"); if(!f){perror("create");return 1;}
            fwrite("OMIBIN1\0",1,8,f); uint32_t c32=(uint32_t)BOOT_COUNT; fwrite(&c32,4,1,f);
            for(int i=0;i<BOOT_COUNT;i++){OmiInst inst;parse_omi_addr(BOOT_ROM[i],&inst);fwrite(&inst,32,1,f);}
            fclose(f); printf("seed=%s instructions=%d\n",cfg.command_arg,BOOT_COUNT); return 0;
        }
        if(cfg.command==OMICRON_COMMAND_SERVE){
            int port = cfg.command_arg ? atoi(cfg.command_arg) : SERVE_PORT;
            serve_http(port); return 0;
        }
        if(cfg.command==OMICRON_COMMAND_CHECK){return check_incidence();}
        if(cfg.command==OMICRON_COMMAND_HELP){
            printf("OPENCORE v2 \342\200\224 deterministic autonomous AGI seed\n");
            printf("Usage: omicron.bin [mode] [args]\n");
            printf("Modes:\n");
            printf("  (no args)     autonomous: self-generates from ring + stdin\n");
            printf("  --eval <s>    evaluate S-expression, print receipt\n");
            printf("  --repl        interactive S-expression REPL\n");
            printf("  --auto        autonomous ring mode (self-generating)\n");
            printf("  --boot        run boot sequence (9-stage init)\n");
            printf("  --run <file>  process OMI address lines from file\n");
            printf("  --cpu <addr>  execute OMI address on nibble CPU\n");
            printf("  --geom        print geometry frame (Polybius/Fano/BQF)\n");
            printf("  --seed <path> write packed boot ROM binary\n");
            printf("  --ring        dump persistent receipt ring\n");
            printf("  --watch       monitor ring evolution live\n");
            printf("  --twin        display digital twin universe geometry\n");
            printf("  --render-frame  output twin geometry as JSON frame\n");
            printf("  --scribe <surface>  output Metatron scribe notation\n");
            printf("  --render-obj    output solid wireframe as OBJ\n");
            printf("  --render-gltf  output solid wireframe as glTF 2.0\n");
            printf("  --render-ppm    output Polybius grid as PPM image\n");
            printf("  --smith         output Smith chart as SVG\n");
            printf("  --serve [port]  HTTP server for portal build (default 8080)\n");
            printf("  --check       run incidence self-checks\n");
            printf("  --help        this message\n");
            return 0;
        }
        if(cfg.command==OMICRON_COMMAND_WATCH){
            printf("OPENCORE v2 \342\200\224 ring watcher\n");
            ring_dump();
            RingSlot *prev=malloc(sizeof(ring)); if(!prev)return 1;
            while(g_running){
                memcpy(prev,ring,sizeof(ring));
                fd_set rfds; FD_ZERO(&rfds); FD_SET(0,&rfds);
                struct timeval tv={1,0};
                if(select(1,&rfds,NULL,NULL,&tv)>0){
                    char buf[256]; if(fgets(buf,sizeof(buf),stdin)){
                        size_t llen=strlen(buf); if(llen>0&&buf[llen-1]=='\n')buf[llen-1]=0;
                        if(strcmp(buf,":quit")==0)break;
                        if(strcmp(buf,":ring")==0){ring_dump();continue;}
                        printf("unknown command: %s\n",buf);
                    }
                }
                int fd=open(RING_FILE,O_RDONLY); if(fd<0)continue;
                ssize_t rv=read(fd,ring,sizeof(ring)); close(fd);
                if(rv!=(ssize_t)sizeof(ring))continue;
                int new_count=0;
                for(size_t i=0;i<RING_SIZE;i++){
                    if(ring[i].hash!=prev[i].hash||strcmp(ring[i].receipt,prev[i].receipt)){
                        if(ring[i].hash||ring[i].receipt[0]){
                            printf("NEW[%zu] cyc=%llu h=0x%016llx %s\n",i,
                                (unsigned long long)ring[i].cycle,(unsigned long long)ring[i].hash,ring[i].receipt);
                            new_count++;
                        }
                    }
                }
                size_t filled=0; uint64_t min_cyc=UINT64_MAX,max_cyc=0;
                for(size_t i=0;i<RING_SIZE;i++){if(ring[i].hash||ring[i].receipt[0]){filled++;if(ring[i].cycle<min_cyc)min_cyc=ring[i].cycle;if(ring[i].cycle>max_cyc)max_cyc=ring[i].cycle;}}
                if(new_count||filled)printf("watch: filled=%zu min=%llu max=%llu changes=%d\n",filled,
                    (unsigned long long)(min_cyc==UINT64_MAX?0:min_cyc),(unsigned long long)max_cyc,new_count);
            }
            free(prev); return 0;
        }
        {SxResult pr=process_sexpr(argv[1],g_cycle);g_cycle++;printf("%s\n",pr.receipt);int ok=pr.accepted;sx_free(&pr);return ok?0:1;}
    }

    /* ─── Autonomous default mode: self-generate + stdin ─── */
    printf("OPENCORE v2 \342\200\224 autonomous AGI seed\n");
    printf("Self-generating from ring memory. External proposals accepted.\n");
    CpuState cpu; cpu_init(&cpu);
    for(int i=0;i<BOOT_COUNT;i++){OmiInst inst;if(parse_omi_addr(BOOT_ROM[i],&inst))cpu_run(&cpu,&inst);}
    ring[ring_idx()].hash=OMI_FRAME; ring[ring_idx()].cycle=g_cycle++;

    while(g_running){
        uint16_t xf=ring_xor_fold();
        uint16_t sf=ring_sum_fold();
        uint16_t rf=ring_rot_fold();
        uint16_t base_x=u16(xf^rotl16(sf,(uint16_t)(g_cycle&15)));
        uint16_t base_y=u16(rf^delta16(sf,xf));
        int fano_pt = u16(xf ^ sf) % 7;
        int fano_line = 0;
        for (int fi = 0; fi < 7; fi++) {
            if (FANO_LINES[fi][0] == fano_pt || FANO_LINES[fi][1] == fano_pt || FANO_LINES[fi][2] == fano_pt) {
                fano_line = fi; break;
            }
        }
        static const uint16_t opcodes[]={0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,0x0009,0x000d,0x000e};
        uint8_t opcode_idx = (uint8_t)(((unsigned)fano_line * 3u + (unsigned)(rf % 3u)) % 11u);
        uint16_t opc=opcodes[opcode_idx];
        uint16_t x=u16(base_x^rotl16(u16(g_cycle),opc));
        uint16_t y=u16(base_y^rotr16(u16(g_cycle),opc));
        OmiInst inst; memset(&inst,0,sizeof(inst));
        inst.s0=u16(g_cycle); inst.s1=0; inst.s2=u16(g_cycle%RING_SIZE); inst.s3=opc;
        inst.s4=x; inst.s5=y; inst.s6=u16(x^y^opc); inst.s7=delta16(x,y);
        inst.payload=((uint32_t)x<<16)|y; inst.mask=((uint32_t)inst.s6<<16)|inst.s7;
        inst.car=((uint32_t)inst.s0<<16)|inst.s1; inst.cdr=((uint32_t)inst.s2<<16)|inst.s3;
        uint16_t result=execute_omi_op(&inst);

        char lb[256]; snprintf(lb,sizeof(lb),"0x%04x-0x%04x-0x%04x-0x%04x/0x%04x/0x%04x/0x%04x/0x%04x?0x%08x?0x%08x@0x%08x@0x%08x",
            inst.s0,inst.s1,inst.s2,inst.s3,inst.s4,inst.s5,inst.s6,inst.s7,inst.payload,inst.mask,inst.car,inst.cdr);
        SxResult pr=process_sexpr(lb,g_cycle);
        printf("AGI cyc=%llu op=0x%04x res=0x%04x xf=0x%04x %s\n",
            (unsigned long long)g_cycle,opc,result,xf,pr.result_canon);

        g_cycle++;
        if(result==0)break;
        if(ring_has_receipt(result))break;
        sx_free(&pr);

        fd_set rfds; FD_ZERO(&rfds); FD_SET(0,&rfds);
        struct timeval tv={0,0};
        if(select(1,&rfds,NULL,NULL,&tv)>0){
            char buf[8192]; if(fgets(buf,sizeof(buf),stdin)){
                size_t llen=strlen(buf); if(llen>0&&buf[llen-1]=='\n')buf[llen-1]=0;
                if(strcmp(buf,":quit")==0)break;
                if(strcmp(buf,":ring")==0){ring_dump();continue;}
                if(buf[0]){SxResult sp=process_sexpr(buf,g_cycle);g_cycle++;printf("EXT %s\n",sp.receipt);sx_free(&sp);}
            }
        }
    }
    write_ring_omi("omi.auto.ring"); ring_save(); return 0;
}

int main(int argc, char **argv) {
    OmicronConfig cfg;
    int rc;
    signal(SIGINT,handle_signal); signal(SIGTERM,handle_signal);
    ring_load();
    omicron_config_init(&cfg);
    omicron_config_from_cli(&cfg,argc,argv);
    if(omicron_command_needs_boot_context(cfg.command)){
        rc=omicron_boot(&cfg);
        if(rc!=0)return rc;
    }
    return omicron_dispatch_legacy(&cfg,argc,argv);
}
