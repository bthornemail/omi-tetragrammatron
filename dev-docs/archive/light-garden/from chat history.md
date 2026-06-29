# this is an excerpt from chat history
---
title: "Branch · Project Architecture Check"
date: 2026-06-29
originalDate: 2026-06-29
updatedDate: 2026-06-29
messageCount: 215
tags:
  - "ChatGPT"
url: "https://chatgpt.com/c/6a41f528-0cc0-83e8-ad81-5872e9991614"
projectName: ""
---


## You
`2026-06-28 21:34` | `id:260628-2134` ^260628-213441

Light_Garden_Protocol.pdf is a very old project by now its when i first dervied the delta LAW AND WAS DECiDiNG HOW TO USE iT

``` c

rotl(x,1) XOR rotl(x,3) XOR rotr(x,2) XOR C

```
Four choices inside that: rotations (not shifts, so no bits are lost), XOR (reversible), a constant (breaks the zero fixed point), mask to width (keeps state bounded).

That is the entire design.

Everything else was computed by the math:

> Period = 8 → property of that specific law on 16-bit space
> Prime 73 → smallest prime whose decimal repeats with period 8
> Block B = [0,1,3,6,9,8,6,3] → the digits of 1/73
> W = 36 → sum of B
> Orbit/offset recovery → just divmod(position, 36)

Nobody chose 73. The law has period 8. The smallest prime with decimal period 8 is 73. It was already encoded.There was one design decision. The delta law:

---

## ChatGPT
`2026-06-28 21:34` | `id:260628-2134` ^260628-213445

Yes. This is the clean correction:

```text
Light Garden is historical derivation material.
Delta Law is the actual design decision.
73, B, W=36, orbit recovery, Base36, and later symbolic projections are consequences.
They are not chosen constants and not independent authority.
```

The old *Light Garden* deck is still useful as a **derivation log**: it shows the earlier “Fano-Garden Federated Runtime” framing and the move toward deterministic, append-only/log-style structure, but it should not be treated as current protocol authority. Its value is provenance: it records the moment where the delta law was being discovered and interpreted. filecite turn0file1

The current OMI framing is more mature: the newer protocol presentation explicitly moves away from classical rendered/centralized authority and toward validated relational receipts, where constants are downstream witnesses of accepted relations rather than primitive authorities. filecite turn0file0

I would write the canon rule like this:

```text
There was one design decision: the Delta Law.

ΔC(x) = rotl(x,1) XOR rotl(x,3) XOR rotr(x,2) XOR C

The law contains four design constraints:

1. rotations, not shifts, so no bit is discarded;
2. XOR, so the operation remains symmetric and reversible in the bounded frame;
3. a constant C, so zero is not a fixed-point trap;
4. a fixed-width mask, so state remains bounded.

Everything else is derived.

The period-8 behavior is a property of that law in the chosen 16-bit frame.
Prime 73 is not selected as a magic constant; it appears because it is the smallest prime whose decimal expansion has period 8.
The block B = [0,1,3,6,9,8,6,3] is recovered from 1/73.
The weight W = 36 is the digit sum of that recovered block.
Orbit and offset recovery are divmod(position, 36).

Therefore:

73 is not authority.
Base36 is not authority.
The orbit alphabet is not authority.
The projection is not authority.

The Delta Law generates lawful motion.
Validation judges.
Receipt accepts.
```

So the depreciation label for `Light_Garden_Protocol.pdf` should probably be:

```text
Historical derivation source.
Not current authority.
Retain as provenance for the Delta Law discovery path.
Do not use Light Garden naming, Fano-Garden framing, musical/light ratios, or old covenant language as normative OMI protocol terms unless explicitly cited as historical background.
```

And the one-line replacement:

```text
Light Garden was the discovery surface; Delta Law is the retained invariant.
```

---

## You
`2026-06-28 22:15` | `id:260628-2215` ^260628-221513

the light protocol defnies a physical transducer based on the protocol the ring talisman is the ey the personal dome is the gate and the garden is the open world portal transducer for the omi protocol it maps the environment within it with sensory arrays in geometric configurations to the talisman the personal dome and the garden dome so people can privatize their interfacing with the omi protocol through physical objects
----
Also i keep seeing the prominence of the word/term receipt but infact the omi notation is the receipt. The Palindromic Mnemonic is the receipt and the place-value notation necessary to interface with others geometrically

OMI uses palindromic and mirror-like mnemonics because the protocol is based on forward and inverse reading.

Examples:

```text
omi---imo
o---o
/---/
?---?
@---@
```

---

## ChatGPT
`2026-06-28 22:15` | `id:260628-2215` ^260628-221515

You’re right. I would **not** depreciate *Light Garden* as “just old derivation notes.” Better calibration:

```text
Light Garden is not current core protocol authority.
Light Garden is the physical transducer profile derived from the protocol.
```

