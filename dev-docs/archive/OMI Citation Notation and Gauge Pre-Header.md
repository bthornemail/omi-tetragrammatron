# OMI Citation Notation and Gauge Pre-Header

> Current vocabulary note: this archive document is retained as historical source material. Active doctrine now distinguishes `Omi-Ring` from `receipt`: the Omi-Ring is the addressed palindromic notation witness, while a receipt is the accepted state of that witness. See `dev-docs/archive/Omi-Ring as Addressed Notation Witness.md` and `dev-docs/OMI_ADDRESSING.md`.

## A Palindromic Monotonic Annotation Protocol for the OMI Object Model

Version: `0.1.0`  
Status: Canonical Draft  
Protocol: OMI---IMO  
Authority Layer: Citation Notation / Pre-Language Gauge / Object Model  
Applies to: documents, `.imo` carriers, binary streams, boot envelopes, LLM resolvers, repository annotations, storage targets, and hardware adapters

---

## Abstract

OMI defines a citation and annotation protocol for embedding deterministic memory topology into documents, files, byte streams, and hardware carriers before ordinary language interpretation occurs.

The protocol is built around a palindromic mnemonic:

```text
OMI---IMO
```

This mnemonic names the forward and inverse relation surface of the OMI Object Model.

OMI is not first a programming language, markup language, database, or coordinate system. It is first a bounded relation frame.

The pre-language control envelope uses the ASCII control range:

```text
0x00..0x20
```

and the gauge byte family:

```text
0xF0..0xFF
```

to identify, delimit, orient, and synchronize OMI-compatible carriers.

The canonical binary pre-header is:

```text
FF 00 1C 1D 1E 1F 20 FF
```

read as:

```text
GAUGE NUL FS GS RS US SP GAUGE
```

This pre-header acts like a binary shebang and in-stream escape sequence. It recognizes an OMI carrier, stages FS/GS/RS/US topology, opens readable surface interpretation, and seals the gauge. It does not accept state.

Canonical invariant:

```text
Recognition is not acceptance.
Citation is not acceptance.
Closure is not acceptance.
Projection is not acceptance.
Validation and receipt accept.
```

---

# 1. Purpose

This paper defines the final authority model for OMI citation notation.

It explains how OMI annotations can be embedded in documents and binary carriers using:

```text
0xF* gauge encapsulation
0x00 NUL origin
0x1C FS file separator
0x1D GS group separator
0x1E RS record separator
0x1F US unit separator
0x20 SP readable boundary
```

It also explains how the notation derives from the palindromic monotonic mnemonic:

```text
OMI---IMO
```

and resolves into the OMI Object Model:

```text
o---o/---/?---?@---@
```

The result is a citation protocol where files, annotations, model outputs, storage blocks, media streams, and hardware boot targets can carry deterministic reference frames without becoming authority by themselves.

---

# 2. The Palindromic Mnemonic

The canonical mnemonic is:

```text
OMI---IMO
```

This is read as a forward-inverse relation:

```text
OMI → IMO
IMO → OMI
```

The palindrome means the protocol can be approached from either side:

```text
OMI = forward object/address/citation surface
IMO = inverse carrier/memory/observation surface
```

The monotonic reading means the resolver moves in one direction through staged authority:

```text
recognize
  ↓
cite
  ↓
validate
  ↓
record
  ↓
project
  ↓
inspect
```

The forbidden shortcut is:

```text
render → believe
```

or, for LLMs:

```text
generate → believe
```

The mnemonic therefore means:

```text
OMI---IMO is reversible as notation,
but monotonic as authority.
```

A resolver may move from recognition to citation to validation to receipt, but it may not skip validation and treat a projection as accepted state.

---

# 3. OMI Object Model

The OMI Object Model is the bounded relation model represented by:

```text
o---o/---/?---?@---@
```

Expanded:

```text
o-FS-GS-RS-US/FS/GS/RS/US?REGISTER?STACK@CAR@CDR
```

Compact machine ruler form:

```text
o-S0-S1-S2-S3/S4/S5/S6/S7?REGISTER?STACK@CAR@CDR
```

The 256-bit frame consists of:

```text
8 × 16-bit scope/ruler fields = 128 bits
4 × 32-bit relation fields    = 128 bits
----------------------------------------
total                         = 256 bits
```

