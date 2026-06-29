# Project Specification

Status: active standard specification  
Source canon: `dev-docs/archive/OMI-Lisp_Complete_Specification.md`  
Roadmap companion: `dev-docs/PROJECT_OUTLINE.md`

This file is the active project specification for OMI / Tetragrammatron / Metatron / IMO / Omicron. The archived documents remain source material. This file states the implementation contract the repository should converge toward.

## 1. Core Rule

```text
Code is data.
File is port.
Notation is citation.
Omi-Ring witnesses.
Canon validates.
Cosmology projects.
Validation carries forward.
```

No layer is authority by itself. Validation is the exclusive boundary between candidate state and validated carry-forward state. The Omi-Ring is the addressed palindromic notation witness; a receipt is a compatibility name for stored validated state of that witness.

Compatibility rule:

```text
receipt = compatibility record for validated state of an Omi-Ring
receipt ring = compatibility storage/replay surface for validated Omi-Ring states
receipt-like = compatibility shape for validated-state records
```

## 2. Authorities

OMI is the Citation Authority.
- Owns address/ruler notation, CONS/CAR/CDR identity, fixed-width operations, and citation construction.
- Owns `OmiCitationCandidate` construction from normalized/lowered Omi-Log candidates.
- Does not validate, project, transport, or accept.

Tetragrammatron is the Validation Authority.
- Owns deterministic validation, DeltaC/BQF/Polybius/Fano/slot5040 logic, validated carry-forward state, and the 5040-slot validated-state ring.
- Owns `TetragrammatronValidatedState` construction from OMI citation candidates.
- Owns explicit in-memory validated-state storage through `tetragrammatron_store_validated_state`.
- Active guardrail: `dev-docs/TETRAGRAMMATRON.md`.
- Does not render, parse carriers, transport, or mutate identity.

Metatron is the Projection and Scribe-Transducer Authority.
- Owns deterministic validated-state-to-notation transduction, geometry, surfaces, coordinates, solids, Smith/Gnomonic/CSS-style projection meaning, and renderers.
- V0 scribing reads stored validated-state ring entries through validated-slot gating; candidate-only state is not a projection input.
- Does not validate, transport, or accept.

IMO is the Carrier Authority.
- Owns files, S-expression/OMI-Lisp readable forms, `.imo` carrier normalization, HTTP/SSE/WebSocket, browser/device ports, and serialization.
- Does not validate, compose geometry, or change identity.

Omicron is the boot, context, dialect induction, and runtime application layer.
- Orchestrates the four authorities without becoming a fifth authority.
- Stages gauge pre-headers and boot context.
- Binds system object declarations for `omi.o`, `imo.o`, `tetragrammatron.o`, and `metatron.o`.
- Induces the readable OMI-Lisp surface from pre-language/gauge context.
- Replaced the legacy `opencode` application name.
- Build target is `core/omicron.bin`; object target is `core/omicron.o`; scaffold header is `core/omicron.h`; orchestration source is `core/omicron.c`.
- Does not validate, accept receipts, project meaning, or own carrier side effects.

## 3. OMI-Lisp Language Model

OMI-Lisp is the project language model: a deterministic notation-citation-Omi-Ring-validation-projection protocol.

The complete language specification is `dev-docs/archive/OMI-Lisp_Complete_Specification.md`.

Required language layers:
- Pre-language: the 32 non-printing ASCII controls `0x00..0x1F` stage address places.
- Separator spine: `FS/GS/RS/US` define file/group/record/unit topology.
- Minimal notation: `omi---imo`.
- Omi-Ring grammar: `o---o/---/?---?@---@`.
- Machine frame: `o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR`.
- Dot notation: visible CAR/CDR relation syntax across memory, hardware, events, intents, declarations, receipts, and lazy evaluation.

Required semantic rules:
- Notation is citation, not acceptance.
- Dot pairs preserve CAR/CDR closure.
- Lazy evaluation applies down to hardware.
- No side effects before validated carry-forward state.
- Events and intents are symmetric candidate forms.
- Accepted Omi-Ring states preserve identity across carriers and projections.

## 4. Addressing Model

The active addressing scaffold is `dev-docs/OMI_ADDRESSING.md`.

OMI addressing treats CIDR-style notation as a readable CONS surface:

```text
address/prefix = (address . prefix)
```

Canonical rule:

```text
Frame identifies.
Slash derives.
Path selects.
Clauses validate.
Validation carries forward.
```

The Omi-Ring is the bounded addressed notation witness of an `omi---imo` relation. A receipt is only a compatibility name for stored validated state of that witness. The receipt ring stores and replays validated Omi-Ring states.

Required V0 surface forms:

```text
omi---imo
omi-<frame>
imo-<frame>
omi-<frame>-imo
omi-<frame>/<path...>-imo
```

The path is derivation, not identity. Prefix depth is the resolution horizon. Slash derivation, path selection, and Omi-Ring formation do not accept state.

Omicron owns non-authoritative address dialect induction and lowering:

```text
CIDR notation
ip4-notation
ip6-notation
@CAR36@CDR64 closure
```

All readable dialect forms must lower to canonical hexadecimal OMI candidate frames before validation.

Omi-Log is the readable `.omi` source surface for addressed declarations. It may use CIDR scopes, O-expressions, readable keywords, and CONS closure notation, but every record must lower into canonical OMI object form before validation.

## 5. Processing Pipeline

Canonical pipeline:

```text
carrier input
  -> IMO recognizes and normalizes representation
  -> OMI cites identity and constructs candidate relation
  -> Tetragrammatron validates candidate
  -> validated-state ring records validated Omi-Ring state
  -> Metatron scribes/projects validated state
  -> IMO carries projected surface/output
```

Hard boundaries:
- Recognition is not acceptance.
- Citation is not acceptance.
- Closure is not acceptance.
- Bridge staging is not acceptance.
- Carrier decode is not acceptance.
- Projection is not acceptance.
- Only validation plus receipt accepts.

## 6. Determinism

The implementation must encode determinism and must not encode nondeterminism.

Forbidden in computation:
- random values
- wall-clock time
- process IDs
- clocks
- UUIDs
- `/dev/urandom`
- host pointer addresses as identity

Permitted carrier behavior:
- terminal input
- file I/O
- signals
- network transport
- browser/device I/O

Carrier behavior may deliver inputs, but deterministic output must depend only on declared inputs and declared rules.

## 7. Runtime Files

Core runtime source files:
- `core/omi.h` / `core/omi.c`: Citation Authority and deterministic instruction core.
- `core/tetragrammatron.h` / `core/tetragrammatron.c`: Validation Authority and validated-state ring.
- `core/metatron.h` / `core/metatron.c`: Projection Authority and geometry surfaces.
- `core/imo.h` / `core/imo.c`: Carrier Authority, parsers, files, server, and ports.
- `core/omicron.h` / `core/omicron.c`: boot/context/dialect scaffold and orchestration application, replacing legacy `opencode.c`.

Legacy compatibility:
- Archive-only `opencode` references remain historical.
- Current CLI behavior must remain stable after the rename.

## 8. Public Portal

The `portal/` directory is the public React/TypeScript web-portal interface. Its policy docs are distinct from root repository docs and live in `agent-docs/`.

Public-interface docs:
- `agent-docs/AGENTS.md`
- `agent-docs/SKILLS.md`
- `agent-docs/ADAPTERS.md`
- `agent-docs/REPO.md`

Agent REPO target:
- Treat `agent-docs/REPO.md` as shared public-interface context for LLM agents, alongside `agent-docs/AGENTS.md` and `agent-docs/SKILLS.md`.
- Merge source material from `dev-docs/archive/REPO.md - First Draft.md`, `dev-docs/archive/REPO.md - OMI Request for Collaboration and Role-Repo Based Access Control.md`, `dev-docs/archive/REPO.md Addendum - Bootstrapping an OMI-Compatible LLM Agent.md`, and `dev-docs/archive/REPO.md Addendum - Emergent P2P and Multimedia Use Cases for OMI.md`.
- Keep `agent-docs/REPO.md` scoped to public LLM-agent bootstrapping and role/repo policy through the published portal.
- Do not merge portal public-interface policy into root `README.md`, root `AGENTS.md`, or root `SKILLS.md`.

## 9. Carriers And Adapters

Carriers transport representation. They do not validate or accept.

Active canonical carrier notation:
- `omi---imo`
- `o---o/---/?---?@---@`
- `o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR`

Barcode policy:
- `omi-barcode` is the canonical barcode carrier family.
- `omi-jabcode` is the current custom OMI barcode form under that family.
- The current `omi-jabcode` implementation is debug-only and must not be promoted as standards-compliant JABCode unless implemented from `dev-docs/archive/JabCode.BSI-TR-03137.pdf`.
- Barcode distinctions should be expressed through OMI/IMO addressing and receipt context, not verbose profile names.

Adapter policy:
- Browser, serial, DOM/CSSOM/Canvas/WebGL, ESP32, file, network, and P2P adapters must be receipt-gated for side effects.
- Hardware and network projection require validated carry-forward state, declared effect, role/scope permission, and bridge authorization.

## 10. Repository And Version Policy

Root repository docs govern the runtime repository:
- `README.md`
- `AGENTS.md`
- `SKILLS.md`

Agent docs govern public web-portal use only.

Versioning model:
- File diffs are carriers.
- OMI-Lisp declarations are candidates.
- Validation determines acceptance.
- Receipts are compatibility records for validated Omi-Ring version state.
- `.imo` files may carry normalized candidate or accepted declarations.

## 11. Required Tests

Every phase must keep these passing:
- `make smoke`
- `make coq`
- C scaffold/unit tests
- portal TypeScript module tests
- deterministic banned-call scan

Required test classes:
- fixed-width struct sizes
- endian round trips
- gauge table and bitboard witnesses
- bridge staging independence
- OMI-Lisp notation parser fixtures
- Omi-Log candidate parser fixtures
- candidate validation and rejection
- projection denial before receipt
- same input produces same output

## 12. Non-Goals

Do not:
- add dependencies without approval
- introduce nondeterministic computation
- move carrier I/O into OMI, Tetragrammatron, or Metatron
- let parser output perform side effects
- treat bridge recognition, carrier decode, or projection as acceptance
- implement partial JABCode while calling it standards-compliant
- collapse portal public-interface policy into root repository policy

## 13. Completion Standard

The project is specification-complete when:
- OMI-Lisp notation can be parsed into deterministic candidate relations.
- Omi-Log declarations lower into Omi-Ring candidates without creating validated state.
- OMI constructs citation candidates from lowered declaration candidates without validating or accepting them.
- Candidates validate through Tetragrammatron into validated carry-forward state and may then be recorded through explicit validated-state storage.
- Accepted Omi-Ring states record identity in the ring.
- Metatron scribing and projections derive only from validated states.
- IMO carries input/output without owning truth.
- The public portal can bootstrap LLM agents from `agent-docs/` without overriding root repository policy.