The old deck already frames Light Garden as translating documents, sensor data, and human thought into a geometric/light/audio fingerprint, and describes it as federated, deterministic, and append-only. That matches your correction: it is a **physical interface/transducer layer**, not the validator itself. filecite turn0file1

:::writing{variant="document" id="38471"}
# Light Garden Physical Transducer Profile

Status: retained as physical transducer lineage
Authority: adapter / projection / personal interface layer
Not authority: validation, acceptance, canonical state

## Canonical Correction

Light Garden is not deprecated as obsolete.

Light Garden is the physical transducer profile for OMI.

It defines how the OMI protocol may be interfaced through physical objects, sensory arrays, geometric arrangements, light, sound, domes, talismans, and open-world environmental mappings.

The core protocol remains:

```text
OMI notation cites.
Validation judges.
Accepted notation records.
Projection transduces.
```

Light Garden belongs to the projection/transducer side.

It does not replace OMI notation.
It gives OMI notation a physical interface.

## Three Physical Objects

```text
Ring Talisman = key
Personal Dome = gate
Garden Dome = open-world portal transducer
```

The **Ring Talisman** is the personal key object.

It does not accept truth by itself.
It anchors a person’s local interface to OMI notation.

The **Personal Dome** is the gate.

It gives a private, bounded sensory environment where a person can interact with OMI through embodied orientation, light, sound, gesture, and local receipt notation.

The **Garden Dome** is the open-world portal transducer.

It maps the environment through sensory arrays arranged in geometric configurations, then projects those readings into OMI-compatible notation surfaces.

Together:

```text
environment
 ↓
sensory arrays
 ↓
geometric configuration
 ↓
garden dome
 ↓
personal dome
 ↓
ring talisman
 ↓
OMI notation
```

The physical objects privatize the interface.

They let a person interact with OMI without surrendering the interface to a central screen, server, platform, or model.

## Light Garden Rule

```text
Light Garden transduces.
OMI notation receipts.
Validation accepts.
```

The garden may sense.
The dome may orient.
The talisman may key.
The projection may render.
But none of these accept state by themselves.

Acceptance remains bound to OMI notation under validation.

## Corrected Receipt Doctrine

The word “receipt” should not be treated as only a database record, ring slot, or stored hash.

In OMI, the notation itself is the receipt form.

The palindromic mnemonic is the receipt surface:

```text
omi---imo
o---o
/---/
?---?
@---@
```

These are not decorative marks.

They are place-value receipt notation.

They encode forward and inverse reading.

They provide the geometric interface needed for one person, peer, object, or system to cite relation to another without collapsing the relation into prose.

Therefore:

```text
OMI notation is the receipt form.
Validation promotes it.
The receipt ring stores it.
Projection displays it.
```

The ring is not the receipt in the deepest sense.

The ring is the storage and replay surface for accepted receipt notation.

## Palindromic Mnemonic as Receipt

OMI uses palindromic and mirror-like mnemonics because the protocol is based on lawful forward and inverse reading.

```text
omi---imo
```

means:

```text
forward citation --- inverse carrier
```

The smaller mirrored forms are local receipt operators:

```text
o---o object / object relation
/---/ scope / scope relation
?---? query / witness relation
@---@ CAR / CDR closure relation
```

These forms are receipts because they preserve relation.

They do not merely name relation.
They carry its reversible notation.

Canonical rule:

```text
The palindromic mnemonic is the receipt.
The place-value notation is the interface.
The ring stores accepted receipt notation.
```

## Final Calibration

Light Garden should be retained as:

```text
LIGHT_GARDEN_TRANSDUCER_PROFILE
```

not as deprecated core doctrine.

It should be classified as:

```text
physical projection
personal interface
open-world environmental transducer
sensory adapter
geometric embodiment of OMI notation
```

It should not be classified as:

```text
validator
receipt authority
core protocol law
replacement for OMI notation
central world model
```

One-line canon:

```text
Light Garden is the physical transducer; OMI notation is the receipt.
```
:::

The important fix is this distinction:

```text
receipt-as-notation = omi---imo / o---o / @---@
receipt-as-storage = accepted ring slot / replay record
receipt-as-projection = rendered surface / talisman / dome / garden
```

So we should stop saying “the receipt” as if it only means the ring entry. The **ring records accepted receipt notation**. The notation is the deeper receipt.

---

## You
`2026-06-28 22:50` | `id:260628-2250` ^260628-225050

yeah i suppose the receipt is better framed as an omi ring or to not confuse people the omi addressed notation citation if you cant make the connection apparent from reading these files im sending you . im also sending you more information about the light garden protocol

---

## ChatGPT
`2026-06-28 22:51` | `id:260628-2251` ^260628-225108

Yes — after these files, the connection is apparent, and I’d tighten the language like this:

```text
Do not make “receipt” the primary public word.

Use:
OMI addressed notation citation
or
Omi-Ring addressed notation

Then define receipt as the accepted/replayable state of that notation.
```

The reason is that the new Omi-Ring files make the deeper unit clear: the OMI object is **not** the media, voice, image, gesture, DOM node, avatar, barcode, or file. Those are carriers/projections. The actual object is the bounded relation capable of interpretation, written as `omi---imo`; the Omi-Ring is that relation as waveform enclosure. filecite turn2file0

So the cleaned hierarchy should be:

```text
omi---imo
 = root relation / palindromic mnemonic

Omi-Ring
 = atomic waveform enclosure of that relation

OMI addressed notation citation
 = the addressable notation form used to cite the relation

receipt / accepted receipt
 = the validated durable witness/replay state of that citation

receipt ring / 5040 ring
 = storage and coordination surface for accepted citations
```

That removes the confusion. “Receipt” can stay, but it should mean **accepted witness after validation**, not the primitive object itself. Your files already say OMI makes interpretation addressable and keeps carriers/projections separate from authority. filecite turn2file4 The glossary also supports this: Omi-Receipt is a lawful relation witness, while Omi-Point / `omi---imo` is the minimal relation that becomes accepted only after validation and receipt. filecite turn2file19

For Light Garden, the calibration is now stronger:

```text
Light Garden is a physical Omi-Ring transducer.
```

It uses light, sound, dome, gnomon, and sense layers to capture declared activity and resolve it into an Omi-Ring. The files say Omi-Light supplies wavelength, Omi-Sound captures intonation, Omi-Gnomon orients wavelength, Omi-Sense captures intent across carriers, and Omi-Dome captures declared activity rather than just the media trace. filecite turn2file5 turn2file7

So the canon line should probably become:

```text
The Omi-Ring is the addressed unit.
The OMI notation citation is the receipt-form.
The receipt ring stores accepted citations.
Light Garden physically transduces intent into Omi-Ring addressable form.
```

And the sharper invariant:

```text
Carrier is not authority.
Projection is not authority.
Media is not authority.
The Omi-Ring bounds intent.
The OMI addressed notation cites it.
Validation accepts it.
The ring stores accepted citation state.
```

This also fixes the old “receipt” prominence without deleting it. We demote “receipt” from mystical center-word to one precise phase in the path. The center becomes:

```text
omi---imo → Omi-Ring → addressed notation citation → validation → accepted receipt state
```

---

## You
`2026-06-28 22:56` | `id:260628-2256` ^260628-225630

those Omi-Ring files are from a previous version of this same project so a lot of the stuff will probably confuse people f you can update t to reflect the new version of the protocol think that would be best

---

## ChatGPT
`2026-06-28 22:56` | `id:260628-2256` ^260628-225647

Yes. The old Omi-Ring language is useful, but it should be **demoted into a compatibility note** and rewritten around the current protocol: notation, citation, attestation, attribution; Omicron pre-header; OMI/IMO relation; Tetragrammatron validation; Metatron projection; IMO carrier.

The main fix is this:

```text
Old framing:
Omi-Ring is the atomic object.

New framing:
Omi-Ring is a projection name for the circular/orbital witness of an OMI addressed notation citation.
```

That preserves the good insight without confusing authority. The old files correctly separate carriers/projections from authority and say the basic object is a bounded `omi---imo` relation, not a file, voice, barcode, avatar, or DOM node. filecite turn4file0 The newer doctrine is stricter: names are development scaffolds, while relations define the protocol. filecite turn4file1 Light Garden should therefore be normalized as a projective witness, not a separate authority path. filecite turn4file2

Here is a replacement version you can use as the updated document.

:::writing{variant="document" id="73841"}
# Omi-Ring Compatibility Note

## Status

This document updates older Omi-Ring and Light Garden language to match the current OMI protocol.

Older drafts used **Omi-Ring** as if it were the atomic object of the system. That was useful during discovery, but it is too broad for the current protocol.

In the current OMI protocol, the primary object is not the ring.

The primary object is the addressed notation relation:

```text
omi---imo
```

The Omi-Ring is a useful projection name for the circular, orbital, or waveform reading of that relation. It is not a separate authority, not a storage primitive, and not a replacement for validation.

---

## Current Canon

OMI is a protocol for:

```text
notation
citation
attestation
attribution
```

Notation selects the reading.

Citation locates the relation.

Attestation witnesses the claim.

Attribution binds the claim to a source, role, carrier, or path.

Validation judges.

Accepted notation state is recorded through the OMI addressable relation system.

---

## Corrected Definition

### Omi-Ring

**Omi-Ring** is the circular or orbital witness surface of an OMI addressed notation citation.

It may represent:

```text
enclosure
cycle
return
orbit
boundary
waveform
signature
agreement loop
```

