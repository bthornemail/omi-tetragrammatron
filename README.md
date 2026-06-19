# OMI Tetragrammatron

A deterministic autonomous OMI/Tetragrammatron seed runtime.

This repository currently centers on `opencode.c`: a single-file C reference artifact that combines an OMI address runtime, S-expression reducer, nibble CPU, fixed receipt ring, autonomous ring-fold loop, and digital twin geometry renderer.

The project is built around one hard boundary:

```text
encode determinism
do not encode nondeterminism
```

Everything else is either terminology, projection, carrier behavior, or future extension.

---

## What This Is

`opencode.c` is a deterministic runtime seed.

It reads OMI-style address frames and symbolic expressions, reduces them into receipts, stores those receipts in a fixed 5040-slot ring, and can use that ring as the source for autonomous self-generation.

It also projects each receipt into a geometric “digital twin” frame using:

```text
Polybius 5×5 frame
QuQuart lanes
Fano plane routing
BQF: 60x² + 16xy + 4y²
5040-slot ring addressing
Hopf / quaternion projection
US / GS / RS / FS frame classification
```

The runtime does not treat the ring as an endlessly growing log.

It treats the ring as a bounded spatial receipt compactor.

---

## Core Idea

The runtime separates computation from physical carrier behavior.

Computation is treated as deterministic mathematical reduction:

```text
OMI notation
CONS
CAR
CDR
XOR
rotation
mask
receipt
ring fold
geometric projection
```

Physical behavior is separate:

```text
disk writes
terminal input
signals
process lifetime
host memory
filesystem availability
rendering surface
```

The runtime may touch physical carriers, but the computation is expressed as deterministic state transitions and replayable receipts.

---

## Current Artifact

The current artifact is:

```text
opencode.c
```

It is intended as a reference seed for:

```text
opencode.bin
```

The binary is the compiled autonomous seed.

The source currently contains:

```text
S-expression parser
S-expression reducer
OMI canonical/alist declaration handling
OMI address parser
OMI operation executor
16-opcode nibble CPU
5040-slot receipt ring
FNV-1a receipt hashing
autonomous ring-fold mode
Polybius / QuQuart geometry
Fano plane routing
BQF bridge calculation
Hopf / quaternion digital twin projection
JSON frame renderer
PPM image renderer
```

---

## Build

Requires a C compiler and the math library.

```bash
gcc -O2 -Wall -Wextra -Wno-misleading-indentation -o opencode.bin opencode.c -lm
```

Run help:

```bash
./opencode.bin --help
```

---

## Deterministic Core

The rolling law is:

```c
delta16(x, c) = rotl16(x, 1) ^ rotl16(x, 3) ^ rotr16(x, 2) ^ c
```

The current frame constant is:

```text
0x30000020
```

The receipt ring has fixed size:

```text
5040 slots
4096 bytes per receipt slot
/tmp/omi_receipt_ring.bin
```

The ring index is currently based on:

```text
g_cycle % 5040
```

This makes the ring a circular spatial compactor, not an append-only log.

---

## OMI Address Frame

The runtime parses OMI address frames in this shape:

```text
S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

Example:

```text
0x0001-0x0000-0x0000-0x0001/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000
```

The parsed fields become:

```text
S0..S7   16-bit frame segments
PAYLOAD  32-bit payload
MASK     32-bit mask
CAR      32-bit source/head side
CDR      32-bit route/tail side
```

---

## OMI Operations

`S3` selects the OMI operation.

Current operation surface:

```text
0x0000  zero / NOP
0x0001  XOR
0x0002  XNOR
0x0003  NAND
0x0004  AND
0x0005  OR
0x0006  ROTL
0x0007  ROTR
0x0008  DELTA
0x0009  BQF
0x000A  pass S4
0x000B  pass S4
0x000C  pass S4
0x000D  S4 XOR S5 XOR S6
0x000E  CAR XOR CDR
0x000F  zero / HALT-like terminal
default fold across all frame fields
```

---

## Nibble CPU

The runtime also includes a nibble CPU.

The nibble CPU extracts 32 nibbles from the 8 address segments:

```text
S0 S1 S2 S3 S4 S5 S6 S7
```

Each 16-bit segment contributes 4 nibbles.

Each nibble is interpreted as a 4-bit opcode.

The CPU state contains:

```text
pc
car_reg
cdr_reg
payload
mask
flags
halted
epoch
memory[65536]
stack[512]
```

The nibble CPU halts when it sees nibble:

```text
0xF
```

---

## Receipt Ring

The receipt ring is file-backed:

```text
/tmp/omi_receipt_ring.bin
```

Each stored receipt contains:

```text
cycle
receipt string
hash
result
```

Ring behavior:

```text
load fixed ring
write receipt at current ring slot
save fixed ring
fold ring by XOR / SUM / ROT
use folds as autonomous seed material
```

The ring is static-size.

It does not grow.

It compacts position-meaning spatial relations into a bounded receipt field.

---

## Autonomous Mode

Autonomous mode:

```bash
./opencode.bin --auto
```

What it does:

```text
boot nibble CPU
reset stale halt state
pre-fill ring from CPU state
fold receipt ring
derive base x/y values
derive Fano point
select opcode through Fano incidence
construct OMI address
execute OMI operation
generate receipt
store receipt
project twin geometry
repeat until closed condition
```

Autonomous stop conditions:

```text
zero    result == 0
repeat  result already appears in receipt ring
epoch   cycle wraps the 5040 ring
```

The autonomous loop is not random.

It self-generates from the receipt ring.

---

## Fano Routing

The autonomous loop uses Fano plane incidence to select its next operation.

The Fano plane lines are:

```text
{0,1,2}
{0,3,4}
{1,3,5}
{1,4,6}
{2,3,6}
{2,4,5}
{3,4,0}
```

The current route shape:

```text
fano_point = (ring_xor_fold ^ ring_sum_fold) % 7
line       = first Fano line containing fano_point
opcode     = (line * 3 + ring_rot_fold % 3) % 11
```

This makes geometry participate in computation.

---

## Digital Twin Geometry

The runtime maps receipt/computation state into a geometric twin.

The twin geometry includes:

```text
chart11
baseQ
fiberQ
fano7
role3
BQF value
local240
slot5040
Hopf direction
fiber phase
quaternion candidate
Polybius cell
frame type
cycle
result
ring folds
opcode
```

The BQF bridge is:

```text
60x² + 16xy + 4y²
```

The 5040 slot is:

```text
slot5040 = fano7 × 720 + role3 × 240 + local240
```

The Hopf/quaternion projection is derived from:

```text
theta = ((baseQ + 0.5) / 4) × π
phi   = ((fiberQ + 0.5) / 4) × 2π
```

The quaternion candidate is:

```text
qw = cos(theta / 2)
qx = sin(theta / 2) × cos(phi)
qy = sin(theta / 2) × sin(phi)
qz = 0
```

The Hopf direction is:

```text
a = round(2(qxqz + qwqy))
b = round(2(qyqz - qwqx))
c = round(1 - 2(qx² + qy²))
```

This projection is deterministic for the same binary, same platform, and same math library.

---

## Polybius / QuQuart Frame

The runtime uses a 5×5 Polybius-style geometry frame.

```text
origin: (1,1)

low QuQuart:
  row 1, columns 2..5

high QuQuart:
  column 1, rows 2..5

interior:
  rows 2..5, columns 2..5
