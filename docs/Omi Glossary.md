
# OMI Glossary

## Omicron Object Model, Omi-Gauge, Omi-Lisp, Omi-CONS, and Projection Surfaces

Status: calibrated for current OMI doctrine

Source: previous OMI glossary, retained as historical vocabulary

Current calibration rule:

```text
Omi-* name = development word
Omi relation = protocol form
Receipt = accepted state
```

All entries should be read through:

```text
(name . face)
(face . role)
(role . edge)
(edge . receipt-path)
```

Names do not create modules.

Markers do not create authority.

Projections do not accept state.

Rendering does not accept state.

Bridges do not accept state.

Validation judges.

Receipt accepts.

## A

### Aegean Pointer

An Aegean numeral or separator used as a pointer into a geometry, stream, or rendering LUT.

Aegean pointers do not need variable names. Their codepoints carry their values, and their active frame determines how they are interpreted.

```text
𐄀 → frame / separator / point
𐄁 → dot / line / relation
𐄂 → double dot / triangle / constructor
𐄇 → 1 / tetrahedral pointer
𐄈 → 2 / 5-cell pointer
𐄉 → 3 / 8-cell pointer
```

### Agreement

The OMI principle that a relation becomes accepted only after validation and receipt.

Agreement does not erase difference.

It binds orientation without collapse.

### Agreement Without Collapse

The rule that two surfaces may agree without becoming identical.

```text
projection is not authority
receipt accepts relation
identity remains bounded
orientation is preserved
```

### Allowed Epistemology

The post-address rule that external payloads must declare what kind of knowing they carry.

In Omi-CONS:

```text
CAR = what is carried
CDR = how it continues
CID = whether it agrees
```

### Archimedean Surface

The runtime traversal shell used for walking, projecting, or coordinating an OMI relation.

Its dual coordination shell is Catalan.

### Authority

A receipt-bound accepted relation.

Authority is not the rendered character, glyph, SVG, DOM element, barcode, matrix, or floating-point measurement.

```text
authority = accepted o---o relation
projection = visible/rendered face
```

---

## B

### Base64

A transport encoding for payload bytes.

It may carry CAR/CDR payloads, worker scripts, matrices, or other external data after an OMI relation has already been scoped.

Base64 does not define OMI identity.

Base64 does not accept state.

Encoding is adapter metadata.

Topology is protocol metadata.

Receipt is acceptance metadata.

Canonical payload placement:

```text
omi-<frame>/<control>/<scale>/<relation>/<unit>-imo?<payload>
```

### Block B

The period-8 decimal orbit derived from the Delta law.

```text
B = [0,1,3,6,9,8,6,3]
```

It comes from:

```text
1 / 73 = 0.01369863...
```

The law has period 8. The smallest prime with decimal period 8 is 73. Therefore Block B may be read as a recovered witness pattern.

In OMI, Block B is witness bookkeeping, not authority.

It does not accept state.

### BOM / Bridge Order Marker

The `0x20` row pivot used to fold lower control rows into readable/operator rows.

```text
x XOR 0x20
```

Examples:

```text
0x0D XOR 0x20 = 0x2D "-"
0x0E XOR 0x20 = 0x2E "."
0x0F XOR 0x20 = 0x2F "/"
```

### Boundary

The 16th value of a 4-bit row.

In the 15-of-16 row model:

```text
0x0–0xE = fifteen earned terms
0xF = boundary / Gnomon / Omicron marker
```

### Bridge Row

The `0x20–0x2F` row.

It acts as the barycentric pivot and transition row between low control and readable/operator surfaces.

### Buckyball Orientation Surface

The 60-state orientation surface derived from the 11-cell / L2(11) relation.

```text
L2(11) / Z11 = 660 / 11 = 60
```

This is not the 11-cell itself. It is the 60-coset orientation surface associated with the 11-cell identity shell.

---

## C

### CAR