The first 128 bits are the FS/GS/RS/US cascade:

```text
S0 = FS
S1 = GS
S2 = RS
S3 = US

S4 = inverse/carrier FS
S5 = inverse/carrier GS
S6 = inverse/carrier RS
S7 = inverse/carrier US
```

The second 128 bits are the relation surface:

```text
REGISTER
STACK
CAR
CDR
```

Canonical statement:

```text
The first 128 bits stage topology.
The second 128 bits stage relation.
The whole 256-bit frame is a bounded OMI---IMO memory relation.
```

---

# 4. The Pre-Language Control Alphabet

The minimal control alphabet is:

```text
0x00  NUL  origin / null / zero-point
0x1C  FS   file scope
0x1D  GS   group scope
0x1E  RS   record scope / closure witness
0x1F  US   unit scope
0x20  SP   readable boundary / visible separator
```

Readable names:

```text
NUL = Null
FS  = File Separator
GS  = Group Separator
RS  = Record Separator
US  = Unit Separator
SP  = Space
```

Canonical OMI meaning:

```text
NUL opens origin.
FS selects file scope.
GS selects group scope.
RS selects record scope and may witness closure.
US selects unit scope.
SP opens readable surface interpretation.
```

The controls stage topology.

They do not accept state.

```text
Control is not acceptance.
Scope is not acceptance.
Closure is not acceptance.
Validation and receipt accept.
```

---

# 5. The Gauge Pre-Header

The OMI gauge pre-header is:

```text
G 00 1C 1D 1E 1F 20 G
```

where:

```text
G ∈ 0xF0..0xFF
```

The canonical OMI form is:

```text
FF 00 1C 1D 1E 1F 20 FF
```

Readable:

```text
GAUGE NUL FS GS RS US SP GAUGE
```

Escaped documentation form:

```text
\xFF\x00\x1C\x1D\x1E\x1F\x20\xFF
```

The pre-header performs four jobs:

```text
1. fast-fails non-OMI carriers
2. selects gauge dialect or orbit offset
3. stages the FS/GS/RS/US control cascade
4. marks a binary stream synchronization boundary
```

It does not validate the payload.

It does not accept the annotation.

It does not trust the carrier.

---

# 6. The F* Gauge Family

The range:

```text
0xF0..0xFF
```

is the OMI gauge family.

The high nibble:

```text
F
```

marks the gauge row.

The low nibble:

```text
0..F
```

selects dialect, stream profile, or orbit offset.

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

The first and last gauge bytes must match.

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

Canonical rule:

```text
0xF* is the gauge row.
F is the family.
* is the dialect or orbit offset.
```

---

# 7. Fast-Fail Recognition

A parser may identify an OMI raw carrier by checking eight bytes.

Reference rule:

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

Canonical-only rule:

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

Fast-fail rule:

```text
If byte 0 is not in 0xF0..0xFF, this is not a raw OMI gauge carrier.
If bytes 1..6 are not 00 1C 1D 1E 1F 20, reject gauge interpretation.
If byte 7 does not equal byte 0, reject gauge interpretation.
```

---

# 8. OMI Citation Notation

OMI citation notation binds a visible or hidden annotation to an FS/GS/RS/US scope.

Canonical visible form:

```text
omi:<gauge>/<fs>/<gs>/<rs>/<us>#<relation>
```

Example:

```text
omi:FF/README.md/adapters/webserial/open#effect.network
```

Equivalent OMI object scope:

```text
fs.o/README.md/gs.o/adapters/rs.o/webserial/us.o/open
```

Equivalent relation:

```omi-lisp
(effect . network)
(adapter . webserial)
```

The annotation means:

```text
Inside README.md,
under adapters,
inside the webserial record,
at the open unit,
there is a candidate relation declaring a network effect through the webserial adapter.
```

It does not mean the effect is accepted.

It does not mean the adapter may run.

It means a scoped citation exists.

---

# 9. Raw and Escaped Annotation Forms

OMI supports two annotation forms.

## 9.1 Raw Carrier Form

For `.imo`, binary streams, flash images, HNSW indexes, media streams, and storage carriers:

```text
FF 00 1C 1D 1E 1F 20 FF
[256-bit OMI---IMO frame]
[declarations]
[validation material]
[receipts]
```

