# OMICRON_CIDR_DIALECT_SPEC.md

## Omicron CIDR Dialect Specification

Status: Draft  
Layer: Omicron dialect induction / address notation adapter  
Authority: Non-authoritative notation dialect  
Depends on: OMI hexadecimal frame, Omi-CONS, Omi-Ring addressed notation witness, Tetragrammatron validation, Metatron projection, IMO carrier mapping

---

## 0. Purpose

This document defines the Omicron CIDR dialect.

The Omicron CIDR dialect gives OMI a readable address notation for local, system, userland, and projection scopes without replacing the canonical hexadecimal OMI runtime frame.

The dialect exists to translate between:

```text
CIDR notation
IPv4 notation
IPv6 notation
Base36 symbolic orbit labels
Base64 payload carriers
Omi-CONS CAR/CDR closure
canonical hexadecimal OMI frames
```

Omicron owns this translation layer.

The four authority modules do not change their internal authority surfaces:

```text
OMI              uses hexadecimal citation frames
Tetragrammatron  validates hexadecimal citation state
Metatron         projects accepted state
IMO              carries serialized forms
Omicron          induces CIDR/address dialects
```

Canonical rule:

```text
Hex validates.
CIDR resolves.
Omicron induces the dialect.
```

---

## 1. Authority Boundary

The Omicron CIDR dialect MUST NOT replace the hexadecimal OMI frame.

It MAY:

```text
parse CIDR notation
parse ip4-notation
parse ip6-notation
resolve longest-prefix matches
lower readable address forms into hexadecimal OMI frames
project CAR/CDR closure into Base36/Base64 notation
produce candidate Omi-CONS records
```

It MUST NOT:

```text
validate truth
accept state
write accepted ring state
replace OMI hexadecimal identity
replace Tetragrammatron validation
treat Base36 as protocol law
treat Base64 as protocol law
treat IP syntax as network authority
```

CIDR is a notation dialect.

Hexadecimal remains the kernel/runtime authority frame.

---

## 2. Core Reduction

The dialect begins with one reduction:

```text
CIDR is CONS.
The slash is Omicron.
Longest-prefix match is resolution.
```

CIDR form:

```text
address/prefix
```

CONS form:

```lisp
(address . prefix)
```

OMI reading:

```text
CAR = address payload
CDR = factoring depth / continuation condition
/   = Omicron factoring operator
```

The slash `/` does not merely separate strings.

It declares the depth at which the address is resolved.

---

## 3. Address Families

The Omicron CIDR dialect defines two user-facing address families.

```text
ip6-notation = 128-bit high continuation frame
ip4-notation = 32-bit compact projection frame
```

### 3.1 ip6-notation

`ip6-notation` maps to the 128-bit OMI frame.

An IPv6-style address contains eight 16-bit groups.

OMI contains eight 16-bit segments.

Therefore the mapping is direct:

```text
ip6 group 0 -> S0
ip6 group 1 -> S1
ip6 group 2 -> S2
ip6 group 3 -> S3
ip6 group 4 -> S4
ip6 group 5 -> S5
ip6 group 6 -> S6
ip6 group 7 -> S7
```

Canonical role:

```text
ip6-notation = local/system/runtime memory scope
```

Use cases:

```text
compiled .imo object addressing
system memory scoping
local kernel frame inspection
service bus declaration
pre-runtime object layout
```

### 3.2 ip4-notation

`ip4-notation` maps to a 32-bit compact projection dialect.

Canonical role:

```text
ip4-notation = remote/userland/surface projection scope
```

Use cases:

```text
browser DOM projection
CSSOM selection
remote peer surface labels
compact user-facing aliases
local loopback projection
portable .omi readable source
```

`ip4-notation` MUST lower into a canonical OMI frame before validation.

It is not itself validation authority.

---

## 4. Hexadecimal Kernel Frame

The kernel/runtime frame remains hexadecimal.

Canonical machine-readable OMI frame:

```text
o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

Each `S` segment is a 16-bit hexadecimal field.

`PAYLOAD`, `MASK`, `CAR`, and `CDR` are 32-bit hexadecimal fields unless a dialect explicitly declares a different external encoding.

The Omicron CIDR dialect MUST lower all accepted address candidates into this form before authority modules process them.

---

## 5. File Scope Convention

The dialect uses the existing source/object distinction.

```text
.omi = readable source / userland declaration / projection-facing
.imo = compiled mirrored object / native OMI plane / system-facing
.o   = compiled machine object / kernel runtime artifact
```

Recommended mapping:

```text
.omi  may use ip4-notation and readable CIDR dialects
.imo  may use ip6-notation and native low-plane OMI forms
.o    uses compiled binary/hexadecimal runtime representation
```

This is a convention, not an authority substitution.

---

## 6. Scope Table

| Layer | Notation | Width | Scope |
|---|---:|---:|---|
| Kernel/runtime | hex OMI | 128/256 | validation, object ABI, compiled `.o` |
| System memory | ip6-notation | 128 | local/system, compiled `.imo` |
| Userland surface | ip4-notation | 32 | remote/userland, readable `.omi` |
| Browser projection | DOM/CSSOM | variable | `id`, `data-*`, bidi, projection |
| Carrier storage | gossip/repair profile | variable | transport and recovery only |

---

## 7. CIDR Prefix Semantics

A CIDR prefix declares resolution depth.

Examples:

```text
0.0.0.0/0
10.0.0.0/8
10.120.0.0/16
10.120.78.0/24
10.120.78.40/32
```

Resolution chooses the deepest matching lawful prefix.

Equivalent OMI rule:

```text
resolve(address):
  parse candidate address
  collect matching CIDR CONS rows
  choose deepest prefix
  verify proof/witness field
  lower to canonical OMI frame
```

For `ip6-notation`, standard useful prefix depths include:

```text
/7    private fabric base
/32   boot or route entry
/48   canonical frame family
/80   predicate or ABI gate
/96   mapped carrier boundary
/112  narrow rule/register boundary
/120  byte-specific local narrowing
/128  exact OMI frame
```

For `ip4-notation`, standard useful prefix depths include:

```text
/0    default projection surface
/8    large userland region
/16   local region
/24   surface board or session scope
/32   exact projection object
```

---

## 8. Omi-CONS Record

The canonical runtime record for this dialect is `CONS256`.

```text
CONS256
├── CAR64
├── CDR128
└── META64
```

Roles:

```text
CAR64  = semantic head / identity / route key
CDR128 = continuation / edge / resolver target
META64 = proof witness over the CAR/CDR relation
```

Locked predicate:

```text
CAR = head
CDR = edge
META = proof witness
```

`META64` proves the relation.

`CDR128` carries the edge.

`CAR64` names the head.

---

## 9. @---@ Closure Dialect

The `@---@` surface names CAR/CDR closure.

Readable mnemonic:

```text
@---@
```

Expanded machine form:

```text
@CAR@CDR
```

Omicron CIDR dialect MAY expose a distinct CONS dialect over this closure:

```text
@CAR36@CDR64
```

Where:

```text
CAR36 = Base36-encoded CAR label
CDR64 = Base64url-encoded CDR payload or continuation bytes
```

Canonical public form:

```text
o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR36@CDR64
```

This is a readable transport dialect.

It MUST lower to canonical hexadecimal CAR/CDR before validation:

```text
@CAR36@CDR64
  ↓
decode CAR36
decode CDR64
  ↓
