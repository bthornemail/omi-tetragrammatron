# OMI Addressing and the Factorial Tower

## A Whitepaper on the 8!–12! Interpretation of the 128-Bit OMI Wire Frame

**Status:** Conceptual Whitepaper / v1.0.0-RC1 Alignment Draft
**Scope:** Addressing ontology, factorial envelopes, rule/fact derivation, semantic routing, and clock/provider phase
**Core Claim:** OMI does not need more than 128 physical wire bits to express higher-order structure. The 8-segment address frame is the physical surface; 9! through 12! are interpretive envelopes derived around that frame.

---

## Abstract

The Omicron Object Model, or OMI, is a protocol architecture for turning addresses into executable, replayable, visible ontology.

Its core object is the OMI pointer:

```text
omi-S0-S1-S2-S3-S4-S5-S6-S7/prefix
```

This pointer is physically an eight-segment, 128-bit frame. Each segment is a 16-bit word. At the wire level, this is the complete carrier.

The apparent question is: if the physical address frame has only eight segments, how can higher factorial structures such as 9!, 10!, 11!, and 12! participate in the addressing schema?

The answer is that 8! describes the physical permutation universe of the eight address segments, while 9!–12! describe higher interpretive envelopes wrapped around that same 128-bit frame:

```text
8!  → physical address-frame permutation universe
9!  → route/gate envelope
10! → declaration envelope
11! → semantic/proof witness envelope
12! → clock/provider phase envelope
```

Thus, OMI does not expand the wire frame beyond 128 bits. It expands the interpretation of the frame through deterministic derived layers.

The result is a compact addressing system where a single pointer can bind:

```text
address
rule
fact
route
test
replay receipt
semantic witness
clock phase
visual projection
failure surface
```

This paper explains the position fully.

---

# 1. The OMI Problem

Most systems separate:

```text
addressing
routing
typing
validation
execution
logging
visualization
semantic meaning
clocking
```

OMI collapses these into a single address-centered model.

The central OMI principle is:

```text
Every meaningful state should have an address.
Every address should have a rule.
Every rule should have a test.
Every test should be replayable.
Every replayable state should be visible.
```

This gives the provider contract:

```text
No address without a rule.
No rule without a test.
No test without a replay path.
No replay path without visible state.
```

OMI addresses are not passive labels. They are executable reference pointers.

---

# 2. The Physical Address Form

The physical OMI frame is:

```text
omi-S0-S1-S2-S3-S4-S5-S6-S7/prefix
```

Each segment is 16 bits:

```text
S0 = 16 bits
S1 = 16 bits
S2 = 16 bits
S3 = 16 bits
S4 = 16 bits
S5 = 16 bits
S6 = 16 bits
S7 = 16 bits
```

Together:

```text
8 × 16 = 128 bits
```

The prefix defines scope:

```text
/48   local canonical runtime frame
/80   rule-family or ABI boundary
/96   gateway, mapped-prefix, or transport boundary
/112  narrow rule/register boundary
/128  exact state pointer
```

The frame is stable:

```text
OMI wire frame = 8 physical segments = 128 bits
```

Nothing in the 9!–12! model requires adding physical segments. Those layers are derived interpretive envelopes.

---

# 3. The Canonical Instruction Shape

The canonical binary quadratic lexer uses this shape:

```text
S0 = 0xLL00    low byte 0x00, high byte = LL
S1 = 0x03BF    chiral delimiter ο
S2 = 0xNNNN    free payload variable N
S3 = 0x2bLL    high byte 0x2B, low byte = LL
S4 = 0x2fLL    high byte 0x2F, low byte = LL
S5 = 0xMMMM    free payload variable M
S6 = 0x039F    cardinal delimiter Ο
S7 = 0xLLff    high byte = LL, low byte 0xFF
```

Canonical address form:

```text
omi-LL00-03bf-NNNN-2bLL-2fLL-MMMM-039f-LLff/48
```