The source/head payload face in Omi-CONS.

Mnemonic:

```text
CAR = OR
```

Meaning:

```text
CAR admits source presence.
CAR carries the head.
CAR is what is carried.
```

### Catalan Shell

The dual coordination shell to an Archimedean traversal shell.

```text
Archimedean = traversal
Catalan = chiral coordination
```

### CDR

The continuation/tail payload face in Omi-CONS.

Mnemonic:

```text
CDR = XOR
```

Meaning:

```text
CDR carries differential continuation.
CDR is how the payload continues.
```

### CID

The agreement/witness face in Omi-CONS.

Mnemonic:

```text
CID = XNOR
```

Meaning:

```text
CID witnesses lawful agreement between CAR and CDR.
```

### Closure

The bounded completion of a seed into a stable relation.

In OMI, closure is not just termination. It is a lawful fixed state under a declared transformation.

### Codepoint

A symbolic position in a character space.

In OMI, a codepoint is not automatically a character meaning. It is a position that may be rendered through an active LUT.

### Cons

The fundamental dotted pair relation.

```text
(a . b)
```

In OMI:

```text
CAR . CDR
```

becomes:

```text
source/head . continuation/tail
```

### CONS.omi / CONS.imo

The source/runtime cons structure of OMI.

```text
CONS.omi = source-side cons declaration
CONS.imo = runtime-side cons continuation
```

### C0 Control Rows

The low control range:

```text
0x00–0x1F
```

Split:

```text
0x00–0x0F = .omi side
0x10–0x1F = .imo side
```

Relation:

```text
imo = omi XOR 0x10
omi = imo XOR 0x10
```

---

## D

### DataView

The canonical runtime byte interpretation surface for post-address payloads.

Rule:

```text
Base64URL carries bytes.
ArrayBuffer stores bytes.
DataView interprets bytes.
TypedArrays specialize bytes.
Receipt validates attachment.
```

### Delta Law

The one transformer law:

```text
Δ_C(x) = rotl(x,1) XOR rotl(x,3) XOR rotr(x,2) XOR C
```

Its design properties:

```text
rotations preserve bits
XOR is reversible
C breaks the zero fixed point
masking bounds the state
```

The Delta law is the transformer. It does not know geometry. It only moves state.

Width is part of the law.

Masking is part of the law.

Unbounded integers are not the OMI Delta law.

A valid Delta law declaration must specify width, mask, rotation rule, constant `C`, overflow behavior, and test vectors.

The Delta law transforms state.

It does not accept state.

Receipt accepts validated results.

### Derived Character

A character produced from an earned row value through an active LUT.

The character is not authority. It is a rendering.

### Dot

The one instruction of Omi-Lisp and the cons relation.

```text
.
```

In Lisp form:

```text
(a . b)
```

In OMI:

```text
o---o
```

### Dotted Pair

The minimal pair structure.

It is the root of:

```text
list
tree
a-list
closure
stream
frame
receipt path
```

### Dual

A geometry relation where one structure exchanges roles with another.

Examples:

```text
cube ↔ octahedron
dodecahedron ↔ icosahedron
vertex ↔ cell
edge ↔ face
omi ↔ imo
```

---

## E

### Earned Position

A row value that becomes available through permutation, closure, or gauge construction.

OMI does not begin by assigning character meanings. It earns positions first, then renders them.

### Encode

One of the nine universal edges.

```text
encode = project state into carrier form
```

### Epistemic Selector

A selector for one of the four epistemic states:

```text
11 = known known
10 = known unknown
01 = unknown known
00 = unknown unknown
```

### Epistemology Row

The selector region that determines what kind of knowing is attached to a relation.

In post-address form, epistemology is carried by Omi-CONS.

### External Query Plane

The post-address `?---?` surface.

```text
omi-<frame>/<control>/<scale>/<relation>/<unit>-imo?<payload>
```