## 9.2 Escaped Documentation Form

For Markdown, HTML, README files, prompts, papers, and LLM-visible text:

```text
\xFF\x00\x1C\x1D\x1E\x1F\x20\xFF
```

or:

```text
\GAUGE \NUL \FS \GS \RS \US \SP \GAUGE
```

Both forms resolve to the same topology.

Canonical rule:

```text
Raw control bytes are the carrier form.
Escaped control names are the documentation form.
Both resolve to the same OMI scope.
```

---

# 10. Dot Relation Notation

Once scope is established, relations are declared using dot notation:

```omi-lisp
(car . cdr)
```

Examples:

```omi-lisp
(effect . hardware)
(adapter . esp32)
(receipt.result . accepted)
(index.kind . hnsw)
(seed.video . o.video.o)
```

A relation is scoped by the active FS/GS/RS/US envelope.

The same relation under two different scopes is not the same memory relation.

Example:

```text
fs.o/README.md/gs.o/adapters/rs.o/webserial/us.o/open
```

is different from:

```text
fs.o/SKILLS.md/gs.o/adapters/rs.o/webserial/us.o/open
```

Scope is part of identity.

---

# 11. Annotation as Citation, Not Acceptance

An OMI annotation may recognize a relation.

It may cite a relation.

It may locate a relation.

It may stage a validation candidate.

It may not accept the relation.

Canonical annotation invariant:

```text
Annotation is not acceptance.
Citation is not acceptance.
Relation is not acceptance.
Scope is not acceptance.
Validation and receipt accept.
```

An OMI-compatible document may say:

```omi-lisp
(change.status . candidate)
(change.scope . fs.o/README.md/gs.o/adapters/rs.o/webserial/us.o/open)
(change.effect . network)
```

But it may not claim acceptance unless a receipt exists:

```omi-lisp
(receipt.accepts . change-id)
(receipt.scope . fs.o/README.md/gs.o/adapters/rs.o/webserial/us.o/open)
(receipt.effect . network)
(receipt.result . accepted)
```

---

# 12. In-Stream Escape Use

The gauge pre-header may appear at the beginning of a carrier or inside a stream.

At the beginning of a carrier, it acts as a binary shebang.

Inside a stream, it acts as a resynchronization escape.

Stream form:

```text
[ordinary bytes]
[GAUGE PRE-HEADER]
[scoped OMI packet]
[ordinary bytes]
[GAUGE PRE-HEADER]
[scoped OMI packet]
```

The pre-header lets parsers recover alignment in binary data.

Stream rule:

```text
A gauge pre-header marks an interpretation boundary.
It does not prove the packet.
The following packet must still validate.
```

---

# 13. HNSW and Binary Data Structure Annotation

OMI can annotate HNSW-like graph and vector index data without replacing the index.

OMI supplies scoped citation and receipt layers around binary structures.

Suggested scope mapping:

```text
FS = index file or carrier
GS = graph layer
RS = node or neighborhood record
US = vector, edge list, payload, or metadata unit
```

Example scope:

```text
fs.o/vector.index/gs.o/layer-03/rs.o/node-000042/us.o/neighbors
```

Example OMI-Lisp annotation:

```omi-lisp
(index.kind . hnsw)
(index.carrier . vector.index)
(index.layer . 3)
(index.node . 42)
(index.unit . neighbors)
(index.effect . read-only)
(index.status . candidate)
```

Boundary:

```text
HNSW nearest-neighbor result is not acceptance.
Vector similarity is not acceptance.
Graph traversal is not acceptance.
Validation and receipt accept.
```

The gauge pre-header lets OMI metadata appear in-stream:

```text
[HNSW binary bytes]
FF 00 1C 1D 1E 1F 20 FF
[OMI-IMO frame]
[scoped annotation]
[receipt candidate]
[more HNSW binary bytes]
```

---

# 14. Platform-Agnostic Boot Envelope

The same notation can be used as a flashable boot envelope.

Canonical boot envelope:

```text
512 bits = 64 bytes
```

Split:

```text
first 256 bits  = gauge / prefix / topology half
second 256 bits = OMI---IMO bootstrap frame
```

Layout:

```text
0x00..0x1F = prefix half
0x20..0x3F = bootstrap frame half
```

