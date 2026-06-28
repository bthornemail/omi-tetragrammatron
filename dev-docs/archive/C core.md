Yes. The clean way to read the C core is that these four files are **not four random implementation files**. They are the four authority modules:

```text
omi.c              = Citation / address / deterministic instruction witness
tetragrammatron.c  = Validation / receipt ring / finite governor
metatron.c         = Projection / geometry / surfaces
imo.c              = Carrier / files / HTTP / S-expression runtime / ports
```

The README already states this authority split: `omi.c` owns OMI addresses, CPU, and FNV-1a; `tetragrammatron.c` owns ring, folds, and acceptance; `metatron.c` owns geometry, renderers, and incidence; `imo.c` owns HTTP, parser, and serialization.

The full flow is:

```text
IMO receives input
  ↓
OMI parses / cites / executes deterministic address witness
  ↓
Tetragrammatron records / folds / validates receipt ring state
  ↓
Metatron projects accepted or inspectable state into geometry/surface
  ↓
IMO carries that projection back out as stdout, file, HTTP, SSE, WebSocket, etc.
```

## 1. `omi.c` — Citation and deterministic instruction core

`omi.c` is the **address-ruler and deterministic micro-operation layer**.

It does not own HTTP, files, geometry, browser views, or ring persistence. Its job is to take an OMI address frame and make it computable.

At the top, it defines the core bit laws:

```c
uint16_t u16(uint32_t x)
uint32_t rotl32(uint32_t x, int n)
uint32_t rotr32(uint32_t x, int n)
uint16_t rotl16(uint16_t x, uint16_t n)
uint16_t rotr16(uint16_t x, uint16_t n)
uint16_t delta16(uint16_t x, uint16_t c)
uint32_t bqf32(uint32_t x, uint32_t y)
```

The important laws are:

```text
delta16(x,c) = rotl16(x,1) ^ rotl16(x,3) ^ rotr16(x,2) ^ c

bqf32(x,y) = 60x² + 16xy + 4y²
```

Those are implemented directly in `omi.c`, along with FNV-1a hashing helpers.

### Address parsing

`parse_omi_addr()` parses the long OMI frame:

```text
S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

into an `OmiInst`.

It expects each field as a hex token like:

```text
0x0000-0x0000-0x0000-0x0000/...
```

So this file is where notation becomes a structured citation object:

```text
text address → OmiInst
```

That is why `omi.c` is the **citation authority**. It makes the address frame legible to the runtime. It does not decide final truth.

### `execute_omi_op()`

This is the stateless operation resolver. It uses `inst->s3` as the operation selector and computes a 16-bit result.

The important cases are:

```text
0x0000 → zero
0x0001 → s4 XOR s5
0x0002 → XNOR
0x0003 → NAND
0x0004 → AND
0x0005 → OR
0x0006 → rotl16(s4,s5)
0x0007 → rotr16(s4,s5)
0x0008 → delta16(s4,s5)
0x0009 → BQF fold from s4/s5 byte parts
0x000d → s4 XOR s5 XOR s6
0x000e → CAR XOR CDR
0x000f → zero / halt-like surface
default → XOR fold across all instruction fields
```

So `execute_omi_op()` is where the address frame becomes an **instruction witness**. It still does not validate the receipt. It only computes the witnessed result.

### Nibble CPU

`omi.c` also contains a tiny nibble CPU:

```c
cpu_exec(CpuState *cpu, uint8_t op)
extract_nibbles(...)
cpu_run(...)
cpu_init(...)
```

`cpu_run()` extracts 32 nibbles from `S0..S7` and runs those nibbles as 4-bit CPU operations. The CPU has:

```text
payload
mask
car_reg
cdr_reg
memory
pc
stack
flags
halted
epoch
```

The nibble opcodes include:

```text
0x0 no-op
0x1 shift CAR/CDR/PAYLOAD relation
0x2 payload = CAR
0x3 payload = CDR
0x4 payload ^= mask
0x5 payload |= mask
0x6 payload &= mask
0x7 rotate payload left
0x8 memory read
0x9 memory write
0xA jump
0xB call-like jump
0xC push
0xD pop
0xE compare
0xF halt
```

This is not a full OS CPU yet. It is a deterministic **address-ruler execution model**. It lets an OMI address carry a sequence of micro-steps inside its own nibbles.

So the practical summary is:

```text
omi.c parses notation into OmiInst.
omi.c computes deterministic instruction witnesses.
omi.c owns delta16 and BQF.
omi.c owns the tiny nibble CPU.
omi.c does not accept receipts.
```

## 2. `tetragrammatron.c` — Validation governor and receipt ring

`tetragrammatron.c` is the **finite receipt governor**.

It owns:

```text
5040 ring
Fano lines
Polybius/ququart layout
slot5040 computation
ring folds
ring import/export
chirality decision
```

At the top it defines the global ring:

```c
RingSlot ring[RING_SIZE];
uint64_t g_cycle = 0;
volatile int g_running = 1;
```

It also defines the Fano incidence table:

```c
FANO_LINES[7][3]
FANO_PT_LINES[7][3]
CONFIG_MATRIX[7]
QUQUART_PHASE[4]
```

The Fano lines and Polybius/ququart helpers are the finite routing surface. The code has helpers for origin, low ququart, high ququart, interior cells, rail cells, and interior detection.

### `compute_slot5040()`

This is one of the most important functions:

```c
uint32_t compute_slot5040(int fano7, int role3, int local240) {
    return (fano7 % 7) * 720
         + (role3 % 3) * 240
         + (local240 % 240);
}
```

Protocol meaning:

```text
7 Fano positions × 3 roles × 240 local fold = 5040 slots
```

That is the finite ring address for projection/receipt indexing.

### Ring storage

`tetragrammatron.c` stores receipts in the circular ring:

```c
ring_idx()
ring_store()
ring_dump()
ring_has_receipt()
count_filled_ring_slots()
tetragrammatron_export()
tetragrammatron_import()
```

The ring functions make the receipt surface durable inside memory:

```text
hash + receipt string + cycle → RingSlot
```

It also computes three folds across the whole ring:

```c
ring_xor_fold()
ring_sum_fold()
ring_rot_fold()
```

These are not decorative. They are the ring’s current **aggregate witnesses**:

```text
XOR fold = parity-like ring witness
SUM fold = accumulation witness
ROT fold = position-sensitive fold witness
```

The search result shows these ring fold functions and export/import functions in `tetragrammatron.c`.

### Chirality selection

`tetragrammatron.c` also implements:

```c
tetra_chiral_pi(uint64_t hash)
```

This scans the 16 nibbles of a 64-bit hash and accumulates two diagonal classes:

```text
D+ = {0,5,A,F}
D- = {3,6,9,C}
```

It returns:

```text
CHIRAL_POSTING
CHIRAL_PULLING
CHIRAL_BALANCED
CHIRAL_INCOMPLETE
```

So this is the C-level seed of the chirality-selection law: the hash is inspected through two diagonal accumulators, and whichever diagonal crosses the threshold first determines direction.

Practical summary:

```text
tetragrammatron.c owns the finite acceptance surface.
It maps Fano/role/local240 into slot5040.
It stores receipts in the ring.
It computes ring witnesses.
It decides chirality from hash diagonals.
It does not render geometry.
It does not carry HTTP.
```

## 3. `metatron.c` — Projection, geometry, surfaces

`metatron.c` is the **projection authority**.

It is large because it contains the geometry database and renderers.

It owns:

```text
seed edge tables
vertex tables
SHAPE_DB
Hopf/quaternion route
Smith chart state
solid lookup
geometry frame generation
OBJ / glTF / PPM / SVG / JSON renderers
incidence self-check
```

At the top it defines seed crystals and edge tables for many solids:

```text
tetrahedron
stellated tetrahedron
octahedron
cube
icosahedron
dodecahedron
cuboctahedron
truncated octahedron
truncated cube
rhombic dodecahedron
icosidodecahedron
truncated icosahedron
truncated dodecahedron
truncated icosidodecahedron
tetrakis hexahedron
triakis octahedron
pentakis dodecahedron
triakis icosahedron
disdyakis triacontahedron
```

The file also defines vertex tables like `T0`, `T1`, `OCTA`, and `CUBOCT`, then builds the `SHAPE_DB` table from names, Fano roots, family sequence, vertex/edge/face counts, edge arrays, and Schläfli-like parameters.

### `SHAPE_DB`

`SHAPE_DB` is the main projection catalog.

A `ShapeDef` entry says:

```text
name
fano_root
family_seq
nverts
nedges
nfaces
edge list
schlafli_p
schlafli_q
```

This is where the abstract receipt/ring state can become a named geometry.

Protocol meaning:

```text
receipt/ring/fano role → shape family → projected solid
```

### Twin geometry resolution

The file contains functions that derive a `TwinGeometry` from ring folds and current result. The output seen in smoke tests includes fields like:

```text
cycle
result
opcode
frame_type
chart11
baseQ
fiberQ
fano7
role3
bqf
local240
slot5040
hopf
fiberPhase
polybius
quaternion
solid
```

That matches the `print_twin_geometry()` output in `metatron.c`.

So Metatron takes the accepted or inspectable ring state and says:

```text
What surface should this become?
Which solid?
Which vertex?
Which edge?
Which quaternion?
Which Smith coordinate?
Which output format?
```

### Smith chart projection

`resolve_smith()` maps a `slot5040` and hash into a Smith-like coordinate:

```text
rho   from low hash bits
theta from slot5040 / 5040
gamma = rho * e^(i theta)
z     = impedance transform
y     = admittance transform
```

The code computes:

```text
gr, gi
zr, zi
yr, yi
```

This is why Smith is a projection surface, not authority. It reads ring/citation material and emits coordinates.

### Renderers

`metatron.c` includes renderers such as:

```text
render_frame_json()
render_obj()
render_gltf()
render_ppm()
render_smith_svg()
```

The `render_obj()` snippet shows how it resolves a shape, computes rotated vertices using quaternion rotation, and prints OBJ wireframe vertices and lines.

### Incidence self-check

`check_incidence()` validates the internal geometry database consistency:

```text
Fano table matches expected lines
CONFIG_MATRIX points into SHAPE_DB correctly
shape family sequence is coherent
edges are in range
handshake lemma holds: sum(degrees)=2*nedges
Euler check V-E+F=2 for convex solids where applicable
isolated vertices are rejected
semi-regular degree ranges are reported
```

The uploaded source shows the incidence self-check walking `SHAPE_DB`, computing degree counts, checking handshake sums, checking Euler characteristic, and reporting semi-regular degree ranges.

Practical summary:

```text
metatron.c reads receipt/ring state.
It resolves geometry.
It computes Smith/Hopf/quaternion surfaces.
It renders JSON, OBJ, glTF, PPM, SVG.
It checks geometry database consistency.
It does not validate acceptance.
It does not own input transport.
```

## 4. `imo.c` — Carrier, files, parser, ports, HTTP

`imo.c` is the **carrier authority**.

It owns the messy-but-necessary world of representation:

```text
files
ring persistence
S-expression input
parser
canonicalization
small reducer
receipt generation
HTTP server
SSE / WebSocket streams
static file serving
BOOT_ROM strings
memory utilities
SHA-1 / base64 for WebSocket
```

The README says IMO owns ring serialization, S-expression parser/compiler/reducer, HTTP/SSE/WebSocket server, SHA-1, `BOOT_ROM[21]`, signal handling, and allocation helpers. It carries surfaces into runtime representations but does not validate, compose geometry, or change identity.

### Ring serialization

At the top, `imo.c` defines:

```c
#define RING_FILE "/tmp/omi_receipt_ring.bin"

