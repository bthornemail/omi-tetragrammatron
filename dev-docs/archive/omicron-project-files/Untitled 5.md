# OMI Gauge Header and Orbit Authority

## F* Dialect Framing, Base36 Orbit Sync, and Tetragrammatron 5! Resolution

Version: `0.1.0`  
Status: Canonical Draft  
Authority Layer: Gauge Header / Orbit Frame / Nomogram Resolution  
Depends on: OMI Pre-Language Control Envelope, FS/GS/RS/US, Receipt Ring, Tetragrammatron Governor

---

## 0. Purpose

This document defines the canonical role of the `0xF*` gauge prefix family.

The `0xF*` byte range is the OMI meta-initiator range.

It provides a fast-fail header, dialect selector, gauge offset, and orbit-framing byte for synchronizing OMI documents, peers, LLM resolvers, and receipt rings.

The canonical OMI gauge header is:

```text
0xFF 0x00 0x1C 0x1D 0x1E 0x1F 0x20 0xFF
```

Readable:

```text
GAUGE NUL FS GS RS US SP GAUGE
```

This means:

```text
0xFF = canonical OMI gauge
0x00 = NUL / origin
0x1C = FS / file scope
0x1D = GS / group scope
0x1E = RS / record scope / closure witness
0x1F = US / unit scope
0x20 = SP / readable boundary
0xFF = sealed canonical OMI gauge
```

The header recognizes an OMI pre-language carrier.

It does not accept content.

Validation and receipt accept.

---

## 1. Canonical Invariant

```text
Header is not acceptance.
Gauge is not acceptance.
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

Adapter form:

```text
External bridge does not accept.
Hardware does not accept.
Browser API does not accept.
Semantic graph does not accept.
Validation and receipt accept.
```

---

## 2. F* Meta-Initiator Range

The byte range:

```text
0xF0..0xFF
```

is reserved for OMI gauge initiation and dialect framing.

The high nibble:

```text
0xF
```

marks the byte as a gauge/meta initiator.

The low nibble:

```text
0x0..0xF
```

selects the dialect or gauge offset.

Canonical rule:

```text
0xF* = gauge meta-initiator
high nibble 0xF = OMI gauge family
low nibble * = dialect / orbit offset / sync selector
```

Examples:

```text
F0 ... F0 = Omicron dialect
F1 ... F1 = reserved dialect
F2 ... F2 = reserved dialect
...
FE ... FE = reserved dialect
FF ... FF = canonical OMI
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

## 3. Fast-Fail Gauge Header

A parser MAY fast-fail OMI gauge interpretation by checking the first eight bytes:

```text
G 00 1C 1D 1E 1F 20 G
```

where:

```text
G ∈ 0xF0..0xFF
```

Canonical OMI requires:

```text
G = 0xFF
```

Omicron dialect MAY use:

```text
G = 0xF0
```

Fast-fail rule:

```text
If the first byte is not 0xF0..0xFF, the carrier is not a raw OMI gauge carrier.
If bytes 1..6 are not 00 1C 1D 1E 1F 20, reject gauge interpretation.
If byte 7 does not equal byte 0, reject gauge interpretation.
```

The parser may then continue as ordinary text, ordinary binary, or rejected OMI carrier depending on local policy.

---

## 4. Gauge Header as Encapsulation

Canonical form:

```text
0xFF{NUL,FS,GS,RS,US,SP}0xFF
```

Omicron dialect form:

```text
0xF0{NUL,FS,GS,RS,US,SP}0xF0
```

Generic dialect form:

```text
0xFδ{NUL,FS,GS,RS,US,SP}0xFδ
```

where:

```text
δ = low nibble dialect selector, 0..15
```

This is the prefix form of:

```text
|OMI---IMO> = |FS>-|GS>-|RS>-|US> encapsulation
```

The header opens the control envelope.

The 256-bit OMI-IMO frame follows.

---

## 5. Relationship to the 256-Bit OMI-IMO Frame

The OMI-IMO frame is:

```text
256 bits = 32 bytes = 64 nibbles
```

It consists of:

```text
128-bit ruler half
128-bit relation half
```

Ruler half:

```text
S0 S1 S2 S3 S4 S5 S6 S7
8 × 16 bits = 128 bits
8 × 4 nibbles = 32 nibbles
```

Relation half:

```text
REGISTER STACK CAR CDR
4 × 32 bits = 128 bits
4 × 8 nibbles = 32 nibbles
```

Expanded form:

```text
o-FS-GS-RS-US/FS/GS/RS/US?REGISTER?STACK@CAR@CDR
```

The gauge header is not part of the 256-bit frame.

It precedes and identifies the frame.

Canonical carrier layout:

```text
[8-byte gauge header]
[32-byte OMI-IMO frame]
[scoped declarations]
[validation material]
[receipts]
```

---

## 6. F* as x/y Gauge Coordinate

The `0xF*` gauge byte is interpreted as a two-nibble coordinate:

```text
0xFδ
```

where:

```text
x = 0xF = 15
y = δ  = low nibble, 0..15
```

Thus:

```text
F0 → x=15, y=0
F1 → x=15, y=1
F2 → x=15, y=2
...
FF → x=15, y=15
```

This places every OMI dialect in the high gauge row.

The high nibble `F` marks the gauge family.

The low nibble selects the dialect/orbit offset.

Canonical interpretation:

```text
x selects the high gauge family.
y selects the dialect offset inside that family.
```

---

## 7. Quadratic Gauge Fold

The OMI quadratic fold is:

```text
Q(x,y) = 60x² + 16xy + 4y²
```

Equivalent factored form:

```text
Q(x,y) = 4(15x² + 4xy + y²)
```

For a gauge byte `0xFδ`:

```text
x = 15
y = δ
```

Therefore:

```text
Q(F,δ) = 60(15²) + 16(15δ) + 4δ²
       = 13500 + 240δ + 4δ²
```

For local 240 resolution:

```text
local240 = Q(x,y) mod 240
```

Since:

```text
240δ mod 240 = 0
```

the `Fδ` family resolves locally as:

```text
local240(F,δ) = (60 + 4δ²) mod 240
```

This means the `F*` row is a stable high-plane gauge row.

Its dialect variation is carried by the quadratic term `4δ²`, while the `16xy` term provides the bridge coupling into the 240-grid.

---

## 8. Meaning of the 16xy Term

In:

```text
60x² + 16xy + 4y²
```

the term:

```text
16xy
```

is the bridge term.

For `0xFδ`:

```text
16xy = 16 × 15 × δ = 240δ
```

Thus the bridge term is exactly one full local-240 revolution per dialect step.

This gives the clean interpretation:

```text
The F* gauge byte uses 16xy as the dialect-to-orbit bridge.
Each dialect step moves by one 240-cycle bridge revolution.
```

Because `240δ` vanishes under `mod 240`, it does not disturb the local residue.

But it remains meaningful before reduction.

It acts as the synchronization bridge between:

```text
dialect selection
local240 fold
slot5040 receipt orbit
meta-circular array sync
```

Canonical statement:

```text
The low nibble of F* selects the dialect.
The 16xy term carries that dialect through full 240-cycle bridge revolutions.
```

---

## 9. The 11x² Signing Frame

The factored form is:

```text
Q(x,y) = 4(15x² + 4xy + y²)
```

The high-plane term inside the parentheses is:

```text
15x²
```

This may be decomposed as:

```text
15x² = 4x² + 11x²
```

Here:

```text
4x²  = tetragrammatron square frame
11x² = residual signing/orientation frame
```

Therefore:

```text
Q(x,y) = 4(4x² + 11x² + 4xy + y²)
```

Canonical interpretation:

```text
4x² is the square/tetragrammatron base.
11x² is the signing or orientation residue.
4xy is the internal bridge.
y² is the dialect/local variation.
```

The `11x²` term is not a separate external dimension.

It is the residual orientation/signing frame that remains when the 15-fold high-plane term is decomposed into tetragrammatron base plus signature residue.

Authority rule:

```text
11x² may sign orientation.
11x² may not accept state.
Validation and receipt accept.
```

---

## 10. Targeting 5! with the Tetragrammatron Governor

The local tetragrammatron governor uses the 5! base:

```text
5! = 120
```

The doubled local orbit is:

```text
2 × 5! = 240
```

This is the local240 fold.

Canonical relation:

```text
5!  = 120 = local half-orbit
240 = 2 × 5! = signed local orbit
```

The `Q(x,y) mod 240` fold resolves into the local signed 5! orbit.

The Tetragrammatron governor uses this as a circular slide-rule/nomogram resolution layer:

```text
Q(x,y)
  ↓
local240
  ↓
5! half-orbit pair
  ↓
tetragrammatron resolution
  ↓
receipt candidate
```

The gauge header selects the dialect orbit.

The Tetragrammatron governor resolves the local 5! fold.

---

## 11. Targeting 7! with the Receipt Orbit

The receipt orbit uses:

```text
7! = 5040
```

The canonical decomposition is:

```text
5040 = 7 × 3 × 240
```

Therefore:

```text
slot5040 = fano7 × 720 + role3 × 240 + local240
```

where:

```text
fano7   ∈ 0..6
role3   ∈ 0..2
local240 ∈ 0..239
```

This connects:

```text
7       = Fano selector
3       = role selector
240     = doubled 5! local orbit
5040    = 7! full receipt orbit
```

Canonical statement:

```text
The F* gauge header selects a dialect offset.
The quadratic fold resolves local240.
The Fano selector and role selector lift local240 into slot5040.
The receipt ring stores the accepted validation result in the 7! orbit.
```

---

## 12. Base36 Orbit Labels

Base36 is used as a compact human-readable orbit label.

Digits:

```text
0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
```

Useful OMI constants:

```text
5!   = 120  = 3C base36
240  = 2×5! = 6O base36
720  = 6×5! = K0 base36
5040 = 7!   = 3W0 base36
```

A slot in `0..5039` may be encoded as a three-character base36 orbit label:

```text
000 .. 3VZ
```

The full size `5040` is:

```text
3W0
```

but the highest valid zero-based slot is:

```text
5039 = 3VZ
```

Canonical orbit key form:

```text
Gδ:SLOT36:CYCLE36
```

Examples:

```text
FF:000:0000
FF:3VZ:0001
F0:06O:000A
```

Meaning:

```text
Gδ      = gauge dialect
SLOT36  = slot5040 in base36
CYCLE36 = local logical cycle in base36
```

This gives peers and LLMs a compact visible sync label for the receipt orbit.

---

## 13. Meta-Circular Array Sync

The meta-circular array is synchronized by:

```text
gauge dialect
scope cascade
local240 fold
slot5040
cycle
receipt hash
```

Canonical sync tuple:

```text
(gauge, fs, gs, rs, us, local240, slot5040, cycle, receipt-hash)
```

Compact visible form:

```text
Gδ/fs/gs/rs/us/SLOT36/CYCLE36
```

Example:

```text
FF/fs.o/README.md/gs.o/adapters/rs.o/webserial/us.o/open/1ZC/0008
```

This does not make the remote peer’s state true.

It gives a comparable orbit coordinate.

Local validation still decides acceptance.

---

## 14. Nomogram Resolution Circular Array

The nomogram/circular slide-rule model resolves candidates by reducing high-dimensional relation claims into stable circular coordinates.

Pipeline:

```text
gauge header
  ↓
FS/GS/RS/US topology
  ↓
OMI-IMO frame
  ↓
Q(x,y) fold
  ↓
local240
  ↓
slot5040
  ↓
validation
  ↓
receipt
```

The Tetragrammatron governor controls the 5! local fold.

The 7! receipt ring records the accepted orbit.

The LLM, parser, or peer may inspect the coordinate.

Only validation and receipt accept.

---

## 15. F* Dialect Table

Candidate dialect table:

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

A repository MAY define reserved dialects in `REPO.md` or `ADAPTERS.md`.

A dialect declaration SHOULD include:

```omi-lisp
(dialect.byte . 0xF0)
(dialect.name . omicron)
(dialect.scope . fs.o/REPO.md/gs.o/dialects/rs.o/omicron)
(dialect.effect . pure)
(dialect.status . candidate)
```

A dialect MUST NOT accept itself.

A dialect must be validated and receipted before becoming canonical in a repository.

---

## 16. Parser Recognition Rule

Reference parser rule:

```c
#define OMI_NUL 0x00u
#define OMI_FS  0x1Cu
#define OMI_GS  0x1Du
#define OMI_RS  0x1Eu
#define OMI_US  0x1Fu
#define OMI_SP  0x20u

static int omi_is_gauge_byte(omi_u8 b) {
    return b >= 0xF0u;
}

static int omi_gauge_header_match(const omi_u8* p, size_t len) {
    if (len < 8) return 0;

    omi_u8 gauge = p[0];

    if (!omi_is_gauge_byte(gauge)) return 0;
    if (p[1] != OMI_NUL) return 0;
    if (p[2] != OMI_FS)  return 0;
    if (p[3] != OMI_GS)  return 0;
    if (p[4] != OMI_RS)  return 0;
    if (p[5] != OMI_US)  return 0;
    if (p[6] != OMI_SP)  return 0;
    if (p[7] != gauge)   return 0;

    return 1;
}
```

