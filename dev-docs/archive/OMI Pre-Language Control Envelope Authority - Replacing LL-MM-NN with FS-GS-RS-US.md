# OMI Pre-Language Control Envelope Authority

## Replacing LL/MM/NN with FS/GS/RS/US

Version: `0.1.0`  
Status: Canonical Draft  
Authority Layer: Pre-Language Memory Topology  
Applies to: OMI-Lisp, `.imo` carriers, document-embedded declarations, LLM resolver memory, repository receipts

---

## 0. Purpose

This document defines the canonical replacement for the older `LL/MM/NN` argument model.

The older model treated OMI declarations as if they were primarily argument slots.

The corrected model treats OMI declarations as **pre-language memory topology**.

The replacement is:

```text
LL/MM/NN  →  FS/GS/RS/US
```

This is not merely a rename.

It is a change of authority:

```text
old: arguments first
new: scope topology first
```

The new rule is:

```text
OMI does not begin as a function call.
OMI begins as a scoped memory envelope.
```

---

## 1. Canonical Control Alphabet

The minimal OMI pre-language control alphabet is:

```text
0x00  NUL  origin / null / zero-point
0x1C  FS   file scope
0x1D  GS   group scope
0x1E  RS   record scope / closure witness
0x1F  US   unit scope
0x20  SP   readable boundary / visible separator
```