Example:

```text
omi-0500-03bf-000c-2b05-2f05-0002-039f-05ff/48
```

Here:

```text
LL = 0x05
N  = 0x000c
M  = 0x0002
```

The repeated `LL` field binds the instruction to a coherent selector rail.

---

# 4. The Deep Law: Lexing Is Algebraic Membership

OMI does not treat lexing as a string parser.

OMI treats lexing as algebraic membership.

The frame is valid iff:

```text
Q(S) = 0
```

The frame is invalid iff:

```text
Q(S) > 0
```

The error function is:

```text
Q(S) = E_var + E_const
```

`E_var` checks repeated variable coherence:

```text
L0 = S0 >> 8
L3 = S3 & 0x00FF
L4 = S4 & 0x00FF
L7 = S7 >> 8

E_var = (L0 - L3)^2 + (L3 - L4)^2 + (L4 - L7)^2
```

`E_const` checks fixed delimiter alignment:

```text
E_const =
    (S0 & 0x00FF)^2 +
    (S1 - 0x03BF)^2 +
    ((S3 & 0xFF00) - 0x2B00)^2 +
    ((S4 & 0xFF00) - 0x2F00)^2 +
    (S6 - 0x039F)^2 +
    ((S7 & 0x00FF) - 0x00FF)^2
```

Because every term is squared, no mismatch can cancel another mismatch.

Therefore:

```text
Q(S) = 0 iff every structural condition holds
```

This is the Binary Quadratic Meta-Mask Lexer.

---

# 5. The Meaning of 8!

The OMI frame has eight physical segments.

Therefore:

```text
8! = 40,320
```

means:

```text
all possible segment-order permutations of the 8-part OMI frame
```

In OMI, 8! is the **physical addressing universe**.

It does not mean that the frame stores 40,320 states directly as raw bits. Rather, it means the frame has eight structural positions, and those eight positions can be permuted, punctured, folded, routed, or interpreted in 40,320 possible orders.

So:

```text
8! = the permutation universe of S0..S7
```

This is the outer local frame.

---

# 6. The 8! → 7! Puncture

To pass from 8! to 7!, OMI selects one of the eight address rails as the active carrier, boundary, omitted rail, or control rail.

Mathematically:

```text
8! / 7! = 8
```

This means:

```text
choose 1 of 8 segment positions
then permute the remaining 7
```

The remaining seven active rails form:

```text
7! = 5,040
```

This is OMI’s factorial replay universe.

In implementation language:

```text
8! = physical frame
7! = active Fano/replay state after one rail is punctured
```

That gives the 5040-slot replay ring:

```text
5040 = 7!
```

A single 128-bit OMI frame can therefore be interpreted as:

```text
one selected carrier rail
+ seven active permutation rails
```

This is why 7! is the natural runtime ring, even though the physical address has eight segments.

---

# 7. The Inner Factorial Collapse

After 7!, OMI descends through the already established factorial cascade.

```text
7! = 5040
6! = 720
5! = 120
4! = 24
3! = 6
2! = 2
1! = 1
```

Each layer has an OMI role:

| Factorial |  Value | OMI role                                   |
| --------: | -----: | ------------------------------------------ |
|      `7!` | `5040` | Fano replay universe                       |
|      `6!` |  `720` | promote sweep / target map                 |
|      `5!` |  `120` | disjoint packet set / 5-cell core          |
|      `4!` |   `24` | 24-cell / hex selector / face lattice      |
|      `3!` |    `6` | Fano-line permutations / tetrahedral edges |
|      `2!` |    `2` | binary switch / complement / reflection    |
|      `1!` |    `1` | identity anchor                            |

This gives OMI a strict non-leaking collapse:

```text
global permutation state
→ target state
→ packet state
→ face state
→ line state
→ binary state
→ identity state
```

---

# 8. The Prime Exponent Signature

