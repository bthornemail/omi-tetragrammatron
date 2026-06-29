# OMI Protocol Write-Up

> Current vocabulary note: this archive document is retained as historical source material. Active doctrine now distinguishes `Omi-Ring` from `receipt`: the Omi-Ring is the addressed palindromic notation witness, while a receipt is the accepted state of that witness. See `dev-docs/archive/Omi-Ring as Addressed Notation Witness.md` and `dev-docs/OMI_ADDRESSING.md`.

## Omicron Object Model / Tetragrammatron Receipt Protocol

### 0. Protocol Sentence

OMI treats **code as data**, **file as port**, and **notation as citation**, while separating **canon** from **cosmology**.

Canon validates.  
Cosmology projects.  
Receipt accepts.

The protocol is not a website, not a rendering engine, and not a belief system. It is a deterministic notation-citation protocol for carrying, validating, resolving, recording, and projecting structured receipts across multiple surfaces.

---

# 1. Purpose

The purpose of the OMI protocol is to define a deterministic way to move from notation to receipt without allowing any visual surface, file boundary, carrier format, or rendered interface to falsely claim authority.

The protocol answers four questions:

```text
What was cited?
What rule validates it?
What receipt records it?
What surface projects it?
```

The system must avoid the trap:

```text
render → believe
```

and preserve the production posture:

```text
validate → resolve → record → project → inspect
```

The OMI protocol therefore separates:

```text
citation
validation
resolution
recording
projection
inspection
```

No layer is allowed to silently replace another.

---

# 2. Final Covariant Rule

The final covariant rule is:

```text
Code is data.
File is port.
Notation is citation.
Canon validates.
Cosmology projects.
Receipt accepts.
```

These pairs define the authority boundary of the protocol.

## 2.1 Code as Data

Code is executable only inside a runtime context. Within the protocol, code is first treated as carried material.

A script, function, expression, or worker body does not become true because it can run. It becomes inspectable data until a validator accepts its frame.

```text
Code execution does not imply truth.
```

## 2.2 File as Port

A file is not merely inert storage. A file is a bounded ingress or egress surface.

A file may carry notation, source, frames, receipts, optical carriers, logs, or projections, but file existence does not imply acceptance.

```text
File IO does not imply acceptance.
```

## 2.3 Notation as Citation

Notation names a structure. Citation binds that structure to resolvable receipt history.

An OMI string, DOM attribute, pointer, CSS variable, barcode, or visual symbol remains notation until it can be resolved as a citation.

```text
Notation does not imply receipt.
```

## 2.4 Canon and Cosmology

Canon is the accepted rule-space.

Cosmology is the projected world-space.

Canon is not the page.  
Canon is not the 3D scene.  
Canon is not the SVG.  
Canon is not the CSS.

Those are cosmological surfaces. They may display, inspect, route, and compare receipts, but they do not accept authority.

```text
No cosmology writes canon.
```

---

# 3. Authorities

The protocol is divided into authority roles.

```text
OMI             Citation Authority
Tetragrammatron Validation Authority
Metatron        Projection Authority
IMO             Carrier Authority
Receipt         Shared invariant
```

## 3.1 OMI — Citation Authority

OMI preserves identity and citation.

It answers:

```text
What is being referred to?
Where is the address-ruler?
What frame is being cited?
```

OMI does not prove truth by itself. It preserves the citation body.

## 3.2 Tetragrammatron — Validation Authority

Tetragrammatron preserves validation.

It answers:

```text
Does this frame satisfy the accepted rule?
Does this citation close?
Does this receipt pass the validator?
```

Tetragrammatron does not create identity. It tests structure.

## 3.3 Metatron — Projection Authority

Metatron preserves interpretation and projection.

It answers:

```text
How should this accepted or inspectable state be projected?
Which geometry, surface, view, or route displays it?
```

Metatron does not validate truth. It renders or projects the state.

## 3.4 IMO — Carrier Authority

IMO preserves representation.

It answers:

```text
How is the frame transported?
How is it encoded?
How is it recovered from a surface?
```

IMO does not define identity and does not validate authority.

## 3.5 Receipt — Shared Invariant

The receipt is the shared invariant across authorities.

A receipt is the accepted record that survives movement across carriers, projections, files, ports, and visual surfaces.

The central rule is:

```text
The reader may recognize.
The resolver may promote.
Only validation and receipt may accept.
```

---

# 4. Address and Frame Model

The protocol uses an OMI ruler frame.

The current canonical long-word form is:

```text
o-[FS]:0x0000-[GS]:0x0000-[RS]:0x0000-[US]:0x0000
 /[FS]:0x0000/[GS]:0x0000/[RS]:0x0000/[US]:0x0000
 ?[REGISTER]:0x00000000
 ?[STACK]:0x00000000
 @[CAR]:0x00000000
 @[CDR]:0x00000000
```

Compactly:

```text
o-S0-S1-S2-S3/S4/S5/S6/S7?REGISTER?STACK@CAR@CDR
```

The current C frame uses sixteen-bit fields:

```text
S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

A pointer form may appear as:

```text
omi-0100-03bf-7c00-2b01-2f01-1434-039f-01ff/48
```

The eight main `S` segments form a 128-bit reference frame:

```text
S = uint16_t[8]
```

The canonical carrier frame pattern is:

```text
0xLL00-0x03bf-0xNNNN-0x2bLL-0x2fLL-0xMMMM-0x039f-0xLLff
```

Where:

```text
LL = local lane / layer / logical selector
NN = citation or payload word
MM = mask / metric / resolving word
```

The protocol treats the address as a **ruler**, not as a conventional instruction field.

The opcode is not a stored field. It is the witnessed difference between ruler position and route closure.

```text
Instruction witness = OMI XOR IMO
```

---

# 5. Canonical Processing Pipeline

The production pipeline is:

```text
recognize
  ↓
validate
  ↓
resolve
  ↓
record
  ↓
project
  ↓
inspect
```

More concretely:

```text
carrier input
  ↓
decode S
  ↓
shape check
  ↓
Q(S)=0 validation
  ↓
δC resolver
  ↓
receipt written
  ↓
projection surfaces update
```

No downstream layer may claim acceptance before validation.

A carrier may recover a frame.  
A surface may display a frame.  
A resolver may compute a candidate.  
Only the validation/receipt boundary accepts.

---

# 6. Deterministic Law

The reduced instruction law is:

```text
δC(x) = rotl(x,1) XOR rotl(x,3) XOR rotr(x,2) XOR C
```

For the 16-bit frame:

```text
delta16(x,c)=rotl16(x,1)^rotl16(x,3)^rotr16(x,2)^c
```

Rules:

```text
rotation preserves bit width
XOR preserves reversibility
constant C breaks the zero fixed point
masking bounds state to the selected width
```

The protocol must not silently widen a deterministic width-sensitive rule. A period claim that depends on 16-bit width must remain fixed to 16-bit width.

Fault occurs when an invariant is broken, such as allowing the width to float.

---

# 7. Binary Quadratic Form

The current projection law uses the binary quadratic form:

```text
Q(x,y)=60x²+16xy+4y²
```

This is the distance and folding expression used in the local projection layer.

The terms are interpreted as:

```text
60x²  high-plane / cosmic orbit
16xy  bridge / interaction term
4y²   low-plane / atomic delta
```

The local fold is:

```text
local240 = Q(x,y) mod 240
```

The 5040 slot is:

```text
slot5040 = fano7 * 720 + role3 * 240 + local240
```

This links:

```text
5!  = 120
240 = 2 × 5!
6!  = 720
7!  = 5040
```

The 5040 ring is the finite cyclic inspection/receipt surface used by the current runtime.

---

# 8. Carrier Layer

A carrier transports a frame. It does not validate the frame.

The carrier invariant is:

```text
decode(encode(S)) = S
```

A carrier may provide:

```text
checksum
error correction
orientation recovery
color palette
metadata
redundancy
scan recovery
```

but none of these replace algebraic validation.

The locked carrier statement is:

```text
The carrier preserves S.
The validator proves S.
The resolver interprets LL, NN, MM.
The ring records the accepted row.
```

Or:

```text
transport the frame
prove the frame
resolve the row
record the receipt
```

## 8.1 JABCode Optical Carrier

JABCode is the optical record carrier for the current browser/page surface.

Current project status:

```text
OMI-JAB-RS Profile v0
debug matrix carrier only
not full BSI TR 03137 compliance yet
```

The debug carrier is deterministic and transports the 16-byte big-endian `uint16_t[8]` frame.

It must visibly remain labeled as debug/non-BSI until a real standards-compliant encoder/decoder replaces the debug implementation.

Acceptance still requires downstream validation.

---

# 9. DOM Binding

The minimal DOM binding is:

```text
id       = spatial binding anchor
data-omi = forward / CAR binding
data-imo = inverse / CDR binding
```

The DOM is not a database.

The DOM is not canon.

The DOM is a projection and inspection surface.

Elements may be used as pixels, nodes, ports, or receipts, but the browser surface does not become authority.

---

# 10. Browser Projection Stack

The browser is not an afterthought. It is a native projection and cascade resolver.

The current browser projection stack is:

```text
Receipt
  ↓