This plane carries external payloads, DataView buffers, CAR/CDR/CID frames, worker scripts, matrices, or BLOBs.

### External Witness Mask

A post-address mask proving a folded gauge result.

Canonical example:

```text
0xFFFFFF
```

This is a saturated 24-bit Omi-Gauge witness surface, not the native OMI address.

---

## F

### Fano Closure

The 7-bit bounded closure kernel tied to the Fano plane and Hamming(7,4) structure.

It uses:

```text
B7 = {0,1}^7
MASK7 = 0x7F
```

### Fano Plane

The 7-point, 7-line incidence structure used by OMI as a minimal incidence scheduler.

```text
point = addressable identity position
line = valid triplet relation
triple = selected closure
```

### Fiber

A contextual rendering or interpretation layer.

The same abstract value may render differently depending on the active fiber:

```text
ASCII
Aegean
Braille
BCD
Unicode private-use
geometry
DOM
barcode
shader
```

### Float / Floating Projection

A runtime measurement projection.

Floating point may render, accelerate, or approximate, but it does not define identity.

```text
floating point = projection
receipt = accepted relation witness
```

### Fold

A reversible relation between rows or surfaces.

Example:

```text
0x10–0x1F XOR 0x20 = 0x30–0x3F
```

### Frame

A bounded OMI address or carrier context.

Canonical address form:

```text
omi-<frame>/<control>/<scale>/<relation>/<unit>-imo
```

### Function Scale

A selected runtime interpretation scale from Omi-Nomogram.

Examples:

```text
logarithmic
square/root
cube/root
trigonometric
Pythagorean
sexagesimal
quadratic
LFSR period
```

---

## G

### Gauge

A bounded table of row positions used to interpret OMI state.

### Gauge Orbit

A row-cyclic interpretation of Omi-Gauge.

The key visible orbit block is:

```text
0x40–0x4F
```

### Geometry Map

A LUT from abstract pointers to renderable geometries.

Example:

```text
𐄀 → point
𐄁 → line
𐄂 → triangle
𐄇 → tetrahedron
𐄈 → 5-cell
𐄋 → 24-cell
𐄎 → Hopf fiber
```

The map is for rendering. It is not the transformer.

### Gnomon

The orientation/shadow/pointer surface produced when a relation is measured against another.

In algebraic form:

```text
a² - b² = (a+b)(a-b)
```

In OMI:

```text
larger frame - smaller frame = bridge surface
```

### Gnomon Boundary

The `0xF` value in a 4-bit row when it acts as the 16th fold/separator.

### Greek Numeral Overlay

A symbolic mnemonic layer using Greek numeral history.

Examples:

```text
ϛ = stigma = sixfold tick mnemonic
ϟ = koppa = turning/q-gate mnemonic
ϡ = sampi = high terminal carrier mnemonic
```

This is a mnemonic overlay, not the machine authority.

---

## H

### Handoff

A transition from symbolic pre-runtime derivation into runtime or external payload surface.

Canonical sentinel:

```text
0x7C
```

### HNSW Projection

A runtime nearest-neighbor or distance measurement projection.

HNSW navigates a relation after Omi-Nomogram and Omi-Gauge have selected and resolved the relation.

HNSW result is not acceptance.

Nearest-neighbor search is not acceptance.

It is a measurement/projection surface, not authority.

Validation and receipt accept.

### Hopf Terms

The 15 earned abstract terms of the local nibble row.

```text
0x0–0xE = Hopf terms
0xF = boundary
```

They should be treated as pure row values first, not as fixed ASCII characters.

---

## I

### Identity

A receipt-bound OMI relation.

Identity is not the rendered character. Identity is the accepted address and lawful relation.

### IMO

The runtime/closure side of OMI.

Readable gate:

```text
imo
```

Compiled gate:

```text
Ο = U+039F
```

### Incidence

A valid relation selected by geometry, Fano/Horn closure, or scheduler rules.

