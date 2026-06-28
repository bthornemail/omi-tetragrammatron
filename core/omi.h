#ifndef OMI_H
#define OMI_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define OMI_VERSION_MAJOR 1
#define OMI_VERSION_MINOR 0
#define OMI_VERSION_PATCH 1

typedef uint8_t  omi_u8;
typedef uint16_t omi_u16;
typedef uint32_t omi_u32;
typedef uint64_t omi_u64;

#define OMI_NUL 0x00u
#define OMI_FS 0x1cu
#define OMI_GS 0x1du
#define OMI_RS 0x1eu
#define OMI_US 0x1fu
#define OMI_SP 0x20u

typedef omi_u32 omi_handle_t;

#define OMI_HANDLE_NIL ((omi_handle_t)0)
#define OMI_HANDLE_TAG_SHIFT 24
#define OMI_HANDLE_TAG_MASK 0xff000000u
#define OMI_HANDLE_IDX_MASK 0x00ffffffu
#define OMI_HANDLE_TAG_CONS 0x01u
#define OMI_HANDLE_TAG_SYMBOL 0x02u
#define OMI_HANDLE_TAG_STRING 0x03u
#define OMI_HANDLE_TAG_ENV 0x04u
#define OMI_HANDLE_TAG_FUNC 0x05u
#define OMI_HANDLE_TAG_RECEIPT 0x06u
#define OMI_MAKE_HANDLE(tag, idx) ((omi_handle_t)(((omi_u32)(tag) << OMI_HANDLE_TAG_SHIFT) | ((idx) & OMI_HANDLE_IDX_MASK)))
#define OMI_HANDLE_TAG(h) (((h) >> OMI_HANDLE_TAG_SHIFT) & 0xffu)
#define OMI_HANDLE_IDX(h) ((h) & OMI_HANDLE_IDX_MASK)

static inline bool omi_is_nil(omi_handle_t h) { return h == OMI_HANDLE_NIL; }
static inline bool omi_is_cons(omi_handle_t h) { return OMI_HANDLE_TAG(h) == OMI_HANDLE_TAG_CONS; }

static inline omi_u16 omi_load16le(const omi_u8 *p) { return (omi_u16)(p[0] | ((omi_u16)p[1] << 8)); }
static inline void omi_store16le(omi_u8 *p, omi_u16 v) { p[0] = (omi_u8)(v & 0xffu); p[1] = (omi_u8)((v >> 8) & 0xffu); }
static inline omi_u32 omi_load32le(const omi_u8 *p) { return (omi_u32)p[0] | ((omi_u32)p[1] << 8) | ((omi_u32)p[2] << 16) | ((omi_u32)p[3] << 24); }
static inline void omi_store32le(omi_u8 *p, omi_u32 v) { p[0] = (omi_u8)(v & 0xffu); p[1] = (omi_u8)((v >> 8) & 0xffu); p[2] = (omi_u8)((v >> 16) & 0xffu); p[3] = (omi_u8)((v >> 24) & 0xffu); }
static inline omi_u64 omi_load64le(const omi_u8 *p) { return (omi_u64)omi_load32le(p) | ((omi_u64)omi_load32le(p + 4) << 32); }
static inline void omi_store64le(omi_u8 *p, omi_u64 v) { omi_store32le(p, (omi_u32)(v & 0xffffffffull)); omi_store32le(p + 4, (omi_u32)(v >> 32)); }

#define OMI_GET8(a, o) (((a)->base)[(o)])
#define OMI_SET8(a, o, v) (((a)->base)[(o)] = (omi_u8)(v))
#define OMI_GET16(a, o) omi_load16le((a)->base + (o))
#define OMI_SET16(a, o, v) omi_store16le((a)->base + (o), (omi_u16)(v))
#define OMI_GET32(a, o) omi_load32le((a)->base + (o))
#define OMI_SET32(a, o, v) omi_store32le((a)->base + (o), (omi_u32)(v))
#define OMI_GET64(a, o) omi_load64le((a)->base + (o))
#define OMI_SET64(a, o, v) omi_store64le((a)->base + (o), (omi_u64)(v))

#define STATE_CANONICAL 0x01u
#define STATE_ALIST     0x02u
#define MEMORY_SIZE 65536u
#define STACK_SIZE 512u
#define OMI_FRAME 0x30000020u