@CAR_HEX@CDR_HEX
```

---

## 10. CAR36

`CAR36` is a Base36 symbolic orbit label for the head side of the CONS pair.

Base36 alphabet:

```text
0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ
```

Use:

```text
CAR36 = compact visible name for a CAR orbit/head coordinate
```

It may encode:

```text
slot5040 label
local240 coordinate
semantic route key
projection head label
short human-readable citation handle
```

`CAR36` is not the source of truth.

It projects a law-derived position.

Canonical rule:

```text
Base36 names the orbit position.
Base36 does not create the orbit.
```

Recommended zero-padded widths:

```text
slot5040  -> 3 Base36 chars, 000..3VZ
local240  -> 2 Base36 chars, 00..6N
cycle     -> implementation-defined Base36 width
```

Examples:

```text
000
06O
3VZ
```

---

## 11. CDR64

`CDR64` is a Base64url transport encoding for continuation bytes.

Use:

```text
CDR64 = compact byte-safe continuation carrier
```

It may carry:

```text
CDR128 bytes
external payload pointer
projection capsule bytes
worker payload
matrix/vector bytes
serialized continuation edge
```

`CDR64` is not identity authority.

It is a carrier encoding for the continuation side.

Canonical rule:

```text
Base64 carries bytes.
Base64 does not define OMI identity.
```

The dialect SHOULD use URL-safe Base64:

```text
A-Z a-z 0-9 - _
```

Padding MAY be omitted in readable dialect forms but MUST be restored by decoders before byte decoding.

---

## 12. @CAR36@CDR64 Syntax

The distinct CONS dialect has this shape:

```text
@<CAR36>@<CDR64>
```

Grammar:

```text
car36      := [0-9A-Z]+
cdr64      := [A-Za-z0-9_-]+
closure    := "@" car36 "@" cdr64
```

Full address candidate:

```text
omi-<ip6-or-hex-frame>/<prefix>?<payload>?<mask>@<CAR36>@<CDR64>
```

Examples:

```text
omi-0000-0000-0000-0000/32?0?0@3VZ@AAECAwQ
omi-8-ffff-127-0-0-1/128?0?0@06O@AAC_QEAAAL_AykAQA
omi-0000-0000-0000-0000-0000-0000-007c-0000/128?0?0@000@T01J
```

The examples are candidate notation only.

They require decoding and validation before acceptance.

---

## 13. Lowering Algorithm

Given a candidate Omicron CIDR address:

```text
A = frame/prefix?payload?mask@car36@cdr64
```

Lower as follows:

```text
1. Parse frame dialect.
2. Determine address family: hex, ip4-notation, or ip6-notation.
3. Parse CIDR prefix depth.
4. Resolve CIDR as CONS: (address . prefix).
5. Decode CAR36 into unsigned integer.
6. Decode CDR64 into byte sequence.
7. Normalize CAR into canonical 32-bit or 64-bit head field.
8. Normalize CDR into canonical 32-bit field or CDR128 continuation record.
9. Construct canonical hexadecimal OMI frame.
10. Submit candidate citation to OMI.
11. Submit citation to Tetragrammatron validation.
12. Record only if validation accepts.
```

Pseudocode:

```text
parse_omicron_cidr(s):
  parts = split_frame_payload_closure(s)
  frame = parse_frame(parts.frame)
  prefix = parse_prefix(parts.prefix)

  cons = {
    car: frame.address,
    cdr: prefix
  }

  closure = {
    car36: parse_base36(parts.car36),
    cdr64: decode_base64url(parts.cdr64)
  }

  return lower_to_hex_omi(frame, prefix, closure)
