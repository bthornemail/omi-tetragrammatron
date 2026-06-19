# Architecture

## Source layout

```text
opencode.c     single-file merged runtime
viewer/        WebGL frontend assets
SKILLS.md      full OMI runtime specification
AGENTS.md      coding-agent instructions and invariants
README.md      repository overview
```

`opencode.c` is the computation authority. It contains the parser, reducer, OMI address executor, nibble CPU, receipt ring, autonomous loop, geometry projection, renderers, and HTTP server.

The `viewer/` directory is a visualization layer. It reads JSON from `opencode.bin --serve` and renders the digital twin in the browser.

## Computation layer

The computation layer is deterministic. Given the same accepted input and compatible platform math behavior, it produces the same receipts and projections.

Major pieces:

- S-expression parser and reducer.
- OMI canonical and alist declaration handling.
- OMI address parser.
- OMI operation executor.
- 16-opcode nibble CPU.
- FNV-1a receipt hashing.
- Fixed-size receipt ring.
- Autonomous ring-fold mode.
- Polybius, QuQuart, Fano, BQF, and Hopf geometry.
- JSON and PPM renderers.

## Carrier layer

Carrier behavior moves bytes but does not define computation. Examples include disk writes, terminal input, signals, process lifetime, HTTP sockets, browser rendering, and filesystem availability.

The runtime may use these carriers to receive input, persist the ring, emit frames, or serve viewer assets, but deterministic reduction remains replayable from the same accepted inputs.

## Determinism rule

Do not add computation that depends on nondeterministic sources such as random numbers, clocks, process IDs, UUID generators, or entropy devices. If a future feature needs changing deterministic guarantees, it should be treated as a design decision rather than an incidental implementation detail.