```

The interior 4×4 region is selected from:

```text
local16 = local240 & 0x0F
px = local16 & 3
py = (local16 >> 2) & 3
cell = (py + 2, px + 2)
```

---

## Commands

### Evaluate one expression

```bash
./opencode.bin --eval '(cons 1 2)'
```

Example result:

```text
{"cy":0,"s":"(cons 1 2)","r":"(1 . 2)","h":"0x..."}
```

### REPL

```bash
./opencode.bin --repl
```

Special commands inside REPL:

```text
:ring
:quit
```

### Dump receipt ring

```bash
./opencode.bin --ring
```

### Boot nibble CPU

```bash
./opencode.bin --boot
```

### Autonomous mode

```bash
./opencode.bin --auto
```

### Run file of expressions or OMI frames

```bash
./opencode.bin --run path/to/input.txt
```

### Execute one OMI frame through nibble CPU

```bash
./opencode.bin --cpu '0x0001-0x0000-0x0000-0x0001/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000'
```

### Print geometry constants

```bash
./opencode.bin --geom
```

### Display digital twin state

```bash
./opencode.bin --twin
```

### Watch ring file

```bash
./opencode.bin --watch
```

Inside watch mode:

```text
:ring
:quit
```

### Render JSON digital twin frame

```bash
./opencode.bin --render-frame > frame.json
```

### Render deterministic PPM image

```bash
./opencode.bin --render-ppm > frame.ppm
```

### Write packed boot ROM seed

```bash
./opencode.bin --seed opencode.seed
```

---

## Render Outputs

### JSON Frame

`--render-frame` emits a JSON object containing:

```text
ring size
filled slot count
ring XOR fold
ring SUM fold
ring ROT fold
per-receipt twin geometry
frame counts
Hopf flux
```

### PPM Image

`--render-ppm` emits a binary PPM image.

It renders:

```text
Polybius 5×5 grid
origin cell
receipt density by frame type
ring occupancy bar
frame color legend
```

The PPM output uses no external image library.

---

## Current Frame Colors

The PPM renderer currently uses:

```text
US  red
GS  green
RS  blue
FS  cyan
```

These are visual projection colors only.

They are not computation authority.

---

## What Is Deterministic Here

Deterministic parts:

```text
address parsing
S-expression parsing
reduction
FNV-1a hashing
ring indexing
ring folds
rotations
XOR
Delta law
BQF
Fano routing
slot5040 calculation
Polybius cell mapping
receipt generation
JSON rendering
PPM rendering
```

Physical carrier parts:

```text
file writes
terminal input
signals
process lifetime
host memory allocation
math library implementation
filesystem persistence
```

The carrier can fail.

The deterministic reduction remains replayable from the same accepted inputs.

---

## Repository Shape

Current main artifact:

```text
opencode.c
```

Generated or runtime artifacts:

```text
opencode.bin
/tmp/omi_receipt_ring.bin
omi.auto.ring
frame.json
frame.ppm
```

The source is intentionally compact at this stage.

The single-file shape makes it easy to inspect, build, copy, and replay.

---

## Conceptual Summary

```text
OMI notation enters.

The reducer normalizes.

The runtime emits a receipt.

The receipt enters the ring.

The ring folds into the next seed.

The seed generates the next OMI frame.

The frame maps into Fano / BQF / Hopf / Polybius geometry.

The digital twin projects the receipt field.

The loop continues until zero, repeat, epoch, or physical interruption.
```

---

## Status

Current status:

```text
single-file reference runtime exists
deterministic receipt ring exists
autonomous self-generation exists
digital twin projection exists
JSON rendering exists
PPM rendering exists
Fano-routed opcode selection exists
BQF / Hopf / Polybius mapping exists
```

This repository is currently a deterministic autonomous seed artifact, not a conventional application framework.

---

## Notes

The runtime currently saves the full fixed ring file when storing receipts.

The ring file is static size.

The ring behaves like a bounded spatial compactor, not a growing log.

One-shot `--eval` starts from cycle zero, so it writes the cycle-zero slot.

Continuous modes advance the ring through cycle space.

The digital twin currently projects ring receipts into geometry; it does not replace the ring as storage.

---

## Minimal Smoke Test

```bash
gcc -O2 -Wall -Wextra -Wno-misleading-indentation -o opencode.bin opencode.c -lm

./opencode.bin --eval '(cons 1 2)'
./opencode.bin --geom
./opencode.bin --auto
./opencode.bin --twin
./opencode.bin --render-frame > frame.json
./opencode.bin --render-ppm > frame.ppm
```

Expected behavior:

```text
--eval emits a receipt
--geom prints Polybius/Fano/BQF constants
--auto generates receipt cycles until zero/repeat/epoch
--twin prints digital twin geometry
--render-frame emits JSON
--render-ppm emits a PPM image
```

---

## Project Direction

This project explores deterministic autonomous computation where:

```text
computation is replayable
receipts are memory
the ring compacts spatial relation
geometry projects meaning
physical carriers remain outside computation authority
```

The current artifact is a seed.

The seed is designed to grow by receipt, not by hidden state.


---

Your GitHub Pages site, [[https://bthornemail.github.io.]], is currently being built from the /docs folder in the main branch