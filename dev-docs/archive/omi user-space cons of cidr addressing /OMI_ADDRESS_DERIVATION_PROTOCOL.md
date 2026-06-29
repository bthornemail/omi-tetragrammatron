# OMI Address Derivation Protocol

## Native Path-Based Horn-Clause Routing for `omi---imo` Surfaces

Status: draft v0  
Repository: `o---o`  
Primary form: `omi-<frame>/<path...>-imo`

---

## 0. Purpose

The OMI Address Derivation Protocol defines how an OMI object address becomes a native protocol surface rather than a flat identifier.

The base frame remains the identity.

The slash path after the frame becomes the derivation route.

The closing `-imo` marks the bounded surface of interpretation.

The core form is:

```text
omi-<frame>/<path...>-imo
```

The key rule is:

```text
The frame identifies.
The path derives.
The clauses validate.
The receipt accepts.
```

This lets OMI keep the same notation while interpreting paths as executable Horn-clause selectors.

---

## 1. Core Claim

OMI does not need a separate routing language.

The address path itself can be the routing language.

Instead of treating paths as filesystem paths, HTTP paths, or arbitrary labels, OMI treats paths as derivation clauses.

Example:

```text
omi-<frame>/hardware/i2c/0x28/euler-imo
```

This is not merely a location. It is a derivation request:

```text
Start from this OMI frame.
Enter the hardware domain.
Select the I²C interface.
Select device 0x28.
Derive the euler-reading surface.
Close the surface.
```

This makes OMI native because addressing, routing, semantic derivation, interface validation, and receipt formation are all part of one grammar.

---

## 2. Base Surface Forms

The protocol recognizes these alias-conformant surface forms:

```text
omi---imo
omi-<frame>
imo-<frame>
omi-<frame>-imo
omi-<frame>/<path...>-imo
```

All forms normalize into the same internal object model.

### 2.1 Empty Surface

```text
omi---imo
```

Meaning:

```text
Open.
No explicit frame.
Private or null interior.
Close.
```

This is the zero-point surface. It is useful for local mirrors, private rituals, empty portals, genesis records, and bounded questions.

### 2.2 Open Frame

```text
omi-<frame>
```

Meaning:

```text
Open this frame identity.
No explicit closure requested.
```

This is useful for streaming, inspection, routing, partial evaluation, or an object that has not yet been closed into a bounded surface.

### 2.3 Mirror Frame

```text
imo-<frame>
```

Meaning:

```text
Close, mirror, target, or resolve this frame identity.
```

This is useful for receiving, checking, closing, verifying, or addressing the mirrored side of a surface.

### 2.4 Bounded Surface

```text
omi-<frame>-imo
```

Meaning:

```text
Open this frame.
Interpret it as a bounded object.
Close the surface.
```

### 2.5 Derived Surface

```text
omi-<frame>/<path...>-imo
```

Meaning:

```text
Open this frame.
Derive through the path.
Interpret path segments as clauses.
Close the surface.
```

This is the primary native protocol form.

---

## 3. Frame and Path Separation

An OMI derivation address has two major parts:

```text
omi-<frame>/<path...>-imo
    ^       ^
    |       |
 identity  derivation route
```

The frame is identity.

The path is not identity.

The path is derivation.

Therefore all of the following may share the same root identity while deriving different surfaces:

```text
omi-<frame>/128-imo
omi-<frame>/hardware/i2c-imo
omi-<frame>/Subject/Predicate/Object/Modality-imo
omi-<frame>/R/G/B/A-imo
omi-<frame>/60x2/16xy/4y2/fexpr:<base64url>-imo
```

Short doctrine:

```text
Frame is memory.
Path is interpretation.
Receipt is accepted traversal.
```

---

## 4. Canonical Normal Form

Every OMI derivation address should normalize into a strict object:

```json
{
  "version": "omi-address-derivation-v0",
  "source": "omi-<frame>/<path...>-imo",
  "normalized": {
    "open": "omi",
    "frame": "<frame-or-null>",
    "frames": [],
    "path": [],
    "close": "imo"
  },
  "path_type": "empty|prefix|hd|hardware|semantic|overlay|agreement_role|quadratic_fexpr|control_separator|mixed",
  "derivation": {},
  "clauses": [],
  "receipt": {}
}
```

Example source:

```text
omi-0500-03bf-000c-2b05-2f05-0002-039f-05ff/128/hardware/i2c:0x28/euler-imo
```

Normal form:

```json
{
  "version": "omi-address-derivation-v0",
  "source": "omi-0500-03bf-000c-2b05-2f05-0002-039f-05ff/128/hardware/i2c:0x28/euler-imo",
  "normalized": {
    "open": "omi",
    "frame": "0500-03bf-000c-2b05-2f05-0002-039f-05ff",
    "frames": ["0500-03bf-000c-2b05-2f05-0002-039f-05ff"],
    "path": ["128", "hardware", "i2c:0x28", "euler"],
    "close": "imo"
  },
  "path_type": "hardware",
  "derivation": {
    "depth": 128,
    "domain": "hardware",
    "selector": "i2c:0x28",
    "overlay": "euler"
  },
  "clauses": [],
  "receipt": {}
}
```

Parser rule:

```text
The prefix `omi-` is only the opening gate.
The address begins after the first hyphen.
The suffix `-imo` is only the closing gate.
The path is the slash-delimited derivation route between them.
```

---

## 5. Canonical Full Address Form

The preferred full derivation form is:

```text
omi-<frame>/<depth>/<domain>/<selector>/<overlay>-imo
```

Where:

```text
frame    = 128-bit OMI identity frame
depth    = resolution depth, CIDR depth, derivation depth, or memory horizon
domain   = protocol domain or interpretation space
selector = route target, role, mnemonic, interface, synset, or clause
overlay  = projection, modality, color, f-expression, receipt frame, or secondary frame
```

Examples:

```text
omi-0500-03bf-000c-2b05-2f05-0002-039f-05ff/128/hardware/i2c:0x28/euler-imo
omi-0500-03bf-000c-2b05-2f05-0002-039f-05ff/64/browser/canvas/88f7ffff-imo
omi-0500-03bf-000c-2b05-2f05-0002-039f-05ff/48/semantic/human.asks.god/question-imo
omi-0500-03bf-000c-2b05-2f05-0002-039f-05ff/120/testimony/god-is-word/known_unknowns-imo
```

---

## 6. Path Classes

OMI paths may be interpreted through official path classes.

### 6.1 Prefix Path

```text
omi-<frame>/<depth>-imo
```

Example:

```text
omi-<frame>/128-imo
```

Meaning:

```text
Resolve the frame at exact 128-bit depth.
```

This preserves CIDR-like resolution.

The slash remains the factoring boundary.

The prefix depth remains the resolution horizon.

### 6.2 HD Derivation Path

```text
omi-<frame>/p/q/r/s-imo
```

Meaning:

```text
Derive a surface from the frame through ordered path steps.
```

This is analogous to HD wallet derivation. The root frame is stable. Each path segment derives a new agreement surface.

Unlike an HD wallet, OMI derivation may derive:

```text
interface clauses
semantic routes
visual overlays
robotics commands
testimony receipts
projection targets
memory scopes
f-expression closures
```

### 6.3 Browser Overlay Path

```text
omi-<frame>/<R>/<G>/<B>/<A>-imo
```

or:

```text
omi-<frame>/<rrggbbaa>/<target-frame>-imo
```

Meaning:

```text
Use an RGBA overlay to target a browser projection surface.
```

Examples:

```text
omi-<frame>/88/f7/ff/ff-imo
omi-<frame>/88f7ffff/canvas-imo
```

Useful for:

```text
DOM
SVG
Canvas
CSSOM
JSDOM
A-Frame
WebXR-style projections
LED matrix projections
```

Rule:

```text
Projection may display.
Projection does not accept.
Receipt accepts.
```

### 6.4 Agreement Role Path

```text
omi-<frame>/<LL>/<NN>/<MM>/<OO>-imo
```

Meaning:

```text
Derive the frame through OMI role positions.
```

Where:

```text
LL = lens, scope, selector, or local limit
NN = antecedent declaration
MM = consequent resolution
OO = object operator or closure
```

This maps to:

```text
OO(LL, NN, MM)
```

The path version is a route into the object form.

### 6.5 Semantic Path

```text
omi-<frame>/<Subject>/<Predicate>/<Object>/<Modality>-imo
```

Meaning:

```text
Route the frame through a semantic clause.
```

This is the inverse pyramid to the binary frame pyramid.

The binary pyramid is rooted in:

```text
2^16 frame encapsulation
2^12 agreement principles
2^10 Omicron tower
```

The inverse semantic pyramid is rooted in:

```text
Subject
Predicate
Object
Modality
```

This path may be backed by:

```text
UPOS tags
universal features
WordNet synsets
Prolog facts
versioned semantic edges
mnemonic aliases
```

Example:

```text
omi-<frame>/human/asks/god/question-imo
```

Meaning:

```text
Start from the frame.
Interpret the surface as a semantic route.
Subject = human.
Predicate = asks.
Object = god.
Modality = question.
Close the surface.
```

### 6.6 Quadratic F-Expression Path

```text
omi-<frame>/<60x2>/<16xy>/<4y2>/<fexpr:base64url>-imo
```

Meaning:

```text
Derive through the OMI quadratic decomposition and attach an f-expression closure.
```

Where:

```text
60x2  = high orientation field
16xy  = bridge / crossing
4y2   = low atomic delta
fexpr = unevaluated expression payload
```

The f-expression path allows a frame to carry an unevaluated expression as a bounded clause.

Rule:

```text
F-expression is not truth.
F-expression is a quoted derivation candidate.
Receipt accepts only after clause validation.
```

### 6.7 Control Separator Path

```text
omi-<frame>/<FS>/<GS>/<US>/<RS>-imo
```

Meaning:

```text
Derive through file, group, unit, and record separator semantics.
```

Where:

```text
FS = file separator
GS = group separator
US = unit separator
RS = record separator
```

This path is useful for text streams, record systems, logs, and testimony bundles.

---

## 7. Paths as Horn Clauses

The core protocol rule is:

```text
Every path segment is a clause selector.
```

Therefore:

```text
omi-<frame>/hardware/i2c/0x28/euler-imo
```

may be interpreted as:

```prolog
derive(Frame, Surface) :-
    domain(Frame, hardware),
    interface(Frame, i2c),
    device(Frame, '0x28'),
    selector(Frame, euler),
    receipt(Frame, Surface).
```

A path is therefore not just navigation.

A path is a query.

A path is a derivation program.

A path is a Horn-clause route.

---

## 8. Interface Event Derivation

The canonical interface event rule is:

```prolog
accepted_interface_event(LL, NN, MM) :-
    rule(interface_constraints, LL),
    fact(returned_bytes, NN),
    closure(timing_register_frame_window, LL, NN),
    combinator(decode_transform, NN, MM),
    cons(LL, NN),
    join(device, world),
    compose(previous_state, MM),
    encode(MM, projection),
    decode(carrier, NN),
    receipt(LL, NN, MM).
```

The path can supply rule inputs.

Example address:

```text
omi-<frame>/128/hardware/i2c:0x28/euler-imo
```

Derived inputs:

```text
depth    = 128
domain   = hardware
selector = i2c:0x28
overlay  = euler
```

Clause interpretation:

```prolog
rule(interface_constraints, i2c_0x28).
fact(returned_bytes, NN).
closure(timing_register_frame_window, i2c_0x28, NN).
combinator(decode_transform, NN, euler_state).
cons(i2c_0x28, NN).
join(device_0x28, world).
compose(previous_state, euler_state).
encode(euler_state, projection).
decode(carrier, NN).
receipt(i2c_0x28, NN, euler_state).
```

Therefore the address does not merely identify data.

It describes the route by which data becomes eligible for receipt.

---

## 9. Native OMI Protocol Model

To make OMI more native, define four protocol components:

```text
1. Address grammar
2. Path derivation rules
3. Clause resolution engine
4. Receipt format
```

### 9.1 Address Grammar

The address grammar defines valid OMI surfaces.

```text
omi---imo
omi-<frame>
imo-<frame>
omi-<frame>-imo
omi-<frame>/<path...>-imo
```

### 9.2 Path Derivation Rules

Path derivation rules define how slash segments become clause selectors.

Example:

```text
/depth/domain/selector/overlay
```

maps to:

```text
resolution depth
domain router
selector route
overlay or modality
```

### 9.3 Clause Resolution Engine

The clause engine maps path selectors to Horn clauses.

Example:

```text
/hardware/i2c:0x28/euler
```

maps to:

```prolog
domain(hardware).
interface(i2c).
device(0x28).
selector(euler).
```

### 9.4 Receipt Format

The receipt records:

```text
original address
normalized address
frame
path
resolved clauses
input facts
output state
projection
acceptance
timestamp
signature or hash when available
```

---

## 10. Native Packet Shape

A native OMI packet may use this envelope:

```json
{
  "version": "omi-native-v0",
  "address": "omi-<frame>/<path...>-imo",
  "normalized": {
    "open": "omi",
    "frame": "<frame>",
    "path": ["<segment>", "<segment>"],
    "close": "imo"
  },
  "path_type": "hardware|semantic|overlay|prefix|quadratic_fexpr|control_separator|mixed",
  "clauses": [],
  "payload": {},
  "receipt": {}
}
```

For MCRSGSP:

```json
{
  "version": "omi-mcrsgsp-v0",
  "address": "omi-<frame>/<depth>/<domain>/<selector>/<overlay>-imo",
  "fragment": {
    "object_id": "<frame>",
    "fragment_id": 0,
    "k": 3,
    "n": 5,
    "bytes": "base64url..."
  },
  "causality": {
    "clock": {}
  },
  "receipt": {
    "accepted": false,
    "reason": "candidate"
  }
}
```

---

## 11. Binary Compatibility

The path protocol must not break the 128-bit frame.

The frame remains:

```text
8 × 16-bit segments
S0-S1-S2-S3-S4-S5-S6-S7
```

Path derivation is outside the frame.

That means:

```text
omi-<frame>
```

remains valid forever.

The path only adds interpretation.

Legacy or low-power nodes may ignore the path and still route by frame.

Higher-level nodes may interpret the path and derive clauses.

Rule:

```text
Frame-compatible nodes route.
Path-aware nodes derive.
Receipt-aware nodes accept.
```

---

## 12. ESP32, Browser, QEMU, and x86_64 Profiles

### 12.1 ESP32 Profile

ESP32 nodes may support only:

```text
omi-<frame>
omi-<frame>/hardware/<selector>-imo
```

ESP32 role:

```text
read digital boundary
emit frame
attach minimal path
export compact receipt candidate
```

### 12.2 Browser Profile

Browser clients may support:

```text
omi-<frame>/browser/<target>/<overlay>-imo
omi-<frame>/semantic/<subject>/<predicate>/<object>/<modality>-imo
```

Browser role:

```text
render projection
edit testimony
import/export receipts
never become authority by display alone
```

### 12.3 QEMU / x86_64 Profile

Native OMI nodes may support full derivation:

```text
prefix
semantic
hardware
overlay
quadratic_fexpr
control separators
MCRSGSP fragments
eBPF validation
```

Native node role:

```text
parse
normalize
derive
validate
gossip
repair
receipt
```

---

## 13. Alias Conformance

All surface aliases must normalize to the same internal structure.

Alias examples:

```text
omi---imo
omi-<frame>
imo-<frame>
omi-<frame>-imo
omi-<frame>/<path>-imo
omi<frame><frame><frame>imo
```

Normalization target:

```json
{
  "open": "omi",
  "frames": [],
  "path": [],
  "close": "imo"
}
```

For concatenated frame notation:

```text
omi<frame><frame><frame>imo
```

normalize to:

```json
{
  "open": "omi",
  "frames": ["<frame>", "<frame>", "<frame>"],
  "path": [],
  "close": "imo"
}
```

Meaning:

```text
A composed multi-frame surface.
```

Rule:

```text
Aliases may vary.
Normalization must not.
```

---

## 14. JSON Schema

A machine-readable schema is stored at:

```text
schemas/omi-address-derivation-v0.schema.json
```

The schema validates the normalized derivation object, not every possible source alias string.

---

## 15. Prolog Interpretation Schema

The path should be convertible into Prolog facts.

Example:

```text
omi-<frame>/128/hardware/i2c:0x28/euler-imo
```

Prolog facts:

```prolog
omi_frame(frame_id).
omi_depth(frame_id, 128).
omi_domain(frame_id, hardware).
omi_selector(frame_id, 'i2c:0x28').
omi_overlay(frame_id, euler).
omi_closed(frame_id, imo).
```

Then:

```prolog
accepted_surface(Frame, Surface) :-
    omi_frame(Frame),
    derive_path(Frame, Path),
    resolve_clauses(Path, Clauses),
    validate_clauses(Clauses),
    receipt(Frame, Surface).
```

This lets the address path become a native logic route.

---

## 16. Making It More Native

To make OMI more native, do not make paths decorative.

Make paths executable as derivation clauses.

A native OMI node should be able to do:

```text
parse address
normalize address
classify path
derive clauses
validate clause route
produce receipt
gossip receipt or fragment
```

That is the difference between:

```text
OMI as notation
```

and:

```text
OMI as protocol
```

The protocol begins when an address can be executed as a derivation route.

---

## 17. Final Canon

The complete native derivation form is:

```text
omi-<frame>/<depth>/<domain>/<selector>/<overlay>-imo
```

Aliases are allowed:

```text
omi---imo
omi-<frame>
imo-<frame>
omi-<frame>-imo
omi-<frame>/<depth>-imo
omi-<frame>/<overlay>/<frame>-imo
omi-<frame>/<LL>/<NN>/<MM>/<OO>-imo
omi-<frame>/<Subject>/<Predicate>/<Object>/<Modality>-imo
omi-<frame>/<60x2>/<16xy>/<4y2>/<fexpr:base64url>-imo
omi-<frame>/<FS>/<GS>/<US>/<RS>-imo
```

All aliases normalize into:

```text
open + frame + derivation path + closure
```

The final protocol doctrine is:

```text
Address is identity.
Path is derivation.
Derivation is clause routing.
Clause routing validates traversal.
Receipt accepts memory.
```

One-line canon:

```text
OMI Address Derivation treats `omi-<frame>` as the bounded identity object and every slash path after it as a Horn-clause derivation route into depth, domain, selector, overlay, semantic mnemonic, interface rule, or f-expression closure, so `omi---imo`, `omi-<frame>`, `imo-<frame>`, `omi-<frame>-imo`, and `omi-<frame>/<path>-imo` become alias-conformant surfaces of one native OMI protocol.
```