```

---

## 14. Validation Gate

A decoded Omicron CIDR dialect form is only a candidate until validated.

The validator MUST check:

```text
frame parses
prefix is bounded
ip4/ip6 family is explicit
CIDR resolution is deterministic
CAR36 is canonical uppercase Base36
CDR64 decodes without ambiguity
decoded CDR width is declared
lowered hexadecimal frame is reproducible
META or witness field matches CAR/CDR relation when present
authority boundary is preserved
```

Reject if:

```text
ambiguous IP family
mixed ip4/ip6 field without mapped declaration
invalid prefix length
non-canonical Base36
invalid Base64url
decoded continuation exceeds declared width
CAR/CDR lower to conflicting values
symbol attempts to override frame validity
projection attempts to accept state
```

---

## 15. Local and Remote Scoping

The dialect distinguishes local/system memory from remote/userland projection.

Recommended convention:

```text
ip6-notation = local/system memory scope
ip4-notation = remote/userland projection scope
```

Loopback convention:

```text
ip6 local system envelope: ::ffff:127.0.0.1
ip4 local projection base: 127.0.0.1
```

A dialect adapter MAY treat objects under the loopback envelope as local.

A dialect adapter MUST NOT treat loopback presence as validation.

---

## 16. DOM/CSSOM Projection Form

Browser projection MAY use hyphenated token form:

```html
<div id="omi-8-ffff-127-0-0-1-127-0-0-0-25-NOUN-VERB-SYM-AAC_QEAAAL_AykAQA"></div>
```

Interpretation:

```text
omi                dialect root
8                  local service bus / ip6 shorthand
ffff-127-0-0-1     mapped loopback envelope
127-0-0-0-25       ip4 projection subnet 127.0.0.0/25
NOUN-VERB          context port pair
SYM                unit leaf class
AAC_...            Base64url payload carrier
```

This is projection syntax.

It is not validation authority.

---

## 17. Context Ports

The dialect MAY define context ports after the content/CIDR layer.

Recommended context-port layer:

```text
<content-cidr>/<context-port>/<unit-leaf>
```

Readable hyphen form:

```text
omi-<content-address>-<src-port>-<dst-port>-<leaf>-<payload>
```

UPOS tags MAY be used as semantic context ports:

```text
ADJ ADP ADV AUX CCONJ DET INTJ NOUN NUM PART PRON PROPN PUNCT SCONJ SYM VERB X
```

Context ports are dispatch hints.

They do not validate the address.

---

## 17A. Omi-Log Integration

Omi-Log is the readable declaration language that uses the Omicron CIDR dialect.

It is not a replacement for the hexadecimal OMI frame.

It is the source-level place where humans write addressed declarations, CIDR scopes, O-expression bodies, source blocks, and CONS closure notation before lowering into native `.imo` object form.

Canonical source/object rule:

```text
.omi = readable Omi-Log source
.imo = compiled mirrored OMI object
```

Omi-Log MAY use:

```text
ip4-notation
ip6-notation
CIDR prefix scopes
@CAR36@CDR64 closure
omi- / -imo source blocks
O-expressions
association lists
readable keywords
UPOS context ports
symbolic carrier atoms
```

But every Omi-Log record MUST lower to canonical OMI object form before validation.

The Omi-Log record shape is:

```text
<address>/<prefix> <KEYWORD> <assignment>

omi-
  <O-expression body>
-imo
```

The address line selects.

The CIDR prefix scopes.

The source block declares.

The O-expression body carries the declaration structure.

The compiler lowers the readable source into native `.imo` object form.

Canonical Omi-Log interpretation:

```text
OMI address selects.
CIDR prefix scopes.
Omi-Log declares.
O-expression structures.
@CAR36@CDR64 closes CONS.
.omi sources.
.imo traverses.
Hex validates.
```

### 17A.1 Omi-Log and CIDR

In Omi-Log, an address head MAY be written with CIDR scope:

```text
omi-0000-0000-0000-0000-0000-0000-0036-0072/128 MUST project-base36-orbital-symbols
```

or in dialect form:

```text
ip6::<frame>/128 MUST project-base36-orbital-symbols
ip4:127.0.0.1/32 FACT local-projection-surface-present
```

Dialect forms are source conveniences.

They MUST compile to canonical hexadecimal OMI frame form before validation.

### 17A.2 Omi-Log and @CAR36@CDR64

Omi-Log MAY use the distinct CONS closure dialect in declaration heads or source bodies:

```text
omi-0000-0000-0000-0000-0000-0000-0036-0072/128?0?0@3VZ@T01J FACT base36-car-cdr-closure-declared
```

Readable O-expression form:

```lisp
(
  (car36 . "3VZ")
  (cdr64 . "T01J")
  (closure . "@3VZ@T01J")
  (meaning . "candidate-cons-closure")
)
```

Compiler lowering:

```text
@3VZ@T01J
  -> decode CAR36 as unsigned integer
  -> decode CDR64 as byte continuation
  -> normalize to hexadecimal CAR/CDR fields
  -> construct canonical OMI candidate
  -> validate before acceptance