Canonical-only parser rule:

```c
static int omi_canonical_header_match(const omi_u8* p, size_t len) {
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

## 17. Authority Summary

The `F*` header has authority to declare:

```text
this carrier is attempting OMI gauge interpretation
this dialect byte is selected
this pre-language envelope begins here
this envelope is sealed by the matching gauge byte
```

It does not have authority to declare:

```text
the payload is valid
the declaration is accepted
the projection is true
the peer is trusted
the receipt is accepted locally
```

Canonical authority split:

```text
F* header recognizes.
FS/GS/RS/US stages topology.
OMI-IMO frame carries relation.
Q(x,y) resolves local fold.
slot5040 assigns receipt orbit.
validator judges.
receipt accepts.
```

---

## 18. One-Sentence Summary

The OMI `0xF*` gauge header defines a fast-fail meta-initiator where the high nibble `F` selects the gauge family and the low nibble selects the dialect/orbit offset; this byte is interpreted as the `(x,y)` coordinate of the quadratic fold `60x²+16xy+4y²`, where `16xy` bridges dialect selection into 240-cycle local resolution, `11x²` acts as the residual signing/orientation frame, the Tetragrammatron resolves the doubled `5!` local orbit, and the resulting local240 coordinate lifts through `7×3×240 = 5040` into the 7! receipt ring for base36 meta-circular synchronization.

---

# OMI Delta Law Derivation Authority

## What Was Chosen and What Was Discovered

Version: `0.1.0`  
Status: Canonical Draft  
Authority Layer: Delta Law / Orbit Derivation / Gauge Consequence  
Applies to: OMI gauge header, Tetragrammatron governor, 5! local orbit, 7! receipt ring, base36 sync, F* dialect framing

---

## 0. Purpose

This document defines the foundational derivation rule for OMI.

The OMI orbit structure was not chosen by preference.

The chosen object was the delta law.

Everything after that is a consequence, normalization, or carrier interpretation.

Canonical statement:

```text
One thing was chosen:

  delta(x, C) = rotl(x,1) XOR rotl(x,3) XOR rotr(x,2) XOR C

The rest followed from the behavior of that law under fixed-width bounded rotation.
```

---

## 1. What Was Actually Chosen

The chosen law is:

```text
delta(x, C) = rotl(x,1) XOR rotl(x,3) XOR rotr(x,2) XOR C
```

For a fixed width `w`, the reference form is:

```text
delta_w(x, C) =
  rotl_w(x,1)
  XOR rotl_w(x,3)
  XOR rotr_w(x,2)
  XOR C
```

with final masking:

```text
delta_w(x, C) & ((1 << w) - 1)
```

For the 16-bit OMI frame:

```text
delta16(x, C) =
  rotl16(x,1)
  XOR rotl16(x,3)
  XOR rotr16(x,2)
  XOR C
