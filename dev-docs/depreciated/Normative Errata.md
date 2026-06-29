# DEPRECATED / IMPLEMENTED

This document is retained for historical context only.

Its actionable C scaffold corrections have been carried into the current source and tests.

Current authority order:

```text
Doctrine of Determinism
Constants Without Constants
Omi Surface Projection Faces
OMI Glossary
current source code and tests
```

Reason for depreciation:

```text
The errata no longer lives as active canon beside implementation.
Gauge entries no longer own bridge fields.
Gauge processing returns witnesses only.
Bridge constants are owned by the bridge resolver.
0x1E is closure, not acceptance.
Projection is gated after receipt.
Validation judges.
Receipt accepts.
```

---

# Final Reference Implementation

## Errata 0.1 + Remaining Catches Applied

Status: Reference-Final Direction  
Scope: Gauge, bridge, tokenizer, parser, receipt flow, and hardware projection

---

## 0. Final Canon Lock

```text
0x1E is closure, not acceptance.

The gauge table witnesses byte placement.

The bridge resolver interprets bridge constants.

Each bridge word has its own storage slot.

Boot may be synthetic or external.

The tokenizer stages byte witnesses and explicit bridge events.

The parser builds full declaration forms.

The validator accepts or rejects candidates.

The receipt ring records only validation results.

Hardware projection requires:
  - accepted receipt
  - declared hardware effect
  - role/scope permission
  - bridge authorization

No byte, delimiter, bridge word, source block, projection, or model output may write acceptance by itself.
```

---

# 1. Bridge Storage Layout

```c
/* omi.h / omi_arena.h */

#define OMI_OFFSET_BOOT_BRIDGE 0x00014000u

/* Each bridge word has its own storage slot. */
#define OMI_BRIDGE_OFFSET_RECORD_CLOSE    0x00u  /* 0x001E */
#define OMI_BRIDGE_OFFSET_SYSTEM_WITNESS  0x04u  /* 0x0078 */
#define OMI_BRIDGE_OFFSET_BOOT_PAGE       0x08u  /* 0x7C00 */
#define OMI_BRIDGE_OFFSET_LOCAL_SEAL      0x0Cu  /* 0x007F */
#define OMI_BRIDGE_OFFSET_EXTERNAL        0x10u  /* 0xAA55 */
#define OMI_BRIDGE_OFFSET_STAGED_FLAGS    0x14u  /* bitset */

/* Staged flag bit indices. */
#define OMI_BRIDGE_FLAG_RECORD_CLOSE      0u
#define OMI_BRIDGE_FLAG_SYSTEM_WITNESS    1u
#define OMI_BRIDGE_FLAG_BOOT_PAGE         2u
#define OMI_BRIDGE_FLAG_LOCAL_SEAL        3u
#define OMI_BRIDGE_FLAG_EXTERNAL          4u

#define OMI_BRIDGE_RECORD_CLOSE           0x001Eu
#define OMI_BRIDGE_SYSTEM_WITNESS         0x0078u
#define OMI_BRIDGE_LOCAL_SEAL             0x007Fu
#define OMI_BRIDGE_BOOT_PAGE              0x7C00u
#define OMI_BRIDGE_EXTERNAL               0xAA55u
```

---

# 2. Bridge Result Type

```c
typedef struct {
    omi_u16 value;
    omi_u8  action;
    omi_u8  recognized;
    omi_u8  staged;
    omi_u8  reserved;
} omi_bridge_result_t;
```

---

# 3. Corrected Bridge Resolver

