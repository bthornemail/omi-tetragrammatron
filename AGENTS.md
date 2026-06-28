AGENTS.md — instructions for coding agents working on this project

## Core constraint

encode determinism
do not encode nondeterminism

Never use: rand, time, getpid, clock, uuid, random, srand, /dev/urandom
or any source of non-determinism in computation.

I/O (terminal input, file writes, signals, network) is carrier behavior,
not computation. It is permitted but must not affect deterministic output.

## Authorities

The system has four authorities, two orthogonal axes, and one invariant.

```
                    Metatron
             (Projection Authority)
                     ▲
                     │
                     │
OMI ◄──────────── Receipt ────────────► IMO
(Citation)                               (Carrier)
                     │
                     │
                     ▼
             Tetragrammatron
           (Validation Authority)
```

### Horizontal axis — Representation

Answers: *How is something carried?*

| Module | Authority | Preserves | Never Changes |
|--------|-----------|-----------|---------------|
| OMI    | Citation  | Identity  | Interpretation |
| IMO    | Carrier   | Representation | Identity |

OMI creates invariant citations. IMO adapts that invariant into whatever
representation is needed. Neither decides truth. They only preserve identity.

### Vertical axis — Interpretation

Answers: *What does an accepted citation become?*

| Module | Authority | Preserves | Never Changes |
|--------|-----------|-----------|---------------|
| Tetragrammatron | Validation | Truth | Identity |
| Metatron | Projection | Interpretation | Truth |

Tetragrammatron decides whether something is accepted. Metatron decides
how an accepted receipt is projected. Neither changes identity.

### The receipt — sole shared invariant

The receipt is the only object every authority may share. It is simultaneously
the protocol witness, the provenance record, the replay unit, the synchronization
unit, the geometric seed, the symbolic seed, and the transport payload.

Everything else is private to its module.

## Source layout

Four modules, single authority each, orchestrated by omicron.c:

- core/omi.h/.c    — Citation Authority: addresses, registers, CONS/CAR/CDR, CID,
                     16 opcodes, nibble CPU, address parser/formatter, FNV-1a.
                     Produces OMI Cells. Never validates. Never projects. Never transports.
- core/tetragrammatron.h/.c — Validation Authority: DeltaC, Polybius, Diagonal Law, Rotation,
                     Governor, 5040 ring, acceptance, three folds, tetragrammatron_export/import
                     (pure memory). Produces Receipts. Never renders. Never serializes. Never parses.
- core/metatron.h/.c — Projection Authority: all geometry (seed crystal tables, SHAPE_DB[21],
                     vertex coordinates, resolve_vertex, cite_to_sphere, quaternion/Hopf,
                     Schläfli, Betti), 6 renderers (frame/PPM/SVG/OBJ/glTF/smith),
                     check_incidence. Produces Surfaces. Never validates. Never transports.
- core/imo.h/.c    — Carrier Authority: ring_save/load (file I/O), S-expression parser/
                     compiler/reducer, HTTP/SSE/WebSocket server, SHA-1, BOOT_ROM[21],
                     signal handling, xmalloc family. Produces Runtime Representations.
                     Never validates. Never composes geometry. Never changes identity.
- core/omicron.c   — Thin orchestration: main() dispatch to all 4 authorities.
- proof/phi_proof.v — Coq proof: Schläfli symbol {3,5} forces golden ratio φ
- portal/          — React + TypeScript + Vite public web portal
- agent-docs/      — public agent bootstrap docs
- SKILLS.md        — full OMI runtime specification
- AGENTS.md        — this file

## Build

make              # build binary + Coq proof
make coq          # build Coq proof only (proof/phi_proof.vo)
make smoke        # full smoke test

## Coq proof

cd proof && coqc phi_proof.v  # produces proof/phi_proof.vo

Proves:
  phi_sq_eq           — φ² = φ + 1
  icosa_forces_phi    — {3,5} edge-length equality forces φ
  pi_gt_three / pi_lt_four — rational bounds on PI via Leibniz series

## Code style

- No comments (unless absolutely necessary)
- Single-letter variable names in hot paths
- Run-on if/for statements without braces for simple guards
- FNV-1a for all hashing
- u16/u32 macros for wrapping
- Deterministic math only (cos/sin from -lm is allowed — pure function)

## Git conventions

- Commit source + binary together
- Commit message: "Add/Update <feature> — <reason>"
- Do not commit other agent's work (lib/, public/, SKILLS.md originally)
  unless explicitly asked

## Architecture flow

User → IMO → OMI → Tetragrammatron → Receipt → Metatron → IMO → Surface

IMO orchestrates pipeline, calls Metatron for projection.
Tetragrammatron provides tetragrammatron_export/import (pure memory);
IMO calls these for ring_save/load (disk serialization).

## Modes in omicron.c

--eval <s>      evaluate one S-expression
--repl          interactive REPL
--auto          autonomous ring mode (verbose)
--boot          run 21-stage boot ROM on nibble CPU
--run <file>    process OMI addresses from file
--cpu <addr>    execute single OMI address on nibble CPU (0x-prefixed hex)
--geom          print geometry constants
--seed <path>   write packed boot ROM binary
--ring          dump receipt ring
--watch         live ring monitor (polls file)
--twin          digital twin universe summary
--render-frame  JSON geometry frame to stdout
--render-ppm    PPM image to stdout
--render-obj    OBJ mesh to stdout
--render-gltf   glTF JSON to stdout
--smith         Smith SVG to stdout
--check         incidence self-check
--serve [port]  HTTP server for portal build (default 8080)

## Key formulas

delta16(x, c) = rotl16(x,1) ^ rotl16(x,3) ^ rotr16(x,2) ^ c
BQF(x, y) = 60x^2 + 16xy + 4y^2
slot5040 = fano7 * 720 + role3 * 240 + local240
Hopf: quaternion from theta/phi, thrust from quat components

## Ring

Fixed 5040 slots, 4096 bytes each, file at /tmp/omi_receipt_ring.bin
Indexed by g_cycle % RING_SIZE
Three folds: XOR, SUM, ROT

## Fano routing (autonomous mode)

fano_pt = (xor_fold ^ sum_fold) % 7
line    = first Fano line containing fano_pt
opcode  = (line * 3 + rot_fold % 3) % 11 -> opcodes[opcode]

## When in doubt

Ask the user before:
- Adding dependencies
- Changing deterministic guarantees
- Committing other agent's work
