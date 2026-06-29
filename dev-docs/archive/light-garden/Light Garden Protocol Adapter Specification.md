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
Ring Talisman  = personal key / local orientation object
Personal Dome  = private gate / bounded interaction chamber
Garden Dome    = open-world environmental transducer
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
    uint8_t  preheader[8];
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
bit 0  = light
bit 1  = sound
bit 2  = touch
bit 3  = gesture
bit 4  = motion
bit 5  = orientation
bit 6  = proximity
bit 7  = camera/depth
bit 8  = environmental scalar
bit 9  = network observation
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
0  forward OMI direction
1  inverse IMO direction
2  CAR relation direction
3  CDR continuation direction
4  read polarity
5  write polarity
6  candidate polarity
7  result polarity
8  adapter projection polarity
9  comparison polarity
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