### Instantiate

The runtime action beneath Intent.

```text
Intent → instantiate
```

### Internal RPC Bridge

The surrogate-based bridge used for world-length payload split/rejoin.

```text
0xD800–0xDBFF = high bridge half
0xDC00–0xDFFF = low bridge half
```

### Invalidate

The runtime action beneath Interrupt.

```text
Interrupt → invalidate
```

---

## J

### Join

One of the nine universal edges.

```text
join = bind worlds, frames, keys, or relations
```

In Omi-CONS256:

```text
bits 32–63 = JOIN()
```

---

## K

### Ket Axis

The significant axis or payload direction used during XOR interpolation.

Used in Omi-CAR and Omi-CDR profiles.

### Known Known

The epistemic state:

```text
11
```

In high-order Omi-CONS256, the Omicron closure marker may act as a known-known witness bit.

---

## L

### Lisp 1.5 Surface

The historical Lisp-like dot notation and association-list layer that Omi-Lisp uses as its minimal symbolic surface.

### LUT

Lookup table.

In OMI, the LUT renders abstract row positions into concrete forms.

```text
row value → active fiber → rendered character/geometry
```

The LUT is renderer-side.

The transformer does not need it.

---

## M

### Matrix

A relation field.

In OMI:

```text
Omi-Matrix = observation/relation field
```

It instantiates the relations selected by Omi-Nomogram and resolved by Omi-Gauge.

### Meta-Object

The high-order payload or object surface carried after identity.

In Omi-CONS256:

```text
bits 128–255 = META-OBJECT()
```

### Mirror

A chiral reverse or paired relation.

Example:

```text
0xNM ↔ 0xMN
```

### Monoid

The associative stream composition layer of Omi-Lisp.

```text
omi::lisp : monoid (M, •)
```

### Monster Rupture

The symbolic boundary where positional representation no longer contains the system and a larger symbol space must be earned.

This is a conceptual rupture, not an ordinary numeric literal.

---

## N

### Native OMI Gauge

The local OMI gauge region used for pre-language control, local declaration, and deterministic interpretation.

```text
0x00..0x20 = pre-language control envelope
0x00..0x3F = Tetragrammatron governor region
0x00..0x7F = local declaration region
0xF*       = external gauge pre-header family
```

Gauge values recognize, stage, or orient.

They do not accept state.

### Native OMI Frame

The `2¹⁶` core identity/address field.

```text
2¹⁶ = native OMI frame/gauge field
```

### Nomogram

A declarative alignment of scales.

In OMI:

```text
Omi-Nomogram = declarative runtime function-scale surface
Omi-SlideRule = operational/mechanical behavior of that surface
```

### NULL

The beginning axis.

NULL is not a digit string.

```text
NULL != 1
NULL != 10
NULL != 1000
```

NULL is the position from which representation breaks and symbols are earned.

---

## O

### Ο

Uppercase Omicron.

```text
Ο = U+039F
```

Compiled closure gate.

### ο

Lowercase omicron.

```text
ο = U+03BF
```

Compiled entry gate.

### o---o

Minimal tangent / Omi-Point relation notation.

### Omi-Alist

Association-list declaration surface.

A stream of paired relations.

### Omi-Bidi

Directionality and chirality steering surface.

### Omi-CAR

Source/head payload view in Omi-CONS.

### Omi-CDR

Continuation/tail payload view in Omi-CONS.

### Omi-CID

Agreement/witness view in Omi-CONS.

### Omi-CONS

The post-address payload binding frame of allowed epistemology.

Canonical compact form:

```text
?car:<OR>;cdr:<XOR>;cid:<XNOR>
```

CAR carries the source/head payload.

CDR carries the continuation/tail payload.

CID witnesses lawful agreement between CAR and CDR.

Omi-CONS attaches payload after the OMI address has been scoped.

It does not replace FS/GS/RS/US identity.