The factorial tower can also be read through prime factorization.

For example:

```text
8! = 40,320 = 2^7 × 3^2 × 5^1 × 7^1
```

This gives the exponent vector:

```text
[7, 2, 1, 1]
```

The sum is:

```text
7 + 2 + 1 + 1 = 11
```

OMI uses this as the bridge into the 11-bit significand precision of binary16.

This gives a hardware-aligned reading:

```text
8! gives an 11-dimensional prime exponent signature
binary16 gives an 11-bit significand boundary
```

That does not mean the whole factorial space fits literally inside a half-float. It means the prime-exponent structure of the factorial universe maps cleanly onto an 11-bit local control surface.

This is why binary16 is useful as a compact “local Omicron header” model.

---

# 9. The Binary16 Local Header

IEEE 754 binary16 has:

```text
1 sign bit
5 exponent bits
10 stored significand bits
11 significand precision bits including the hidden bit
```

OMI interprets this as:

```text
sign bit        → Omicron/CAR switch
5 exponent bits → Tetragrammaton/CDR-car scale window
11 precision    → Metatron/CDR-cdr payload rail
```

In OMI language:

```text
binary16 = local state header
```

This aligns with the earlier cons-cell interpretation:

```text
(car . cdr)
```

where:

```text
sign      = car
exponent  = cdr-car
mantissa  = cdr-cdr
```

The point is not to force all OMI data into a float. The point is to use the bit layout as a hardware-native structural metaphor for local state.

---

# 10. Why 9! Does Not Add a Ninth Wire Segment

The OMI wire frame remains eight segments.

So what is 9!?

```text
9! = 362,880
```

Since:

```text
9! / 8! = 9
```

9! means:

```text
the 8-segment frame plus one higher-order selector
```

This ninth selector is best understood as the **route/gate envelope**.

It answers:

```text
Which route owns this frame?
```

Possible 9-way route classes:

```text
0 kernel/eBPF
1 WAN telemetry
2 NAT64/page frame
3 semantic/WordNet
4 emoji/POS graph
5 QEMU clock
6 canvas/CSSOM
7 replay ring
8 provider/service bus
```

These route classes are not extra physical segments. They are derived from the existing frame.

Example derivation:

```text
route9 = Δ_C(S2) mod 9
```

or:

```text
route9 = hash(S0..S7) mod 9
```

Thus:

```text
9! = routed OMI address universe
```

---

# 11. Why 10! Is the Declaration Envelope

```text
10! = 3,628,800
10! / 9! = 10
```

The tenth layer should represent the **declaration role**.

OMI has a document and rule stack:

```text
POSTULATE
AXIOM
RULE
FACT
DECLARATION
TEST
REPLAY
PROJECTION
FAILURE
COMPLEMENT
```

That is a natural 10-role declaration space.

So:

```text
10! = address + route + declaration role
```

It answers:

```text
What kind of declared object is this pointer currently acting as?
```

The same pointer may appear as:

```text
RULES.omi row
FACTS.omi row
DECLARATIONS.md derivation expression
AXIOMS.md fold law
POSTULATES.md permitted construction
test target
replay receipt
CSSOM projection
failure surface
complement surface
```

Therefore 10! is the declaration envelope.

---

# 12. Why 11! Is the Semantic / Proof Witness Envelope

```text
11! = 39,916,800
11! / 10! = 11
```

The eleventh layer should represent the semantic/proof witness.

A useful 11-role witness layer is:

```text
0 subject
1 predicate
2 object
3 source rule
4 derived fact
5 test result
6 replay receipt
7 visual projection
8 failure surface
9 complement surface
10 provenance/witness
```

This layer connects the OMI pointer to meaning.

It is where S-P-O triples, WordNet synsets, Unicode emoji semantic routes, and proof receipts live.

So:

```text
11! = address + route + declaration + semantic/proof witness
```

It answers:

```text
What meaning or proof witness binds this declared state?
```

This is also why the earlier binary16 exponent-signature bridge matters: 11 is already the local significand precision boundary of the 8! prime-exponent signature.

Thus, 11! is the higher semantic envelope, while 11-bit binary16 is the local compact witness rail.

---

# 13. Why 12! Is the Clock / Provider Envelope

```text
12! = 479,001,600
12! / 11! = 12
```

The twelfth layer should represent the clock/provider phase.

OMI already uses the twelve regular divisors of 60:

```text
1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60
```

These divide one hour into clean base-60 slices:

```text
60, 30, 20, 15, 12, 10, 6, 5, 4, 3, 2, 1 minutes
```

Thus 12! is the full **clocked provider envelope**.

It answers:

```text
When and by which provider phase is this state emitted?
```

It binds:

```text
Tetragrammaton scheduler
QEMU TYPE_CLOCK model
WAN telemetry cadence
provider runtime phase
base-60 clock slot
visual update cadence
```

So:

```text
12! = address + route + declaration + proof + clock/provider phase
```

---

# 14. The Complete 8!–12! Addressing Ladder

The full ladder is:

| Layer |         Value | Ratio | Meaning                           |
| ----: | ------------: | ----: | --------------------------------- |
|  `8!` |      `40,320` |     — | physical 8-segment frame universe |
|  `9!` |     `362,880` |  `×9` | route/gate envelope               |
| `10!` |   `3,628,800` | `×10` | declaration role envelope         |
| `11!` |  `39,916,800` | `×11` | semantic/proof witness envelope   |
| `12!` | `479,001,600` | `×12` | clock/provider phase envelope     |

In plain language:

```text
8!  = What are the eight address fields doing?
9!  = Which route owns the frame?
10! = Which declaration role is active?
11! = Which proof or semantic witness binds it?
12! = Which provider/clock phase emits it?
```

This is the clean position.

---

# 15. The Full Derived OMI State

The physical pointer is:

```text
omi-S0-S1-S2-S3-S4-S5-S6-S7/prefix
```

The full interpreted state is:

```text
OMI_STATE =
  pointer128
+ route9
+ declaration10
+ witness11
+ phase12
```

Where:

```text
pointer128    = physical 8-segment address
route9        = derived route/gate class
declaration10 = derived declaration role
witness11     = derived semantic/proof witness
phase12       = derived clock/provider phase
```

Example derivation:

```text
route9        = Δ_C(S2) mod 9
declaration10 = Δ_C(S3) mod 10
witness11     = Δ_C(S4) mod 11
phase12       = Δ_C(S5) mod 12
```

This preserves the 128-bit wire frame while giving the system a much richer interpretive envelope.

---

# 16. Why This Is Not “Too Much Meaning” for 128 Bits

The frame is not expected to store all possible 12! states as raw enumerated values.

Instead, the frame acts like a seed.

The higher layers are deterministic interpretations.

This is similar to how:

```text
an IP address can imply route table behavior
a file path can imply permissions
a URL can imply protocol handling
a hash can imply content identity
a public key can imply verification behavior
```

OMI does the same thing, but explicitly:

```text
pointer
→ route
→ rule
→ fact
→ test
→ replay
→ projection
→ clock phase
```

The state space is large because the possible interpretations are large, not because every interpretation is stored as a separate wire field.

---

# 17. Relation to POSTULATES.md

`POSTULATES.md` defines what OMI permits us to construct.

In this factorial model:

```text
8! permits construction of address points.
9! permits construction of route lines.
10! permits construction of declaration roles.
11! permits construction of proof/semantic witnesses.
12! permits construction of provider clock phases.
```

So the factorial ladder becomes a construction ladder:

```text
address point
→ route path
→ declaration surface
→ witness/proof surface
→ clock/provider envelope
```

---

# 18. Relation to AXIOMS.md

