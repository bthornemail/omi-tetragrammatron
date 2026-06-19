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
- [Viewer](viewer.md): WebGL digital twin viewer and HTTP endpoints.

## Current artifact

The source of authority is `opencode.c`. Build it into `opencode.bin` with a C compiler and the math library:

```bash
gcc -O2 -Wall -Wextra -Wno-misleading-indentation -o opencode.bin opencode.c -lm
```

The compiled binary can evaluate expressions, execute OMI frames, fold receipts into a fixed ring, run autonomous mode, render JSON/PPM frames, and serve the WebGL viewer.

## What is deterministic

The deterministic layer includes parsing, reduction, FNV-1a hashing, rotations, XOR, the delta law, ring indexing, ring folds, Fano routing, BQF, Polybius mapping, Hopf projection, receipt generation, JSON rendering, and PPM rendering.

Physical carriers such as terminal input, file writes, signals, process lifetime, filesystem persistence, and the browser rendering surface are permitted but are not computation authority.
