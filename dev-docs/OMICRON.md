# Omicron

Status: active V0 scaffold

Omicron is the boot, context, and dialect induction layer for the runtime. It is not a fifth authority. It prepares execution context, stages the gauge pre-header, binds system object declarations, induces the readable OMI-Lisp surface, and then dispatches to the four authorities.

```text
Omicron boots and configures.
OMI cites.
Tetragrammatron validates.
Metatron scribes and projects.
IMO carries.
Receipt accepts.
```

## Authority Boundary

Omicron owns:
- boot mode selection
- dialect selection
- pre-header staging
- pre-language induction
- system object binding
- CLI and embedded entrypoint configuration
- bitboard bootstrap context

Omicron does not own:
- OMI address law
- validation
- receipt storage
- projection meaning
- carrier I/O except wrapper dispatch
- hardware, browser, network, or filesystem side effects

## V0 API

The V0 scaffold is declared in `core/omicron.h` and implemented in `core/omicron.c`.

Public types:
- `OmicronMode`
- `OmicronCommand`
- `OmicronDialect`
- `OmicronConfig`

Public functions:
- `omicron_config_init`
- `omicron_config_from_cli`
- `omicron_boot`
- `omicron_stage_preheader`
- `omicron_induce_omi_lisp`
- `omicron_load_system_objects`
- `omicron_parse_address_candidate`
- `omilog_parse_candidate`
- `omilog_format_candidate_head`
- `omicron_mode_name`
- `omicron_command_name`
- `omicron_dialect_name`

## Dialects

Current dialect bytes:

```text
0xF0 = Omicron bootstrap dialect
0xFF = canonical OMI dialect
```

Reserved dialect bytes:

```text
0xF1..0xFE
```

## Gauge Pre-Header

Omicron stages the deterministic eight-byte gauge pre-header:

```text
Fdelta 00 1C 1D 1E 1F 20 Fdelta
```

Canonical OMI:

```text
FF 00 1C 1D 1E 1F 20 FF
```

Omicron bootstrap:

```text
F0 00 1C 1D 1E 1F 20 F0
```

The first and last dialect bytes must match. This is staging and recognition only. It is not acceptance.

## OMI-Lisp Induction

OMI-Lisp induction means the Omicron boot context has staged the pre-language gauge and declared the visible dot-notation surface available for later parser/runtime use.

V0 induction requires:
- pre-header staged
- FS/GS/RS/US spine present
- readable boundary `0x20` present
- dot operator `0x2E` recorded as the visible CAR/CDR relation surface

OMI-Lisp induction does not parse or evaluate user declarations. It does not validate relations, accept receipts, project surfaces, load files, or perform side effects.

## CIDR Address Dialect

The active addressing scaffold is `dev-docs/OMI_ADDRESSING.md`.

Omicron owns induction and lowering for the non-authoritative CIDR address dialect:

```text
CIDR notation
ip4-notation
ip6-notation
Base36 CAR labels
Base64url CDR carriers
@CAR36@CDR64 closure
```

Canonical rule:

```text
Hex validates.
CIDR resolves.
Omicron induces the dialect.
```

Omicron may parse readable dialect forms and lower them into canonical hexadecimal OMI candidate frames.

Omicron does not validate, accept, store receipts, project effects, or treat IP/Base36/Base64 syntax as authority.

## Omicron Parser Vocabulary Guardrail

The Omicron address parser is a dialect induction and notation-lowering scaffold.

Parser fields MAY describe:

```text
candidate syntax
normalized notation
lowered frames
dialect surfaces
path segments
prefix scopes
payload/mask candidates
CAR/CDR closure candidates
```

Parser fields MUST NOT use authority-bearing terms such as:

```text
valid
accepted
receipt
proof
projection
```

unless the field is explicitly marked as candidate-only, compatibility-only, or delegated to the owning authority module.

Allowed:

```text
lowered_candidate
normalized_candidate
candidate_frame
candidate_closure
candidate_path
```

Forbidden inside Omicron parser API:

```text
lowered_valid_candidate
accepted_candidate
receipt_candidate
proof_candidate
projection_candidate
```

Authority ownership remains:

```text
Omicron parses and lowers dialect candidates.
OMI cites canonical addressed notation.
Tetragrammatron validates and accepts.
Metatron projects accepted state.
IMO carries serialized input and output.
```

Review rule:

```text
If a new Omicron parser field sounds like it validated, accepted, proved, stored, projected, or received state, the name is wrong unless the behavior is explicitly delegated outside Omicron.
```

## Omi-Log Candidate Parser

`omilog_parse_candidate` is the V0 readable declaration scaffold.

It parses:

```text
<address-candidate> <KEYWORD> <assignment>
```

