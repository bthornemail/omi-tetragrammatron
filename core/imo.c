#define _GNU_SOURCE
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "imo.h"
#include "metatron.h"

/* ─── Ring Serialization ─── */

#define RING_FILE "/tmp/omi_receipt_ring.bin"

void ring_load(void) {
    int fd=open(RING_FILE,O_RDONLY); if(fd>=0){ssize_t rv=read(fd,ring,sizeof(ring));(void)rv;close(fd);}
}
void ring_save(void) {
    int fd=open(RING_FILE,O_WRONLY|O_CREAT,0666); if(fd>=0){ssize_t wv=write(fd,ring,sizeof(ring));(void)wv;close(fd);}
}

int write_ring_omi(const char *path) {
    FILE *f=fopen(path,"wb"); if(!f)return 0;
    fwrite("OMIRING1",1,8,f);
    for(size_t i=0;i<8;i++){unsigned char z=0;fwrite(&z,1,1,f);}
    for(size_t i=0;i<RING_SIZE;i++){uint64_t v=ring[i].hash;fwrite(&v,8,1,f);}
    fclose(f); return 1;
}

void handle_signal(int sig) { (void)sig; ring_save(); g_running=0; }

/* ─── Utility ─── */

void die(const char *msg) { fprintf(stderr, "FATAL: %s\n", msg); _exit(1); }

void *xmalloc(size_t n) { void *p = malloc(n ? n : 1); if (!p) die("malloc"); return p; }
void *xrealloc(void *p, size_t n) { p = realloc(p, n ? n : 1); if (!p) die("realloc"); return p; }
char *xstrdup(const char *s) { size_t n = strlen(s); char *p = (char *)xmalloc(n+1); memcpy(p,s,n+1); return p; }
char *xstrndup(const char *s, size_t n) { char *p = (char *)xmalloc(n+1); memcpy(p,s,n); p[n]=0; return p; }

/* ─── Buffer ─── */

void breserve(Buffer *b, size_t extra) {
    size_t need = b->len+extra+1;
    if (need <= b->cap) return;
    size_t cap = b->cap ? b->cap : 128;
    while (cap < need) cap *= 2;
    b->data = (char *)xrealloc(b->data, cap);
    b->cap = cap;
}
void bputc(Buffer *b, unsigned char c) { breserve(b,1); b->data[b->len++]=(char)c; b->data[b->len]=0; }
void bappend(Buffer *b, const char *s) { size_t n = strlen(s); breserve(b,n); memcpy(b->data+b->len,s,n); b->len+=n; b->data[b->len]=0; }
void bappend_bytes(Buffer *b, const unsigned char *d, size_t n) { breserve(b,n); memcpy(b->data+b->len,d,n); b->len+=n; b->data[b->len]=0; }
void bfree(Buffer *b) { free(b->data); b->data=NULL; b->len=b->cap=0; }

/* ─── Node operations ─── */

Node *nnew(NodeKind k) {
    Node *n = (Node *)xmalloc(sizeof(Node));
    n->kind = k; n->text = NULL; n->items = NULL; n->count = n->cap = 0; n->car = n->cdr = NULL;
    return n;
}
Node *natom(const char *t) { Node *n = nnew(NODE_ATOM); n->text = xstrdup(t); return n; }
Node *nstring(const char *t) { Node *n = nnew(NODE_STRING); n->text = xstrdup(t); return n; }
Node *npair(Node *a, Node *d) { Node *n = nnew(NODE_PAIR); n->car = a; n->cdr = d; return n; }

void npush(Node *l, Node *item) {
    if (l->count == l->cap) {
        size_t cap = l->cap ? l->cap*2 : 8;
        l->items = (Node **)xrealloc(l->items, cap*sizeof(Node*));
        l->cap = cap;
    }
    l->items[l->count++] = item;
}

void nfree(Node *n) {
    if (!n) return;
    free(n->text);
    if (n->kind == NODE_LIST) { for (size_t i = 0; i < n->count; i++) nfree(n->items[i]); free(n->items); }
    else if (n->kind == NODE_PAIR) { nfree(n->car); nfree(n->cdr); }
    free(n);
}

Node *ncopy(Node *n) {
    if (!n) return NULL;
    if (n->kind == NODE_ATOM) return natom(n->text ? n->text : "");
    if (n->kind == NODE_STRING) return nstring(n->text ? n->text : "");
    if (n->kind == NODE_LIST) { Node *l = nnew(NODE_LIST); for (size_t i=0;i<n->count;i++) npush(l,ncopy(n->items[i])); return l; }
    if (n->kind == NODE_PAIR) return npair(ncopy(n->car), ncopy(n->cdr));
    return NULL;
}

/* ─── S-expression Parser ─── */

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

Node *parse_sexpr(const char *src, char *err, size_t errlen) {
    Parser p; p.src=src; p.len=strlen(src); p.pos=0; p.line=1; p.col=1; p.error[0]=0;
    Node *root=pexpr(&p); if(!root){snprintf(err,errlen,"%s",p.error);return NULL;} pskip(&p);
    if(!pdone(&p)){nfree(root);perr(&p,"trailing");snprintf(err,errlen,"%s",p.error);return NULL;} return root;
}