```c
/* omi_bridge.c */

#include "omi.h"

static void omi_bridge_set_flag(omi_arena_t* arena, omi_u8 flag) {
    omi_u8 flags = OMI_GET8(arena, OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_STAGED_FLAGS);
    flags = (omi_u8)(flags | (omi_u8)(1u << flag));
    OMI_SET8(arena, OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_STAGED_FLAGS, flags);
}

void omi_bridge_init(omi_arena_t* arena) {
    OMI_SET16(arena, OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_RECORD_CLOSE, 0);
    OMI_SET32(arena, OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_SYSTEM_WITNESS, 0);
    OMI_SET32(arena, OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_BOOT_PAGE, 0);
    OMI_SET32(arena, OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_LOCAL_SEAL, 0);
    OMI_SET16(arena, OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_EXTERNAL, 0);
    OMI_SET8(arena, OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_STAGED_FLAGS, 0);
}

omi_bridge_result_t omi_bridge_resolve(omi_arena_t* arena,
                                        omi_u16 word,
                                        omi_handle_t env) {
    (void)env;

    omi_bridge_result_t result = {0};
    result.value = word;

    switch (word) {
        case OMI_BRIDGE_RECORD_CLOSE:
            result.action = 1;
            result.recognized = 1;
            result.staged = 1;
            OMI_SET16(arena,
                      OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_RECORD_CLOSE,
                      word);
            omi_bridge_set_flag(arena, OMI_BRIDGE_FLAG_RECORD_CLOSE);
            break;

        case OMI_BRIDGE_SYSTEM_WITNESS:
            result.action = 2;
            result.recognized = 1;
            result.staged = 1;
            OMI_SET32(arena,
                      OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_SYSTEM_WITNESS,
                      word);
            omi_bridge_set_flag(arena, OMI_BRIDGE_FLAG_SYSTEM_WITNESS);
            break;

        case OMI_BRIDGE_BOOT_PAGE:
            result.action = 3;
            result.recognized = 1;
            result.staged = 1;
            OMI_SET32(arena,
                      OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_BOOT_PAGE,
                      word);
            omi_bridge_set_flag(arena, OMI_BRIDGE_FLAG_BOOT_PAGE);
            break;

        case OMI_BRIDGE_LOCAL_SEAL:
            result.action = 4;
            result.recognized = 1;
            result.staged = 1;
            OMI_SET32(arena,
                      OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_LOCAL_SEAL,
                      word);
            omi_bridge_set_flag(arena, OMI_BRIDGE_FLAG_LOCAL_SEAL);
            break;

        case OMI_BRIDGE_EXTERNAL:
            result.action = 5;
            result.recognized = 1;
            result.staged = 1;
            OMI_SET16(arena,
                      OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_EXTERNAL,
                      word);
            omi_bridge_set_flag(arena, OMI_BRIDGE_FLAG_EXTERNAL);
            break;

        default:
            result.action = 0;
            result.recognized = 0;
            result.staged = 0;
            break;
    }

    return result;
}

omi_u8 omi_bridge_is_recognized(omi_arena_t* arena, omi_u16 word) {
    switch (word) {
        case OMI_BRIDGE_RECORD_CLOSE:
            return OMI_GET16(arena,
                OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_RECORD_CLOSE) == word;

        case OMI_BRIDGE_SYSTEM_WITNESS:
            return OMI_GET32(arena,
                OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_SYSTEM_WITNESS) == word;

        case OMI_BRIDGE_BOOT_PAGE:
            return OMI_GET32(arena,
                OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_BOOT_PAGE) == word;

        case OMI_BRIDGE_LOCAL_SEAL:
            return OMI_GET32(arena,
                OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_LOCAL_SEAL) == word;

        case OMI_BRIDGE_EXTERNAL:
            return OMI_GET16(arena,
                OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_EXTERNAL) == word;

        default:
            return 0;
    }
}

omi_u8 omi_bridge_is_staged(omi_arena_t* arena, omi_u16 word) {
    omi_u8 flags = OMI_GET8(arena,
        OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_STAGED_FLAGS);

    switch (word) {
        case OMI_BRIDGE_RECORD_CLOSE:
            return (flags & (1u << OMI_BRIDGE_FLAG_RECORD_CLOSE)) ? 1 : 0;

        case OMI_BRIDGE_SYSTEM_WITNESS:
            return (flags & (1u << OMI_BRIDGE_FLAG_SYSTEM_WITNESS)) ? 1 : 0;

        case OMI_BRIDGE_BOOT_PAGE:
            return (flags & (1u << OMI_BRIDGE_FLAG_BOOT_PAGE)) ? 1 : 0;

        case OMI_BRIDGE_LOCAL_SEAL:
            return (flags & (1u << OMI_BRIDGE_FLAG_LOCAL_SEAL)) ? 1 : 0;

        case OMI_BRIDGE_EXTERNAL:
            return (flags & (1u << OMI_BRIDGE_FLAG_EXTERNAL)) ? 1 : 0;

        default:
            return 0;
    }
}
```

