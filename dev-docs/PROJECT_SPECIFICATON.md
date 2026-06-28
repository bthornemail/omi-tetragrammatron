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
Canon validates.
Cosmology projects.
Receipt accepts.
```

No layer is authority by itself. Validation is the exclusive boundary between candidate state and accepted state. A receipt is the durable invariant shared across authorities.

## 2. Authorities

OMI is the Citation Authority.
- Owns address/ruler notation, CONS/CAR/CDR identity, hashes, fixed-width operations, and citation construction.
- Does not validate, project, transport, or accept.

Tetragrammatron is the Validation Authority.
- Owns deterministic validation, DeltaC/BQF/Polybius/Fano/slot5040 logic, receipt acceptance, and the 5040-slot receipt ring.
- Does not render, parse carriers, transport, or mutate identity.

Metatron is the Projection Authority.
- Owns geometry, surfaces, coordinates, solids, Smith/Gnomonic/CSS-style projection meaning, and renderers.
- Does not validate, transport, or accept.

IMO is the Carrier Authority.
- Owns files, S-expression/OMI-Lisp readable forms, `.imo` carrier normalization, HTTP/SSE/WebSocket, browser/device ports, and serialization.
- Does not validate, compose geometry, or change identity.

Omicron is the thin runtime application.
- Orchestrates the four authorities.
- Replaced the legacy `opencode` application name.
- Build target is `core/omicron.bin`; object target is `core/omicron.o`; orchestration source is `core/omicron.c`.

## 3. OMI-Lisp Language Model

OMI-Lisp is the project language model: a deterministic notation-citation-receipt-projection protocol.

The complete language specification is `dev-docs/archive/OMI-Lisp_Complete_Specification.md`.

Required language layers:
- Pre-language: the 32 non-printing ASCII controls `0x00..0x1F` stage address places.
- Separator spine: `FS/GS/RS/US` define file/group/record/unit topology.
- Minimal notation: `omi---imo`.
- Receipt grammar: `o---o/---/?---?@---@`.
- Machine frame: `o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR`.
- Dot notation: visible CAR/CDR relation syntax across memory, hardware, events, intents, declarations, receipts, and lazy evaluation.

Required semantic rules:
- Notation is citation, not acceptance.
- Dot pairs preserve CAR/CDR closure.
- Lazy evaluation applies down to hardware.
- No side effects before accepted receipt.
- Events and intents are symmetric candidate forms.
- Receipts preserve identity across carriers and projections.

## 4. Processing Pipeline

Canonical pipeline:

```text
carrier input
  -> IMO recognizes and normalizes representation
  -> OMI cites identity and constructs candidate relation
  -> Tetragrammatron validates candidate
  -> receipt ring records accepted result
  -> Metatron projects accepted receipt
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

## 5. Determinism

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

## 6. Runtime Files

Core runtime source files:
- `core/omi.h` / `core/omi.c`: Citation Authority and deterministic instruction core.
- `core/tetragrammatron.h` / `core/tetragrammatron.c`: Validation Authority and receipt ring.
- `core/metatron.h` / `core/metatron.c`: Projection Authority and geometry surfaces.
- `core/imo.h` / `core/imo.c`: Carrier Authority, parsers, files, server, and ports.
- `core/omicron.c`: thin orchestration application, replacing legacy `opencode.c`.

Legacy compatibility:
- Archive-only `opencode` references remain historical.
- Current CLI behavior must remain stable after the rename.

## 7. Public Portal

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

## 8. Carriers And Adapters

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
- Hardware and network projection require accepted receipt, declared effect, role/scope permission, and bridge authorization.

## 9. Repository And Version Policy

Root repository docs govern the runtime repository:
- `README.md`
- `AGENTS.md`
- `SKILLS.md`

Agent docs govern public web-portal use only.

Versioning model:
- File diffs are carriers.
- OMI-Lisp declarations are candidates.
- Validation determines acceptance.
- Receipts are accepted version identity.
- `.imo` files may carry normalized candidate or accepted declarations.

## 10. Required Tests

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
- candidate validation and rejection
- projection denial before receipt
- same input produces same output

## 11. Non-Goals

Do not:
- add dependencies without approval
- introduce nondeterministic computation
- move carrier I/O into OMI, Tetragrammatron, or Metatron
- let parser output perform side effects
- treat bridge recognition, carrier decode, or projection as acceptance
- implement partial JABCode while calling it standards-compliant
- collapse portal public-interface policy into root repository policy

## 12. Completion Standard

The project is specification-complete when:
- OMI-Lisp notation can be parsed into deterministic candidate relations.
- Candidates validate through Tetragrammatron.
- Accepted receipts record identity in the ring.
- Metatron projections derive only from accepted receipts.
- IMO carries input/output without owning truth.
- The public portal can bootstrap LLM agents from `agent-docs/` without overriding root repository policy.