Receipt accepts the lawful attachment.

### Omi-CONS256

A candidate post-address symbolic meta-object envelope.

It is distinct from the canonical 256-bit OMI---IMO frame.

Canonical OMI---IMO frame:

```text
8 × 16-bit scope fields
+
4 × 32-bit relation fields
=
256 bits
```

Omi-CONS256 may describe a payload organization above or beside that frame.

It must not redefine the canonical OMI---IMO frame.

### Omi-Compass

Agreement orientation face.

It gives direction to a relation.

### Omi-Dali

Unfolded hypercube lookup and subsurface matrix.

### Omi-Form

Structural projection of an OMI object.

### Omi-Gate

Neutral origin where payloads are located, replayed, and compared.

### Omi-Glyph

Symbolic/glyph projection of an OMI object.

### Omi-Gnomon

Orientation, shadow, pointer, and right-angle synchronization surface.

### Omi-Gauge

A spatial resolver face.

```text
64 × 64 × 16 = 65,536
```

It may map row, x, y, and local cell positions into a 2^16 gauge surface.

It computes or resolves candidate positions.

It does not accept them.

Validation judges.

Receipt accepts.

### Omi-Hash

Digest identity check.

A hash says bytes match.

### Omi-Image

Package/carrier projection for universal edges.

### Omi-Barcode

Canonical barcode carrier face.

A barcode surface carries or recovers representation.

It does not validate, accept, or create authority.

Barcode differences are resolved through OMI/IMO addressing, receipt context, payload relation, file relation, code relation, data relation, and port relation.

Omi-JabCode is a polychromatic matrix barcode face under Omi-Barcode.

### Omi-Lisp

The declarative dot-notation computation layer.

It derives lawful computation from relations instead of defining variables imperatively.

### Omi-Matrix

Observation/relation field.

Instantiates relation matrices for rendering, geometry, and measurement.

### Omi-Mesh

Relation-located network/field projection.

### Omi-Nomogram

Declarative function-scale selector surface.

Canonical row:

```text
0x30–0x3F
```

The nomogram selects the scale.

It does not accept state.

Receipt accepts.

### Omi-Notation

The streamable, loggable a-list of pointer/reference tuples.

### Omi-Plane Ceiling

The Unicode external ceiling sentinel:

```text
Ωmax = U+10FFFF
```

### Omi-Point

The minimal OMI relation.

```text
omi---imo
o---o
```

As notation, it is a candidate relation.

After validation and receipt, it becomes the smallest accepted relation.

### Omi-Receipt

Lawful relation witness.

A receipt says the relation was resolved under OMI protocol, not merely that bytes match.

### Omi-Ring

Spectral/circular orbit witness.

Connects to `60x²`, sexagesimal degree, and circular measurement.

### Omi-Shadow

Secondary projection tied back to a source rule.

### Omi-SlideRule

Operational behavior of Omi-Nomogram.

It describes how the selected scales align, fold, invert, and compute.

It does not accept state.

Receipt accepts.

### Omi-Tape

Sequential barcode/script carrier.

### Omi-Torus

Gray-code / two-cube minimization surface.

### Omi-Voxel

Tile-map / extrusion / architectural surface.

### Omi-World

Environment or persistent world projection.

### OMI-GPIO

Physical voltage/agreement transduction surface.

### Omicron

The OMI gate concept.

Readable:

```text
omi ... imo
```

Compiled:

```text
ο ... Ο
```

### Omicron Object Model

The full name of OMI.

```text
OMI = Omicron Object Model
```

### Ωmax

The Unicode ceiling sentinel:

```text
U+10FFFF
```

It does not replace `ο` or `Ο`.

---

## P

### Path Plane

The slash descent surface:

```text
/---/
```

It belongs to internal identity descent.

### Payload Plane

The query surface:

```text
?---?
```

It belongs to external data attachment.

### Period-Prime Anchor

The row value:

```text
0x49 = 73
```

in the `0x40–0x4F` Omi-Gauge orbit block.

### Pointer

A possible route or address-bearing relation.

In OMI, a pointer may be nullable or unresolved until receipt.

### Private Unicode Mirror

A safe Unicode private-use rendering of the OMI gauge.

```text
0x00–0x7F → U+E000–U+E07F
```

### Projection

A rendered face of an OMI relation.

Projection may be visual, symbolic, geometric, audio, DOM, barcode, matrix, or payload.

Projection is not authority.

Projection exposes a face.

Receipt accepts the relation.

This surface may render, carry, expose, or orient an OMI relation.

It does not accept state.

Validation and receipt accept.

---

## Q

### Q(x,y)

The OMI quadratic stack:

```text
Q(x,y) = 60x² + 16xy + 4y²
```

Meaning:

```text
4y² = local control kernel
16xy = bridge/spatial resolver
60x² = orientation/world surface
```

### Query Plane

The `?---?` external payload plane.

### Quasigroup

The algebraic recovery structure of Omi-Lisp.

```text
(Q, ∗, \, /)
```

Meaning:

```text
∗ = compose
/ = recover from right/output side
\ = recover from left/input side
```

---

## R

### Receipt

A lawful witness of accepted relation.

Receipt records acceptance after validation.

Receipt is the durable witness, provenance record, replay unit, synchronization unit, geometric seed, symbolic seed, and transport payload.

### Reference

An accepted binding.

Unlike a pointer, a reference is non-null after acceptance.

### Render

To project an abstract value through a LUT into a visible or executable face.

### Renderer

The component that knows the geometry map, glyph map, DOM map, shader map, or carrier map.

The renderer does not define the transformer.

### Row

A bounded group of positions in the OMI gauge.

Examples:

```text
0x00–0x0F = .omi control row
0x10–0x1F = .imo control row
0x20–0x2F = bridge row
0x30–0x3F = Omi-Nomogram row
0x40–0x4F = Omi-Gauge orbit block
```

### RPC Bridge

The internal split/rejoin mechanism using surrogate structure.

### Rupture

A symbolic break where the current representation cannot contain the next relation, so a new symbol or surface must be earned.

---

## S

### Schläfli Surface

A geometry descriptor surface for regular polytope configuration.

### Selector

A path or row value that chooses the active relation, function, epistemic state, or geometry.

### Sexagesimal Gate

The `0x3C` Omi-Nomogram slot.

```text
0x3C = 60 decimal
```

Used for circular degree, 60x² orientation, Omi-Ring, and Omi-Compass.

### Significand Axis

The payload axis used in Omi-CAR/Omi-CDR interpolation.

### SlideRule

The operational face of Omi-Nomogram.

### Snub / Truncation

Geometry operations used as projection transformations.

### Surrogate Band

The UTF-16 surrogate range:

```text
0xD800–0xDFFF
```

Used by OMI as internal RPC bridge math, not as public text.

### Symbolic Authority

Identity based on exact symbolic position, row, relation, and receipt.

Not floating point.

Not rendered glyph.

Not approximate coordinate.

---

## T

### Tetragrammatron

The four-scope naming body plus validation anchor.

```text
FS = file scope
GS = group scope
RS = record scope
US = unit scope
V  = validation / resolution anchor
```

The four scopes form the tetrahedral scope body.

The validation anchor lifts the scope body into the 5-cell local resolution body.

The Tetragrammatron names scope.

The 5-cell resolves local incidence.

The 11-cell orients signed relation flow.

Validation and receipt accept.

### Transformer

The component that applies the Delta law.

It knows:

```text
bits
rotations
XOR
constant
mask
```

It does not know:

```text
geometry
glyphs
rendering
DOM
meaning
```

### Tuple

An ordered payload.

In Omi-Notation, tuples are carried in a replayable a-list stream.

### Two-Cube

