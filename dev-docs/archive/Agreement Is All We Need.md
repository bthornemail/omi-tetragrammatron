# Agreement Is All We Need

## The OMI Object Model Protocol Specification

### 1. Agreement Doctrine

In the OMI Object Model, **agreement** is not passive approval or social consensus.

Agreement is the moment an intuition, dispute, declaration, or relation is lowered into a bounded structure that can be:

```text
declared
validated
receipted
replayed
projected
```

Disagreement is not failure. Disagreement is an unresolved path.

```text
Disagreement = missing structure.
Agreement    = validated structure with receipt.
```

OMI therefore treats agreement as a compiler primitive.

A disagreement becomes useful when it produces one of the following structural outputs:

| Disagreement Type  | Constructive Output           |
| ------------------ | ----------------------------- |
| Missing boundary   | new OMI address or scope      |
| Missing rule       | protocol invariant            |
| Missing fact       | bound citation                |
| Missing test       | accept/reject validation case |
| Missing closure    | bounded logic surface         |
| Missing combinator | lawful operation              |
| Missing replay     | receipt path in the ring      |
| Missing projection | OMI-Surface adapter           |

The canon rule is:

```text
The reader may recognize.
The resolver may promote.
Only validation and receipt may accept.
```

Agreement is therefore not belief.

Agreement is accepted structure.

---

### 2. Four Authorities

OMI is organized by four authorities. These are not merely source files. They are boundaries of responsibility.

```text
OMI               = Citation Authority
Tetragrammatron   = Validation Authority
Metatron          = Projection Authority
IMO               = Carrier Authority
```

#### OMI — Citation Authority

OMI declares identity.

It owns:

```text
addresses
cells
CONS
CAR
CDR
CID = CAR XOR CDR
REGISTER
STACK
```

OMI does not validate truth.
OMI does not render geometry.
OMI does not transport bytes.

#### Tetragrammatron — Validation Authority

The Tetragrammatron validates.

It owns:

```text
DeltaC
rotation
Polybius closure
diagonal law
receipt emission
5040 replay ring
accept / reject / defer
```

It does not project surfaces.
It does not serialize files.
It does not render geometry.

#### Metatron — Projection Authority

Metatron composes accepted receipts into surfaces.

It owns:

```text
Smith chart view
Genaille-Lucas view
CSS cascade view
TwinViewer geometry
Betti / Schläfli surfaces
Platonic / Archimedean / Catalan projections
GPIO topology views
```

Metatron does not validate.
Metatron does not mutate canon.
It only projects accepted receipts.

#### IMO — Carrier Authority

IMO carries representations.

It owns:

```text
files
HTTP
MQTT
WebRTC
Web Serial
Git
flash
JSON
DOM
GPIO packets
```

IMO does not validate.
IMO does not decide truth.
It carries what has already been structured.

---

### 3. The Receipt Cross

OMI is not a pipeline. It is a cross centered on the receipt.

```text
                    Metatron
              Projection Authority
                     ▲
                     │
OMI ◄──────────── Receipt ────────────► IMO
Citation Authority                       Carrier Authority
                     │
                     ▼
              Tetragrammatron
              Validation Authority
```

The receipt is the only shared invariant.

```text
OMI cites.
Tetragrammatron validates.
Metatron projects.
IMO carries.
Receipt accepts.
```

No surface accepts.
No carrier accepts.
No notation accepts.
No projection accepts.

Only the receipt accepts.

---

### 4. Canonical OMI Cell

The canonical OMI machine cell is a 256-bit bitboard:

```text
S0-S1-S2-S3/S4/S5/S6/S7?REGISTER?STACK@CAR@CDR
```

Packed layout:

```text
S0       bits 0..15
S1       bits 16..31
S2       bits 32..47
S3       bits 48..63
S4       bits 64..79
S5       bits 80..95
S6       bits 96..111
S7       bits 112..127
REGISTER bits 128..159
STACK    bits 160..191
CAR      bits 192..223
CDR      bits 224..255
```

The address is syntax.

The fields are grammar.

The receipt is the accepted result.

```text
S3      = opcode / active verb
S4-S7   = operands / subpath controls
REGISTER = frame surface
STACK    = buffer surface
CAR      = source face
CDR      = route face
CID      = CAR XOR CDR
```

---

### 5. Rotation-Only Delta Law

OMI uses rotation, not shifting, for protocol motion.

A shift loses information.

A rotation preserves information.

The canonical transition law is:

```text
DeltaC(x) =
  rotl(x,1)
  XOR rotl(x,3)
  XOR rotr(x,2)
  XOR C
```

Where `C` is the carried closure constant.

The rule is:

```text
Rotations belong to validation.
Shifts belong only to external carriers, timers, queues, or CONS ingestion.
```

Inside the circular slide rule, nothing falls off the edge.

The edge returns through the orbit.

---

### 6. Polybius Base60 Closure

The hexadecimal nibble square is:

```text
0  1  2  3
4  5  6  7
8  9  A  B
C  D  E  F
```

The fixed diagonal is:

```text
D+ = {0,5,A,F}
sum(D+) = 0x1E
xor(D+) = 0
```

The inverse diagonal is:

```text
D- = {3,6,9,C}
sum(D-) = 0x1E
xor(D-) = 0
```

Together:

```text
D+ ∪ D- = {0,3,5,6,9,A,C,F}
sum = 0x3C
```

The non-diagonal carrier set is:

```text
K = {1,2,4,7,8,B,D,E}
sum = 0x3C
```

Therefore the nibble wheel partitions into two base60 surfaces:

```text
diagonal closure surface = 0x3C
carrier function surface = 0x3C
whole wheel              = 0x78
```

Canonical boot relation:

```text
0x78 = carry-close
0x7C = boot delimiter
0x7C00 = boot page
```

---

### 7. Structural Validation

A frame is not accepted because it renders.

A frame is accepted only when the Tetragrammatron validates its finite relation.

Validation checks:

```text
address shape
field bounds
CID = CAR XOR CDR
DeltaC transition
Polybius closure
diagonal surface balance
receipt consistency
ring position
```

A projection can reveal a structure.

A projection cannot authorize a structure.

This is the non-collapse criterion:

```text
Projection MUST NOT validate.
Carrier MUST NOT validate.
Notation MUST NOT validate.
Receipt MUST validate.
```

---

### 8. Fano Receipt Layer

The Fano plane supplies the finite projective receipt layer.

It has:

```text
7 points
7 lines
3 points per line
3 lines through each point
```

Validation rule:

```text
Each point appears on exactly three lines.
Each line contains exactly three points.
The point-line dual agrees.
```

The Fano layer is not a drawing.
It is a receipt structure.

A broken Fano table is rejected before projection.

---

### 9. Omicron Binary Quadratic Form

OMI uses the quadratic form:

```text
Q(x,y) = 60x² + 16xy + 4y²
```

This is not first a metric equation.
It is an incidence partition.

```text
4y²:
  tetrahedral self-dual seed
  local centroid unit
  visible selector face

16xy:
  cube/octahedron tangent branch
  crossing
  chirality
  traversal
  common-core relation

60x²:
  icosahedron/dodecahedron high-shell field
  5-fold incidence
  golden-ratio projection witness
```

The visible bridge is:

```text
Q(x,y) mod 240
```

The replay ring is:

```text
5040 = 7 × 3 × 240
```

So:

```text
240  = local visible bridge
720  = semantic sweep
5040 = replay ring
```

---

### 10. Replay Ring

The accepted receipt enters the 5040-slot replay ring.

A receipt records:

```text
provenance
step count
gauge / lens
source
route
result
epoch
slot
```

The ring is logical authority.

Files are only serialization.

```text
Tetragrammatron owns the ring.
IMO owns disk, network, flash, and transport.
```

Overwrite policy:

```text
cold slot:
  overwrite permitted

warm slot:
  re-verify before change

stale slot:
  overwrite with drift warning

CAS failure:
  retry and re-check
```

No file write implies acceptance.

Only receipt validation implies acceptance.

---

### 11. OMI-Surfaces

An OMI-Surface is a deterministic projection of a receipt.

Examples:

```text
Smith chart
Genaille-Lucas ruler
CSS cascade
TwinViewer geometry
SVG
WebGL
GPIO topology
WordNet surface
JAB/debug carrier
```

All surfaces read the same receipt.

No surface mutates canon.

```text
Receipt
  ├─ Smith view       continuous coordinate
  ├─ Genaille view    discrete nomogram route
  ├─ CSS view         declarative browser cascade
  └─ Twin view        geometric projection
```

The rule is:

```text
Receipts are source.
Surfaces are views.
Validation remains outside the surface.
```

---

### 12. Mediant Surface Transition

A receipt can be read as a point.

A receipt pair can be read as a transition.

Given two rational slopes:

```text
a/b
c/d
```

their mediant is:

```text
(a+c)/(b+d)
```

The determinant is:

```text
bc - ad
```

Canonical interpretation:

```text
receipt A = point
receipt B = point
receipt pair = transition
mediant = admissible next surface coordinate
determinant = boundary witness
mean ladder = pressure profile
```

The mean ladder is:

```text
HM ≤ GM ≤ AM ≤ QM
```

This gives Metatron a deterministic way to project transition pressure without letting projection validate canon.

---

### 13. Constants Without Constants

OMI does not reject classical constants.

It relocates them.

```text
square root appears when squared incidence is read as length
π appears when chiral diagonal closure is rendered as angular half-turn
φ appears when {3,5}/{5,3} incidence is projected into metric form
```

Constants are not protocol premises.

They are projection witnesses.

```text
Incidence
→ validation
→ receipt
→ projection
→ constant witness
```

Not:

```text
constant
→ coordinate
→ authority
```

This keeps deterministic validation independent of floating-point projection.

---

### 14. Agreement Loop

The OMI collaboration loop is:

```text
1. receive intuition or disagreement
2. name boundary
3. declare address
4. write rule
5. seal closure
6. ground fact
7. compose combinator
8. write test
9. validate frame
10. emit receipt
11. replay ring
12. project surface
```

Disagreement becomes construction.

Agreement becomes receipt.

---

### 15. Minimal Canon

```text
Code is data.
File is port.
Notation is citation.
Canon validates.
Cosmology projects.
Receipt accepts.
```

And:

```text
OMI cites.
Tetragrammatron validates.
Metatron projects.
IMO carries.
Receipt accepts.
```

Minimal formal statement:

```text
valid(frame) → receipt
receipt → agreement
```

Or:

```text
Q_frame(S) = 0
  does not mean belief.

Q_frame(S) = 0
  means the structure may be receipted.
```

Final canonical statement:

```text
Agreement is the successful lowering of shared intelligence into validated, replayable, receipt-bearing structure.
```
