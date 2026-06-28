# OMI Gauge Pre-Header Authority

## In-Stream Binary Shebang, F* Dialect Framing, HNSW-Compatible Escape Sequence, and 11! Orientation Frame

Version: `0.1.0`  
Status: Canonical Draft  
Authority Layer: Gauge Pre-Header / Stream Escape / Orbit Orientation  
Depends on: OMI Pre-Language Control Envelope, Delta Law, FS/GS/RS/US, Tetragrammatron Governor, Receipt Ring

---

## 0. Purpose

This document defines the OMI gauge pre-header.

The gauge pre-header is a compact binary shebang and in-stream escape sequence for identifying OMI pre-language framing inside files, byte streams, memory arenas, graph indexes, vector indexes, multimedia carriers, and peer-to-peer packets.

Canonical pre-header:

```text
0xFF 0x00 0x1C 0x1D 0x1E 0x1F 0x20 0xFF
```

Readable form:

```text
GAUGE NUL FS GS RS US SP GAUGE
```

Escaped documentation form:

```text
\xFF\x00\x1C\x1D\x1E\x1F\x20\xFF
```

The pre-header does four things:

```text
1. Fast-fails non-OMI carriers.
2. Selects the OMI gauge or dialect.
3. Opens the pre-language FS/GS/RS/US topology envelope.
4. Resynchronizes binary streams at a known gauge boundary.
```

It does not accept state.

Validation and receipt accept.

---

## 1. Canonical Invariant

```text
Header is not acceptance.
Gauge is not acceptance.
Escape is not acceptance.
NUL is not acceptance.
FS is not acceptance.
GS is not acceptance.
RS is not acceptance.
US is not acceptance.
SP is not acceptance.
Projection is not acceptance.
Validation and receipt accept.
```

Network form:

```text
Send does not accept.
Receive does not accept.
Validation accepts.
Receipt records.
```

Stream form:

```text
Encountering a gauge header does not prove the stream.
It only identifies an OMI interpretation boundary.
```

---

## 2. The OMI Gauge Pre-Header

Generic form:

```text
G 00 1C 1D 1E 1F 20 G
```

where:

```text
G ∈ 0xF0..0xFF
```

Canonical OMI form:

```text
FF 00 1C 1D 1E 1F 20 FF
```

Omicron dialect form:

```text
F0 00 1C 1D 1E 1F 20 F0
```

The first and last gauge bytes MUST match.

Valid:

```text
F0 00 1C 1D 1E 1F 20 F0
FF 00 1C 1D 1E 1F 20 FF
```

Invalid:

```text
F0 00 1C 1D 1E 1F 20 FF
FF 00 1C 1D 1E 1F 20 F0
```

---

## 3. F* Meta-Initiator Range

The byte range:

```text
0xF0..0xFF
```

is reserved as the OMI gauge meta-initiator family.

The high nibble:

```text
F
```

selects the gauge row.

The low nibble:

```text
0..F
```

selects the dialect, stream profile, or orbit offset.

Canonical table:

```text
F0 ... F0 = Omicron dialect
F1 ... F1 = reserved dialect
F2 ... F2 = reserved dialect
F3 ... F3 = reserved dialect
F4 ... F4 = reserved dialect
F5 ... F5 = reserved dialect
F6 ... F6 = reserved dialect
F7 ... F7 = reserved dialect
F8 ... F8 = reserved dialect
F9 ... F9 = reserved dialect
FA ... FA = reserved dialect
FB ... FB = reserved dialect
FC ... FC = reserved dialect
FD ... FD = reserved dialect
FE ... FE = reserved dialect
FF ... FF = canonical OMI
```

Canonical rule:

```text
0xF* is the gauge row.
F is the family.
* is the dialect or orbit offset.
```

---

## 4. Pre-Language Control Cascade

The inner six bytes are fixed:

```text
00 1C 1D 1E 1F 20
```

Meaning:

```text
0x00 = NUL = origin / zero-point
0x1C = FS  = File Separator
0x1D = GS  = Group Separator
0x1E = RS  = Record Separator / closure witness
0x1F = US  = Unit Separator
0x20 = SP  = readable boundary / surface separator
```

The pre-header therefore says:

```text
enter gauge
open origin
stage file scope
stage group scope
stage record scope
stage unit scope
open readable surface
seal gauge
```