`AXIOMS.md` defines how OMI constructions fold.

The important fold is ones-complement:

```text
fold16(x) = x XOR 0xFFFF
```

For a full frame:

```text
fold128(S) = S XOR ffff-ffff-ffff-ffff-ffff-ffff-ffff-ffff
```

This creates the complement surface:

```text
state      → anti-state
mask       → anti-mask
rule       → counter-rule
valid rail → eviction rail
presence   → absence
```

In factorial terms:

```text
2! = the binary fold switch
```

The 2! layer is buried inside the 8! → 1! collapse, but it also appears at every higher envelope as the ability to define a complement or failure surface.

Thus, every 8!–12! interpretation must be able to answer:

```text
What is its complement?
What is its failure surface?
What happens when it does not validate?
```

---

# 19. Relation to DECLARATIONS.md

`DECLARATIONS.md` explains how rules become facts.

In the factorial addressing model:

```text
10! = declaration role envelope
```

A rule-to-fact declaration is:

```text
RULES.omi clause
→ axiom family
→ a-list state transition
→ bitboard mask
→ bitblip policy
→ FACTS.omi declaration
→ test
→ replay path
→ visible projection
```

The a-list is the concrete expression of the 10! declaration layer.

Example:

```lisp
(
  (rule . "omi-.../112 MUST initialize-xdp-packet-parsing-gates")
  (state-before . "unverified")
  (transition . "compile-and-load-xdp-gate")
  (state-after . "configured")
  (fact . "omi-.../112 FACT xdp-packet-parsing-gate-configured")
  (test . "test/ebpf-pipeline.test.js")
  (projection . "bpf-map + cssom")
)
```

This is how the abstract factorial envelope becomes an inspectable provider declaration.

---

# 20. Relation to Unicode Emoji Semantic Routing

The active Unicode 17.0 emoji work fits naturally in the 11! witness layer.

Emoji code points and sequences are not just decorations. In OMI, they become non-alphanumeric semantic carriers.

The route is:

```text
emoji sequence
→ exact Unicode validation
→ CLDR group/subgroup
→ WordNet proxy lattice
→ S-P-O triple
→ polyhedral route
→ OMI frame
```

This means:

```text
emoji/POS route = 9! route envelope
semantic S-P-O binding = 11! witness envelope
polyhedral projection = visible state
```

The current mapping can be summarized as:

```text
open/closed-class + inflection
→ icosahedron/dodecahedron
→ cyan preset

other + lexical
→ stellated octahedron
→ purple preset

other + other
→ point-line-plane pointer
→ green preset
```

This connects emoji to the same address ontology rather than treating emoji as external text.

---

# 21. Relation to eBPF/XDP

The eBPF/XDP layer fits in the 9! route envelope.

The packet gate is one possible route owner:

```text
route9 = kernel/eBPF
```

At that layer:

```text
raw packet
→ XDP hook
→ OMI frame or signature
→ pass/drop result
→ BPF telemetry map
→ user-space mirror
```

The 128-bit frame remains the same, but the route envelope says:

```text
This pointer is being interpreted at the kernel packet boundary.
```

Failure behavior may be:

```text
XDP_DROP
```

Projection may be:

```text
BPF map counter
CSSOM telemetry rail
WAN SSE event
```

This is how packet validation fits into the same factorial address model.

---

# 22. Relation to QEMU TYPE_CLOCK

The QEMU clock model fits in the 12! provider phase envelope.

A QEMU clock state has:

```text
integer period
2^-32 ns unit
period 0 = gated/inactive
```

OMI treats this as addressable state:

```text
clock pointer
→ period
→ gated/non-gated condition
→ provider phase
→ projection
```

The base-60 scheduler also fits here because 12! maps naturally to the twelve regular divisors of 60.

Thus:

```text
QEMU TYPE_CLOCK = clock/provider phase object
```

inside the 12! envelope.

---

