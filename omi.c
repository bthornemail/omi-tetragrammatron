#define _GNU_SOURCE
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "omi.h"

uint16_t u16(uint32_t x) { return (uint16_t)(x & 0xffffu); }

uint32_t rotl32(uint32_t x, int n) { n &= 31; if(!n) return x; return (x << n) | (x >> (32 - n)); }
uint32_t rotr32(uint32_t x, int n) { n &= 31; if(!n) return x; return (x >> n) | (x << (32 - n)); }

uint16_t rotl16(uint16_t x, uint16_t n) { n &= 15; if (!n) return x; return u16(((uint32_t)x << n) | ((uint32_t)x >> (16-n))); }
uint16_t rotr16(uint16_t x, uint16_t n) { n &= 15; if (!n) return x; return u16(((uint32_t)x >> n) | ((uint32_t)x << (16-n))); }
uint16_t delta16(uint16_t x, uint16_t c) { return u16(rotl16(x,1) ^ rotl16(x,3) ^ rotr16(x,2) ^ c); }

uint32_t bqf32(uint32_t x, uint32_t y) { return (60u * x * x) + (16u * x * y) + (4u * y * y); }

uint64_t fnv1a64(const unsigned char *data, size_t len) {
    uint64_t h = 1469598103934665603ull;
    for (size_t i = 0; i < len; i++) { h ^= data[i]; h *= 1099511628211ull; }
    return h;
}

uint32_t fnv1a32_buf(const unsigned char *data, size_t len) {
    uint32_t h = 0x811c9dc5;
    for (size_t i = 0; i < len; i++) { h ^= data[i]; h *= 0x01000193; }
    return h;
}

uint32_t fnv1a32_str(const char *s) {
    return fnv1a32_buf((const unsigned char *)s, strlen(s));
}

int parse_hex_field(const char *text, size_t len, uint32_t *out) {
    uint32_t v=0; if(len<3||text[0]!='0'||text[1]!='x') return 0;
    for(size_t i=2;i<len;i++){unsigned char c=(unsigned char)text[i];uint32_t d;if(c>='0'&&c<='9')d=(uint32_t)(c-'0');else if(c>='a'&&c<='f')d=10u+(uint32_t)(c-'a');else if(c>='A'&&c<='F')d=10u+(uint32_t)(c-'A');else return 0;v=(v<<4)|d;}
    *out=v; return 1;
}

int take_hex(const char **p, char delim, uint32_t *out) {
    const char *start=*p; while(**p&&**p!=delim) (*p)++;
    if(!parse_hex_field(start,(size_t)(*p-start),out)) return 0;
    if(delim&&**p!=delim) return 0; if(delim)(*p)++; return 1;
}