#define OMI_ARENA_SIZE (1024u * 1024u)
#define OMI_OFFSET_BOOT_ROM 0x00000000u
#define OMI_OFFSET_CONTROL_PLACE 0x00001000u
#define OMI_OFFSET_REGISTER_INJECT 0x00002000u
#define OMI_OFFSET_KERNEL_READER 0x00003000u
#define OMI_OFFSET_GAUGE_TABLE 0x00004000u
#define OMI_OFFSET_BITBOARD 0x00005800u
#define OMI_OFFSET_RING_WINDOW 0x00006000u
#define OMI_OFFSET_STACK 0x00007000u
#define OMI_OFFSET_HEAP 0x00008000u
#define OMI_OFFSET_SYMBOL_TABLE 0x0000a000u
#define OMI_OFFSET_CONS_TABLE 0x0000c000u
#define OMI_OFFSET_IO_BUFFER 0x00010000u
#define OMI_OFFSET_HARDWARE_REGS 0x00012000u
#define OMI_OFFSET_PROJECTION 0x00013000u
#define OMI_OFFSET_BOOT_BRIDGE 0x00014000u
#define OMI_OFFSET_RING_STORAGE 0x00020000u

#define OMI_GAUGE_COUNT 128u
#define OMI_RING_SLOTS 5040u
#define OMI_RING_SLOT_SIZE 64u
#define OMI_RING_STORAGE_SIZE (OMI_RING_SLOTS * OMI_RING_SLOT_SIZE)

#define OMI_BRIDGE_OFFSET_RECORD_CLOSE 0x00u
#define OMI_BRIDGE_OFFSET_SYSTEM_WITNESS 0x04u
#define OMI_BRIDGE_OFFSET_BOOT_PAGE 0x08u
#define OMI_BRIDGE_OFFSET_LOCAL_SEAL 0x0cu
#define OMI_BRIDGE_OFFSET_EXTERNAL 0x10u
#define OMI_BRIDGE_OFFSET_STAGED_FLAGS 0x14u

#define OMI_BRIDGE_FLAG_RECORD_CLOSE 0u
#define OMI_BRIDGE_FLAG_SYSTEM_WITNESS 1u
#define OMI_BRIDGE_FLAG_BOOT_PAGE 2u
#define OMI_BRIDGE_FLAG_LOCAL_SEAL 3u
#define OMI_BRIDGE_FLAG_EXTERNAL 4u

#define OMI_BRIDGE_RECORD_CLOSE 0x001eu
#define OMI_BRIDGE_SYSTEM_WITNESS 0x0078u
#define OMI_BRIDGE_LOCAL_SEAL 0x007fu
#define OMI_BRIDGE_BOOT_PAGE 0x7c00u
#define OMI_BRIDGE_EXTERNAL 0xaa55u

#define OMI_BB_GAUGE_MASK 0x000000000000007full
#define OMI_BB_DPLUS_SHIFT 7u
#define OMI_BB_DMINUS_SHIFT 12u
#define OMI_BB_BRIDGE_1E_BIT 17u
#define OMI_BB_BRIDGE_78_BIT 18u
#define OMI_BB_SEAL_7F_BIT 19u
#define OMI_BB_BOOT_7C00_BIT 20u
#define OMI_BB_BRIDGE_AA55_BIT 21u
#define OMI_BB_PLACE_SHIFT 22u
#define OMI_BB_RESULT_SHIFT 32u
#define OMI_BB_FOLD_SHIFT 48u

#define OMI_GAUGE_FLAG_ACTIVE 0x0001u
#define OMI_GAUGE_CLASS_CONTROL 0u
#define OMI_GAUGE_CLASS_PRINTABLE 1u
#define OMI_GAUGE_CLASS_DEL 2u
#define OMI_DIAG_NONE 0u
#define OMI_DIAG_PLUS 1u
#define OMI_DIAG_MINUS 2u
#define OMI_DIAG_BALANCED 3u
#define OMI_ACTION_PLACE 0u
#define OMI_ACTION_REGISTER_INJECT 1u
#define OMI_ACTION_KERNEL_READ 2u
#define OMI_ACTION_SYSTEM_WITNESS 3u
#define OMI_ACTION_SEAL 4u
#define OMI_ACTION_RECORD_CLOSE 5u
#define OMI_ACTION_DECLARATION_OPEN 6u
#define OMI_ACTION_DOT_RELATION 7u
#define OMI_ACTION_DECLARATIVE 8u

typedef struct {
    omi_u8 code, cls, diag, action;
    omi_u16 opcode, bridge, place, flags;
    omi_u32 payload_seed, mask_seed, car_seed, cdr_seed, next;
} omi_gauge_entry_t;

_Static_assert(sizeof(omi_gauge_entry_t) == 32, "omi_gauge_entry_t must be exactly 32 bytes");

typedef struct {
    omi_handle_t car, cdr;
    omi_u8 allocated, mark;
    omi_u16 reserved;
    omi_u32 next;
} omi_cons_cell_t;

