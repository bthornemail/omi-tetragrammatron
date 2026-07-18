# OMI Addressing

Status: active V0 specification scaffold

Source material:

```text
dev-docs/archive/omi user-space cons of cidr addressing /
dev-docs/archive/omi user-space cons of cidr addressing /Chat Conversation Of Bulid.md
dev-docs/archive/omi user-space cons of cidr addressing /OMICRON_OMI-LOG_CIDR_DIALECT_SPEC.md
dev-docs/archive/Omi-Ring as Addressed Notation Witness.md
dev-docs/archive/OMI Version Control from Place-Value Notation, Betti-Schlafli Interpretation, and Seed Objects.md
dev-docs/archive/OMI Citation Notation and Gauge Pre-Header.md
dev-docs/archive/OMI Pre-Language Control Envelope Authority - Replacing LL-MM-NN with FS-GS-RS-US.md
dev-docs/archive/OMI Gauge Pre-Header Authority.md
dev-docs/archive/OMI Gauge Header and Orbit Authority.md
dev-docs/archive/OMI eMMC Target as Tetrahedral Centroid Memory Model.md
dev-docs/archive/OMI eMMC Boot Target Example.md
dev-docs/archive/OMI Doctrine -- Notation as Cipher, Cipher as Notation.md
dev-docs/archive/Derived Non-Printing OMI-Lisp.md
```

This file states the current addressing boundary for implementation. It does not replace the archive documents; it extracts the stable V0 contract.

## Canonical Frame

OMI addressing is the place-value identity surface for the `omi---imo` relation.

It does not identify citations by hashes, digests, fingerprints, encrypted labels, or signatures. The addressed relation itself is the identity.

Root mnemonic:

```text
omi---imo
```

Receipt-ruler mnemonic:

```text
o---o/---/?---?@---@
```

Expanded machine form:

```text
o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

Reading:

```text
o---o   object/object relation
/---/   scoped derivation or return surface
?---?   unresolved witness/query boundary
@---@   CAR/CDR closure
```

The receipt is not stored beside the address as a separate truth token. After validation, the address-ruler itself is the carry-forward receipt surface.

## Null Ring And Full Notation

The capstone seed is:

```lisp
(NULL . NULL)
```

This is not shallow emptiness. It is the folded form of the whole relation before interpretation.

The unfolded byte-ring is:

```lisp
((0x00 . 0x20)
 (0x20 . 0x7F)
 (0x7F . 0xFF)
 (0xFF . 0x00))
```

The ring reads:

```text
0x00 -> 0x20 -> 0x7F -> 0xFF -> 0x00
```

Those are four forms of absence:

```text
0x00  NUL origin / absence before inscription
0x20  SPACE / absence inside readable text
0x7F  DEL / absence after removal / seal threshold
0xFF  saturated absence / full carrier horizon
```

The OMI dot may be read as the witnessed difference between CAR and CDR:

```text
0x00 XOR 0x20 = 0x20
0x20 XOR 0x7F = 0x5F
0x7F XOR 0xFF = 0x80
0xFF XOR 0x00 = 0xFF
```

The cycle closes:

```text
0x20 XOR 0x5F XOR 0x80 XOR 0xFF = 0x00
```

Therefore:

```text
(NULL . NULL)
  folded null

byte-ring
  unfolded null

full XOR cycle
  resolved null