---

# 4. Gauge Entry Without Bridge Field

```c
typedef struct {
    omi_u8  code;       /* ASCII code 0x00-0x7F */
    omi_u8  cls;        /* GaugeClass */
    omi_u8  diag;       /* DiagClass */
    omi_u8  action;     /* GaugeAction */

    omi_u16 opcode;     /* OMI opcode / S3 witness */
    omi_u16 place;      /* Place controller n00..n31 */
    omi_u16 flags;      /* bit0: active */
    omi_u16 reserved;   /* alignment / reserved */

    omi_u32 payload_seed;
    omi_u32 mask_seed;
    omi_u32 car_seed;
    omi_u32 cdr_seed;
    omi_u32 next;
} omi_gauge_entry_t;

_Static_assert(sizeof(omi_gauge_entry_t) == 32,
               "omi_gauge_entry_t must be exactly 32 bytes");
```

---

# 5. Corrected Gauge Processing

```c
omi_u32 omi_gauge_process(omi_arena_t* arena,
                           omi_u8 code,
                           omi_handle_t env) {
    (void)env;

    omi_gauge_entry_t* e = omi_gauge_get(arena, code);
    if (!e || !(e->flags & OMI_GAUGE_FLAG_ACTIVE)) {
        return 0;
    }

    omi_ruler_t ruler;
    omi_gauge_derive_ruler(e, env, &ruler);

    omi_u64 board = OMI_GET64(arena, OMI_OFFSET_BITBOARD);

    /* Last gauge byte witness. */
    board = (board & ~0x7FULL) | (omi_u64)(code & 0x7F);

    /* Diagonal counters. */
    if (e->diag == OMI_DIAG_PLUS) {
        board += (1ULL << 7);
    } else if (e->diag == OMI_DIAG_MINUS) {
        board += (1ULL << 12);
    }

    /*
     * Byte-level witnesses only.
     * These do not store receipts.
     */
    if (code == 0x1E) board |= (1ULL << 17); /* RS close witness */
    if (code == 0x78) board |= (1ULL << 18); /* system witness byte */
    if (code == 0x7F) board |= (1ULL << 19); /* local seal byte */

    /*
     * Bridge-state flags are read from the bridge resolver.
     * Gauge does not interpret bridge words.
     */
    if (omi_bridge_is_staged(arena, OMI_BRIDGE_BOOT_PAGE)) {
        board |= (1ULL << 20);
    }

    if (omi_bridge_is_staged(arena, OMI_BRIDGE_EXTERNAL)) {
        board |= (1ULL << 21);
    }

    OMI_SET64(arena, OMI_OFFSET_BITBOARD, board);

    /*
     * Return witness only.
     * Never return receipt handles from gauge processing.
     */
    return (omi_u32)board;
}
```

---

# 6. Boot Mode