cosmology / sphere / orientation space
  ↓
gnomonic projection
  ↓
Smith coordinate
  ↓
mediant / determinant resolver
  ↓
mean ladder pressure
  ↓
Genaille quantizer
  ↓
CSS cascade
  ↓
DOM / SVG / WebGL paint
```

The browser performs what it already does well:

```text
selector matching
cascade
computed style
layout
paint
```

The protocol inserts deterministic receipt-derived coordinates ahead of selector matching.

The browser resolves the surface.  
It does not validate the receipt.

---

# 11. ReceiptSurfaceCascade

`ReceiptSurfaceCascade` projects receipts onto browser-visible surfaces.

Its job is to translate a receipt or receipt pair into:

```text
lane
band
mediant
determinant
mean pressure
CSS variables
root data attributes
Genaille SVG state
```

It exposes functions such as:

```text
receiptToCascadeSlot
cascadeSlotToCSSVars
applyCSSVars
applyCascadeSlotToRoot
renderGenailleSVG
ingestFromReceipt
receiptToRational
mediant
determinant
determinantClass
meanLadder
receiptPairToSurface
applySurfaceToRoot
```

A single receipt projects position.

A receipt pair projects motion or resolution.

```text
Receipt[n-1] + Receipt[n]
        ↓
positive rational slopes
        ↓
mediant coordinate
        ↓
determinant boundary witness
        ↓
HM ≤ GM ≤ AM ≤ QM pressure ladder
        ↓
lane / band
        ↓
CSS variables + data attributes
        ↓
native browser cascade
```

---

# 12. Mediant Resolver

A positive rational `a/b` is represented by the point:

```text
(b,a)
```

The slope from the origin is:

```text
a/b
```

For two receipt slopes:

```text
left  = a/b
right = c/d
```

the mediant is:

```text
(a+c)/(b+d)
```

Graphically:

```text
(b,a) + (d,c) = (b+d, a+c)
```

The mediant is the graphical receipt resolver.

It gives the next rational surface coordinate between two receipt directions.

---

# 13. Determinant Boundary Witness

The determinant is:

```text
det = b*c - a*d
```

Interpretation:

```text
det = 0
  equivalent slope / same surface direction

abs(det) = 1
  adjacent Stern-Brocot neighbor / tight boundary

abs(det) > 1
  wider unresolved gap / more mediants exist between them
