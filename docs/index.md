# OMI Tetragrammatron Documentation

OMI Tetragrammatron is a deterministic autonomous seed runtime centered on `opencode.c`, a single-file C artifact that combines an OMI address runtime, S-expression reducer, nibble CPU, fixed receipt ring, autonomous ring-fold loop, and digital twin geometry renderer.

The project boundary is:

```text
encode determinism
do not encode nondeterminism
```

## Documentation map

- [Getting started](getting-started.md): build, run, and smoke-test the runtime.
- [Architecture](architecture.md): deterministic core, carrier behavior, and repository layout.
- [Command reference](cli.md): all `opencode.bin` modes and their expected outputs.
- [Geometry and ring](geometry-and-ring.md): receipt ring, Fano routing, BQF, Polybius, and Hopf projection.
- [Deterministic geometry validation](deterministic-geometry.md): `--check`, Fano incidence, shape integrity, dodecahedron edges, and φ derivation.
- [Tetrahedral π carry](tetrahedral-pi.md): π as an internal geometry-of-motion carry between discrete OMI states and smooth projection.
- [Viewer](viewer.md): WebGL digital twin viewer and HTTP endpoints.
- [S-expressions and OMI frames](sexpr-and-omi.md): accepted input shapes, receipts, lanes, and address execution.
- [Runtime data model](runtime-data-model.md): major in-memory structures and how state moves through the runtime.
- [Examples](examples.md): copy-paste workflows for evaluation, file runs, frames, images, and serving.
- [Development notes](development.md): invariants, documentation workflow, and safe extension points.
- [Troubleshooting](troubleshooting.md): common build, ring, server, and viewer issues.
- [HTTP and render API](http-and-render-api.md): server routes, JSON frame shape, ring JSON, and PPM output.
- [Determinism guide](determinism.md): what must remain deterministic and what is only carrier behavior.
- [Smoke-test matrix](smoke-test-matrix.md): quick command matrix for validating runtime modes.
- [Glossary](glossary.md): project terms and symbols.

## Current artifact

The source of authority is `opencode.c`. Build it into `opencode.bin` with a C compiler and the math library:

```bash
gcc -O2 -Wall -Wextra -Wno-misleading-indentation -o opencode.bin opencode.c -lm
```

The compiled binary can evaluate expressions, execute OMI frames, fold receipts into a fixed ring, run autonomous mode, render JSON/PPM frames, and serve the WebGL viewer.

## What is deterministic

The deterministic layer includes parsing, reduction, FNV-1a hashing, rotations, XOR, the delta law, ring indexing, ring folds, Fano routing, BQF, Polybius mapping, Hopf projection, receipt generation, JSON rendering, and PPM rendering.

Physical carriers such as terminal input, file writes, signals, process lifetime, filesystem persistence, and the browser rendering surface are permitted but are not computation authority.