```

### 17A.3 Omi-Log Authority Boundary

Omi-Log expresses declarations.

It does not validate them.

Forbidden collapses:

```text
Omi-Log source validates frame
CIDR prefix validates frame
O-expression validates frame
Base36 validates frame
Base64 validates frame
@CAR36@CDR64 validates frame
omi- / -imo source block validates frame
```

Allowed roles:

```text
Omi-Log expresses addressed declarations
CIDR scopes declarations
O-expressions structure declarations
Base36 names law-derived orbit positions
Base64 carries continuation bytes
@CAR36@CDR64 marks readable CONS closure
hexadecimal OMI frame is submitted for validation
```

### 17A.4 Omi-Log Compilation Path

The dialect-aware Omi-Log compiler SHOULD run this path:

```text
1. Parse Omi-Log head.
2. Parse address family: hex, ip4-notation, or ip6-notation.
3. Parse CIDR prefix.
4. Parse optional ?PAYLOAD?MASK@CAR36@CDR64 closure.
5. Parse optional omi- / -imo source block.
6. Parse O-expression body when present.
7. Lower dialect address to canonical hexadecimal OMI frame.
8. Decode CAR36 and CDR64 if present.
9. Emit native .imo object form.
10. Submit candidate to validation.
```

Omi-Log is therefore the correct home for the readable Omicron CIDR dialect.

It is the language surface where users write the dialect, while Omicron performs the lowering and the validation layer remains hexadecimal.

---

## 18. Base36 Bridge Law

The Base36 layer is justified by the law-derived orbit width.

Canonical sequence:

```text
Delta Law
→ period 8
→ prime 73
→ block B = 01369863
→ W = 36
→ Base36 alphabet
```

The bridge identity:

```text
240 = 2×5! = 6×36 + 4!
```

OMI reading:

```text
hidden five-fold root + complement orientation
=
six Base36 orbit bands + four-fold selector face
```

This belongs to symbolic projection and orbit labeling.

It does not replace validation.

---

## 19. Base64 Boundary

Base64 belongs to payload carriage.

It MAY carry:

```text
matrix coefficients
projection capsules
worker payloads
external encoded data
serialized CDR continuation bytes
```

It MUST NOT carry authority by itself.

Canonical rule:

```text
Encoding is adapter metadata.
Topology is protocol metadata.
Validation is authority metadata.
```

---

## 20. Minimal Conforming Parser

A minimal conforming Omicron CIDR dialect parser MUST:

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

It MAY:

```text
support UPOS context ports
support DOM/CSSOM projection IDs
support local/remote prefixes
support loopback convenience syntax
support .omi source-level aliases
```

---

## 21. Test Vectors

### 21.1 Base36 slot labels

```text
0      -> 000
239    -> 06N
240    -> 06O
5039   -> 3VZ
5040   -> invalid slot5040, size marker only
```

### 21.2 Base64url payload

```text
bytes: 4f 4d 49
base64url: T01J
meaning: payload bytes for "OMI"
```

### 21.3 Closure candidate

```text
@3VZ@T01J
```

Interpretation:

```text
CAR36 = 3VZ = slot 5039
CDR64 = T01J = bytes 0x4f 0x4d 0x49
```

Status:

```text
candidate only until lowered and validated
```

---

## 22. Security and Safety

Implementations MUST treat decoded Base64 payloads as untrusted bytes.

Implementations MUST NOT execute decoded payloads unless:

```text
the effect class is declared
the carrier policy permits execution
the citation validates
the accepted state authorizes projection or execution
```

CIDR resolution MUST be deterministic.

Longest-prefix match MUST NOT be affected by network arrival order.

Local/remote labels MUST be projection hints only unless bound by validated policy.

---

## 23. Final Canon

```text
CIDR is CONS.
The slash is Omicron.
Longest-prefix match is resolution.

ip6-notation scopes 128-bit local/system memory.
ip4-notation scopes 32-bit userland projection.
hexadecimal remains the runtime validation frame.

@---@ closes the pair.
CAR36 names the head orbit.
CDR64 carries continuation bytes.
META proves the relation.

Base36 names law-derived orbit positions.
Base64 carries bytes.
Neither accepts state.

Omicron induces the dialect.
OMI cites.
Tetragrammatron validates.
Metatron projects.
IMO carries.
```

---

## 24. One-Sentence Version

The Omicron CIDR dialect treats `address/prefix` as `(address . prefix)`, uses `ip6-notation` for 128-bit local/system memory scope and `ip4-notation` for 32-bit userland projection scope, and defines a distinct `@CAR36@CDR64` CONS closure where Base36 names the CAR orbit while Base64url carries the CDR continuation, all lowering back to canonical hexadecimal OMI before validation.