```c
typedef enum {
    OMI_BOOT_SYNTHETIC,  /* self-host boot profile; stages AA55 */
    OMI_BOOT_EXTERNAL    /* external carrier must present AA55 */
} omi_boot_mode_t;

omi_u32 omi_boot_sequence(omi_arena_t* arena, omi_boot_mode_t mode) {
    for (int i = 0; i < 0x20; i++) {
        omi_gauge_stage_place(arena, (omi_u8)i);
    }

    for (int i = 0x20; i < 0x30; i++) {
        omi_gauge_stage_register(arena, (omi_u8)i);
    }

    for (int i = 0x30; i < 0x40; i++) {
        omi_gauge_stage_kernel(arena, (omi_u8)i);
    }

    omi_bridge_resolve(arena, OMI_BRIDGE_SYSTEM_WITNESS, OMI_HANDLE_NIL);
    omi_bridge_resolve(arena, OMI_BRIDGE_BOOT_PAGE, OMI_HANDLE_NIL);
    omi_bridge_resolve(arena, OMI_BRIDGE_LOCAL_SEAL, OMI_HANDLE_NIL);

    if (mode == OMI_BOOT_SYNTHETIC) {
        omi_bridge_resolve(arena, OMI_BRIDGE_EXTERNAL, OMI_HANDLE_NIL);
    }

    arena->boot_staged = 1;

    return OMI_GET32(arena,
        OMI_OFFSET_BOOT_BRIDGE + OMI_BRIDGE_OFFSET_BOOT_PAGE);
}
```

Canon:

```text
Synthetic boot may declare AA55.
External boot must observe AA55.
```

---

# 7. Tokenizer State

The tokenizer MUST read from the buffer passed to it. It MUST NOT silently ignore the input pointer.

```c
typedef struct {
    const omi_u8* input;
    omi_u32 input_ptr;
    omi_u32 input_len;

    omi_handle_t env;
    omi_lex_mode_t mode;

    omi_u8 in_declaration;
    omi_u8 in_comment;
} omi_tokenizer_state_t;

void omi_tokenizer_init(omi_tokenizer_state_t* state,
                        omi_handle_t env,
                        omi_lex_mode_t mode,
                        const omi_u8* input,
                        omi_u32 len) {
    state->input = input;
    state->input_ptr = 0;
    state->input_len = len;
    state->env = env;
    state->mode = mode;
    state->in_declaration = 0;
    state->in_comment = 0;
}
```

---

# 8. Corrected Tokenizer