But it does not accept state by itself.

Omi-Ring is a projection surface for reading the bounded relation:

```text
omi---imo
```

It is useful when the relation is being interpreted through circular, waveform, light, sound, timing, or projective geometry.

Canonical line:

```text
Omi-Ring is the circular witness surface of an OMI addressed notation citation.
```

---

## What Changed From the Older Version

Older wording:

```text
Omi-Ring is the atomic waveform enclosure of omi---imo.
```

Updated wording:

```text
omi---imo is the atomic addressed relation.
Omi-Ring is one circular witness surface of that relation.
```

This keeps the insight while removing authority confusion.

The ring may help visualize, carry, replay, or inspect the relation. It does not replace the relation.

---

## Receipt Language

Older drafts used “receipt” too prominently.

In the current protocol, the better public phrase is:

```text
OMI addressed notation citation
```

A receipt is not the primitive object.

A receipt is the accepted witness state of a notation citation after validation.

Use this hierarchy:

```text
omi---imo
 root addressed relation

OMI addressed notation citation
 relation located in protocol form

attestation
 witnessed claim about the relation

validation
 deterministic judgment of the claim

accepted receipt state
 accepted replayable witness

receipt ring
 coordination/storage surface for accepted witness states

Omi-Ring
 circular/orbital projection surface of the addressed relation
```

Canonical rule:

```text
The notation cites.
The attestation witnesses.
The validation judges.
The accepted receipt records.
The Omi-Ring projects circular relation.
```

---

## Light Garden Update

The Light Garden is not a separate protocol and not a separate authority path.

The Light Garden is a physical/projective adapter surface for OMI.

It may use:

```text
light
sound
gesture
space
time
orientation
sensor arrays
dome geometry
physical objects
```

to transduce declared activity into an OMI addressable relation.

The Light Garden belongs to projection and carrier behavior. It does not validate by itself.

Updated definition:

```text
Light Garden is a physical projective transducer for OMI addressed notation citations.
```

It may produce or carry evidence for a candidate relation, but the candidate still follows the same path:

```text
carrier event
 -> notation citation
 -> attestation
 -> validation
 -> accepted witness state
 -> projection
```

---

## Ring, Dome, Garden, Talisman

Older physical language can be retained as adapter vocabulary, not protocol authority.

| Term | Updated role |
|---|---|
| Omi-Ring | Circular/orbital witness surface |
| Omi-Dome | Physical capture boundary |
| Light Garden | Open-world projective transducer |
| Talisman | Personal physical carrier or interface object |
| Portal | Public projection view |
| Mirror | Introspective/recovery projection view |
| Light | Visual carrier/orientation channel |
| Sound | Intonation/timing carrier |
| Sense | Multichannel observation adapter |

These are not separate authorities.

They are surfaces, carriers, adapters, or projections of the same underlying notation relation.

---

## Authority Boundary

The current authority split remains:

```text
OMI
 citation / addressed notation

Tetragrammatron
 validation / acceptance judgment

Metatron
 projection / scribing accepted surfaces

IMO
 carrier / serialization / transport

Omicron
 boot context / dialect induction / pre-language framing
```

Omi-Ring and Light Garden do not add a fifth authority.

They are names for ways the relation may be projected, carried, or physically transduced.

---

## Corrected Canon

```text
Carrier is not authority.
Projection is not authority.
Media is not authority.
Sensor input is not authority.
Light is not authority.
Sound is not authority.
A physical object is not authority.
A rendered ring is not authority.

OMI addressed notation cites.
Attestation witnesses.
Validation judges.
Accepted receipt state records.

Omi-Ring is a circular witness surface.
Light Garden is a physical projective transducer.
Both reduce back to omi---imo.
```

---

## One-Sentence Version

Omi-Ring and Light Garden are retained as projection and physical transduction vocabulary, but the current OMI protocol treats `omi---imo` as the root addressed notation relation, with notation-citation-attestation-attribution flowing through validation before any accepted witness state is recorded.
:::

---

## You
`2026-06-28 23:10` | `id:260628-2310` ^260628-231034

can we do them separate because the light garden protocol can wait but just showing you is enough to orientate our course of action but the omi-ring framing of the "receipt" is way more important because it declares a definition that can be referenced uniquely compared to the vast possibilities that a "receipt" can represent compared to the conjugation of the term omi-ring

---

## ChatGPT
`2026-06-28 23:10` | `id:260628-2310` ^260628-231037

Yes. Separate them.

```text
Omi-Ring framing = urgent protocol vocabulary correction
Light Garden = later physical transducer / adapter profile
```

The Omi-Ring definition should be handled first because it solves the ambiguity around “receipt.”

:::writing{variant="document" id="49281"}
# Omi-Ring as Addressed Notation Witness

## Status

Canonical vocabulary correction for the current OMI protocol.

