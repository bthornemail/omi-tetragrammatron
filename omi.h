#ifndef OMI_H
#define OMI_H

#include <stdint.h>
#include <stddef.h>

#define OMI_FS 0x1cu
#define OMI_GS 0x1du
#define OMI_RS 0x1eu
#define OMI_US 0x1fu

#define STATE_CANONICAL 0x01u
#define STATE_ALIST     0x02u
#define MEMORY_SIZE 65536u
#define STACK_SIZE 512u
#define OMI_FRAME 0x30000020u

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

uint16_t u16(uint32_t x);
uint32_t rotl32(uint32_t x, int n);
uint32_t rotr32(uint32_t x, int n);
uint16_t rotl16(uint16_t x, uint16_t n);
uint16_t rotr16(uint16_t x, uint16_t n);
uint16_t delta16(uint16_t x, uint16_t c);
uint32_t bqf32(uint32_t x, uint32_t y);

uint64_t fnv1a64(const unsigned char *data, size_t len);
uint32_t fnv1a32_buf(const unsigned char *data, size_t len);
uint32_t fnv1a32_str(const char *s);

int parse_hex_field(const char *text, size_t len, uint32_t *out);
int take_hex(const char **p, char delim, uint32_t *out);
int parse_omi_addr(const char *line, OmiInst *inst);
uint16_t execute_omi_op(const OmiInst *inst);
void cpu_exec(CpuState *cpu, uint8_t op);
void cpu_run(CpuState *cpu, const OmiInst *inst);
void cpu_init(CpuState *cpu);
void extract_nibbles(const OmiInst *inst, uint8_t *nibbles, int *count);

#endif