ASCII names:

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
SP separates readable surface text.
```

`0x1E` remains closure witness.

`0x1E` is not acceptance.

```text
0x1E closes or witnesses a record.
Validation and receipt accept.
```

---

## 2. Replacement Rule

The older symbols:

```text
LL
MM
NN
```

MUST be treated as deprecated aliases for an incomplete argument-frame model.

They SHOULD NOT be used as canonical OMI authority.

If old documents contain `LL/MM/NN`, they SHOULD be migrated into FS/GS/RS/US scope.

Canonical migration:

```text
LL → lexical/local carrier context
MM → middle/module/group context
NN → next/nested/record context
```

becomes:

```text
FS → file scope
GS → group scope
RS → record scope
US → unit scope
```

The new authority statement is:

```text
FS/GS/RS/US replaces LL/MM/NN as the canonical OMI pre-language memory pointer cascade.
```

---

## 3. OMI-IMO Frame Authority

The canonical frame is:

```text
omi---imo
```

or, in expanded relation form:

```text
o---o/---/?---?@---@
```

The compact machine-readable ruler form is:

```text
o-S0-S1-S2-S3/S4/S5/S6/S7?REGISTER?STACK@CAR@CDR
```

The semantic expansion is:

```text
o-[FS]-[GS]-[RS]-[US]/[FS]/[GS]/[RS]/[US]?REGISTER?STACK@CAR@CDR
```

The frame is a 256-bit OMI-IMO memory relation.

It is divided into two 128-bit halves.

---

## 4. The 256-Bit Frame

### 4.1 Control/Ruler Half

The first half is the 128-bit ruler/control cascade:

```text
S0 S1 S2 S3 S4 S5 S6 S7
```

Each `S` field is 16 bits.

```text
8 × 16 bits = 128 bits
```

Each 16-bit field is:

```text
2 bytes = 4 nibbles
```

Therefore:

```text
8 fields × 4 nibbles = 32 nibbles
```

This is the canonical “32” of the ruler half.

### 4.2 Payload/Pair Half

The second half is the 128-bit payload/pair cascade:

```text
REGISTER STACK CAR CDR
```

Each field is 32 bits.

```text
4 × 32 bits = 128 bits
```

Each 32-bit field is:

```text
4 bytes = 8 nibbles
```

Therefore:

```text
4 fields × 8 nibbles = 32 nibbles
```

### 4.3 Full Frame

Together:

```text
ruler half:       128 bits = 16 bytes = 32 nibbles
payload/pair half:128 bits = 16 bytes = 32 nibbles
---------------------------------------------------
full frame:       256 bits = 32 bytes = 64 nibbles
```

Canonical statement:

```text
The OMI-IMO frame is 256 bits.
The first 128 bits are the FS/GS/RS/US ruler cascade.
The second 128 bits are the REGISTER/STACK/CAR/CDR payload-pair cascade.
```

---

## 5. FS/GS/RS/US Encapsulation

The OMI forward side is:

```text
S0-S1-S2-S3
```

Canonical meaning:

```text
S0 = FS
S1 = GS
S2 = RS
S3 = US
```

The IMO inverse/carrier side is:

```text
S4/S5/S6/S7
```

Canonical meaning:

```text
S4 = FS
S5 = GS
S6 = RS
S7 = US
```

Therefore:

```text
OMI side = FS-GS-RS-US
IMO side = FS/GS/RS/US
```

The symbolic form:

```text
|OMI---IMO>
```

MAY be read as:

```text
|FS>-|GS>-|RS>-|US>
```

or more fully:

```text
|OMI---IMO> = |FS>-|GS>-|RS>-|US> encapsulation
```

This is not a claim about quantum mechanics.

It is a compact notation for scoped containment:

```text
file contains group
group contains record
record contains unit
unit carries relation
```

---

## 6. Meaning of the Separators

The visible notation:

```text
o---o/---/?---?@---@
```

uses separator classes to distinguish frame regions.

Canonical interpretation:

```text
-  OMI forward scope / citation route
/  IMO inverse scope / carrier route
?  payload, register, stack, mask, or query field
@  CAR/CDR relation pointer
```

Expanded:

```text
o-FS-GS-RS-US/FS/GS/RS/US?REGISTER?STACK@CAR@CDR
```

This gives four levels of address on the OMI side, four levels of address on the IMO side, and four 32-bit relation fields.

The frame is not just text.

The frame is a memory pointer surface.

---

## 7. The 32 Control Places and the 32-Nibble Ruler

There are two related uses of “32” in OMI.

### 7.1 The 32 Pre-Language Control Places

The ASCII control range:

```text
0x00..0x1F
```

contains 32 non-printing control codes.

These are the pre-language place controllers.

They stage the memory surface before readable language.

### 7.2 The 32-Nibble Ruler Half

The OMI ruler half contains:

```text
8 × 16-bit fields = 128 bits = 32 nibbles
```

This is the encoded ruler cascade.

Canonical relation:

```text
0x00..0x1F stages the 32 control places.
S0..S7 encodes the 32-nibble ruler cascade.
```

The control range is the pre-language staging surface.

The ruler half is the encoded memory frame.

---

## 8. Document Embedding Forms

OMI documents MAY carry the pre-language control envelope in raw or escaped form.

### 8.1 Raw Carrier Form

For binary-safe `.imo` files:

```text
<NUL><FS>README.md<GS>adapters<RS>policy<US>browser-serial
```

Actual byte pattern:

```text
00 1C ... 1D ... 1E ... 1F ...
```

### 8.2 Escaped Documentation Form

For Markdown, README files, HTML, prompts, and LLM-visible documents:

```text
\x00\x1C README.md \x1D adapters \x1E policy \x1F browser-serial
```

or:

```text
\NUL \FS README.md \GS adapters \RS policy \US browser-serial
```

Both forms resolve to the same OMI scope.

Canonical rule:

```text
Raw control bytes are the carrier form.
Escaped control names are the documentation form.
Both resolve to the same FS/GS/RS/US topology.
```

---

## 9. Document as Memory Surface

A normal document is a sequence of visible words.

An OMI document is a scoped memory surface.

Example:

```text
\x00
\x1C README.md
\x1D adapters
\x1E browser-policy
\x1F webserial
WebSerial is an optional adapter. It may carry serial data, but it may not accept repository state.
```

The visible prose is interpreted under this scope:

```text
fs.o/README.md/gs.o/adapters/rs.o/browser-policy/us.o/webserial
```

The LLM or parser does not guess the memory location.

The control envelope gives the location first.

Then the words are interpreted.

Canonical order:

```text
place first
relation second
interpretation third
validation fourth
receipt last
```

---

## 10. LLM Reference Model

Large language models do not need to share the same neural weights to share the same OMI reference model.

They only need to resolve the same external control envelope.

Canonical statement:

```text
LLMs do not need to share the same model.
They can share the same reference memory protocol.
```

The shared layer is:

```text
document carrier
+ FS/GS/RS/US control topology
+ OMI-Lisp relation
+ validation rule
+ receipt
```

An LLM may differ in wording.

It MUST NOT differ in authority boundary.

An OMI-compatible LLM resolver MUST:

```text
read the control envelope
resolve FS/GS/RS/US scope
treat declarations as candidates
validate before acceptance
require receipts for accepted state
```

---

## 11. Topological Memory Pointer Model

The FS/GS/RS/US cascade forms a topological memory pointer.

Interpretation:

```text
FS = connected document component
GS = group or subcomplex
RS = record boundary / relation cell
US = unit / addressable simplex
```

Topology inspection MAY use Betti numbers:

```text
b0 = connected components
b1 = loops / cycles
b2 = enclosed unresolved voids
```

This allows resolvers to inspect memory shape:

```text
one document component
multiple disconnected scopes
looping dependency
missing unit
unclosed record
unresolved semantic void
```

The topology layer is not decorative.

It is how document memory becomes inspectable.

---

## 12. OMI-Lisp Relation Rule

After the pre-language envelope resolves scope, dot notation declares relation.

Canonical dot form:

```omi-lisp
(car . cdr)
```

Example:

```omi-lisp
(effect . hardware)
(adapter . webserial)
(receipt.result . accepted)
```

The dot relation is scoped by the active FS/GS/RS/US envelope.

Therefore:

```text
same dot relation under different scope is not the same memory relation
```

Example:

```text
fs.o/README.md/gs.o/adapters/rs.o/webserial/us.o/effect
```

is different from:

```text
fs.o/SKILLS.md/gs.o/adapters/rs.o/webserial/us.o/effect
```

Scope is part of identity.

---

## 13. Authority Rules

The pre-language control envelope has authority to stage memory topology.

It does not have authority to accept truth.

Rules:

```text
NUL may open origin.
FS may select file scope.
GS may select group scope.
RS may select record scope and closure.
US may select unit scope.
SP may separate readable text.