```

The ring is not earned after NULL. The ring is already implicit in NULL. The byte-ring is the full notation of NULL unfolded.

The equivalent OMI expression is:

```lisp
(omi---imo . '(o---o /---/ ?---? @---@))
```

Reading:

```text
omi---imo
  folded root relation

o---o /---/ ?---? @---@
  unfolded full notation surface
```

This is a full-notation NULL value. It is null because the whole structure closes back to the root, not because it lacks structure.

## Core Rule

```text
Frame identifies.
Slash derives.
Path selects.
Clauses constrain.
Tetragrammatron validates.
Validation carries forward.
```

CIDR-style notation is a readable CONS surface:

```text
address/prefix = (address . prefix)
```

OMI reading:

```text
CAR = address payload / identity frame
CDR = factoring depth / continuation route
/   = Omicron derivation boundary
```

The slash is not acceptance. It is the visible boundary between a frame and the route by which the frame is factored or derived.

## One-Long-Word Address

The OMI address is a one-long-word instruction substrate:

```text
S0-S1-S2-S3/S4/S5/S6/S7
```

Each segment is sixteen bits:

```text
8 x 16 bits = 128 bits
```

The deeper form is thirty-two place-value nibbles:

```text
n00 n01 n02 n03
n04 n05 n06 n07
n08 n09 n10 n11
n12 n13 n14 n15
n16 n17 n18 n19
n20 n21 n22 n23
n24 n25 n26 n27
n28 n29 n30 n31
```

Grouped into ruler segments:

```text
S0 = n00 n01 n02 n03
S1 = n04 n05 n06 n07
S2 = n08 n09 n10 n11
S3 = n12 n13 n14 n15
S4 = n16 n17 n18 n19
S5 = n20 n21 n22 n23
S6 = n24 n25 n26 n27
S7 = n28 n29 n30 n31
```

The same nibble in two different places does not make the same contribution. `0xA` at `n02` is not the same addressed relation as `0xA` at `n27`.

Canonical rule:

```text
address = 32 place-value nibbles grouped into eight 16-bit ruler segments
```

## Polybius Gauge

The local hexadecimal gauge is:

```text
0 1 2 3
4 5 6 7
8 9 A B
C D E F
```

The tetrahedral diagonals are:

```text
D+ = {0,5,A,F}
D- = {3,6,9,C}
```

These diagonals are control classes. They let each nibble contribute to chirality, posting or pulling, CAR/CDR bias, component lane, gauge pressure, receipt boundary, and projection behavior.

The diagonal sums close:

```text
0 + 5 + A + F = 0x1E
3 + 6 + 9 + C = 0x1E
```

So:

```text
0x1E = diagonal closure witness
0x1E = Record Separator
```

The full nibble field also closes:

```text
0 + 1 + 2 + ... + F = 0x78
```

So:

```text
0x78 = full hexadecimal system witness
```

These values are addressing witnesses. They are not acceptance.

## Pattern Register

OMI reads these forms by fitted structure, not by treating hexadecimal
magnitudes as self-explanatory names.

```text
arrangement  shows which positions are high or low
complement   shows an inverse reading of the same bounded field
containment  distinguishes center from boundary
ordering     distinguishes fold-then-carry from carry-then-fold
place       determines what a repeated value contributes in context
```

The compact register is:

```text
0x0000 / 0xFFFF
  null/full horizon
  Omicron centroid viewpoint / Omnicron resolver

0x0FF0 / 0xF00F
  centered-full / boundary-full encapsulation
  Omicron centroid and boundary references

0xAA55 / 0x55AA
  balanced crossed alternation and its inverse
  Metatron observational witness

0x3C78 / 0x783C
  fold/carry order and its reversal
  Tetragrammatron operation witness

FF 00 1C 1D 1E 1F 20 FF
  repeated horizon enclosing the ordered scope spine
  Omnicron resolver envelope
```

These are role witnesses, not independent authorities:

```text
Omicron contains the viewpoint.
Tetragrammatron governs lawful transition.
Metatron makes the crossing observable.
Omi-Ring witnesses the addressed relation.
Receipt accepts replay-stable state.
```

The same hexadecimal form may serve a different carrier role at a different
place. Place and declared context determine contribution. A familiar numeric
value does not carry one universal authority into every surface.

## Authority Boundary

OMI addressing owns:

- bounded notation witnesses
- 128-bit frame identity
- 256-bit OMI---IMO relation frame shape
- slash-delimited derivation paths
- prefix depth as resolution horizon
- readable CONS/CAR/CDR addressing forms
- Omi-Ring formation as addressed notation witness

OMI addressing does not own:

- validation
- acceptance
- receipt storage
- projection meaning
- carrier I/O
- browser, hardware, file, or network side effects

Canonical invariant:

```text
Recognition is not validation.
Citation is not validation.
Omi-Ring formation is not validation.
Slash derivation is not validation.
Projection is not validation.
Validation is the protocol boundary.
```

## Place-Value Identity

OMI notation citations are not identified by hashes.

The addressed place-value relation is the identity. Hashing, checksums,
encryption, signatures, and digest labels are outside the deterministic
identity model of the protocol.

External systems may hash or encrypt an OMI notation citation for private
transport, secrecy, indexing, or compatibility. That transformed value is not
an OMI identity, not an Omi-Ring witness, not validation input, and not
validated state. It is an external carrier artifact used at the operator's own
risk.

Cascade:

```text
omi---imo  names the root relation
o---o      compiles the relation into object form
/---/      derives scoped routes from that object relation
?---?      witnesses or queries the routed relation
@---@      closes the relation as CAR/CDR continuation
```

Place-value rule:

```text
@---@ resolves through ?---?
?---? resolves through /---/
/---/ resolves through o---o
o---o resolves through omi---imo
```

Wider forms do not replace the root. Wider forms must remain resolvable back to
the root addressed relation.

## Omi-Ring Vocabulary

Use `Omi-Ring` for the bounded addressed notation witness.

Use `receipt` only as a compatibility name for a stored validated state of that witness.

Use `receipt ring` only as a compatibility name for storage and replay of validated Omi-Ring states.

Correct stack:

```text
omi---imo
  -> Omi-Ring addressed notation witness
  -> candidate citation
  -> validation
  -> validated carry-forward state
  -> validated state ring storage
  -> scribing / projection / carrier output
```

This avoids treating receipt as the primitive object. The primitive protocol object is the addressed relation witness.

## Surface Forms

V0 recognizes these alias-conformant surface forms:

```text
omi---imo
omi-<frame>
imo-<frame>
omi-<frame>-imo
omi-<frame>/<path...>-imo
```

Meaning:

```text
omi---imo
  empty bounded relation surface

omi-<frame>
  open frame identity

imo-<frame>
  mirror, close, target, or resolve frame identity

omi-<frame>-imo
  bounded object surface

omi-<frame>/<path...>-imo
  derived object surface
```

The preferred full derivation form is:

```text
omi-<frame>/<depth>/<domain>/<selector>/<overlay>-imo
```

Where:

```text
frame    = 128-bit OMI identity frame
depth    = CIDR depth, derivation depth, or memory horizon
domain   = protocol domain or interpretation space
selector = route target, role, mnemonic, interface, synset, or clause
overlay  = projection, modality, color, f-expression, receipt frame, or secondary frame
```

## Frame And Path Separation

The frame is identity.

The path is interpretation.

The path may derive many surfaces from the same identity without changing that identity:

```text
omi-<frame>/128-imo
omi-<frame>/hardware/i2c-imo
omi-<frame>/semantic/subject/predicate/object-imo
omi-<frame>/60x2/16xy/4y2/fexpr:<base64url>-imo
```

Short lock:

```text
Frame is memory.
Path is derivation.
Receipt is accepted traversal state.
```

## Prefix Resolution

Prefix depth is the resolution horizon.

The resolver chooses the most specific lawful prefix that applies.

Useful V0 `ip6-notation` depths:

```text
/7    private fabric base
/32   boot or route entry
/48   canonical runtime frame
/80   predicate or ABI gate
/96   mapped carrier boundary
/112  narrow rule/register boundary
/120  byte-specific local narrowing
/128  exact state pointer
```

Useful V0 `ip4-notation` depths:

```text
/0    default projection surface
/8    large userland region
/16   local region
/24   surface board or session scope
/32   exact projection object
```

This is CIDR-like resolution, but it remains OMI notation:

```text
longest-prefix match = deepest lawful CONS continuation
```

Longest-prefix match must be deterministic and must not depend on network arrival order.

## Omicron CIDR Dialect

The Omicron CIDR dialect is a non-authoritative notation dialect.

It translates between:

```text
CIDR notation
ip4-notation
ip6-notation
Base36 symbolic orbit labels
Base64url continuation carriers
Omi-CONS CAR/CDR closure
canonical hexadecimal OMI frames
```

Canonical rule:

```text
Hex canonically frames.
CIDR resolves.
Omicron induces the dialect.
Tetragrammatron validates the lowered candidate.
```

The dialect may parse readable forms, resolve prefix candidates, and lower them into canonical hexadecimal OMI candidate frames.

The dialect must not:

- replace hexadecimal OMI identity
- validate truth
- accept state
- write accepted ring state
- treat Base36 as protocol law
- treat Base64 as protocol law
- treat IP syntax as network authority

Address-family roles:

```text
ip6-notation = 128-bit local/system memory scope
ip4-notation = 32-bit userland/projection scope
```

`ip4-notation` and `ip6-notation` are source conveniences. They must lower into canonical hexadecimal OMI frame form before validation.

## Omi-Log Source Surface

Omi-Log is the readable declaration surface that may use the Omicron CIDR dialect.

Canonical source/object convention:

```text
.omi = readable Omi-Log source
.imo = compiled mirrored OMI object
.o   = compiled machine object / kernel runtime artifact
```

Omi-Log may use:

- `ip4-notation`
- `ip6-notation`
- CIDR prefix scopes
- `@CAR36@CDR64` closure
- `omi-` / `-imo` source blocks
- O-expressions
- association lists
- readable keywords
- UPOS context ports
- symbolic carrier atoms

Every Omi-Log record must lower to canonical OMI object form before validation.

Readable record shape:

```text
<address>/<prefix> <KEYWORD> <assignment>

omi-
  <O-expression body>
-imo
```

Interpretation:

```text
OMI address selects.
CIDR prefix scopes.
Omi-Log declares.
O-expression structures.
.omi sources.
.imo traverses.
Hex canonically frames.
Tetragrammatron validates.
```

## OMI Citation Candidate Handoff

After Omicron parses and normalizes an Omi-Log declaration candidate, OMI may construct a citation-facing candidate:

```text
OmiLogCandidate -> OmiCitationCandidate
```

Current C API:

```text
omi_construct_citation_candidate
```

This handoff copies the lowered address fields, keyword, assignment, source-block flags, CONS closure fields, and deterministic citation text/hash into OMI-owned candidate state.

It does not validate, accept, write the receipt ring, project, evaluate, route, serialize, or perform carrier I/O.

Boundary lock:

```text
Omi-Log declares.
Omicron normalizes and lowers.
OMI constructs citation candidates.
Tetragrammatron validates later.
```

## 128-Bit And 256-Bit Frames

The OMI identity frame is eight 16-bit segments:

```text
S0-S1-S2-S3-S4-S5-S6-S7
```

Size:

```text
8 x 16 bits = 128 bits
```

The 256-bit OMI---IMO relation frame is:

```text
8 x 16-bit scope/ruler fields = 128 bits
4 x 32-bit relation fields    = 128 bits
total                         = 256 bits
```

Relation fields:

```text
REGISTER
STACK
CAR
CDR
```

The first 128 bits stage topology. The second 128 bits stage relation.

## Omi-CONS Closure Dialect

The `@---@` surface names CAR/CDR closure.

Readable mnemonic:

```text
@---@
```

Expanded machine form:

```text
@CAR@CDR
```

V0 may expose this readable closure dialect:

```text
@CAR36@CDR64
```

Where:

```text
CAR36 = Base36-encoded CAR label
CDR64 = Base64url-encoded CDR payload or continuation bytes
```

It must lower to canonical hexadecimal CAR/CDR before validation:

```text
@CAR36@CDR64
  -> decode CAR36
  -> decode CDR64
  -> @CAR_HEX@CDR_HEX
```

`CAR36` names a law-derived orbit/head position. It does not create the orbit.

`CDR64` carries continuation bytes. It does not define identity.

Canonical rules:

```text
Base36 names law-derived orbit positions.
Base64 carries bytes.
Neither accepts state.
```

Recommended Base36 alphabet:

```text
0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ
```

Recommended test vectors:

```text
0    -> 000
239  -> 06N
240  -> 06O
5039 -> 3VZ
```

Base64url test vector:

```text
bytes: 4f 4d 49
base64url: T01J
meaning: "OMI"
```

Closure candidate:

```text
@3VZ@T01J
```

Interpretation:

```text
CAR36 = 3VZ = slot 5039
CDR64 = T01J = bytes 0x4f 0x4d 0x49
```

Status: candidate only until lowered and validated.

## Pre-Language Context

Addressing derives from the pre-language control envelope and from the one-to-one alignment between the 32 non-printing controls and the 32 place-value nibbles.

The control range is:

```text
0x00..0x1F
```

The place cascade is:

```text
0x00 -> n00
0x01 -> n01
0x02 -> n02
...
0x1F -> n31
```

The control code is not the nibble value. The control code is the place controller. The nibble value remains one of `0x0..0xF`.

Stable control roles:

```text
0x00  NUL  origin / null / zero-point
0x1C  FS   file scope
0x1D  GS   group scope
0x1E  RS   record scope / closure witness
0x1F  US   unit scope
0x20  SP   readable boundary / visible separator
```

The gauge pre-header remains:

```text
G 00 1C 1D 1E 1F 20 G
```

where:

```text
G in 0xF0..0xFF
```

Canonical OMI:

```text
FF 00 1C 1D 1E 1F 20 FF
```

## Boot Bridge Constants

The pre-boot order is:

```text
0x00..0x1F  non-printing place controllers
0x20..0x2F  register injection / punctuation
0x30..0x3F  kernel reader
0x78        full nibble-field witness
0x7F        local ASCII seal
0x7C00      boot page witness
0xAA55      external boot bridge
afterward   declarative OMI-Lisp dot notation
```

Constant roles:

```text
0x1E
  record separator / diagonal closure witness

0x78
  full nibble-field system witness

0x7F
  local ASCII seal

0x7C00
  boot page witness

0xAA55
  external bridge word / balanced crossing pattern
```

The byte gauge is `0x00..0x7F`. The bridge word `0xAA55` is a 16-bit external bridge word and enters through a bridge resolver, not through the byte gauge.

At the boot boundary, `0xAA55` is read as an external bridge/signature word.
At the canon pattern layer, the same bit arrangement is the Metatron balanced
crossing witness. Those readings are compatible but not identical:

```text
boot place       -> bridge/signature role
Metatron surface -> observational balance role
```

Neither use gives the word validation or acceptance authority.

Omicron bootstrap:

```text
F0 00 1C 1D 1E 1F 20 F0
```

The first and last gauge bytes must match. Header recognition is not acceptance.

## Horn-Clause Path Reading

Path segments may be read as deterministic derivation clauses.

Example:

```text
omi-<frame>/hardware/i2c:0x28/euler-imo
```

Clause reading:

```text
domain(Frame, hardware).
interface(Frame, i2c).
selector(Frame, 0x28).
overlay(Frame, euler).
```

The path describes how data becomes eligible for validated carry-forward state. It does not validate the relation.

## V0 Implementation Target

The first implementation pass should add a parser/normalizer only.

It should produce a deterministic candidate object with:

```text
source
open
frame
path segments
close
prefix depth when present
path type
optional payload/mask fields
optional CAR36/CDR64 closure
lowered hexadecimal candidate frame
```

It should not:

- validate
- store validated states
- perform longest-prefix routing over live tables
- execute Horn clauses
- project surfaces
- read or write files
- perform browser, network, or hardware effects

Minimal conforming parser requirements:

```text
parse hexadecimal OMI frames
parse ip4-notation
parse ip6-notation
parse prefix length
parse @CAR36@CDR64 closure
decode Base36 CAR
decode Base64url CDR
lower to canonical hexadecimal OMI form
preserve authority boundary
reject ambiguous address family
reject non-canonical encodings
```

Suggested C ownership:

```text
OMI owns canonical address parsing and normalization.
Omicron owns dialect induction and readable address lowering.
Tetragrammatron owns validation of lawful carry-forward.
Metatron owns validated-state scribing and projection.
Receipt owns acceptance.
IMO owns carrier parsing, files, server, and output.
```

## Lock

Omi-Ring witnesses.
Slash derives.
CONS relates.
Prefix scopes.
Path selects.
Validation judges.
Validation carries forward.
