# Project Codebase Outline

Status: active standard codebase outline  
Specification companion: `dev-docs/PROJECT_SPECIFICATON.md`  
Language source: `dev-docs/archive/OMI-Lisp_Complete_Specification.md`

This file describes the structure of the current project codebase. It is not the verbose update roadmap. For planned migration work, use `dev-docs/PROJECT_UPDATE_OUTLINE.md`.

## 1. Root Runtime

The C runtime is organized around four authority modules and one thin application/orchestration file.

```text
omi.h / omi.c
tetragrammatron.h / tetragrammatron.c
metatron.h / metatron.c
imo.h / imo.c
opencode.c        legacy application name; target rename is omicron.c
```

Current build target:

```text
opencode.bin
```

Target application name:

```text
omicron.bin
```

The runtime rename is pending. Until then, `opencode.c`, `opencode.o`, and `opencode.bin` are the current working application files.

## 2. OMI Module

Files:

```text
omi.h
omi.c
```

Authority:

```text
Citation
```

Owns:

- fixed-width deterministic helpers
- FNV-1a hashes
- OMI address parsing
- one-long-word instruction fields
- 16 opcode execution
- nibble CPU
- current arena/gauge/bridge scaffold APIs
- bitboard masks and bridge constants

Does not own:

- validation
- receipt acceptance
- projection/rendering
- file/network/device transport

## 3. Tetragrammatron Module

Files:

```text
tetragrammatron.h
tetragrammatron.c
```

Authority:

```text
Validation
```

Owns:

- Polybius layout
- Fano routing data
- slot5040 computation
- 5040-slot receipt ring
- ring folds: XOR, SUM, ROT
- receipt export/import as pure memory
- chiral diagonal law

Does not own:

- disk serialization
- parsing carrier syntax
- projection/rendering
- browser or hardware adapters

## 4. Metatron Module

Files:

```text
metatron.h
metatron.c
```

Authority:

```text
Projection
```

Owns:

- geometry constants and derived constants
- `SHAPE_DB[21]`
- seed solids and incidence data
- vertex resolution
- Hopf/quaternion route geometry
- Smith state
- JSON frame, PPM, SVG, OBJ, and glTF render surfaces
- incidence self-check

Does not own:

- validation or receipt acceptance
- carrier parsing
- file/network authority

## 5. IMO Module

Files:

```text
imo.h
imo.c
```

Authority:

```text
Carrier
```

Owns:

- ring save/load file I/O
- `.omi` ring export
- S-expression parser/compiler/reducer
- OMI lane collection for FS/GS/RS/US
- receipt JSON generation
- SHA-1 and base64 helpers
- HTTP/SSE/WebSocket server
- BOOT_ROM
- signal handling
- allocation helpers

Does not own:

- validation truth
- geometry interpretation
- citation identity

## 6. Application / CLI

Current file:

```text
opencode.c
```

Target file:

```text
omicron.c
```

Current role:

- thin application entry point
- CLI mode dispatch
- runtime orchestration across OMI, Tetragrammatron, Metatron, and IMO
- web server entry mode

Main modes:

```text
--eval <s>
--repl
--auto
--boot
--run <file>
--cpu <addr>
--geom
--seed <path>
--ring
--watch
--twin
--render-frame
--render-ppm
--render-obj
--render-gltf
--smith
--check
--serve [port]
```

Rename rule:

- Keep behavior stable while renaming `opencode` to `omicron`.
- Update `Makefile`, smoke tests, docs, and help output together.

## 7. Build And Generated Artifacts

Build file:

```text
Makefile
```

Current source objects:

```text
omi.o
tetragrammatron.o
metatron.o
imo.o
opencode.o
```

Current binary:

```text
opencode.bin
```

Generated/local artifacts:

```text
frame.json
frame.ppm
omi.auto.ring
proof/*.vo
proof/*.glob
proof/*.vok
proof/*.vos
scripts/test-c-scaffold
```

Project convention currently commits source and binary together. Generated test executables should be removed unless intentionally committed.

## 8. Proofs

Files:

```text
proof/phi_proof.v
```

Build commands:

```text
make coq
cd proof && coqc phi_proof.v
```

Proves:

- golden ratio relation
- Schläfli `{3,5}` implication for phi
- rational PI bounds

## 9. Tests

Make targets:

```text
make smoke
make test-c
make test-js
make coq
```

C test files:

```text
scripts/test-c-scaffold.c
```

JS test files:

```text
scripts/test-jabcode-carrier.js
scripts/test-receipt-surface-cascade.js
scripts/test-gnomonic-surface.js
```

Smoke coverage:

- S-expression eval
- geometry constants
- boot ROM
- twin summary
- incidence self-check
- frame rendering
- PPM rendering
- C scaffold checks
- JS viewer/carrier/surface checks

## 10. Viewer Web Portal

Directory:

```text
viewer/
```

Main page:

```text
viewer/index.html
```

Viewer scripts:

```text
viewer/geometry-decoder.js
viewer/ring-loader.js
viewer/twin-viewer.js
```

Public viewer modules:

```text
viewer/public/device-inspector.js
viewer/public/flasher.js
viewer/public/gnomonic-surface.js
viewer/public/jabcode-carrier.js
viewer/public/receipt-surface-cascade.js
viewer/public/web-serial.js
```

Viewer role:

- public web-portal interface
- receipt and geometry inspection
- browser-side carrier/projection experiments
- LLM-agent bootstrap surface for published viewer usage

Viewer does not replace root repository policy.

## 11. Viewer Public-Interface Docs

Viewer docs are separate from root docs.

Files:

```text
viewer/AGENTS.md
viewer/SKILLS.md
viewer/docs/ADAPTERS.md
viewer/docs/REPO.md
viewer/README Addendum - Bootstrapping an OMI-Compatible LLM Agent.md
viewer/README Addendum - Emergent P2P and Multimedia Use Cases for OMI.md
```

Target:

```text
viewer/README.md
```

Purpose:

- public LLM-agent bootstrapping through the viewer portal
- viewer-local adapter and collaboration policy
- public interface guidance

Boundary:

- Do not merge viewer policy into root `AGENTS.md` or root `SKILLS.md`.
- Do not let viewer docs override root repository instructions.

## 12. Root Documentation

Root active docs:

```text
README.md
AGENTS.md
SKILLS.md
```

Developer docs:

```text
dev-docs/PROJECT_SPECIFICATON.md
dev-docs/PROJECT_OUTLINE.md
dev-docs/PROJECT_UPDATE_OUTLINE.md
```

Archive:

```text
dev-docs/archive/
```

Temporary/source PDFs:

```text
dev-docs/temp/
```

Archive rule:

- `dev-docs/archive/` is source material.
- Active requirements must be pulled into active docs before implementation.

## 13. OMI-Lisp Language Source

Primary language source:

```text
dev-docs/archive/OMI-Lisp_Complete_Specification.md
```

Core notation:

```text
omi---imo
o---o/---/?---?@---@
o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

Core rule:

```text
Code is data.
File is port.
Notation is citation.
Canon validates.
Cosmology projects.
Receipt accepts.
```

Implementation status:

- Current S-expression evaluator exists in IMO.
- Full OMI-Lisp notation parser is pending.
- Parser output must create candidates only, not accepted receipts.

## 14. Carrier Policy

Active canonical carrier language:

```text
OMI citation notation
```

JABCode status:

- `viewer/public/jabcode-carrier.js` exists as a debug/development carrier.
- JABCode must not be treated as standards-compliant unless implemented from `dev-docs/archive/JabCode.BSI-TR-03137.pdf`.
- If official compliance is not implemented, JABCode remains out of the active carrier roadmap.

Carrier boundary:

- Carrier decode is not acceptance.
- Projection is not acceptance.
- Only validation plus receipt accepts.

## 15. Determinism Guardrails

Forbidden in deterministic computation:

```text
rand
random
srand
time
getpid
clock
uuid
/dev/urandom
```

Also avoid:

- host pointer addresses as canonical identity
- hidden environment state
- nondeterministic ordering

Permitted as carrier behavior:

- terminal input
- file I/O
- signals
- network transport
- browser/device I/O

## 16. Standard Work Order

For codebase changes:

1. Keep existing CLI behavior passing.
2. Update focused source modules only.
3. Add or update local tests for changed behavior.
4. Run `make smoke`.
5. Run `make coq` when proof-facing behavior changes.
6. Run deterministic banned-call scan for runtime source.
7. Keep root docs, viewer docs, and archive context separate.

## 17. Pending Structural Work

Known structural changes still pending:

- Rename `opencode` application files and build target to `omicron`.
- Merge viewer README addenda into `viewer/README.md`.
- Implement full OMI-Lisp notation parser.
- Connect candidates to Tetragrammatron validation and receipt storage.
- Enforce projection gates across adapters.
- Decide whether to implement official JABCode compliance or leave it out of active carriers.