and may also retain a following source block:

```text
omi-
  (...)
-imo
```

The parser lowers the declaration head through `omicron_parse_address_candidate`, records the uppercase keyword, records the assignment text, and marks whether a following `omi-...-imo` block contains an O-expression body.

`omilog_format_candidate_head` emits the deterministic normalized declaration head:

```text
<address-candidate> <KEYWORD> <assignment>
```

Output type:

```text
OmiLogCandidate
```

This is candidate-only state.

It does not validate the declaration, accept an Omi-Ring state, write the receipt ring, route live tables, evaluate OMI-Lisp, project surfaces, perform carrier I/O, or create side effects.

V0 parser hardening:

```text
MUST, FACT, DECLARE, and other non-authority uppercase keywords may parse.
VALID, ACCEPT, ACCEPTED, RECEIPT, PROOF, and PROJECTION do not parse as declaration keywords.
Source blocks containing authority-bearing terms are rejected.
Malformed addresses, missing keywords, missing assignments, and unclosed source blocks are rejected.
```

Implementation lock:

```text
Omi-Log declares.
Omicron lowers.
OMI cites.
Tetragrammatron validates later.
No accepted state is created in this parser.
```

## Boot Order

V0 records the intended construction method:

1. Select boot mode.
2. Select gauge dialect.
3. Stage pre-header.
4. Stage pre-language controls.
5. Open readable boundary at `0x20`.
6. Stage punctuation/register band.
7. Stage kernel reader band.
8. Bind system object declarations.
9. Load readable `.omi` declarations.
10. Load `.imo` carrier declarations.
11. Induce OMI-Lisp dialect.
12. Dispatch authorities.

## Object Surfaces

System object declarations are context surfaces:

```text
omi.o
imo.o
tetragrammatron.o
metatron.o
```

V0 only checks deterministic pointer/length pair integrity for these bindings.

`OBJECTS_BOUND` means object surface pointer/length bindings were checked for deterministic integrity.

It does not mean object declarations were parsed, validated, accepted, serialized, projected, loaded from disk, or executed.

## Ladder Phase Audit

This audit maps the normalized OMI-Lisp ladder onto the current `core/omicron.c` implementation.

Audit lock:

```text
OMI-Lisp declares.
Omicron lowers.
OMI cites.
Tetragrammatron validates.
Receipt records.
Metatron projects.
IMO carries.
```

| Ladder phase | Current implementation | Status | Boundary note |
|--------------|------------------------|--------|---------------|
| stage pre-header | `omicron_stage_preheader` and `omicron_boot` | Implemented V0 | Stages `Fdelta 00 1C 1D 1E 1F 20 Fdelta`; recognition only, not acceptance. |
| induce readable boundary | `omicron_induce_omi_lisp` | Implemented V0 | Requires staged pre-header and records readable boundary `0x20`. |
| earn dot operator | `omicron_induce_omi_lisp` sets `dot_operator = 0x2E` | Implemented V0 | Declares dot notation available after SP; does not parse or evaluate by itself. |
| parse declaration | `omilog_parse_candidate` | Implemented V0 | Parses readable declaration heads and optional `omi-...-imo` blocks as candidate-only state. |
| lower to OMI frame candidate | `omicron_parse_address_candidate`; `omilog_parse_candidate` calls it | Implemented V0 | Lowers address/CIDR/closure syntax into `lowered_frame` and `lowered_candidate`; no validation. |
| submit to Tetragrammatron | Tests call `omi_construct_citation_candidate` then `tetragrammatron_validate_citation_candidate` | Implemented as tested authority handoff | Exists as explicit API/test rail, not yet exposed as a dedicated CLI command. |
| record receipt | `tetragrammatron_store_validated_state`; legacy dispatch still has local `ring_store` paths | Split state | New validated-state storage is explicit and Tetragrammatron-owned; legacy S-expression paths still use compatibility receipt storage inside monolithic dispatch. |
| project/carry | `--scribe` reads stored validated ring slots; render/server/carrier commands remain in legacy dispatch | Partially boxed | Metatron scribe gate exists for validated slots; broader render/carrier behavior still needs modular ownership cleanup. |

Current implementation shape:

```text
main()
-> signal setup and ring load
-> omicron_config_init
-> omicron_config_from_cli
-> omicron_boot when command needs boot context
-> omicron_dispatch_legacy
```

The named ladder phases exist, but not all are wired as one public end-to-end CLI path. The candidate-to-validation-to-storage rail is currently covered by `scripts/test-omicron-boot.c`.

Next safe code boundary:

```text
Add a dedicated declaration-processing helper:

Omi-Log source
-> OmiLogCandidate
-> OmiCitationCandidate
-> Tetragrammatron validated state
-> explicit validated-state ring storage
```