# 23. Relation to Browser DOM/CSSOM

The visual layer is the final projection path.

An OMI pointer can appear as a DOM id:

```html
<section id="omi-0000-0000-0000-0000-0000-0000-00eb-0066/112"></section>
```

CSS can target it:

```css
[id$="-00eb-0066/112"] {
  stroke: #ffaa00;
}
```

This means:

```text
address → visible state
```

The browser becomes a projection surface for protocol state.

This is why the earlier `omi---imo` runtime matters: it established the idea that OMI state can be reflected into DOM attributes, CSS transforms, color channels, and 2.5D geometry.

---

# 24. The Addressing Schema in One Diagram

```text
                         12! Clock / Provider Envelope
                                      │
                         11! Proof / Semantic Witness
                                      │
                         10! Declaration Role Envelope
                                      │
                         9! Route / Gate Envelope
                                      │
              ┌──────────────────── 8! ────────────────────┐
              │                                             │
              │       128-bit OMI Physical Wire Frame        │
              │                                             │
              │  S0 S1 S2 S3 S4 S5 S6 S7                    │
              │  16 16 16 16 16 16 16 16 = 128 bits         │
              │                                             │
              └─────────────────────────────────────────────┘
                                      │
                         7! Replay / Fano Ring
                                      │
                         6! Target / Promote Sweep
                                      │
                         5! 5-Cell / Packet Core
                                      │
                         4! 24-Cell / Face Selector
                                      │
                         3! Line / Tetrahedral Edge
                                      │
                         2! Complement / Binary Switch
                                      │
                         1! Identity Anchor
```

This diagram is the full OMI factorial position.

---

# 25. Implementation Rule

Do not store 9!, 10!, 11!, and 12! as extra wire segments.

Derive them.

Recommended rule:

```text
physical frame is fixed
interpretive envelopes are derived
```

Example:

```js
function deriveEnvelope(S) {
  return {
    route9: deltaC(S[2]) % 9,
    declaration10: deltaC(S[3]) % 10,
    witness11: deltaC(S[4]) % 11,
    phase12: deltaC(S[5]) % 12
  };
}
```

This gives deterministic interpretation without changing the wire format.

---

# 26. Why This Position Is Strong

This model is strong because it separates:

```text
physical carrier
logical interpretation
semantic witness
clocked emission
```

The 128-bit frame remains simple enough to validate at kernel speed.

The higher envelopes remain rich enough to express:

```text
rules
facts
proofs
semantic triples
emoji routes
clock states
DOM projections
provider phases
```

This is the correct balance.

If 9!–12! were treated as extra physical fields, OMI would lose the elegance of the 128-bit frame.

If they are treated as derived envelopes, OMI keeps both:

```text
fast wire validation
deep semantic interpretation
```

---

# 27. Final Position

OMI’s addressing schema should be understood as two towers around the same frame.

The downward tower:

```text
8! → 7! → 6! → 5! → 4! → 3! → 2! → 1!
```

collapses the physical address frame into replay, packet, line, complement, and identity state.

The upward tower:

```text
8! → 9! → 10! → 11! → 12!
```

wraps the physical frame in route, declaration, proof, semantic, and clock/provider envelopes.

Therefore:

```text
8! is the physical OMI address universe.
7!–1! are inner collapse layers.
9!–12! are outer interpretive envelopes.
```

The complete OMI state is:

```text
complete OMI state =
  128-bit pointer
+ factorial collapse path
+ route envelope
+ declaration envelope
+ semantic/proof witness
+ clock/provider phase
+ complement surface
+ replay path
+ visible projection
```

That is our current position.

---

# 28. One-Sentence Summary

OMI uses an eight-segment, 128-bit wire frame as the physical 8! address universe; it collapses inward through 7!–1! for replay, packet, face, line, complement, and identity, while expanding outward through 9!–12! for route, declaration, semantic proof, and clocked provider state.