```

The design choices inside the law are exactly four:

```text
1. Use rotations, not shifts.
2. Use XOR.
3. Use a non-trivial constant C.
4. Mask to fixed width.
```

These four choices define the machine.

---

## 2. Why Rotations

Rotations were chosen because no bits are lost.

A shift discards bits.

A rotation recirculates bits.

OMI needs a circular slide-rule memory model, so lost bits would break the orbit.

Canonical rule:

```text
Rotation preserves circulation.
Shift leaks state.
OMI uses rotation.
```

---

## 3. Why XOR

XOR was chosen because it is symmetric, local, reversible as an operation under known operands, and bitwise deterministic.

XOR gives:

```text
a XOR b = b XOR a
a XOR a = 0
a XOR 0 = a
```

In OMI terms:

```text
XOR witnesses difference without destroying the field.
```

Canonical rule:

```text
XOR gives witnessed difference.
Witnessed difference becomes instruction identity.
```

This aligns with:

```text
Instruction witness = OMI XOR IMO
```

---

## 4. Why a Non-Trivial Constant

Without a constant, zero can remain a fixed point.

The constant breaks the zero trap.

Canonical rule:

```text
C breaks the zero fixed point.
```

The constant is not authority.

The constant is a circulation starter.

Validation and receipt still accept.

---

## 5. Why Fixed Width

The law only has stable orbit meaning under fixed width.

The mask is not optional.

Without fixed width, the orbit claim is invalid.

Canonical rule:

```text
Width is part of the law.
Masking is part of the law.
Unbounded integers are not the OMI delta law.
```

For the canonical 16-bit frame:

```text
mask16 = 0xFFFF
```

Every delta step MUST return to:

```text
0x0000..0xFFFF
```

---

## 6. What Was Not Chosen

The following were not arbitrary choices:

|Value|Why it exists|
|---|---|
|Period `8`|Property of the bounded delta law|
|Prime `73`|Smallest prime with decimal period `8`|
|Block `B = [0,1,3,6,9,8,6,3]`|Digits of `1/73`|
|`W = 36`|Sum of `B`|
|Orbit structure|Derived by `divmod` over `W`|
|Local `240`|Doubled `5!` local orbit|
|Ring `5040`|`7! = 7 × 3 × 240`|
|Base36 labels|Human-readable sync notation for orbit positions|

Canonical statement:

```text
The orbit constants were recognized after the delta law was fixed.
They are not independent axioms.
```

---

## 7. The Period-8 Witness

The delta law exposes an 8-step behavior.

That period is the first orbit witness.

The matching decimal-period prime is:

```text
73
```

because:

```text
1 / 73 = 0.0136986301369863...
```

The repeating block is:

```text
B = [0,1,3,6,9,8,6,3]
```

The block sum is:

```text
W = 0 + 1 + 3 + 6 + 9 + 8 + 6 + 3 = 36
```

Canonical rule:

```text
Period 8 gives the block.
The block gives W = 36.
W gives base36 orbit notation.
```

---

## 8. Why Base36 Appears

Base36 was not chosen as decoration.

It appears because the period-8 block sums to:

```text
36
```

Therefore base36 becomes the compact visible orbit label system.

Useful orbit labels:

```text
5!   = 120  = 3C base36
240  = 2×5! = 6O base36
720  = 6×5! = K0 base36
5040 = 7!   = 3W0 base36
```

The valid zero-based 5040-slot ring is:

```text
000 .. 3VZ
```

because:

```text
5039 = 3VZ base36
5040 = 3W0 base36
```

Canonical rule:

```text
Base36 labels the derived orbit.
Base36 does not create the orbit.
```

---

## 9. From Delta Law to 5!

The Tetragrammatron governor resolves local orbit structure through:

```text
5! = 120
```

The signed local orbit is:

```text
240 = 2 × 5!
```

The quadratic fold resolves into this local orbit:

```text
Q(x,y) = 60x² + 16xy + 4y²
local240 = Q(x,y) mod 240
```

Canonical relation:

```text
delta law
  ↓
period witness
  ↓
local circular fold
  ↓
5! half-orbit
  ↓
240 signed local orbit
```

The Tetragrammatron governs this local circular resolution.

---

## 10. From 5! to 7!

The receipt ring lifts the local 240 orbit into:

```text
7! = 5040
```

using:

```text
5040 = 7 × 3 × 240
```

The canonical slot rule is:

```text
slot5040 = fano7 × 720 + role3 × 240 + local240
```

where:

```text
fano7   ∈ 0..6
role3   ∈ 0..2
local240 ∈ 0..239
```

Thus:

```text
local240 gives local orbit.
role3 selects role band.
fano7 selects Fano band.
slot5040 records receipt orbit.
```

Canonical rule:

```text
5! resolves local circulation.
7! records receipt circulation.
```

---

## 11. From Delta Law to F* Gauge Header

The `F*` gauge header is the fast-fail meta-initiator for the derived orbit model.

Generic form:

```text
0xFδ 0x00 0x1C 0x1D 0x1E 0x1F 0x20 0xFδ
```

Canonical OMI:

```text
0xFF 0x00 0x1C 0x1D 0x1E 0x1F 0x20 0xFF
```

Readable:

```text
GAUGE NUL FS GS RS US SP GAUGE
```

The high nibble:

```text
F
```

selects the gauge family.

The low nibble:

```text
δ
```

selects the dialect or orbit offset.

Canonical rule:

```text
0xF* is the gauge row.
F is the family.
* is the dialect.
```

---

## 12. F* as Quadratic Coordinate

The byte:

```text
0xFδ
```

is interpreted as:

```text
x = 15
y = δ
```

in the quadratic fold:

```text
Q(x,y) = 60x² + 16xy + 4y²
```

Therefore:

```text
Q(F,δ) = 60(15²) + 16(15δ) + 4δ²
       = 13500 + 240δ + 4δ²
