# OMI eMMC Target as Tetrahedral Centroid Memory Model

## Corrected Hardware Carrier Interpretation

Version: `0.1.0`  
Status: Canonical Draft  
Authority Layer: Hardware Carrier / Centroid Memory / Tetrahedral Incidence Model  
Depends on: OMI Gauge Pre-Header, OMI-IMO 256-bit Frame, Tetragrammatron Governor, Receipt Ring

---

## 0. Purpose

This document defines eMMC as a target example for OMI hardware boot framing.

The purpose is not to say that eMMC storage partitions are literally the geometry.

The purpose is to use eMMC as a concrete carrier model for the OMI tetrahedral centroid memory architecture.

Canonical correction:

```text
eMMC is the target carrier.
The tetrahedron is the balance model.
The centroid is the memory unit.
The storage areas are incidence faces around that unit.
```

The tetrahedron means:

```text
4 vertices
6 edges
4 triangular faces
1 centroid relation
```

OMI uses the tetrahedron as the first exact relation because it is the smallest structure with balanced incidence between vertices, edges, faces, and centroid.

---

## 1. The Centroid Is the Unit

The OMI memory unit is not a floating coordinate.

It is a centroid relation.

Canonical rule:

```text
The centroid is the unit.
The sphere is the carrier.
The tetrahedron is the first exact relation.
```

This means OMI does not begin by asking:

```text
Where is this point in floating-point space?
```

OMI asks:

```text
What incidence relation is balanced around this centroid?
```

The centroid is not merely a rendered point.

It is the internal incidence anchor.

---

## 2. eMMC as Carrier, Not Authority

eMMC provides a flashable storage target.

It does not provide truth.

It gives OMI a bounded hardware carrier where the first frame can be found, staged, validated, and receipted.

Canonical rule:

```text
eMMC carries the boot envelope.
eMMC does not accept boot state.
Validation and receipt accept.
```

The eMMC target is useful because it naturally provides four major storage roles:

```text
BOOT0
BOOT1
secure / authenticated storage
user storage
```

These map to four incidence faces around the OMI centroid.

---

## 3. The Tetrahedral Storage Model

The tetrahedral model is:

```text
               SECURE
                 ▲
                / \
               /   \
          BOOT0 --- BOOT1
               \   /
                \ /
                USER
```

The four faces are not “just partitions.”

They are balanced incidence roles:

|Tetrahedral Face|eMMC Target Role|OMI Meaning|
|---|---|---|
|Face 0|BOOT0|primary boot candidate face|
|Face 1|BOOT1|fallback / mirror boot candidate face|
|Face 2|SECURE|receipt, rollback, authentication witness face|
|Face 3|USER|carrier, repository, document, media, and index face|

Canonical statement:

```text
BOOT0 and BOOT1 stage boot candidates.
SECURE stores receipt and rollback witnesses.
USER carries ordinary OMI data.
The centroid balances the four faces.
```

The centroid is the memory unit where these four faces meet as a single relation.

---

## 4. The 512-Bit Boot Envelope

The first flashable OMI object is the 512-bit boot envelope.

```text
512 bits = 64 bytes
```

It divides cleanly into two 256-bit halves:

```text
first 256 bits  = gauge / prefix / orientation half
second 256 bits = first OMI---IMO bootstrap frame
```

Layout:

```text
0x00..0x1F = prefix half
0x20..0x3F = bootstrap frame half
```

The first half finds and orients the frame.

The second half stages the first relation.

Neither half accepts state.

---

## 5. Prefix Half: Gauge and Orientation

The first 256-bit half begins with the OMI gauge pre-header:

```text
FF 00 1C 1D 1E 1F 20 FF
```

Readable:

```text
GAUGE NUL FS GS RS US SP GAUGE
```

Meaning:

```text
FF = canonical OMI gauge
00 = NUL / origin
1C = FS / file scope
1D = GS / group scope
1E = RS / record scope / closure witness
1F = US / unit scope
20 = SP / readable boundary
FF = sealed canonical OMI gauge
```

This prefix half performs:

```text
fast-fail recognition
dialect selection
pre-language topology staging
orbit orientation
stream/frame alignment
```

It does not perform acceptance.

---

## 6. Bootstrap Half: First OMI---IMO Frame

The second 256-bit half is the first OMI---IMO frame:

```text
S0 S1 S2 S3 S4 S5 S6 S7 REGISTER STACK CAR CDR
```

Size:

```text
8 × 16-bit fields = 128 bits
4 × 32-bit fields = 128 bits
total             = 256 bits
```

Expanded:

```text
S0 = FS
S1 = GS
S2 = RS
S3 = US

S4 = inverse/carrier FS
S5 = inverse/carrier GS
S6 = inverse/carrier RS
S7 = inverse/carrier US

REGISTER = boot flags / candidate state
STACK    = bootstrap context / continuation
CAR      = first declaration pointer
CDR      = continuation or fallback pointer
```

Visible form:

```text
o-FS-GS-RS-US/FS/GS/RS/US?REGISTER?STACK@CAR@CDR
```