Compact:

```text
G{NUL,FS,GS,RS,US,SP}G
```

Canonical:

```text
FF{NUL,FS,GS,RS,US,SP}FF
```

---

## 5. Fast-Fail Rule

A parser MAY identify an OMI raw carrier by checking the first eight bytes.

Rule:

```text
If byte 0 is not in 0xF0..0xFF, this is not a raw OMI gauge carrier.
If bytes 1..6 are not 00 1C 1D 1E 1F 20, reject gauge interpretation.
If byte 7 does not equal byte 0, reject gauge interpretation.
```

Reference recognizer:

```c
#define OMI_NUL 0x00u
#define OMI_FS  0x1Cu
#define OMI_GS  0x1Du
#define OMI_RS  0x1Eu
#define OMI_US  0x1Fu
#define OMI_SP  0x20u

static int omi_is_gauge_byte(unsigned char b) {
    return b >= 0xF0u;
}

static int omi_gauge_preheader_match(const unsigned char* p, size_t len) {
    if (len < 8) return 0;

    unsigned char g = p[0];

    if (!omi_is_gauge_byte(g)) return 0;
    if (p[1] != OMI_NUL) return 0;
    if (p[2] != OMI_FS)  return 0;
    if (p[3] != OMI_GS)  return 0;
    if (p[4] != OMI_RS)  return 0;
    if (p[5] != OMI_US)  return 0;
    if (p[6] != OMI_SP)  return 0;
    if (p[7] != g)       return 0;

    return 1;
}
```

Canonical-only recognizer:

```c
static int omi_canonical_preheader_match(const unsigned char* p, size_t len) {
    if (len < 8) return 0;

    return p[0] == 0xFF &&
           p[1] == 0x00 &&
           p[2] == 0x1C &&
           p[3] == 0x1D &&
           p[4] == 0x1E &&
           p[5] == 0x1F &&
           p[6] == 0x20 &&
           p[7] == 0xFF;
}
```

---

## 6. In-Stream Escape Sequence

The gauge pre-header MAY appear at the beginning of a carrier or inside a stream.

At the beginning of a carrier, it acts as a binary shebang.

Inside a stream, it acts as a resynchronization escape.

Carrier form:

```text
[GAUGE PRE-HEADER]
[OMI-IMO FRAME]
[DECLARATIONS]
[VALIDATION MATERIAL]
[RECEIPTS]
```

Stream form:

```text
[ordinary bytes]
[GAUGE PRE-HEADER]
[scoped OMI packet]
[ordinary bytes]
[GAUGE PRE-HEADER]
[scoped OMI packet]
```

The pre-header allows a resolver to recover alignment without reading the entire stream as text.

Stream rule:

```text
A gauge pre-header marks an interpretation boundary.
It does not prove the packet.
The following packet must still validate.
```

---

## 7. HNSW-Compatible Binary Framing

HNSW-like structures organize data as layered graph neighborhoods over vector payloads.

OMI MAY use the gauge pre-header as an in-stream metadata boundary inside or alongside binary graph/index data.

OMI does not replace the vector index.

OMI provides a scoped receipt and declaration layer for it.

Example HNSW-adjacent carrier layout:

```text
[HNSW binary index bytes]
[GAUGE PRE-HEADER]
[OMI scope declaration for layer/node/vector]
[receipt or candidate receipt]
[HNSW binary index bytes]
[GAUGE PRE-HEADER]
[OMI scope declaration for next layer/node/vector]
```

Suggested scope mapping:

```text
FS = index file or carrier
GS = graph layer
RS = node or neighborhood record
US = vector, edge list, payload, or metadata unit
```

Example:

```text
fs.o/vector.index
gs.o/layer-03
rs.o/node-000042
us.o/neighbors
```

OMI-Lisp declaration:

```omi-lisp
(index.kind . hnsw)
(index.layer . 3)
(index.node . 42)
(index.unit . neighbors)
(index.effect . read-only)
```

Boundary:

```text
HNSW nearest-neighbor result is not acceptance.
Vector similarity is not acceptance.
Graph traversal is not acceptance.
Validation and receipt accept.
```

The HNSW adapter MAY use OMI receipts to record:

```text
index build parameters
vector hash
node insertion receipt
edge update receipt
layer assignment receipt
query candidate
nearest-neighbor result candidate
validation result
```