None of these accept state.
```

Canonical invariant:

```text
Control is not acceptance.
Scope is not acceptance.
Relation is not acceptance.
Closure is not acceptance.
Projection is not acceptance.
Validation and receipt accept.
```

---

## 14. Deprecation of LL/MM/NN

`LL/MM/NN` MAY remain as historical notation in old documents.

New documents SHOULD NOT introduce `LL/MM/NN` as authority.

If encountered, a resolver SHOULD map it to candidate scope only.

Suggested migration:

```text
LL/MM/NN block
  ↓
candidate FS/GS/RS/US envelope
  ↓
validation
  ↓
receipt
```

Example migration:

```text
old:
  (omi- oo (ll . oO (y . OO (z . Oo -omi))))

new:
  \NUL \FS omi \GS expression \RS declaration \US relation
  (omi.scope . fs.o/omi/gs.o/expression/rs.o/declaration/us.o/relation)
```

The old expression may be preserved as historical carrier text.

The new authority is the FS/GS/RS/US envelope.

---

## 15. 128-Character Display Rule

A repository MAY define a fixed-width 128-character display form for a 256-bit OMI-IMO frame.

This is a presentation rule, not the canonical binary size.

Canonical binary size:

```text
256 bits = 32 bytes
```

Canonical nibble size:

```text
64 nibbles
```

A 128-character display MAY include separators, labels, escapes, padding, checksum characters, or visual ruler marks.

Such a display MUST round-trip to the canonical 256-bit frame.

Rule:

```text
Display width is not authority.
Round-trip binary frame is authority.
```

---

## 16. Complete Frame Summary

```text
OMI-IMO frame:
  256 bits
  32 bytes
  64 nibbles

Ruler half:
  128 bits
  16 bytes
  32 nibbles
  S0-S7
  FS-GS-RS-US / FS-GS-RS-US

Payload/pair half:
  128 bits
  16 bytes
  32 nibbles
  REGISTER
  STACK
  CAR
  CDR

Pre-language staging:
  0x00..0x1F = 32 control places
  0x00 = NUL
  0x1C = FS
  0x1D = GS
  0x1E = RS
  0x1F = US
  0x20 = SP
```

---

## 17. Canonical Formula

```text
|OMI---IMO> =
  |FS>-|GS>-|RS>-|US>
  over
  REGISTER / STACK / CAR / CDR
```

Expanded machine form:

```text
o-FS-GS-RS-US/FS/GS/RS/US?REGISTER?STACK@CAR@CDR
```

Expanded binary form:

```text
S0=FS
S1=GS
S2=RS
S3=US
S4=FS'
S5=GS'
S6=RS'
S7=US'
REGISTER
STACK
CAR
CDR
```

The primed FS/GS/RS/US fields are the inverse/carrier-side IMO scope.

---

## 18. One-Sentence Summary

The OMI Pre-Language Control Envelope replaces the older LL/MM/NN argument model with a deterministic FS/GS/RS/US topological memory pointer model, where the first 128 bits of the 256-bit OMI-IMO frame encode the scoped ruler cascade and the second 128 bits encode the REGISTER/STACK/CAR/CDR relation surface, allowing documents, parsers, peers, and LLMs to share the same external reference memory before interpretation, validation, and receipt.