int parse_omi_addr(const char *line, OmiInst *inst) {
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

uint16_t execute_omi_op(const OmiInst *inst) {
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

void cpu_exec(CpuState *cpu, uint8_t op) {
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

void extract_nibbles(const OmiInst *inst, uint8_t *nibbles, int *count) {
    *count=0; uint16_t segs[]={inst->s0,inst->s1,inst->s2,inst->s3};
    for(int i=0;i<4;i++){nibbles[(*count)++]=(segs[i]>>12)&0xF;nibbles[(*count)++]=(segs[i]>>8)&0xF;nibbles[(*count)++]=(segs[i]>>4)&0xF;nibbles[(*count)++]=(segs[i]>>0)&0xF;}
    uint16_t segs2[]={inst->s4,inst->s5,inst->s6,inst->s7};
    for(int i=0;i<4;i++){nibbles[(*count)++]=(segs2[i]>>12)&0xF;nibbles[(*count)++]=(segs2[i]>>8)&0xF;nibbles[(*count)++]=(segs2[i]>>4)&0xF;nibbles[(*count)++]=(segs2[i]>>0)&0xF;}
}

void cpu_run(CpuState *cpu, const OmiInst *inst) {
    uint8_t nibbles[32]; int count=0; extract_nibbles(inst,nibbles,&count);
    cpu->payload=inst->payload; cpu->mask=inst->mask; cpu->car_reg=inst->car; cpu->cdr_reg=inst->cdr;
    for(int i=0;i<count&&!cpu->halted;i++) cpu_exec(cpu,nibbles[i]);
}

void cpu_init(CpuState *cpu) { memset(cpu,0,sizeof(*cpu)); }

static int arena_ok(const omi_arena_t *a, omi_u32 off, size_t n) {
    return a && a->base && off <= a->size && n <= a->size - off;
}

static omi_u8 gauge_diag(omi_u16 v) {
    int p=0,m=0;
    for(int i=0;i<4;i++){omi_u8 n=(omi_u8)((v>>(i*4))&0xfu);if(n==0||n==5||n==0xa||n==0xf)p++;if(n==3||n==6||n==9||n==0xc)m++;}
    if(p>m)return OMI_DIAG_PLUS;
    if(m>p)return OMI_DIAG_MINUS;
    return OMI_DIAG_BALANCED;
}

static omi_u8 gauge_action(omi_u8 c) {
    if(c<0x20)return OMI_ACTION_PLACE;
    if(c<0x30)return OMI_ACTION_REGISTER_INJECT;
    if(c<0x40)return OMI_ACTION_KERNEL_READ;
    if(c==0x78)return OMI_ACTION_SYSTEM_WITNESS;
    if(c==0x7f)return OMI_ACTION_SEAL;
    if(c==0x3c)return OMI_ACTION_DECLARATION_OPEN;
    if(c==0x2e)return OMI_ACTION_DOT_RELATION;
    return OMI_ACTION_DECLARATIVE;
}

static void bridge_set_flag(omi_arena_t *a, omi_u8 f) {
    if(!arena_ok(a, OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_STAGED_FLAGS, 1))return;
    omi_u8 v=OMI_GET8(a, OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_STAGED_FLAGS);
    OMI_SET8(a, OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_STAGED_FLAGS, v | (omi_u8)(1u << f));
}

void omi_arena_init(omi_arena_t *a, void *memory, size_t size) {
    if(!a)return;
    memset(a,0,sizeof(*a)); a->base=(omi_u8*)memory; a->size=size; a->heap_free=OMI_OFFSET_HEAP;
    if(a->base&&a->size)memset(a->base,0,a->size);
    if(size>=OMI_OFFSET_RING_STORAGE+OMI_RING_STORAGE_SIZE){omi_gauge_init(a);omi_bridge_init(a);}
}

void omi_gauge_init(omi_arena_t *a) {
    if(!arena_ok(a, OMI_OFFSET_GAUGE_TABLE, OMI_GAUGE_COUNT*sizeof(omi_gauge_entry_t)))return;
    omi_gauge_entry_t *t=(omi_gauge_entry_t*)(void*)(a->base+OMI_OFFSET_GAUGE_TABLE);
    for(omi_u32 i=0;i<OMI_GAUGE_COUNT;i++){
        omi_gauge_entry_t *e=&t[i]; memset(e,0,sizeof(*e));
        e->code=(omi_u8)i; e->cls=(i<0x20)?OMI_GAUGE_CLASS_CONTROL:(i==0x7f)?OMI_GAUGE_CLASS_DEL:OMI_GAUGE_CLASS_PRINTABLE;
        e->diag=gauge_diag((omi_u16)i); e->action=gauge_action((omi_u8)i); e->opcode=(omi_u16)((i&0xfu)<<8);
        e->place=(i<0x20)?(omi_u16)i:0xffffu; e->flags=OMI_GAUGE_FLAG_ACTIVE;
        e->payload_seed=i; e->mask_seed=0xffffu; e->car_seed=i<<8; e->cdr_seed=i<<16;
        if(i==0x1e){e->bridge=OMI_BRIDGE_RECORD_CLOSE;e->action=OMI_ACTION_RECORD_CLOSE;e->diag=OMI_DIAG_BALANCED;}
        else if(i==0x78){e->bridge=OMI_BRIDGE_SYSTEM_WITNESS;e->diag=OMI_DIAG_BALANCED;e->opcode=0x0078u;}
        else if(i==0x7f){e->bridge=OMI_BRIDGE_LOCAL_SEAL;e->diag=OMI_DIAG_PLUS;e->opcode=0x007fu;}
    }
}

omi_gauge_entry_t *omi_gauge_get(omi_arena_t *a, omi_u8 code) {
    if(code>=OMI_GAUGE_COUNT||!arena_ok(a, OMI_OFFSET_GAUGE_TABLE, OMI_GAUGE_COUNT*sizeof(omi_gauge_entry_t)))return NULL;
    return &((omi_gauge_entry_t*)(void*)(a->base+OMI_OFFSET_GAUGE_TABLE))[code];
}

void omi_gauge_derive_ruler(const omi_gauge_entry_t *e, omi_handle_t env, omi_ruler_t *r) {
    (void)env; if(!e||!r)return;
    memset(r,0,sizeof(*r)); r->s0=(omi_u16)(0x0100u|e->code); r->s1=0x03bfu; r->s2=(omi_u16)(e->code<<4);
    r->s3=e->opcode; r->s4=(omi_u16)(0x2f00u|e->code); r->s5=0x1434u; r->s6=0x039fu; r->s7=(omi_u16)((e->code<<8)|0xffu);
    r->payload=e->payload_seed; r->mask=e->mask_seed; r->car=e->car_seed; r->cdr=e->cdr_seed;
}

void omi_gauge_stage_place(omi_arena_t *a, omi_u8 c) {
    if(c>=0x20||!arena_ok(a, OMI_OFFSET_CONTROL_PLACE+(omi_u32)c*2u, 2))return;
    omi_gauge_entry_t *e=omi_gauge_get(a,c); if(e)OMI_SET16(a, OMI_OFFSET_CONTROL_PLACE+(omi_u32)c*2u, e->place);
}

void omi_gauge_stage_register(omi_arena_t *a, omi_u8 c) {
    if(c<0x20||c>=0x30||!arena_ok(a, OMI_OFFSET_REGISTER_INJECT+(c-0x20u), 1))return;
    OMI_SET8(a, OMI_OFFSET_REGISTER_INJECT+(c-0x20u), c);
}

void omi_gauge_stage_kernel(omi_arena_t *a, omi_u8 c) {
    if(c<0x30||c>=0x40||!arena_ok(a, OMI_OFFSET_KERNEL_READER+(c-0x30u), 1))return;
    OMI_SET8(a, OMI_OFFSET_KERNEL_READER+(c-0x30u), c);
}

omi_u64 omi_gauge_process(omi_arena_t *a, omi_u8 c, omi_handle_t env) {
    omi_gauge_entry_t *e=omi_gauge_get(a,c); if(!e||!(e->flags&OMI_GAUGE_FLAG_ACTIVE)||!arena_ok(a,OMI_OFFSET_BITBOARD,8))return 0;
    omi_ruler_t r; omi_gauge_derive_ruler(e,env,&r);
    omi_u64 b=OMI_GET64(a,OMI_OFFSET_BITBOARD); b=(b&~OMI_BB_GAUGE_MASK)|(omi_u64)c;
    if(e->diag==OMI_DIAG_PLUS)b|=(1ull<<OMI_BB_DPLUS_SHIFT); else if(e->diag==OMI_DIAG_MINUS)b|=(1ull<<OMI_BB_DMINUS_SHIFT);
    if(e->bridge==OMI_BRIDGE_RECORD_CLOSE)b|=(1ull<<OMI_BB_BRIDGE_1E_BIT);
    if(e->bridge==OMI_BRIDGE_SYSTEM_WITNESS)b|=(1ull<<OMI_BB_BRIDGE_78_BIT);
    if(c==0x7f)b|=(1ull<<OMI_BB_SEAL_7F_BIT);
    if(omi_bridge_is_staged(a,OMI_BRIDGE_BOOT_PAGE))b|=(1ull<<OMI_BB_BOOT_7C00_BIT);
    if(omi_bridge_is_staged(a,OMI_BRIDGE_EXTERNAL))b|=(1ull<<OMI_BB_BRIDGE_AA55_BIT);
    if(e->action==OMI_ACTION_PLACE)omi_gauge_stage_place(a,c); else if(e->action==OMI_ACTION_REGISTER_INJECT)omi_gauge_stage_register(a,c); else if(e->action==OMI_ACTION_KERNEL_READ)omi_gauge_stage_kernel(a,c);
    OMI_SET64(a,OMI_OFFSET_BITBOARD,b); return b;
}

void omi_bridge_init(omi_arena_t *a) {
    if(!arena_ok(a, OMI_OFFSET_BOOT_BRIDGE, OMI_BRIDGE_OFFSET_STAGED_FLAGS+1))return;
    memset(a->base+OMI_OFFSET_BOOT_BRIDGE,0,OMI_BRIDGE_OFFSET_STAGED_FLAGS+1);
}

omi_bridge_result_t omi_bridge_resolve(omi_arena_t *a, omi_u16 w, omi_handle_t env) {
    (void)env; omi_bridge_result_t r; memset(&r,0,sizeof(r)); r.value=w;
    if(!arena_ok(a, OMI_OFFSET_BOOT_BRIDGE, OMI_BRIDGE_OFFSET_STAGED_FLAGS+1))return r;
    switch(w){
    case OMI_BRIDGE_RECORD_CLOSE:r.action=1;r.recognized=r.staged=1;OMI_SET16(a,OMI_OFFSET_BOOT_BRIDGE+OMI_BRIDGE_OFFSET_RECORD_CLOSE,w);bridge_set_flag(a,OMI_BRIDGE_FLAG_RECORD_CLOSE);break;
    case OMI_BRIDGE_SYSTEM_WITNESS:r.action=2;r.recognized=r.staged=1;OMI_SET32(a,OMI_OFFSET_BOOT_BRIDGE+OMI_BRIDGE_OFFSET_SYSTEM_WITNESS,w);bridge_set_flag(a,OMI_BRIDGE_FLAG_SYSTEM_WITNESS);break;
    case OMI_BRIDGE_BOOT_PAGE:r.action=3;r.recognized=r.staged=1;OMI_SET32(a,OMI_OFFSET_BOOT_BRIDGE+OMI_BRIDGE_OFFSET_BOOT_PAGE,w);bridge_set_flag(a,OMI_BRIDGE_FLAG_BOOT_PAGE);break;
    case OMI_BRIDGE_LOCAL_SEAL:r.action=4;r.recognized=r.staged=1;OMI_SET32(a,OMI_OFFSET_BOOT_BRIDGE+OMI_BRIDGE_OFFSET_LOCAL_SEAL,w);bridge_set_flag(a,OMI_BRIDGE_FLAG_LOCAL_SEAL);break;
    case OMI_BRIDGE_EXTERNAL:r.action=5;r.recognized=r.staged=1;OMI_SET16(a,OMI_OFFSET_BOOT_BRIDGE+OMI_BRIDGE_OFFSET_EXTERNAL,w);bridge_set_flag(a,OMI_BRIDGE_FLAG_EXTERNAL);break;
    default:break;
    }
    return r;
}

omi_u8 omi_bridge_is_recognized(omi_arena_t *a, omi_u16 w) {
    (void)a;
    return (omi_u8)(w==OMI_BRIDGE_RECORD_CLOSE||w==OMI_BRIDGE_SYSTEM_WITNESS||w==OMI_BRIDGE_BOOT_PAGE||w==OMI_BRIDGE_LOCAL_SEAL||w==OMI_BRIDGE_EXTERNAL);
}

omi_u8 omi_bridge_is_staged(omi_arena_t *a, omi_u16 w) {
    if(!arena_ok(a, OMI_OFFSET_BOOT_BRIDGE+OMI_BRIDGE_OFFSET_STAGED_FLAGS, 1))return 0;
    omi_u8 f=OMI_GET8(a, OMI_OFFSET_BOOT_BRIDGE+OMI_BRIDGE_OFFSET_STAGED_FLAGS), b=0xffu;
    switch(w){case OMI_BRIDGE_RECORD_CLOSE:b=OMI_BRIDGE_FLAG_RECORD_CLOSE;break;case OMI_BRIDGE_SYSTEM_WITNESS:b=OMI_BRIDGE_FLAG_SYSTEM_WITNESS;break;case OMI_BRIDGE_BOOT_PAGE:b=OMI_BRIDGE_FLAG_BOOT_PAGE;break;case OMI_BRIDGE_LOCAL_SEAL:b=OMI_BRIDGE_FLAG_LOCAL_SEAL;break;case OMI_BRIDGE_EXTERNAL:b=OMI_BRIDGE_FLAG_EXTERNAL;break;default:return 0;}
    return (omi_u8)((f&(1u<<b))!=0);
}

omi_handle_t omi_symbol_intern(omi_arena_t *a, const char *s) {
    (void)a; if(!s||!*s)return OMI_HANDLE_NIL; return OMI_MAKE_HANDLE(OMI_HANDLE_TAG_SYMBOL, fnv1a32_str(s));
}

omi_u8 omi_declaration_contains_symbol_pair(omi_arena_t *a, omi_handle_t d, const char *car_symbol, const char *cdr_symbol) {
    (void)a;(void)d;(void)car_symbol;(void)cdr_symbol; return 0;
}

omi_u8 omi_rrbac_allows_effect(omi_arena_t *a, omi_handle_t d, omi_effect_t e) {
    (void)a;(void)d; return (omi_u8)(e==OMI_EFFECT_NONE||e==OMI_EFFECT_PURE);
}

omi_effect_t omi_effect_of_declaration(omi_arena_t *a, omi_handle_t d) {
    (void)a; if(d==OMI_HANDLE_NIL)return OMI_EFFECT_NONE; return OMI_EFFECT_NONE;
}

omi_u8 omi_receipt_is_accepted(omi_arena_t *a, omi_handle_t r) {
    (void)a;(void)r; return 0;
}

omi_receipt_slot_t *omi_receipt_get(omi_arena_t *a, omi_handle_t r) {
    (void)a;(void)r; return NULL;
}

omi_u8 omi_projection_allowed(omi_arena_t *a, omi_handle_t r, omi_effect_t e) {
    if(!omi_receipt_is_accepted(a,r))return 0;
    if(omi_effect_of_declaration(a,r)!=e)return 0;
    if((e==OMI_EFFECT_HARDWARE||e==OMI_EFFECT_NETWORK)&&!omi_bridge_is_staged(a,OMI_BRIDGE_EXTERNAL))return 0;
    return omi_rrbac_allows_effect(a,r,e);
}
