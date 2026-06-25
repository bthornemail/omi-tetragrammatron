#define _GNU_SOURCE
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "omi.h"

uint16_t u16(uint32_t x) { return (uint16_t)(x & 0xffffu); }

uint32_t rotl32(uint32_t x, int n) { n &= 31; return (x << n) | (x >> (32 - n)); }
uint32_t rotr32(uint32_t x, int n) { n &= 31; return (x >> n) | (x << (32 - n)); }

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