The full byte mirror relation:

```text
0xNM ↔ 0xMN
```

Formulas:

```text
cell(N,M) = (N << 4) | M
mirror(N,M) = (M << 4) | N
delta(N,M) = cell(N,M) XOR mirror(N,M)
```

### TypedArray

A specialized runtime view over bytes.

TypedArrays are useful projections, but DataView is the canonical general interpreter.

---

## U

### Universal Edges

The nine universal OMI edges:

```text
rule
fact
closure
combinator
cons
join
compose
encode
decode
```

All projection surfaces reduce to these edges.

### Unicode Plane Surface

The external Unicode hierarchy used by OMI as carrier/plane context.

```text
U+0000–U+10FFFF
```

### U+10FFFF

The maximum Unicode codepoint.

OMI name:

```text
Ωmax
```

### U+E000–U+E07F

The private-use mirror of OMI’s native 0x00–0x7F gauge.

---

## V

### Value

The invariant abstract row position.

Example:

```text
0x0
```

is a value. Its character rendering depends on LUT/fiber.

### VOID

The origin or non-active state, depending on active scale.

In 15-of-16 tick language:

```text
VOID is not part of the active orbit.
```

---

## W

### World Length

The supplementary-plane length of an external/world projection.

Surrogate bridge:

```text
1024 × 1024 = 1,048,576 supplementary positions
```

### Witness

A witness or receipt surface showing lawful attachment or relation.

Example:

```text
0xFFFFFF = saturated 24-bit Omi-Gauge witness mask
```

---

## X

### XOR

The reversible differential operator used by the Delta law and Omi-CDR.

```text
XOR = difference / transition / continuation
```

### XNOR

The agreement/equality operator used by Omi-CID and dotted-pair equivalence.

```text
XNOR = lawful agreement / equality witness
```

---

## Y

### y / 4y²

The local control component of the OMI quadratic.

```text
4y² = local tetrahedral/control kernel
```

---

## Z

### Zero

OMI does not begin from ordinary numeric zero.

It begins from NULL as axis and uses receipt-bound symbolic position.

### 5040 Ring

A receipt coordination ring.

It may support slot lookup, comparison, conflict inspection, lineage replay, and peer synchronization.

It does not globally decide truth.

It does not accept state.

Local validation decides.

Receipt accepts.

### 0x3C

Sexagesimal gate.

```text
0x3C = 60
```

### 0x3F

Omi-Nomogram LFSR/period/query gate.

### 0x40–0x4F

Omi-Gauge orbit block.

Contains:

```text
0x40 = 64 gauge threshold
0x49 = 73 period-prime anchor
0x4F = Omicron-facing closure mnemonic
```

### 0x7C

Handoff / pipe / runtime transition sentinel.

### 0xAA55

External boot bridge witness.

```text
before 0xAA55 = symbolic derivation may be staged
at 0xAA55 = external boot bridge may be recognized
after 0xAA55 = runtime projection may be requested
receipt = accepted boot state
```

In synthetic boot, OMI may stage this bridge word.

In external boot, OMI must observe this bridge word if the carrier requires it.

0xAA55 is not acceptance.

It is not authority.

It is a bridge marker between OMI boot framing and external boot conventions.

Validation and receipt accept boot state.

### 0xFFFFFF

Saturated 24-bit Omi-Gauge external witness mask.

---

# Final Compact Glossary Canon

```text
NULL is the axis.
Dot is the one relation.
Delta is the transformer.
Rows earn abstract values.
LUTs render values into characters or geometry.
Omi-Gauge resolves candidate positions in the 64-lane spatial field.
Omi-Nomogram selects the function scale.
Omi-Matrix instantiates relation fields.
Omi-Gnomon orients the result.
Surrogate RPC bridges world-length payloads.
Omi-CONS carries allowed epistemology through CAR, CDR, and CID.
Validation judges.
Receipt accepts.
```