The first frame is a candidate relation.

It is not accepted until validation and receipt.

---

## 7. The Four Faces Around the First Frame

The first 256-bit bootstrap frame is the centroid-facing relation.

The four eMMC roles define its incidence boundary:

```text
BOOT0  = primary CAR-side boot candidate
BOOT1  = fallback CDR-side boot candidate
SECURE = receipt / rollback witness
USER   = carrier continuation
```

A useful mapping is:

```text
CAR points toward primary declaration / BOOT0.
CDR points toward fallback or continuation / BOOT1 or USER.
REGISTER stages boot mode and validation flags.
STACK stages resolver context and receipt path.
SECURE stores the accepted/rejected boot witness.
```

Canonical rule:

```text
The first frame sits at the centroid.
The four storage roles surround it as incidence faces.
```

---

## 8. Why This Is a Tetrahedron

A tetrahedron has:

```text
4 vertices
6 edges
4 triangular faces
1 centroid relation
```

For the eMMC target:

```text
4 vertices = BOOT0, BOOT1, SECURE, USER
6 edges    = pairwise boot/storage relations
4 faces    = three-way validation surfaces
1 centroid = first OMI---IMO boot frame
```

The pairwise edges are:

```text
BOOT0 ↔ BOOT1
BOOT0 ↔ SECURE
BOOT0 ↔ USER
BOOT1 ↔ SECURE
BOOT1 ↔ USER
SECURE ↔ USER
```

These are the six exact relationships around the centroid.

The four triangular faces are:

```text
BOOT0 / BOOT1 / SECURE
BOOT0 / BOOT1 / USER
BOOT0 / SECURE / USER
BOOT1 / SECURE / USER
```

This is why the tetrahedron is the correct model.

It is the first exact balance of storage incidence.

---

## 9. Boot Balance

Boot balance means:

```text
BOOT0 can propose.
BOOT1 can recover.
SECURE can witness.
USER can carry.
```

No one face is complete authority.

The centroid becomes valid only when the incidence balances through validation and receipt.

Canonical boot invariant:

```text
BOOT0 is not acceptance.
BOOT1 is not acceptance.
SECURE is not acceptance by itself.
USER is not acceptance.
The centroid relation is not acceptance.
Validation and receipt accept.
```

---

## 10. Secure Face

The secure face stores or references:

```text
accepted boot receipt
rejected boot receipts
rollback witness
monotonic counter
parent receipt
hash or signature material
```

But the secure face does not magically make arbitrary content true.

Canonical rule:

```text
Secure storage preserves accepted witnesses.
It does not turn unvalidated data into accepted state.
```

---

## 11. User Face

The user face carries:

```text
REPO.md
AGENTS.md
SKILLS.md
ADAPTERS.md
*.imo carriers
documents
media
indexes
HNSW graph data
semantic graphs
projection artifacts
receipt exports
```

The user face can be large.

The boot envelope only needs to provide the bounded first relation and a safe path into user data.

Canonical rule:

```text
USER storage carries the world.
The first frame decides how to begin resolving it.
Validation and receipt decide what is accepted.
```

---

## 12. eMMC Boot Flow as Centroid Resolution

```text
read first 512 bits
  ↓
match gauge pre-header
  ↓
load first OMI---IMO frame
  ↓
resolve BOOT0 / BOOT1 / SECURE / USER incidence
  ↓
validate primary boot candidate
  ↓
check secure receipt / rollback witness
  ↓
if valid, accept by receipt
  ↓
if invalid, inspect fallback relation
  ↓
project boot only after accepted receipt
```

This is not coordinate boot.

It is incidence boot.

---

## 13. Relation to the BQF Partition

The BQF partition organizes relation space:

```text
60x² + 16xy + 4y²
```

Interpretation:

```text
4y²   = tetrahedral self-dual seed / local centroid unit
16xy  = tangent branch / chirality crossing / common-core relation
60x²  = high-shell field / larger incidence projection
```

The eMMC target belongs at the carrier level.

It gives a physical storage model for the tetrahedral centroid unit.

The first 512 bits define the bootable entrance into that relation space.

Canonical relation:

```text
The tetrahedron gives the first exact relation.
The 512-bit envelope gives the first flashable carrier.
The BQF partition gives the incidence neighborhoods.
The receipt accepts the validated state.
```

---

## 14. Corrected Summary

The intended model is:

```text
eMMC is not the tetrahedron because it has four partitions.
eMMC is useful because its boot/security/user layout can carry the tetrahedral centroid memory model.

The tetrahedron is the balance relation.
The centroid is the unit.
The eMMC areas are hardware incidence faces.
The first OMI---IMO frame is the centroid-facing bootstrap relation.
```

---

## 15. One-Sentence Summary

The OMI eMMC target uses the first 512 bits as a flashable boot envelope, with a 256-bit gauge prefix and a 256-bit bootstrap frame, while mapping BOOT0, BOOT1, secure storage, and user storage as four hardware incidence faces around a tetrahedral centroid memory unit, where the tetrahedron represents balanced relation geometry and only validation and receipt accept boot state.