This helper must not project, carry, render, serve, or perform file I/O. Projection and carrier behavior remain separate downstream steps.

## Technical Debt

`core/omicron.c` is still a legacy monolithic runtime file. The V0 API has been added in place, and `main()` now uses `OmicronConfig`, `omicron_boot`, and a boxed legacy dispatch wrapper. The command bodies inside `omicron_dispatch_legacy` are still monolithic compatibility code.

Current wrapper shape:

```c
OmicronConfig cfg;
omicron_config_init(&cfg);
omicron_config_from_cli(&cfg, argc, argv);
if (omicron_command_needs_boot_context(cfg.command)) {
    int rc = omicron_boot(&cfg);
    if (rc != 0) return rc;
}
return omicron_dispatch_legacy(&cfg, argc, argv);
```

Future reduction target: shrink `omicron_dispatch_legacy` one command at a time while preserving existing CLI behavior.

## Omicron Monolith Reduction Plan

- [x] Keep current CLI behavior unchanged through the V0 reductions.
- [x] Move CLI argument interpretation into `omicron_config_from_cli`.
- [x] Move boot pre-header setup into `omicron_stage_preheader`.
- [x] Move object binding into `omicron_load_system_objects`.
- [x] Move OMI-Lisp dialect setup into `omicron_induce_omi_lisp`.
- [x] Box legacy dispatch behind `OmicronConfig` and `omicron_boot`.
- [ ] Delete local duplicate declarations only after modular headers can be included cleanly.

The special test harness that compiles `core/omicron.c` with `main` renamed exists only because `core/omicron.c` still owns the legacy entrypoint. Remove that harness once `main()` is reduced and Omicron can be tested through the public scaffold without entrypoint rewriting.

## Duplicate Declaration Audit

Status: audit-only. Do not delete local duplicate declarations until the owning modular header compiles cleanly with `core/omicron.c` and CLI parity checks pass.

Audit method:

```text
Compile probe core/omicron.c with forced inclusion of each modular header:
  omi.h
  tetragrammatron.h
  metatron.h
  imo.h
```

Current result: all probes fail because `core/omicron.c` still carries local legacy declarations and definitions from the monolithic runtime.

Classification:

- OMI-owned duplicates:
  - `OmiInst`, `CpuState`
  - wrapping, rotation, delta, BQF, and FNV helpers
  - address parsing helpers
  - nibble CPU helpers
- Tetragrammatron-owned duplicates:
  - `RingSlot`, ring state, cycle state, running flag
  - ring index, store, dump, fold, and receipt-presence helpers
  - Polybius, Fano, configuration matrix, ququart phase, slot routing, and chiral helpers
- Metatron-owned duplicates:
  - scribe surface types and scribe receipt bridge declarations
  - shape, edge, incidence, branch, geometry, Smith, and render data
  - vertex resolution, Hopf/ququart routing, frame, PPM, OBJ, glTF, Smith, and incidence helpers
- IMO-owned duplicates:
  - S-expression node, parser, compiler, reducer, event, buffer, and result types
  - allocation, buffer, node, parser, canonicalization, compiler, receipt-generation, and reduction helpers
  - BOOT ROM, ring file I/O, signal handling, HTTP/SSE/WebSocket serving, SHA-1, and base64 helpers
- Omicron/local legacy-only declarations:
  - Omicron V0 scaffold functions and command dispatch wrapper
  - private HTTP route helpers and stream-client helpers until they move behind IMO
  - private drawing helpers until renderers are fully owned through Metatron

Removal order:

1. Include `omi.h` cleanly, remove only OMI-owned local duplicates, and link through `core/omi.c`.
2. Include `tetragrammatron.h` cleanly, remove only validation and receipt-ring duplicates, and verify ring authority remains with Tetragrammatron.
3. Include `metatron.h` cleanly, remove scribe declarations first, then geometry and renderer duplicates.
4. Include `imo.h` cleanly last, because carrier parsing, BOOT ROM, ring file I/O, signals, and server behavior pull the widest dependency surface.

Deletion gate for every class:

```text
include owning header directly
remove one duplicate class only
preserve CLI behavior
run C, smoke, Coq, and determinism checks
compare --eval, --boot, --twin, --render-frame, --check, and --scribe behavior
```

Ring cleanup is the sensitive boundary. `core/omicron.c` currently has local ring state for legacy dispatch compatibility. Replacing that state must not create a second acceptance path or move receipt authority out of Tetragrammatron.

## Lock

Omicron stages context.
Omicron induces dialect.
Omicron dispatches authorities.
Omicron does not accept.