This document separates the term **Omi-Ring** from the broader and more ambiguous term **receipt**.

The word “receipt” can mean many things in ordinary computing:

```text
database row
audit log
transaction record
hash entry
acknowledgement
proof of payment
network confirmation
storage slot
event record
```

That range is too broad for the current OMI protocol.

OMI needs a term that names the specific protocol object:

```text
a bounded addressed notation citation
with forward/inverse readability
capable of validation, attestation, attribution, replay, and projection
```

That term is:

```text
Omi-Ring
```

---

## Definition

An **Omi-Ring** is the bounded addressed notation witness of an OMI relation.

It is the protocol form that makes a relation uniquely referenceable before, during, and after validation.

Canonical form:

```text
omi---imo
```

Local mnemonic forms:

```text
o---o
/---/
?---?
@---@
```

These are not decorative symbols.

They are palindromic notation surfaces for forward and inverse reading.

The Omi-Ring is therefore not merely a “receipt.”

It is the named protocol object that a receipt-like record points to, stores, replays, or projects.

---

## Why Not Just Say Receipt?

A receipt is a phase.

An Omi-Ring is the addressed object.

Use the distinction:

```text
Omi-Ring
 the bounded addressed notation witness

receipt state
 the accepted validation state of an Omi-Ring

receipt ring
 the storage / replay / coordination surface for accepted Omi-Ring states
```

So the corrected stack is:

```text
omi---imo
 ↓
Omi-Ring addressed notation witness
 ↓
candidate citation
 ↓
validation
 ↓
accepted receipt state
 ↓
receipt ring storage
 ↓
projection / scribing / carrier output
```

---

## Core Rule

```text
The Omi-Ring is the addressed notation witness.
The receipt is the accepted state of that witness.
The receipt ring stores accepted Omi-Ring states.
```

This avoids treating “receipt” as the primitive object.

The primitive protocol object is the addressed relation.

---

## Palindromic Mnemonic

OMI uses palindromic and mirror-like notation because every valid relation must support forward and inverse reading.

```text
omi---imo
```

means:

```text
OMI citation side --- IMO carrier side
```

The relation is readable from citation toward carrier and from carrier back toward citation.

The shorter forms name local relation surfaces:

```text
o---o
 object-to-object relation

/---/
 scope-to-scope relation

?---?
 query-to-witness relation

@---@
 CAR-to-CDR closure relation
```

These are all Omi-Ring forms.

They are not separate objects.

They are readable surfaces of the same addressed notation principle.

---

## Omi-Ring and Authority

An Omi-Ring does not accept itself.

An Omi-Ring makes a relation addressable.

Validation determines whether the relation is accepted.

The accepted state may then be stored, replayed, projected, or transmitted.

Canonical authority boundary:

```text
Recognition is not acceptance.
Citation is not acceptance.
Omi-Ring formation is not acceptance.
Projection is not acceptance.
Validation accepts.
Accepted state may be recorded.
```

---

## Updated Vocabulary

Prefer:

```text
Omi-Ring
Omi-Ring citation
Omi-Ring addressed notation
Omi-Ring witness
accepted Omi-Ring state
Omi-Ring storage ring
```

Avoid using “receipt” alone when the meaning could be confused.

Use “receipt” only when referring to the accepted validation state.

---

## One-Sentence Version

An Omi-Ring is the bounded addressed notation witness of an `omi---imo` relation; a receipt is only the accepted validation state of that Omi-Ring, and the receipt ring is only the storage and replay surface for accepted Omi-Ring states.
:::

Then Light Garden can wait and become a separate doc later:

```text
LIGHT_GARDEN.md
 physical transducer / dome / talisman / garden interface profile

OMI_RING.md
 protocol vocabulary correction for receipt ambiguity
```

That is cleaner. Omi-Ring belongs close to core notation. Light Garden belongs in adapters/projection.

---

## You
`2026-06-28 23:21` | `id:260628-2321` ^260628-232124

can you actually do the LIGHT_GARDEN_SPEC.md over again because want you to include how it works like a platform agnostic through bit-wise operations as a specification instead of explained. SO you can make it as complete as correctly possible so if minimal thats fine

---

## ChatGPT
`2026-06-28 23:21` | `id:260628-2321` ^260628-232127

Yes. Here is a clean `LIGHT_GARDEN_SPEC.md` rewritten as a **platform-agnostic bitwise specification**, not just an explanation. I’ve kept it strict: Light Garden is an adapter/transducer profile, not protocol authority.

:::writing{variant="document" id="57392"}
# LIGHT_GARDEN_SPEC.md

## Light Garden Protocol Adapter Specification

Status: Draft
Layer: Physical Transducer / Adapter Profile
Authority: Non-authoritative projection and capture surface
Depends on: OMI addressed notation, Omi-Ring witness, Omicron pre-header, Tetragrammatron validation, Metatron projection, IMO carrier mapping