ring_load()
ring_save()
write_ring_omi()
handle_signal()
```

This is file-as-port in literal C form.

The ring is held by `tetragrammatron.c`, but `imo.c` carries it to and from the filesystem.

```text
Tetragrammatron owns ring meaning.
IMO owns ring file transport.
```

### Buffer and node runtime

`imo.c` defines reusable runtime helpers:

```text
xmalloc / xrealloc / xstrdup / xstrndup
Buffer
Node
nnew / natom / nstring / npair
npush / ncopy / nfree
```

That gives it a small Lisp/S-expression data model.

### S-expression parser

The parser supports:

```text
atoms
strings
lists
dotted pairs
comments beginning with ;
escaping in strings
line/column errors
```

It parses text into `Node` trees.

This is why `imo.c` is the file that makes the REPL and `--eval` path possible.

### Canonicalization and lane collection

`imo.c` canonicalizes parsed nodes with:

```c
ncanon()
```

It also recognizes OMI lane atoms:

```text
FS
GS
RS
US
```

Then it can compile two shapes:

```text
canonical root: (omi ...)
alist lane pairs: (FS . value), (GS . value), etc.
```

The relevant functions are:

```text
compile_omi()
collect_canon_root()
collect_canon()
collect_alist()
build_tape()
```

`build_tape()` emits a tape with a header containing:

```text
0x00
0x1b
OMI_FS
OMI_GS
OMI_RS
OMI_US
first source byte
state
```

then appends lane records:

```text
lane byte
length bytes
value bytes
```

So IMO turns a textual OMI expression into a carried tape. That tape is representation, not final truth.

### Reducer

`imo.c` includes a small reducer for expressions such as:

```text
cons
car
cdr
xor
rot
mask
hash
quote
normalize
eval / reduce
receipt
list
```

This is not a full Scheme. It is a small deterministic expression reducer that makes the REPL useful.

The key point:

```text
reduce() computes a canonical result.
generate_receipt() wraps source/result/cycle/hash into JSON.
process_sexpr() stores accepted result hash into the ring.
```

Directly from the source, `process_sexpr()` tries `compile_omi()`, falls back to parsing/reducing an ordinary S-expression, generates a receipt, marks the result accepted in that local pathway, hashes the canonical result, and calls `ring_store()`. The grep/source excerpt shows those functions together.

This is the current practical bridge between the Lisp-ish input surface and the receipt ring.

### HTTP / SSE / WebSocket server

`imo.c` serves the browser UI.

Its HTTP side includes:

```text
mime_type()
http_ok()
http_404()
serve_file()
serve_frame_json_http()
serve_ring_json()
serve_solid_json()
serve_incidence_json()
serve_receipt_json()
serve_sse()
serve_ws()
serve_http()
```

The static file server prepends `viewer/` to paths:

```text
/public/foo.js → viewer/public/foo.js
/index.html    → viewer/index.html
```

That behavior is explicitly noted in the project context and is visible in the `serve_file()` code path.

The server also exposes geometry/state endpoints:

```text
/frame
/ring
/solid?id=N
/incidence?id=N
/receipt?id=N
/events or SSE
/ws or WebSocket
```

The source excerpt shows `serve_solid_json()`, `serve_incidence_json()`, `serve_receipt_json()`, `serve_sse()`, and `serve_ws()`, including WebSocket accept generation via SHA-1 and base64.

### BOOT_ROM

At the bottom, `imo.c` contains `BOOT_ROM[]`: a list of OMI address strings used by the boot path. It includes zero frame, diagonal/test constants, `0x7C00`, `0x5A3C`, ququart phases, and final `0xFFFF`. The code defines `BOOT_COUNT` from the array size. This boot ROM is carried as strings and parsed by OMI during boot. The uploaded source shows the `BOOT_ROM[]` strings directly.

Practical summary:

```text
imo.c owns input and output representation.
It persists the ring to files.
It parses and reduces S-expressions.
It compiles OMI lane notation into a tape.
It generates JSON receipts.
It serves HTTP/SSE/WebSocket/browser files.
It carries BOOT_ROM as address strings.
It does not define citation law.
It does not validate geometry.
It does not own projection meaning.
```

## How the four files work together

Here is the clean mental model:

```text
imo.c
  receives text / file / HTTP / REPL input