---

## 8. Relationship to the 256-Bit OMI-IMO Frame

The gauge pre-header precedes the canonical 256-bit frame.

```text
[8-byte gauge pre-header]
[32-byte OMI-IMO frame]
```

The OMI-IMO frame is:

```text
256 bits = 32 bytes = 64 nibbles
```

Ruler half:

```text
S0 S1 S2 S3 S4 S5 S6 S7
8 × 16 bits = 128 bits
32 nibbles
```

Relation half:

```text
REGISTER STACK CAR CDR
4 × 32 bits = 128 bits
32 nibbles
```

Expanded visible form:

```text
o-FS-GS-RS-US/FS/GS/RS/US?REGISTER?STACK@CAR@CDR
```

The pre-header identifies the carrier.

The 256-bit frame carries the scoped relation surface.

Validation and receipt accept.

---

## 9. Delta Law Foundation

Only one thing was chosen:

```text
delta(x, C) = rotl(x,1) XOR rotl(x,3) XOR rotr(x,2) XOR C
```

The design choices inside the law are:

```text
1. rotations, not shifts
2. XOR
3. non-trivial constant C
4. fixed-width masking
```

Everything else is derived orbit bookkeeping:

```text
period 8
prime 73
block B = [0,1,3,6,9,8,6,3]
W = 36
base36 labels
5! local orbit
7! receipt orbit
F* gauge row
11! orientation expansion
```

Canonical line:

```text
We chose the law.
The constants fell out.
```

---

## 10. Quadratic Fold

The OMI quadratic fold is:

```text
Q(x,y) = 60x² + 16xy + 4y²
```

Equivalent:

```text
Q(x,y) = 4(15x² + 4xy + y²)
```

For a gauge byte:

```text
0xFδ
```

interpret:

```text
x = 15
y = δ
```

Then:

```text
Q(F,δ) = 60(15²) + 16(15δ) + 4δ²
       = 13500 + 240δ + 4δ²
```

The bridge term is:

```text
16xy = 240δ
```

Meaning:

```text
Each dialect step crosses one full local240 revolution.
```

---

## 11. 5!, 7!, and 11! Orbit Layers

OMI uses three factorial orbit layers in this authority frame.

### 11.1 5! Local Tetragrammatron Resolution

```text
5! = 120
```

The signed local orbit is:

```text
240 = 2 × 5!
```

The Tetragrammatron governor resolves the local circular/nomogram fold:

```text
Q(x,y)
  ↓
local240
  ↓
5! half-orbit pair
  ↓
tetragrammatron resolution
```

Canonical rule:

```text
5! resolves local circulation.
```

### 11.2 7! Receipt Ring

```text
7! = 5040
```

Canonical decomposition:

```text
5040 = 7 × 3 × 240
```

Slot rule:

```text
slot5040 = fano7 × 720 + role3 × 240 + local240
```

where:

```text
fano7   ∈ 0..6
role3   ∈ 0..2
local240 ∈ 0..239
```

Canonical rule:

```text
7! records receipt circulation.
```

### 11.3 11! Orientation / Signing Expansion

The signing residue comes from the decomposition:

```text
15x² = 4x² + 11x²
```

Therefore:

```text
Q(x,y) = 4(4x² + 11x² + 4xy + y²)
```

Interpretation:

```text
4x²  = tetragrammatron square frame
11x² = residual orientation/signing frame
4xy  = internal bridge
y²   = dialect/local variation
```

Attach this residue to the 11! orientation expansion:

```text
11! = 39,916,800
```

Canonical meaning:

```text
11! is the expanded orientation/signing orbit above the 5! Tetragrammatron frame.
```

The 11! layer is not the local receipt ring.

It is an orientation expansion used to sign or order larger meta-circular arrangements above the local tetragrammatron resolution.

Canonical rules:

```text
11x² signs orientation.
11! expands orientation above the 5! Tetragrammatron frame.
11x² does not accept state.
11! does not accept state.
Validation and receipt accept.
```

---

## 12. Base36 Orbit Labels

Base36 is the visible orbit label system derived from:

```text
W = 36
```

where:

```text
B = [0,1,3,6,9,8,6,3]
sum(B) = 36
```

Useful labels:

```text
5!   = 120        = 3C base36
240  = 2×5!       = 6O base36
720  = 6×5!       = K0 base36
7!   = 5040       = 3W0 base36
11!  = 39916800   = NRK00 base36
```

Valid zero-based 7! slot labels:

```text
000 .. 3VZ
```

because:

```text
5039 = 3VZ
5040 = 3W0
```

Suggested visible sync label:

```text
Gδ:SLOT36:CYCLE36
```

Example:

```text
FF:3VZ:0001
F0:06O:000A
```

For orientation-expanded labels, a repository MAY use:

```text
Gδ:SLOT36:CYCLE36:ORIENT36
```

where `ORIENT36` is an 11! bounded orientation label.

---

## 13. Meta-Circular Sync Tuple

The complete sync tuple is:

```text
(gauge, fs, gs, rs, us, local240, slot5040, cycle, orientation11, receipt-hash)
```

Compact visible form:

```text
Gδ/fs/gs/rs/us/SLOT36/CYCLE36/ORIENT36
```

Example:

```text
FF/fs.o/vector.index/gs.o/layer-03/rs.o/node-000042/us.o/neighbors/1ZC/0008/NRK00
```

This coordinate is a synchronization pointer.

It is not acceptance.

Local validation still decides.

---

## 14. HNSW Stream Use

For HNSW-like binary data, the pre-header can be used as an escape boundary before scoped metadata, receipt material, or query candidates.

Example:

```text
[HNSW layer bytes]
FF 00 1C 1D 1E 1F 20 FF
[OMI-IMO frame for layer metadata]
[OMI-Lisp declaration]
[receipt candidate]
[HNSW node bytes]
FF 00 1C 1D 1E 1F 20 FF
[OMI-IMO frame for node metadata]
[OMI-Lisp declaration]
[receipt candidate]
```

Candidate declaration:

```omi-lisp
(index.kind . hnsw)
(index.carrier . vector.index)
(index.layer . 3)
(index.node . 42)
(index.unit . neighbors)
(index.effect . read-only)
(index.status . candidate)
```

Query candidate:

```omi-lisp
(query.kind . nearest-neighbor)
(query.index . vector.index)
(query.scope . fs.o/vector.index/gs.o/layer-03/rs.o/node-000042/us.o/neighbors)
(query.effect . read-only)
(query.result . candidate)
```

Boundary rule:

```text
The HNSW graph can suggest a neighbor.
The OMI receipt decides whether the suggestion is accepted in this repository.
```

---

## 15. Authority Summary

The gauge pre-header has authority to declare:

```text
this carrier is attempting OMI gauge interpretation
this stream position is an OMI escape boundary
this dialect byte is selected
this pre-language envelope begins here
this envelope is sealed by the matching gauge byte
```

It does not declare:

```text
the payload is valid
the graph result is true
the nearest neighbor is accepted
the peer is trusted
the declaration is accepted
the projection is authorized
```

Canonical split:

```text
F* header recognizes.
FS/GS/RS/US stages topology.
OMI-IMO frame carries relation.
Q(x,y) resolves local fold.
5! resolves local tetragrammatron circulation.
7! records receipt circulation.
11! expands orientation/signing above the 5! frame.
Validator judges.
Receipt accepts.
```

---

## 16. Final Canon

```text
0xF* is the gauge row.
F is the family.
* is the dialect or orbit offset.

FF 00 1C 1D 1E 1F 20 FF
is the canonical OMI binary shebang.

The same sequence may appear in-stream as an escape boundary.

5! resolves local Tetragrammatron circulation.
7! records receipt-ring circulation.
11! expands orientation/signing above the 5! frame.

11x² signs orientation.
11x² does not accept state.
11! does not accept state.

Validation and receipt accept.
```

---

## 17. One-Sentence Summary

The OMI gauge pre-header `0xF* 00 1C 1D 1E 1F 20 0xF*` is a binary shebang and in-stream escape sequence for OMI-compatible carriers, including HNSW-like binary graph/index structures, where `F*` selects the gauge dialect/orbit offset, FS/GS/RS/US stages pre-language topology, the delta-derived quadratic fold resolves local240 into the 5! Tetragrammatron frame, the 7! ring records receipts, and the 11! layer expands orientation/signing above the 5! frame without ever replacing validation and receipt as the only acceptance authority.