---

## 0. Purpose

Light Garden defines a platform-agnostic physical transducer profile for the OMI protocol.

It specifies how physical objects, sensory arrays, geometric configurations, domes, talismans, light, sound, gesture, and environmental input MAY be converted into bounded OMI addressed notation citations.

Light Garden does not validate.

Light Garden does not accept state.

Light Garden does not replace OMI notation.

Light Garden produces candidate observations and projection surfaces that MUST reduce back to OMI addressed notation.

Canonical rule:

```text
Light Garden transduces.
OMI notation cites.
Tetragrammatron validates.
Accepted Omi-Ring state records.
Metatron projects.
IMO carries.
```

---

## 1. Authority Boundary

Light Garden is an adapter profile.

It MAY:

```text
sense
sample
encode
fold
orient
project
transduce
display
emit candidate notation
```

It MUST NOT:

```text
validate
accept
authorize
finalize
replace OMI notation
claim protocol truth
perform irreversible side effects without accepted authorization
```

The following are not authority:

```text
sensor input
light pattern
sound pattern
gesture
dome geometry
ring talisman state
projection output
rendered portal state
environmental reading
```

Only validated OMI addressed notation may become accepted state.

---

## 2. Core Objects

Light Garden defines three physical interface objects.

```text
Ring Talisman = personal key / local orientation object
Personal Dome = private gate / bounded interaction chamber
Garden Dome = open-world environmental transducer
```

These objects are physical or virtual adapter surfaces.

They are not required to be literal objects. A platform MAY implement them as hardware devices, browser surfaces, simulation spaces, robotics fields, wearable devices, embedded boards, camera arrays, audio arrays, or mixed-reality projections.

---

## 3. Data Model

A Light Garden transduction produces a `LightGardenFrame`.

All numeric fields are unsigned unless stated otherwise.

All multi-byte fields MUST be little-endian unless an adapter explicitly declares another byte order and that declaration is validated before use.

### 3.1 Frame Shape

```c
typedef struct {
 uint8_t preheader[8];
 uint32_t cycle;
 uint32_t source_id;
 uint32_t sensor_mask;
 uint32_t geometry_mask;
 uint32_t orientation;
 uint32_t sample_hash;
 uint32_t delta_word;
 uint32_t omi_witness;
 uint32_t imo_witness;
 uint32_t flags;
} LightGardenFrame;
```

### 3.2 Required Pre-Header

Every Light Garden frame MUST begin with the OMI gauge pre-header.

Canonical OMI pre-header:

```text
0xFF 0x00 0x1C 0x1D 0x1E 0x1F 0x20 0xFF
```

Meaning:

```text
0xFF = canonical OMI gauge boundary
0x00 = NUL origin
0x1C = FS / file scope
0x1D = GS / group scope
0x1E = RS / record scope
0x1F = US / unit scope
0x20 = readable boundary / space
0xFF = closing canonical OMI gauge boundary
```

Omicron dialect MAY use:

```text
0xF0 0x00 0x1C 0x1D 0x1E 0x1F 0x20 0xF0
```

The pre-header stages topology only.

It does not accept state.

---

## 4. Bitwise Transduction Law

Light Garden MUST reduce physical input to bounded bitwise witness state.

The retained delta law is:

```c
delta(x, C) = rotl(x, 1) ^ rotl(x, 3) ^ rotr(x, 2) ^ C
```

For width `w`, the result MUST be masked:

```c
delta_w(x, C) = delta(x, C) & ((1 << w) - 1)
```

For the canonical 16-bit witness frame:

```c
uint16_t lg_delta16(uint16_t x, uint16_t C) {
 return rotl16(x, 1) ^ rotl16(x, 3) ^ rotr16(x, 2) ^ C;
}
```

The design constraints are:

```text
rotations, not shifts, so no bits are lost
XOR, so the operation is symmetric within the bounded frame
constant C, so zero does not trap as a fixed point
mask to width, so state remains bounded
```

No other numeric value is authority.

Derived values are witnesses only.

---

## 5. Platform-Agnostic Sensor Sampling

A platform MAY expose any number of physical or virtual sensor channels.

Examples:

```text
light
sound
touch
gesture
temperature
motion
orientation
proximity
magnetic field
camera
depth
network event
DOM event
human input
robotic actuator state
```

Each sensor channel MUST be reduced to a bounded unsigned sample word before transduction.

Canonical sample reduction:

```c
uint16_t lg_sample16(uint32_t raw, uint16_t channel_id) {
 uint16_t lo = (uint16_t)(raw & 0xFFFFu);
 uint16_t hi = (uint16_t)((raw >> 16) & 0xFFFFu);
 return (uint16_t)(lo ^ rotl16(hi, 5) ^ channel_id);
}
```