```c
omi_token_t omi_next_token(omi_arena_t* arena,
                           omi_tokenizer_state_t* state) {
    omi_token_t tok = {0};

    if (state->input_ptr >= state->input_len) {
        tok.type = TOKEN_EOF;
        return tok;
    }

    omi_u8 c = state->input[state->input_ptr++];

    if (state->mode == OMI_LEX_BOOT_GAUGE) {
        /*
         * Boot gauge mode:
         * 0x00..0x1F are place controllers.
         */
        if (c < 0x20) {
            tok.type = TOKEN_CONTROL;
            tok.gauge_code = c;
            tok.place = c;

            /*
             * 0x1E is still a control byte,
             * but it also stages record-close bridge semantics.
             * It does not store a receipt.
             */
            if (c == 0x1E) {
                omi_bridge_resolve(arena, OMI_BRIDGE_RECORD_CLOSE, state->env);
            }

            tok.value = omi_gauge_process(arena, c, state->env);
            tok.text[0] = '^';
            tok.text[1] = (char)('@' + c);
            tok.text[2] = '\0';
            return tok;
        }

        if (c == 0x78) {
            tok.type = TOKEN_BOOT;
            tok.gauge_code = 0x78;
            tok.value = omi_gauge_process(arena, 0x78, state->env);
            strcpy(tok.text, "x");
            omi_bridge_resolve(arena, OMI_BRIDGE_SYSTEM_WITNESS, state->env);
            return tok;
        }

        if (c == 0x7F) {
            tok.type = TOKEN_SEAL;
            tok.gauge_code = 0x7F;
            tok.value = omi_gauge_process(arena, 0x7F, state->env);
            strcpy(tok.text, "DEL");
            omi_bridge_resolve(arena, OMI_BRIDGE_LOCAL_SEAL, state->env);
            return tok;
        }
    }

    /*
     * Readable Lisp mode and non-boot fallback.
     * 0x1E may still appear as structural record close.
     */
    if (c == 0x1E) {
        tok.type = TOKEN_CONTROL;
        tok.gauge_code = 0x1E;
        tok.place = 0x1E;
        tok.value = omi_gauge_process(arena, 0x1E, state->env);
        strcpy(tok.text, "RS");
        omi_bridge_resolve(arena, OMI_BRIDGE_RECORD_CLOSE, state->env);
        return tok;
    }

    if (c == '<') {
        tok.type = TOKEN_DECLARATION_OPEN;
        tok.gauge_code = c;
        tok.value = omi_gauge_process(arena, c, state->env);
        strcpy(tok.text, "<");
        return tok;
    }

    if (c == '>') {
        tok.type = TOKEN_DECLARATION_CLOSE;
        tok.gauge_code = c;
        tok.value = omi_gauge_process(arena, c, state->env);
        strcpy(tok.text, ">");
        return tok;
    }

    if (c == '(') {
        tok.type = TOKEN_LPAREN;
        tok.gauge_code = c;
        tok.value = omi_gauge_process(arena, c, state->env);
        strcpy(tok.text, "(");
        return tok;
    }

    if (c == ')') {
        tok.type = TOKEN_RPAREN;
        tok.gauge_code = c;
        tok.value = omi_gauge_process(arena, c, state->env);
        strcpy(tok.text, ")");
        return tok;
    }

    if (c == '.') {
        tok.type = TOKEN_DOT;
        tok.gauge_code = c;
        tok.value = omi_gauge_process(arena, c, state->env);
        strcpy(tok.text, ".");
        return tok;
    }

    if (c == '\'') {
        tok.type = TOKEN_QUOTE;
        tok.gauge_code = c;
        tok.value = omi_gauge_process(arena, c, state->env);
        strcpy(tok.text, "'");
        return tok;
    }

    if (c >= '0' && c <= '9') {
        omi_u32 value = (omi_u32)(c - '0');

        while (state->input_ptr < state->input_len) {
            omi_u8 n = state->input[state->input_ptr];
            if (n < '0' || n > '9') break;
            state->input_ptr++;
            value = (value * 10u) + (omi_u32)(n - '0');
        }

        tok.type = TOKEN_NUMBER;
        tok.value = value;
        snprintf(tok.text, sizeof(tok.text), "%u", value);
        return tok;
    }

    if ((c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        c == '_' || c == '-' || c == '/') {
        omi_u32 pos = 0;
        tok.text[pos++] = (char)c;

        while (state->input_ptr < state->input_len &&
               pos + 1 < sizeof(tok.text)) {
            omi_u8 n = state->input[state->input_ptr];

            if (!((n >= 'A' && n <= 'Z') ||
                  (n >= 'a' && n <= 'z') ||
                  (n >= '0' && n <= '9') ||
                  n == '_' || n == '-' || n == '/' || n == '.')) {
                break;
            }

            tok.text[pos++] = (char)n;
            state->input_ptr++;
        }

        tok.text[pos] = '\0';
        tok.type = TOKEN_SYMBOL;
        tok.value = 0;
        return tok;
    }

    if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
        return omi_next_token(arena, state);
    }

    tok.type = TOKEN_UNKNOWN;
    tok.gauge_code = c;
    tok.value = omi_gauge_process(arena, c, state->env);
    tok.text[0] = (char)c;
    tok.text[1] = '\0';
    return tok;
}
```

---

# 9. Parser Lookahead Support

A full form parser needs one-token lookahead.

```c
typedef struct {
    omi_tokenizer_state_t tokenizer;
    omi_handle_t env;
    omi_u8 error;
    omi_u8 has_peek;
    omi_token_t peek;
    omi_u8 source;
} omi_parser_state_t;

static omi_token_t omi_parser_next(omi_arena_t* arena,
                                   omi_parser_state_t* state) {
    if (state->has_peek) {
        state->has_peek = 0;
        return state->peek;
    }

    return omi_next_token(arena, &state->tokenizer);
}

static void omi_parser_unread(omi_parser_state_t* state,
                              omi_token_t tok) {
    state->peek = tok;
    state->has_peek = 1;
}
```