The first 8 bytes of the prefix half are:

```text
FF 00 1C 1D 1E 1F 20 FF
```

The second 256-bit half is:

```text
S0 S1 S2 S3 S4 S5 S6 S7 REGISTER STACK CAR CDR
```

Canonical boot rule:

```text
The prefix finds the frame.
The bootstrap stages the first relation.
Validation and receipt accept boot state.
```

This makes OMI flashable to hardware while remaining platform agnostic.

Any storage target that can expose the first 64 bytes can carry the OMI boot envelope.

Examples:

```text
eMMC
SD card
SPI flash
NOR flash
NAND flash
EEPROM
raw disk image
WASM memory
browser storage
firmware blob
network packet
binary stream
```

---

# 15. eMMC as Target Carrier Example

eMMC is a target example, not the definition of OMI.

It provides a concrete carrier for the tetrahedral centroid memory model.

Canonical correction:

```text
eMMC is the target carrier.
The tetrahedron is the balance model.
The centroid is the memory unit.
The storage areas are incidence faces around that unit.
```

Useful eMMC-style faces:

```text
BOOT0
BOOT1
SECURE / RPMB-like authenticated storage
USER storage
```

OMI interpretation:

```text
BOOT0  = primary boot candidate face
BOOT1  = fallback boot candidate face
SECURE = receipt / rollback witness face
USER   = carrier / repo / document / media face
```

The tetrahedral memory model:

```text
4 vertices
6 edges
4 triangular faces
1 centroid relation
```

Canonical statement:

```text
The flash does not create the tetrahedron.
The flash gives the tetrahedron somewhere to land.
```

---

# 16. Centroid Memory Model

The OMI memory unit is not a floating coordinate.

It is a centroid relation.

Canonical line:

```text
The centroid is the unit.
The sphere is the carrier.
The tetrahedron is the first exact relation.
```

The tetrahedral unit has:

```text
4 vertices
6 edges
4 triangular faces
1 centroid relation
```

The balance relation is:

```text
each vertex touches 3 edges
each edge touches 2 vertices
each face has 3 vertices
each edge belongs to 2 faces
```

Exact equality witnesses:

```text
4 × 3 = 6 × 2
4 × 3 = 4 × 3
6 × 2 = 4 × 3
```

OMI uses this as the first exact incidence memory model.

---

# 17. Coordinate Assumption Replaced

Classical computational geometry asks:

```text
Where is this point?
```

It often begins with:

```text
coordinate tuple
floating-point arithmetic
visual geometry
```

OMI asks instead:

```text
What incidence relation is balanced around this centroid?
```

This replaces coordinate-first geometry with incidence-first memory.

Canonical rule:

```text
Coordinates may render.
Incidence resolves.
Receipts accept.
```

This matters because floating-point rendering can be useful for display while still being unsuitable as a deterministic authority boundary.

---

# 18. BQF Partition

The OMI relation space is organized by:

```text
60x² + 16xy + 4y²
```

Interpretation:

```text
4y²   = tetrahedral self-dual seed / local centroid unit
16xy  = tangent branch / chirality crossing / common-core relation
60x²  = high-shell field / larger incidence projection
```

These are not coordinate magnitudes.

They are incidence partitions.

They tell the protocol which geometric forms and constants live in which mathematical neighborhood.

---

# 19. Delta Law Foundation

Only one thing was chosen:

```text
delta(x, C) = rotl(x,1) XOR rotl(x,3) XOR rotr(x,2) XOR C
```