/* ─── S-expression utilities ─── */

void jesc(Buffer *b, const char *s) {
    for(const unsigned char *p=(const unsigned char*)s;*p;p++){unsigned char c=*p;if(c=='"'||c=='\\'){bputc(b,'\\');bputc(b,c);}else if(c=='\n')bappend(b,"\\n");else if(c=='\r')bappend(b,"\\r");else if(c=='\t')bappend(b,"\\t");else if(c<0x20){char tmp[8];snprintf(tmp,sizeof(tmp),"\\u%04x",c);bappend(b,tmp);}else bputc(b,c);}
}

void ncanon(Node *n, Buffer *b) {
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

int compile_omi(const char *src, Compilation *c, char *err, size_t errlen) {
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

void cfree(Compilation*c){nfree(c->root);free(c->normalized);efree(&c->events);bfree(&c->tape);memset(c,0,sizeof(*c));}

Node *reduce(Node *expr) {
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

char *generate_receipt(const char *src, const char *result, uint64_t cycle) {
    Buffer b={0}; bappend(&b,"{\"cy\":"); char tmp[32]; snprintf(tmp,sizeof(tmp),"%llu",(unsigned long long)cycle); bappend(&b,tmp);
    bappend(&b,",\"s\":\""); jesc(&b,src?src:""); bappend(&b,"\",\"r\":\""); jesc(&b,result?result:"");
    bappend(&b,"\",\"h\":\""); uint64_t h=fnv1a64((const unsigned char*)(result?result:""),result?strlen(result):0);
    snprintf(tmp,sizeof(tmp),"0x%016llx",(unsigned long long)h); bappend(&b,tmp); bappend(&b,"\"}");
    char *r=b.data; b.data=NULL; bfree(&b); return r;
}

SxResult process_sexpr(const char *line, uint64_t cycle) {
    SxResult pr; memset(&pr,0,sizeof(pr)); char err[256]; Compilation c; Node *result=NULL; Buffer rb={0};
    int has_omi=compile_omi(line,&c,err,sizeof(err));
    if(has_omi){pr.normalized=xstrdup(c.normalized);snprintf(pr.shape,sizeof(pr.shape),"%s",c.shape);pr.events=c.events.len;}
    if(!has_omi){Node *ast=parse_sexpr(line,err,sizeof(err));if(!ast){Buffer eb={0};bappend(&eb,"{\"e\":\"");jesc(&eb,err);bappend(&eb,"\"}");pr.receipt=generate_receipt(line,eb.data,cycle);pr.result_canon=xstrdup(eb.data?eb.data:"");bfree(&eb);return pr;}result=reduce(ast);ncanon(result,&rb);snprintf(pr.shape,sizeof(pr.shape),"sexpr");nfree(ast);}
    else{result=reduce(c.root);ncanon(result,&rb);}
    pr.accepted=1; pr.receipt=generate_receipt(line,rb.data,cycle); pr.result_canon=rb.data?xstrdup(rb.data):xstrdup(""); bfree(&rb); nfree(result); if(has_omi)cfree(&c);
    uint64_t rh=fnv1a64((const unsigned char*)pr.result_canon,strlen(pr.result_canon)); ring_store(rh,pr.receipt); return pr;
}

void sx_free(SxResult *pr){free(pr->receipt);free(pr->result_canon);free(pr->normalized);memset(pr,0,sizeof(*pr));}

/* ─── SHA-1 ─── */

void sha1(const unsigned char *msg, size_t len, unsigned char out[20]) {
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

static void b64_encode_ws(const unsigned char *in, size_t len, char *out) {
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

/* ─── HTTP Server ─── */

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
    const char *rel = path;
    while (*rel == '/') rel++;
    if (!*rel) rel = "index.html";
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

static void serve_frame_json_http(int fd) {
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
        bappend(&b, ",\"r\":\"");
        jesc(&b, ring[i].receipt);
        bappend(&b, "\"}");
    }
    bappend(&b, "]}");
    http_ok(fd, "application/json;charset=utf-8", b.data, b.len);
    bfree(&b);
}

/* ─── Stream client management ─── */
#define MAX_STREAM_CLIENTS 16
typedef struct { int fd; int type; } StreamClient;
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
    char accept_b64[64]; b64_encode_ws(hash, 20, accept_b64);
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

void serve_http(int port) {
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
                        serve_frame_json_http(c); close(c);
                    } else if (strcmp(path, "/ring") == 0) {
                        serve_ring_json(c); close(c);
                    } else if (strcmp(path, "/events") == 0) {
                        serve_sse(c);
                    } else if (strcmp(path, "/ws") == 0) {
                        serve_ws(c, req);
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

/* ─── Boot ROM ─── */

const char *BOOT_ROM[] = {
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
const int BOOT_COUNT = sizeof(BOOT_ROM)/sizeof(BOOT_ROM[0]);