_Static_assert(sizeof(omi_cons_cell_t) == 16, "omi_cons_cell_t must be exactly 16 bytes");

typedef struct {
    omi_u32 hash, cycle, slot5040, gauge_code;
    omi_handle_t declaration_ref;
    omi_u8 data[44];
} omi_receipt_slot_t;

_Static_assert(sizeof(omi_receipt_slot_t) == 64, "omi_receipt_slot_t must be exactly 64 bytes");

typedef struct {
    omi_u16 s0, s1, s2, s3;
    omi_u16 s4, s5, s6, s7;
    omi_u32 payload, mask, car, cdr;
} omi_ruler_t;

typedef struct {
    omi_u16 value;
    omi_u8 action, recognized, staged, reserved;
} omi_bridge_result_t;

typedef struct {
    omi_handle_t declaration, env;
    omi_u32 cycle, source;
    omi_u8 validated;
} omi_receipt_candidate_t;

typedef enum {
    TOKEN_EOF,
    TOKEN_CONTROL,
    TOKEN_GAUGE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_DOT,
    TOKEN_QUOTE,
    TOKEN_DECLARATION_OPEN,
    TOKEN_DECLARATION_CLOSE,
    TOKEN_SEAL,
    TOKEN_NUMBER,
    TOKEN_SYMBOL,
    TOKEN_STRING,
    TOKEN_OMI,
    TOKEN_BOOT
} omi_token_type_t;

typedef struct {
    omi_token_type_t type;
    omi_u32 value, gauge_code, place;
    char text[64];
} omi_token_t;

typedef enum {
    OMI_LEX_BOOT_GAUGE,
    OMI_LEX_READABLE_LISP
} omi_lex_mode_t;

typedef struct {
    omi_u32 input_ptr, input_len;
    omi_handle_t env;
    omi_lex_mode_t mode;
    omi_u8 in_declaration, in_comment;
} omi_tokenizer_state_t;

typedef enum {
    OMI_EFFECT_NONE = 0,
    OMI_EFFECT_PURE,
    OMI_EFFECT_READ_ONLY,
    OMI_EFFECT_LOCAL_WRITE,
    OMI_EFFECT_REPO_WRITE,
    OMI_EFFECT_NETWORK,
    OMI_EFFECT_HARDWARE,
    OMI_EFFECT_SECURITY_SENSITIVE
} omi_effect_t;

typedef struct {
    omi_u8 *base;
    size_t size;
    omi_u32 heap_free, ring_head, ring_tail, ring_count, cycle;
    omi_u8 boot_staged;
} omi_arena_t;

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

void omi_arena_init(omi_arena_t *arena, void *memory, size_t size);
void omi_gauge_init(omi_arena_t *arena);
omi_gauge_entry_t *omi_gauge_get(omi_arena_t *arena, omi_u8 code);
void omi_gauge_derive_ruler(const omi_gauge_entry_t *entry, omi_handle_t env, omi_ruler_t *ruler);
void omi_gauge_stage_place(omi_arena_t *arena, omi_u8 code);
void omi_gauge_stage_register(omi_arena_t *arena, omi_u8 code);
void omi_gauge_stage_kernel(omi_arena_t *arena, omi_u8 code);
omi_u64 omi_gauge_process(omi_arena_t *arena, omi_u8 code, omi_handle_t env);

void omi_bridge_init(omi_arena_t *arena);
omi_bridge_result_t omi_bridge_resolve(omi_arena_t *arena, omi_u16 word, omi_handle_t env);
omi_u8 omi_bridge_is_recognized(omi_arena_t *arena, omi_u16 word);
omi_u8 omi_bridge_is_staged(omi_arena_t *arena, omi_u16 word);

omi_handle_t omi_symbol_intern(omi_arena_t *arena, const char *symbol);
omi_u8 omi_declaration_contains_symbol_pair(omi_arena_t *arena, omi_handle_t declaration, const char *car_symbol, const char *cdr_symbol);
omi_u8 omi_rrbac_allows_effect(omi_arena_t *arena, omi_handle_t declaration, omi_effect_t effect);
omi_effect_t omi_effect_of_declaration(omi_arena_t *arena, omi_handle_t declaration);
omi_u8 omi_projection_allowed(omi_arena_t *arena, omi_handle_t receipt_handle, omi_effect_t requested_effect);
omi_u8 omi_receipt_is_accepted(omi_arena_t *arena, omi_handle_t receipt);
omi_receipt_slot_t *omi_receipt_get(omi_arena_t *arena, omi_handle_t receipt);

#endif