---

# 10. Parser With Full Form Parsing

```c
omi_handle_t omi_parse_form(omi_arena_t* arena,
                            omi_parser_state_t* state);

omi_handle_t omi_parse_cons_pair(omi_arena_t* arena,
                                 omi_parser_state_t* state) {
    omi_handle_t car_val = omi_parse_form(arena, state);
    if (state->error) return OMI_HANDLE_NIL;

    omi_token_t dot_tok = omi_parser_next(arena, state);
    if (dot_tok.type != TOKEN_DOT) {
        state->error = 1;
        return OMI_HANDLE_NIL;
    }

    omi_handle_t cdr_val = omi_parse_form(arena, state);
    if (state->error) return OMI_HANDLE_NIL;

    omi_token_t close_tok = omi_parser_next(arena, state);
    if (close_tok.type != TOKEN_RPAREN) {
        state->error = 1;
        return OMI_HANDLE_NIL;
    }

    omi_gauge_process(arena, 0x2E, state->env);

    return omi_cons(arena, car_val, cdr_val);
}

omi_handle_t omi_parse_form(omi_arena_t* arena,
                            omi_parser_state_t* state) {
    omi_token_t tok = omi_parser_next(arena, state);

    switch (tok.type) {
        case TOKEN_LPAREN:
            return omi_parse_cons_pair(arena, state);

        case TOKEN_QUOTE: {
            omi_handle_t quoted = omi_parse_form(arena, state);
            if (state->error) return OMI_HANDLE_NIL;

            omi_handle_t quote_sym =
                OMI_MAKE_HANDLE(OMI_HANDLE_TAG_SYMBOL, OMI_SYMBOL_QUOTE);

            return omi_cons(arena, quote_sym, quoted);
        }

        case TOKEN_NUMBER:
            return OMI_MAKE_HANDLE(OMI_HANDLE_TAG_INTEGER, tok.value);

        case TOKEN_SYMBOL:
            return omi_symbol_intern(arena, tok.text);

        case TOKEN_GAUGE:
        case TOKEN_CONTROL:
        case TOKEN_BOOT:
        case TOKEN_SEAL:
            return OMI_MAKE_HANDLE(OMI_HANDLE_TAG_WITNESS, tok.value);

        case TOKEN_DECLARATION_OPEN:
            return omi_parse_declaration(arena, state);

        case TOKEN_EOF:
        case TOKEN_RPAREN:
        case TOKEN_DECLARATION_CLOSE:
        default:
            state->error = 1;
            return OMI_HANDLE_NIL;
    }
}
```

---

# 11. Declaration Parser With Receipt Gate

```c
omi_handle_t omi_parse_declaration(omi_arena_t* arena,
                                   omi_parser_state_t* state) {
    /*
     * Called after TOKEN_DECLARATION_OPEN has already been consumed.
     * Parses:
     *
     *   < form >
     */

    omi_handle_t result = omi_parse_form(arena, state);
    if (state->error) return OMI_HANDLE_NIL;

    omi_token_t close_tok = omi_parser_next(arena, state);
    if (close_tok.type != TOKEN_DECLARATION_CLOSE) {
        state->error = 1;
        return OMI_HANDLE_NIL;
    }

    /*
     * GATE 1:
     * Parsed form becomes receipt candidate.
     */
    omi_receipt_candidate_t candidate = {
        .declaration = result,
        .env = state->env,
        .cycle = arena->cycle,
        .source = state->source,
        .validated = 0
    };

    /*
     * GATE 2:
     * Validation decides acceptance eligibility.
     */
    omi_u32 valid_hash = omi_validate_candidate(arena, &candidate);
    if (valid_hash == 0) {
        return OMI_HANDLE_NIL;
    }

    candidate.validated = 1;
    candidate.hash = valid_hash;

    /*
     * GATE 3:
     * Receipt store is the only acceptance write.
     */
    omi_handle_t receipt = omi_receipt_store(arena, &candidate);
    if (receipt == OMI_HANDLE_NIL) {
        return OMI_HANDLE_NIL;
    }

    /*
     * GATE 4:
     * Projection is separate from acceptance.
     */
    if (omi_projection_allowed(arena, receipt, OMI_EFFECT_HARDWARE)) {
        omi_hardware_project(arena, receipt);
    }

    return receipt;
}
```