```

The determinant is not acceptance. It is a boundary witness.

It tells the projection surface how tightly two receipt directions relate.

---

# 14. Mean Ladder Surface Pressure

The mean inequality chain is:

```text
HM ≤ GM ≤ AM ≤ QM
```

For two positive values `x,y`:

```text
HM = (2xy)/(x+y)
GM = sqrt(xy)
AM = (x+y)/2
QM = sqrt((x²+y²)/2)
```

The protocol uses this as an ordered surface-pressure ladder:

```text
HM = conservative / weakest stable agreement
GM = geometric coherence
AM = ordinary cascade center
QM = deviation-sensitive highlight pressure
```

The mean ladder does not validate.  
It orders projection intensity.

---

# 15. Genaille Surface

The Genaille surface is the discrete quantizer.

It answers:

```text
Which lane?
Which band?
Which cascade slot?
```

It does not accept receipts.

It makes the discrete lane/band state visible as a ruler-like SVG surface.

The stack is:

```text
Smith continuous coordinate
  ↓
Genaille discrete lane/band
  ↓
CSS selector state
```

The Genaille ruler is therefore not merely another visualization. It is the visible quantizer for the cascade coordinate.

---

# 16. CSS Cascade Surface

CSS is the declarative browser resolver.

The protocol writes root variables and root data attributes such as:

```text
--omi-lane
--omi-band
--omi-hue
--omi-angle
--omi-stop-metric
--omi-mediant-a
--omi-mediant-b
--omi-det
--omi-hm
--omi-gm
--omi-am
--omi-qm
```

and:

```text
data-omi-lane
data-omi-band
data-smith
data-mediant
data-mediant-gap
data-mean-band
```

Then CSS selectors resolve matched elements:

```text
:root[data-omi-lane="7"] ...
:root[data-omi-band="3"] ...
:root[data-mediant-gap="adjacent"] ...
:root[data-mean-band="qm"] ...
```

This preserves the boundary:

```text
JavaScript projects the receipt.
CSS resolves the cascade.
DOM displays the matched surface.
Validation remains upstream.
```

---

# 17. Gnomonic Surface

The gnomonic surface is the cosmology-to-plane flattening rule.

It projects a sphere from its center onto a tangent plane.

The crucial property is:

```text
great-circle route → straight line
```

For OMI:

```text
cosmological receipt route
  ↓
gnomonic projection
  ↓
straight inspectable browser line
```

The current browser module is:

```text
viewer/public/gnomonic-surface.js
```

with functions:

```text
normalize3
projectPoint
receiptToSpherePoint
receiptPairToGnomonicLine
projectPointToSVG
ingestReceiptPair
```

The projection uses:

```text
x' = SCALE * x / z
y' = SCALE * y / z
```

with the horizon guard:

```text
z <= 0 → invisible
```

This matters because the gnomonic projection goes to infinity at the projection horizon. The viewer must mark those states invisible rather than pretending the route remains local.

The gnomonic surface does not validate.  
It flattens cosmology into inspectable geometry.

---

# 18. Smith Surface

The Smith surface is treated as a continuous coordinate manifold, not merely as an RF engineering diagram.

It answers:

```text
Where is the receipt coordinate continuously?
```

The Smith chart is a continuous surface view.

It is not parent authority. It is one cyclicable projection surface among others.

Correct hierarchy:

```text
Receipt
  ├─ Smith view
  ├─ Genaille view
  └─ CSS view
```

The receipt is the parent. Smith is a surface.

---

# 19. Projection Surfaces

The current projection surface order is:

```text
Canon validates.
Cosmology curves.
Gnomonic flattens.
Smith measures.
Mediant resolves.
Determinant witnesses.
Mean ladder pressures.
Genaille quantizes.
CSS cascades.
DOM/SVG/WebGL paint.
Receipt accepts.
```

Each surface has a role:

```text
Gnomonic
  sphere-to-plane flattening

Smith
  continuous coordinate view

Mediant
  rational resolver between receipt slopes

Determinant
  boundary witness

Mean ladder
  ordered surface pressure

Genaille
  discrete quantizer

CSS
  declarative selector resolver

DOM/SVG/WebGL
  paint targets