The actual design choices were:

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
11! orientation expansion
F* gauge row
```

Canonical line:

```text
We chose the law.
The constants fell out.
```

---

# 20. Orbit Layers

OMI uses three factorial orbit layers in the current authority frame.

## 20.1 5! Local Tetragrammatron Resolution

```text
5! = 120
```

The signed local orbit is:

```text
240 = 2 × 5!
```

Rule:

```text
5! resolves local Tetragrammatron circulation.
```

## 20.2 7! Receipt Ring

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

Rule:

```text
7! records receipt-ring circulation.
```

## 20.3 11! Orientation Expansion

From:

```text
60x² + 16xy + 4y² = 4(15x² + 4xy + y²)
```

and:

```text
15x² = 4x² + 11x²
```

we get:

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

Attach `11x²` to the 11! orientation expansion:

```text
11! = 39,916,800
```

Rule:

```text
11x² signs orientation.
11! expands orientation above the 5! Tetragrammatron frame.
11x² does not accept state.
11! does not accept state.
Validation and receipt accept.
```

---

# 21. Base36 Sync Labels

Base36 appears because the period-8 witness block from `1/73` sums to 36:

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

Suggested orbit sync label:

```text
Gδ:SLOT36:CYCLE36
```

Extended orientation label:

```text
Gδ:SLOT36:CYCLE36:ORIENT36
```

Example:

```text
FF:3VZ:0001:NRK00
```

This is a synchronization pointer.

It is not acceptance.

---

# 22. OMI Citation Annotation Grammar

A minimal OMI citation annotation may be written:

```text
omi:<gauge>/<fs>/<gs>/<rs>/<us>#<relation>
```

Example:

```text
omi:FF/vector.index/layer-03/node-000042/neighbors#index.kind.hnsw
```

Equivalent expanded scope:

```text
fs.o/vector.index/gs.o/layer-03/rs.o/node-000042/us.o/neighbors
```

Equivalent relation:

```omi-lisp
(index.kind . hnsw)
```

A receipt citation may be written:

```text
omi-receipt:<gauge>/<slot36>/<cycle36>/<orient36>#<receipt-id>
```

Example:

```text
omi-receipt:FF/3VZ/0001/NRK00#receipt-2820
```

This means the receipt is cited at that orbit coordinate.

It does not mean the current repository accepts the receipt unless local validation accepts it.

---

# 23. OMI Object Model Canon

The OMI Object Model can be summarized:

```text
OMI = citation/address/instruction witness
IMO = carrier/memory/inverse observation
Tetragrammatron = local 5! governor
Receipt ring = 7! accepted-state orbit
Orientation layer = 11! signing expansion
Metatron = projection/geometry/surface
Adapter = external system boundary
```

Authority chain:

```text
gauge_process() witnesses.
bridge_resolve() stages.
parse_form() constructs.
validate_candidate() judges.
receipt_store() accepts.
projection_allowed() gates side effects.
hardware_project() acts only after accepted hardware receipt.
```

---

# 24. Complete Authority Boundary

The final OMI annotation authority boundary is:

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
Annotation is not acceptance.
Citation is not acceptance.
Relation is not acceptance.
Closure is not acceptance.
Projection is not acceptance.
Rendering is not acceptance.
Nearest-neighbor search is not acceptance.
Semantic graph is not acceptance.
Hardware boot media is not acceptance.
Validation and receipt accept.
```

Network boundary:

```text
Send does not accept.
Receive does not accept.
Validation accepts.
Receipt records.
```

Boot boundary:

```text
Boot read is not acceptance.
Header match is not acceptance.
Frame parse is not acceptance.
Secure storage reference is not acceptance.
Validation and receipt accept.
```

---

# 25. Final Canon

```text
OMI---IMO is the palindromic mnemonic.

OMI is the forward citation/address surface.
IMO is the inverse carrier/memory surface.

0xF* is the gauge row.
0x00..0x20 is the pre-language control envelope.
FF 00 1C 1D 1E 1F 20 FF is the canonical binary shebang.

FS/GS/RS/US stages memory topology.
The 256-bit OMI---IMO frame stages bounded relation.
The 512-bit boot envelope makes the frame flashable.
The tetrahedral centroid model gives the first exact memory unit.

5! resolves local Tetragrammatron circulation.
7! records receipt-ring circulation.
11! expands orientation/signing above the 5! frame.

11x² signs orientation.
11x² does not accept state.
11! does not accept state.

Validation and receipt accept.
```

---

# 26. One-Sentence Summary

OMI citation notation embeds a palindromic `OMI---IMO` object-memory relation into documents, byte streams, and flashable storage using the `0xF*` gauge family and the `0x00..0x20` pre-language control envelope, where `FF 00 1C 1D 1E 1F 20 FF` acts as a binary shebang, FS/GS/RS/US stages topological memory, dot notation declares scoped CAR/CDR relations, the 256-bit frame bounds the object model, the 512-bit boot envelope makes it hardware-portable, and only validation and receipt accept state.