---

# 12. Effect Classes

```c
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
```

---

# 13. Effect Resolution

The reference implementation should not infer hardware from “CAR exists.” It should resolve an explicit effect.

```c
omi_effect_t omi_effect_of_declaration(omi_arena_t* arena,
                                       omi_handle_t declaration) {
    if (declaration == OMI_HANDLE_NIL) {
        return OMI_EFFECT_NONE;
    }

    /*
     * Reference rule:
     * A declaration may carry an explicit effect pair:
     *
     *   (effect . hardware)
     *   (effect . pure)
     *   (effect . read-only)
     *
     * Production implementation should traverse the declaration form.
     */

    if (omi_declaration_contains_symbol_pair(arena,
                                             declaration,
                                             "effect",
                                             "hardware")) {
        return OMI_EFFECT_HARDWARE;
    }

    if (omi_declaration_contains_symbol_pair(arena,
                                             declaration,
                                             "effect",
                                             "network")) {
        return OMI_EFFECT_NETWORK;
    }

    if (omi_declaration_contains_symbol_pair(arena,
                                             declaration,
                                             "effect",
                                             "repo-write")) {
        return OMI_EFFECT_REPO_WRITE;
    }

    if (omi_declaration_contains_symbol_pair(arena,
                                             declaration,
                                             "effect",
                                             "read-only")) {
        return OMI_EFFECT_READ_ONLY;
    }

    if (omi_declaration_contains_symbol_pair(arena,
                                             declaration,
                                             "effect",
                                             "pure")) {
        return OMI_EFFECT_PURE;
    }

    return OMI_EFFECT_NONE;
}
```

---

# 14. Projection Gate

```c
omi_u8 omi_projection_allowed(omi_arena_t* arena,
                              omi_handle_t receipt_handle,
                              omi_effect_t requested_effect) {
    if (!omi_receipt_is_accepted(arena, receipt_handle)) {
        return 0;
    }

    omi_receipt_slot_t* slot = omi_receipt_get(arena, receipt_handle);
    if (!slot) {
        return 0;
    }

    omi_effect_t declared_effect =
        omi_effect_of_declaration(arena, slot->declaration_ref);

    if (declared_effect != requested_effect) {
        return 0;
    }

    /*
     * Hardware and network projection require bridge authorization.
     */
    if (requested_effect == OMI_EFFECT_HARDWARE ||
        requested_effect == OMI_EFFECT_NETWORK) {
        if (!omi_bridge_is_staged(arena, OMI_BRIDGE_EXTERNAL)) {
            return 0;
        }
    }

    /*
     * Repository role/scope permission should be checked here.
     * This is the REPO.md / AGENTS.md policy gate.
     */
    if (!omi_rrbac_allows_effect(arena,
                                 slot->declaration_ref,
                                 requested_effect)) {
        return 0;
    }

    return 1;
}
```

---

# 15. Hardware Projection