omi.c
  parses OMI address frames
  executes deterministic address/nibble law

tetragrammatron.c
  stores accepted receipt state
  computes ring folds and slot5040
  governs chirality and finite routing

metatron.c
  reads ring/citation state
  projects geometry, Smith, OBJ, glTF, PPM, JSON, SVG

imo.c
  carries the projection back out
```

The README’s architecture flow says the same thing in module form: IMO receives input, OMI parses and produces citation, Tetragrammatron validates and produces receipt, Metatron projects a surface, and IMO carries that surface to stdout, file, or HTTP response.

## The shortest accurate summary

```text
omi.c
  “What is the address and what deterministic witness does it compute?”

tetragrammatron.c
  “Does this fit the finite receipt/ring/governor structure?”

metatron.c
  “What geometry or visual surface does this receipt project into?”

imo.c
  “How does this enter or leave the runtime as text, file, HTTP, stream, or boot material?”
```

And the canon boundary:

```text
OMI cites.
Tetragrammatron validates.
Metatron projects.
IMO carries.
Receipt is the shared invariant.
```

That means your C code already matches the final covariant rule:

```text
Code is data      → imo.c carries source/text/BOOT_ROM
File is port      → imo.c ring files and HTTP files
Notation citation → omi.c parses address frames
Canon validates   → tetragrammatron.c ring/governor
Cosmology projects→ metatron.c geometry surfaces
Receipt accepts   → ring + receipt JSON boundary
```