```

None of these surfaces accept authority.

---

# 20. Ring and Recording

The 5040 ring records accepted or inspectable rows.

The SAB ring uses:

```text
5040 × 8 bytes
```

The ring is a receipt/inspection surface tied to the factorial/Fano layout:

```text
slot5040 = fano7 * 720 + role3 * 240 + local240
```

The ring does not make invalid data valid. It records rows according to the current rule path.

---

# 21. Distributed Protocol Context

The broader distributed production doctrine is:

```text
Gossip moves.
Reed-Solomon recovers.
Causality bounds.
OMI accepts.
Projection displays.
```

This can be summarized as:

```text
MCRSGSP
```

The important separation is:

```text
transport does not validate
recovery does not accept
projection does not prove
receipt records acceptance
```

---

# 22. Interface Rule

The interface should be an object inbox, not a fantasy dashboard.

A useful OMI interface should let a person:

```text
import a citation
inspect its frame
see carrier recovery status
see validation state
see receipt history
project accepted surfaces
export receipt/citation material
```

Developer and hardware tools may exist, but they must remain tools.

The front door is the inbox/receipt workspace.

---

# 23. Current Implementation Checkpoint

The current checkpoint includes:

```text
JABCode carrier tests
ReceiptSurfaceCascade tests
GnomonicSurface tests
```

The current JavaScript test count is:

```text
17 JABCode tests
14 ReceiptSurfaceCascade tests
11 GnomonicSurface tests
= 42 JS tests
```

The test command is:

```text
make test-js
```

and the smoke path includes these tests.

This is the current projection checkpoint:

```text
Projection Surfaces Installed
```

Meaning:

```text
ReceiptSurfaceCascade handles receipt → mediant / mean ladder / cascade.
GnomonicSurface handles receipt pair → sphere → tangent-plane line.
CSS/DOM/SVG/WebGL remain projection surfaces.
Validation remains upstream.
```

---

# 24. Non-Goals

The protocol does not claim:

```text
rendered geometry proves truth
CSS selector match proves acceptance
JABCode debug matrix is full BSI JABCode
file existence implies receipt
code execution implies canon
notation implies validation
browser display implies authority
```

The protocol also does not require nondeterministic runtime behavior.

No randomness, timestamps, UUIDs, or side effects may be allowed into deterministic computation paths.

I/O may exist as carrier behavior, but it must not decide the deterministic output.

---

# 25. Minimal Compliance Rules

A minimal compliant implementation must preserve:

```text
1. Address frame structure.
2. Carrier decode/encode identity.
3. Validation before acceptance.
4. Receipt as shared invariant.
5. Projection as non-authoritative surface.
6. Deterministic computation.
7. Explicit file/port and code/data separation.
8. Notation/citation distinction.
```

A minimal browser implementation should preserve:

```text
1. Receipts as source.
2. CSS variables/data attributes as projection.
3. CSS selectors as browser-native resolver.
4. DOM/SVG/WebGL as paint targets.
5. No surface-level acceptance.
```

A minimal optical carrier implementation should preserve:

```text
1. uint16_t[8] frame recovery.
2. deterministic encode/decode.
3. debug/non-compliant labels where applicable.
4. validation after decode.
```

---

# 26. Canon Summary

The OMI protocol is a deterministic notation-citation protocol.

It separates canon from cosmology, citation from notation, file from port, and code from data.

It carries frames through carriers, validates them through Tetragrammatron, records them as receipts, and projects them through browser surfaces such as gnomonic, Smith, Genaille, CSS, DOM, SVG, and WebGL.

Its central rule is:

```text
The reader may recognize.
The resolver may promote.
Only validation and receipt may accept.
```

Its final covariant rule is:

```text
Code is data.
File is port.
Notation is citation.
Canon validates.
Cosmology projects.
Receipt accepts.
```

Its current projection checkpoint is:

```text
Canon validates.
Cosmology curves.
Gnomonic flattens.
Smith measures.
Mediant resolves.
Determinant witnesses.
Mean ladder pressures.
Genaille quantizes.
CSS cascades.
DOM/SVG/WebGL paint.
Receipt accepts.
```