A platform-specific adapter MAY use another deterministic reduction rule, but the rule MUST be declared in an adapter profile and MUST be reproducible.

---

## 6. Sensor Mask

The `sensor_mask` field identifies which sensor channels contributed to the frame.

Each bit represents one channel.

```text
bit 0 = light
bit 1 = sound
bit 2 = touch
bit 3 = gesture
bit 4 = motion
bit 5 = orientation
bit 6 = proximity
bit 7 = camera/depth
bit 8 = environmental scalar
bit 9 = network observation
bit 10 = DOM/browser observation
bit 11 = hardware observation
bits 12..31 = adapter-defined
```

A set bit means the channel contributed to the candidate witness.

It does not mean the channel is trusted.

---

## 7. Geometry Mask

The `geometry_mask` field identifies which physical or projective geometry was used.

```text
bit 0 = ring talisman
bit 1 = personal dome
bit 2 = garden dome
bit 3 = radial array
bit 4 = tetrahedral array
bit 5 = 5-cell resolution body
bit 6 = 11-cell orientation body
bit 7 = circular/orbital projection
bits 8..31 = adapter-defined
```

The geometry mask declares the capture/projection geometry.

It does not validate the relation.

---

## 8. Orientation Word

The `orientation` field encodes the eleven orientation positions used by current OMI doctrine.

Each position occupies two bits.

```text
00 = unset
01 = forward / positive
10 = inverse / negative
11 = balanced / bidirectional
```

Positions:

```text
0 forward OMI direction
1 inverse IMO direction
2 CAR relation direction
3 CDR continuation direction
4 read polarity
5 write polarity
6 candidate polarity
7 result polarity
8 adapter projection polarity
9 comparison polarity
10 rejection / rollback polarity
```

Packing:

```c
uint32_t lg_set_orientation(uint32_t word, uint8_t pos, uint8_t value) {
 uint32_t shift = (uint32_t)(pos * 2u);
 word &= ~(0x3u << shift);
 word |= ((uint32_t)(value & 0x3u) << shift);
 return word;
}
```

The 11-cell orientation body orients relation flow.

It does not accept state.

---

## 9. Omi-Ring Witness Construction

Light Garden output MUST reduce to an Omi-Ring addressed notation witness.

The Omi-Ring witness is not merely a receipt.

It is the bounded addressed notation witness of an OMI relation.

Canonical relation:

```text
omi---imo
```

Local mnemonic surfaces:

```text
o---o
/---/
?---?
@---@
```

Light Garden MUST produce or reference an OMI addressed notation citation of the form:

```text
omi-lightgarden-<source>-<cycle>-imo
```

or a binary equivalent that resolves to the same relation.

The Omi-Ring witness MAY be represented as:

```c
typedef struct {
 uint32_t omi;
 uint32_t imo;
 uint32_t car;
 uint32_t cdr;
 uint32_t cycle;
 uint32_t hash;
 uint32_t orientation;
 uint32_t flags;
} OmiRingWitness;
```

---

## 10. Witness Derivation

A Light Garden frame derives two directional witness words:

```c
uint32_t lg_omi_witness(uint16_t sample, uint16_t C) {
 uint16_t d = lg_delta16(sample, C);
 return ((uint32_t)sample << 16) | d;
}

uint32_t lg_imo_witness(uint16_t sample, uint16_t C) {
 uint16_t d = lg_delta16(sample, C);
 return ((uint32_t)d << 16) | sample;
}
```

The OMI witness is the forward reading.

The IMO witness is the inverse reading.

Neither witness accepts state.

---

## 11. Candidate Declaration

A Light Garden observation MUST be expressed as a candidate declaration before validation.

Readable OMI-Lisp surface:

```lisp
((light-garden . observe)
 . ((source . ring-talisman)
 (geometry . personal-dome)
 (sensor-mask . 0x00000023)
 (orientation . 0x00000000)
 (omi . 0x00000000)
 (imo . 0x00000000)
 (effect . read-only)))
```

A hardware or environmental capture MAY require a stronger effect class:

```lisp
(effect . hardware)
(effect . network)
(effect . capture)
(effect . security-sensitive)
```

Effect class determines projection permission.

Effect class does not accept state.

---

## 12. Effect Classes

Light Garden adapters MUST declare effect class.

Allowed baseline classes:

```text
pure
read-only
local-write
repo-write
network
hardware
capture
security-sensitive
```

Default Light Garden sensing SHOULD be `read-only`.

Camera, microphone, biometric, private-space, location, and personal dome capture SHOULD be `security-sensitive` unless explicitly reduced to a non-identifying deterministic witness before projection.

---

## 13. Validation Boundary

A Light Garden frame becomes accepted only after validation by the OMI validation authority.

The validator MUST check:

```text
pre-header is present and valid
scope is declared
sensor mask is declared
geometry mask is declared
orientation word is bounded
sample hash matches declared samples
delta word is reproducible
OMI and IMO witnesses are reproducible
effect class is permitted by local policy
adapter profile is recognized
candidate declaration is well-formed
```

The validator MAY reject a frame for:

```text
missing pre-header
unstaged FS/GS/RS/US spine
invalid orientation position
non-reproducible sample hash
undeclared sensor channel
undeclared geometry channel
forbidden effect class
untrusted adapter
privacy violation
hardware permission failure
```

---

## 14. Accepted State

If validation succeeds, the accepted state SHOULD reference the Omi-Ring witness.

Use the phrase:

```text
accepted Omi-Ring state
```

rather than using “receipt” alone.

The storage ring MAY store:

```c
typedef struct {
 uint32_t hash;
 uint32_t cycle;
 uint32_t slot5040;
 uint32_t effect;
 uint32_t source_id;
 uint32_t omi_witness;
 uint32_t imo_witness;
 uint32_t orientation;
} LightGardenAcceptedState;
```

This storage is a replay surface.

The Omi-Ring addressed notation witness remains the semantic unit.

---

## 15. Projection

After acceptance, Metatron or another projection surface MAY render the accepted Omi-Ring state.

Projection MAY include:

```text
light pattern
sound pattern
ring talisman response
personal dome display
garden dome visualization
DOM projection
WebGL projection
SVG projection
hardware indicator
physical actuator
```

Projection MUST be gated by local permission.

Projection MUST NOT be treated as acceptance.

---

## 16. Platform Profiles

A platform profile MUST define:

```text
platform name
available sensors
available geometry channels
sample reduction rule
effect classes
privacy boundaries
adapter permission requirements
projection capabilities
failure behavior
```

Examples:

```text
ESP32 Light Garden profile
Browser Light Garden profile
Wearable Light Garden profile
Dome installation profile
Simulation profile
Robot field profile
```

---

## 17. Minimal Conforming Implementation

A minimal conforming Light Garden implementation MUST provide:

```text
valid 8-byte OMI pre-header
at least one sensor channel
deterministic sample reduction
delta16 witness derivation
sensor_mask
geometry_mask
orientation word
OMI witness
IMO witness
candidate declaration
validation boundary
no acceptance inside adapter
```

A minimal implementation MAY omit:

```text
physical dome
ring talisman
audio projection
visual projection
network sharing
hardware actuation
semantic graph projection
```

---

## 18. Reference Bitwise Operations

```c
static inline uint16_t lg_rotl16(uint16_t x, unsigned n) {
 n &= 15u;
 return (uint16_t)((x << n) | (x >> (16u - n)));
}

static inline uint16_t lg_rotr16(uint16_t x, unsigned n) {
 n &= 15u;
 return (uint16_t)((x >> n) | (x << (16u - n)));
}

static inline uint16_t lg_delta16(uint16_t x, uint16_t C) {
 return (uint16_t)(
 lg_rotl16(x, 1) ^
 lg_rotl16(x, 3) ^
 lg_rotr16(x, 2) ^
 C
 );
}

static inline uint16_t lg_sample16(uint32_t raw, uint16_t channel_id) {
 uint16_t lo = (uint16_t)(raw & 0xFFFFu);
 uint16_t hi = (uint16_t)((raw >> 16) & 0xFFFFu);
 return (uint16_t)(lo ^ lg_rotl16(hi, 5) ^ channel_id);
}

static inline uint32_t lg_pair16(uint16_t a, uint16_t b) {
 return ((uint32_t)a << 16) | (uint32_t)b;
}
```

---

## 19. Security and Privacy Rules

Light Garden implementations MUST treat physical sensing as privacy-sensitive by default.

The following MUST NOT be projected without explicit permission and accepted authorization:

```text
camera capture
microphone capture
biometric signal
location
private-room sensor data
personal dome state
ring talisman identity
gesture profile
body movement profile
voiceprint
faceprint
home environment layout
```

A platform MAY reduce raw input to a bounded non-identifying witness before sharing.

Raw sensor data SHOULD remain local unless explicitly authorized.

---

## 20. Canonical Invariants

```text
Light Garden is not authority.
The ring talisman is not authority.
The personal dome is not authority.
The garden dome is not authority.
A sensor reading is not authority.
A projection is not authority.

Light Garden transduces.
OMI notation cites.
Omi-Ring witnesses.
Validation judges.
Accepted Omi-Ring state records.
Projection follows permission.
```

---

## 21. One-Sentence Version

Light Garden is a platform-agnostic physical transducer profile that reduces sensory and geometric interaction into bounded bitwise OMI addressed notation witnesses, using the delta law and OMI pre-header framing, while leaving validation, acceptance, storage, and projection authority to the current OMI protocol stack.
:::

---