```c
void omi_hardware_project(omi_arena_t* arena,
                          omi_handle_t receipt_handle) {
    if (!omi_projection_allowed(arena,
                                receipt_handle,
                                OMI_EFFECT_HARDWARE)) {
        return;
    }

    omi_receipt_slot_t* slot = omi_receipt_get(arena, receipt_handle);
    if (!slot) return;

    omi_handle_t decl = slot->declaration_ref;
    if (decl == OMI_HANDLE_NIL) return;

#ifdef ESP32
    /*
     * Hardware mapping must be declaration-driven.
     * Example declarations:
     *
     *   <((effect . hardware) . (gpio2 . high))>
     *   <((effect . hardware) . (gpio2 . low))>
     */

    if (omi_declaration_contains_symbol_pair(arena, decl, "gpio2", "high")) {
        gpio_set_level(OMI_HW_GPIO_LED, 1);
    } else if (omi_declaration_contains_symbol_pair(arena, decl, "gpio2", "low")) {
        gpio_set_level(OMI_HW_GPIO_LED, 0);
    }
#endif

    /*
     * Hardware result becomes a new candidate.
     * It still must validate before storage.
     */
    omi_receipt_candidate_t result_candidate = {
        .declaration = decl,
        .env = OMI_HANDLE_NIL,
        .cycle = arena->cycle,
        .source = 0x01,
        .validated = 0
    };

    omi_u32 hash = omi_validate_candidate(arena, &result_candidate);
    if (hash != 0) {
        result_candidate.validated = 1;
        result_candidate.hash = hash;
        omi_receipt_store(arena, &result_candidate);
    }
}
```

---

# 16. Required Stub Contracts

These functions must exist before the implementation can be called reference-complete:

```c
omi_handle_t omi_symbol_intern(omi_arena_t* arena, const char* symbol);

omi_u8 omi_declaration_contains_symbol_pair(omi_arena_t* arena,
                                             omi_handle_t declaration,
                                             const char* car_symbol,
                                             const char* cdr_symbol);

omi_u8 omi_rrbac_allows_effect(omi_arena_t* arena,
                               omi_handle_t declaration,
                               omi_effect_t effect);

omi_u32 omi_validate_candidate(omi_arena_t* arena,
                               omi_receipt_candidate_t* candidate);

omi_handle_t omi_receipt_store(omi_arena_t* arena,
                               const omi_receipt_candidate_t* candidate);

omi_u8 omi_receipt_is_accepted(omi_arena_t* arena,
                               omi_handle_t receipt);

omi_receipt_slot_t* omi_receipt_get(omi_arena_t* arena,
                                    omi_handle_t receipt);
```

Until these are implemented, the system is a correct scaffold, not yet a complete runtime.

---

# 17. Final Errata + Catch Summary

|Item|Issue|Correction|
|---|---|---|
|E1|`0x1E` auto-stored receipt|RS only sets witness; no receipt storage|
|E2|`omi_gauge_process` mixed return types|Always returns bitboard witness|
|E3|Bridge semantics duplicated|Bridge resolver is sole semantic authority|
|E4|Bridge words inside byte gauge|`0x7C00` and `0xAA55` only in bridge resolver|
|C1|`0x1E` bridge unreachable in boot mode|Bridge call added inside boot control block|
|C2|Bridge storage overloaded|Each bridge word has its own offset|
|C3|`0xAA55` staged unconditionally|Synthetic vs external boot mode|
|C4|Parser token-by-token|Added full form parser|
|C5|Hardware projection too broad|Added explicit effect check and bridge gate|
|C6|Tokenizer ignored input pointer|Tokenizer now reads from passed input|
|C7|Quote parser double-read bug|Quote now parses one following form|
|C8|Projection conflated with acceptance|Projection now gated after receipt|

---

# 18. Final Reference Direction

This is now the reference implementation direction:

```text
gauge_process:
  byte → witness

bridge_resolve:
  bridge word → staged bridge meaning

tokenizer:
  input byte stream → tokens + explicit bridge staging

parser:
  tokens → full declaration forms

validator:
  declaration candidate → accepted/rejected result

receipt ring:
  validation result → durable receipt

projection:
  accepted receipt + declared effect + permission + bridge authorization → visible/action surface
```

The architecture is now locked to the invariant:

```text
Recognition is not acceptance.
Citation is not acceptance.
Closure is not acceptance.
Projection is not acceptance.
Validation and receipt accept.
```