```

The bridge term is:

```text
16xy = 240δ
```

This is one full local240 revolution per dialect step.

Canonical statement:

```text
The low nibble of F* selects the dialect.
The 16xy term carries that dialect through full 240-cycle bridge revolutions.
```

---

## 13. The 11x² Signing Frame

The quadratic fold is:

```text
Q(x,y) = 60x² + 16xy + 4y²
       = 4(15x² + 4xy + y²)
```

The high-plane term can be decomposed:

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
11x² = residual signing/orientation frame
4xy  = internal bridge
y²   = dialect/local variation
```

Canonical rule:

```text
11x² signs orientation.
11x² does not accept state.
```

The signing frame helps orient the gauge/orbit relation.

Acceptance still belongs to validation and receipt.

---

## 14. What the Gauge Header Does

The gauge header has authority to declare:

```text
this carrier is attempting OMI gauge interpretation
this dialect byte is selected
this pre-language envelope begins here
this envelope is sealed by matching gauge byte
```

It does not declare:

```text
this payload is valid
this peer is trusted
this projection is true
this declaration is accepted
```

Canonical split:

```text
F* header recognizes.
FS/GS/RS/US stages topology.
OMI-IMO frame carries relation.
Q(x,y) resolves local fold.
slot5040 assigns receipt orbit.
validator judges.
receipt accepts.
```

---

## 15. What Was Never Chosen

Canonical table:

|Value|Status|Source|
|---|--:|---|
|Period `8`|discovered|property of delta law|
|Prime `73`|discovered|smallest prime with decimal period 8|
|Block `B`|discovered|repeating digits of `1/73`|
|`W = 36`|discovered|sum of block `B`|
|Base36|derived|visible label for `W = 36`|
|`5! = 120`|derived|local half-orbit|
|`240 = 2×5!`|derived|signed local orbit|
|`7! = 5040`|derived|receipt orbit|
|`F*` gauge row|derived framing|high-nibble meta-initiator for orbit sync|

---

## 16. What Was Actually Chosen

Canonical table:

|Choice|Reason|
|---|---|
|rotations|no bits are lost|
|XOR|symmetric witnessed difference|
|constant `C`|breaks zero fixed point|
|fixed width|keeps state bounded|

These four are the design.

Everything else is consequence.

---

## 17. Canonical Derivation Chain

```text
delta law
  ↓
fixed-width bounded rotation
  ↓
period-8 witness
  ↓
1/73 repeating block
  ↓
B = [0,1,3,6,9,8,6,3]
  ↓
W = sum(B) = 36
  ↓
base36 orbit labels
  ↓
5! local half-orbit
  ↓
240 signed local orbit
  ↓
Q(x,y) mod 240
  ↓
F* gauge row as x=15, y=dialect
  ↓
16xy bridge as 240δ
  ↓
7×3×240 = 5040
  ↓
7! receipt ring
  ↓
local validation
  ↓
receipt
```

---

## 18. Authority Boundary

The derived structure is strong, but it does not replace validation.

Rules:

```text
Delta law produces orbit behavior.
Gauge header selects framing.
Quadratic fold produces coordinates.
Receipt ring stores validation results.
Only validation and receipt accept.
```

The invariant remains:

```text
Recognition is not acceptance.
Citation is not acceptance.
Closure is not acceptance.
Projection is not acceptance.
Validation and receipt accept.
```

---

## 19. One-Sentence Summary

The OMI gauge header, base36 orbit labels, 5! local Tetragrammatron resolution, 7! receipt ring, F* dialect framing, 16xy bridge, and 11x² signing residue are not arbitrary protocol decorations; they are the derived orbit bookkeeping that follows after choosing the fixed-width rotation/XOR delta law `rotl(x,1) XOR rotl(x,3) XOR rotr(x,2) XOR C`, whose four actual design choices are rotations, XOR, a non-trivial constant, and fixed-width masking.