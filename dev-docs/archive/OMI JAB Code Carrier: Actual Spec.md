# OMI JAB Code Carrier: Actual Specification Binding

## Mapping OMI Carrier Notation to BSI TR-03137 JAB Code

Version: `0.1.0`
Status: Canonical Draft
Protocol: `OMI---IMO`
Carrier Profile: `omi.jabcode.bsi03137.v0`
Replaces: `omi.jabcode.rs.v0` debug-only profile
Depends on: BSI TR-03137 Part 2, OMI Gauge Pre-Header, OMI Object Model, Doctrine of Determinism v0.4

---

## 0. Correction

The previous carrier profile:

```text
omi.jabcode.rs.v0
```

was a deterministic debug matrix.

It was not a standards-compliant JAB Code symbol.

Corrected profile split:

```text
omi.jabcode.debug.v0
  = deterministic internal visual/debug matrix

omi.jabcode.bsi03137.v0
  = actual BSI TR-03137 JAB Code carrier
```

Canonical rule:

```text
Debug matrices may inspect.
Only BSI-conformant JAB symbols may be called JAB Code.
Validation and receipt accept.
```

---

## 1. What JAB Code Is

JAB Code is a color two-dimensional matrix symbology.

Its basic unit is a square color module.

A JAB Code has:

```text
one primary symbol
zero or more secondary symbols
```

The primary symbol contains finder patterns.

Secondary symbols do not contain finder patterns; they use alignment patterns and may dock to the primary symbol or to other secondary symbols.

OMI interpretation:

```text
Primary symbol   = root carrier / frame locator
Secondary symbol = docked continuation carrier
Host symbol      = symbol that receives a docked secondary symbol
Module           = colored square encoding unit
```

Canonical OMI rule:

```text
JAB Code carries OMI declarations.
JAB Code does not accept them.
Validation and receipt accept.
```

---

## 2. Actual JAB Compliance Requirements

A compliant OMI JAB carrier MUST implement the BSI JAB Code pipeline:

```text
1. data analysis
2. data encoding
3. LDPC error correction
4. data interleaving
5. metadata reservation
6. finder/alignment/color-palette/data placement
7. data masking
8. metadata generation and placement
```

Therefore, an implementation is not actual JAB Code if it only:

```text
draws a colored grid
draws four custom finder boxes
packs OMI bytes directly into cells
skips metadata
skips LDPC
skips interleaving
skips masking
skips official color palette placement
```

That output may still be useful, but it must be called:

```text
debug carrier
```

not:

```text
JAB Code
```

---

## 3. OMI Payload Inside JAB Code

OMI does not need a fake outer annotation such as:

```text
o.base64.o
o.jab.o
o.image.o
```

The actual JAB symbol is the carrier.

The OMI payload inside the JAB data stream SHOULD begin with the canonical OMI gauge pre-header:

```text
FF 00 1C 1D 1E 1F 20 FF
```

Then the OMI bootstrap frame:

```text
S0 S1 S2 S3 S4 S5 S6 S7 REGISTER STACK CAR CDR
```

For the current 128-bit S-only pointer carrier, the temporary payload MAY be:

```text
FF 00 1C 1D 1E 1F 20 FF
S0 S1 S2 S3 S4 S5 S6 S7
```

Canonical full payload:

```text
[8-byte OMI gauge pre-header]
[32-byte OMI---IMO frame]
[optional OMI-Lisp declarations]
[optional receipt pointer]
```

The JAB symbol carries this byte stream.

It does not interpret or accept it.

---

## 4. Actual Carrier Profile

```text
profile: omi.jabcode.bsi03137.v0
spec: BSI TR-03137 Part 2
symbol_type: primary
shape: square by default
side_version: minimal fitting version by default
module_color_mode: 8 colors by default
error_correction_level: default or explicitly declared
payload_mode: byte mode for raw OMI bytes
metadata: BSI metadata, not OMI metadata
masking: BSI mask selection
interleaving: BSI interleaving
```

OMI should not invent its own JAB metadata.

The metadata must follow JAB Code.

OMI metadata lives inside the encoded payload as OMI declarations.

---

## 5. Color Mode Binding

The debug carrier currently uses this 8-color palette:

```text
0 black
1 blue
2 green
3 cyan
4 red
5 magenta
6 yellow
7 white
```

For BSI JAB Code, the 8-color mode maps bit values through the color palette index. The spec’s Table 19 gives the 8-color bit mapping as:

```text
black   = 000
magenta = 001
yellow  = 010
cyan    = 011
red     = 100
green   = 101
blue    = 110
white   = 111
```

Therefore the current `PALETTE` order must be changed for actual BSI bit encoding.

Correct BSI 8-color order:

```ts
export const BSI_8_COLOR_PALETTE = [
  '#000000', // 0 black   000
  '#ff00ff', // 1 magenta 001
  '#ffff00', // 2 yellow  010
  '#00ffff', // 3 cyan    011
  '#ff0000', // 4 red     100
  '#00ff00', // 5 green   101
  '#0000ff', // 6 blue    110
  '#ffffff', // 7 white   111
] as const
```

Canonical correction:

```text
Debug palette order is not BSI bit order.
BSI bit order must follow the JAB color-index mapping.
```

---

## 6. Finder Pattern Correction

The current debug carrier draws four generic 5×5 finder boxes.

That is not JAB Code.

A BSI primary symbol requires four oriented finder patterns:

```text
Finder Pattern UL
Finder Pattern UR
Finder Pattern LR
Finder Pattern LL
```

Each finder pattern is made from two 3×3 square references connected by an overlapping core module.

The finder cores are unique:

```text
UL core = blue
UR core = green
LR core = magenta
LL core = yellow
```

Therefore, actual JAB rendering must implement the official four finder patterns, not generic QR-like squares.

Canonical correction:

```text
Four custom debug corner boxes are not JAB finder patterns.
Actual JAB requires oriented color finder patterns.
```

---

## 7. Symbol Size Correction

The debug matrix hardcodes:

```ts
const matrixSize = 21
```

That corresponds to Side-Version 1 size, but actual JAB supports 32 side versions:

```text
side_size = 21 + 4 × (side_version - 1)
```

So:

```text
Side-Version 1  = 21 modules
Side-Version 2  = 25 modules
...
Side-Version 32 = 145 modules
```

Actual encoder rule:

```text
choose the smallest side-version that can contain:
  encoded payload bits
  LDPC parity bits
  stuffing bits
  metadata
  color palette
  function patterns
```

The current code cannot remain fixed at 21×21 unless the encoded payload actually fits Side-Version 1 after real JAB overhead.

---

## 8. Metadata Correction

Actual JAB metadata is not optional.

Primary-symbol metadata includes:

```text
Nc  = module color mode
SS  = symbol shape flag
VF  = side-version flag
MSK = mask reference
SF  = secondary flag
V   = side-version
E   = error correction parameters
S   = docked secondary positions
```

Secondary-symbol metadata differs and includes:

```text
SS = same shape and size flag
SE = same error correction level flag
SF = secondary position flag
V  = side-version
S  = docked secondary positions
E  = error correction parameters
```

Metadata is LDPC-encoded separately by part.

Canonical correction:

```text
OMI declarations are payload.
JAB metadata is carrier metadata.
Do not merge them.
```

---

## 9. Error Correction Correction

Actual JAB Code uses LDPC error correction.

The current debug carrier has no LDPC.

A compliant encoder must implement:

```text
message bitstream
  ↓
LDPC check bits
  ↓
gross payload Pg
  ↓
stuffing bits
  ↓
interleaving
```

JAB Code defines selectable error correction levels 0 through 10.

The OMI default should be declared, not hidden:

```omi-lisp
(jab.error-correction.level . 6)
```

or for compact debug/dev symbols:

```omi-lisp
(jab.error-correction.level . 2)
```

Canonical rule:

```text
Without JAB LDPC, the symbol is not actual JAB Code.
```

---

## 10. Interleaving Correction

Actual JAB Code interleaves the final encoded data sequence.

The interleaving algorithm uses a deterministic random permutation.

For data interleaving, the initial seed is:

```text
226759
```

For LDPC matrix generation, the spec uses separate seeds for message data and metadata.

OMI interpretation:

```text
interleaving is carrier-level determinism
not OMI acceptance
```

Canonical rule:

```text
Interleaving may protect carrier robustness.
It does not accept payload state.
```

---

## 11. Data Placement Correction

Actual JAB data placement starts at the most upper-left available data module and proceeds downward from left to right, skipping finder patterns, alignment patterns, metadata, and color palettes.

Therefore the implementation must reserve all function modules first:

```text
finder patterns
alignment patterns
metadata regions
color palettes
```

Then place data only in remaining modules.

Canonical correction:

```text
Do not place OMI payload by simply scanning every non-debug-finder cell.
Actual JAB placement must skip official reserved modules.
```

---

## 12. Masking Correction

Actual JAB Code defines eight mask patterns:

```text
000
001
010
011
100
101
110
111
```

Each mask is applied only to data modules.

The selected mask is the one with the lowest penalty score after evaluating undesirable patterns.

Therefore `encodeToJABCode()` must:

```text
try all 8 masks
score each result
choose the lowest-penalty mask
write MSK into metadata
```

Canonical rule:

```text
Mask choice is part of carrier validity.
It is not OMI authority.
```

---

## 13. Decode Correction

Actual JAB decode must follow the BSI reference order:

```text
1. preprocess image
2. locate primary finder patterns
3. decode primary metadata
4. locate alignment patterns and sampling grid
5. construct color palettes
6. decode data modules
7. release masking
8. deinterleave data
9. correct errors
10. decode data stream
11. locate/decode secondary symbols recursively
```

The current `decodeFromJABCode()` only decodes a pointer string or 32-character hex string.

That function should be renamed:

```ts
decodeFromDebugCarrier()
```

Actual function name should be reserved for real JAB decoding:

```ts
decodeFromJABCodeImage()
decodeFromJABCodeMatrix()
```

---

## 14. Required TypeScript Refactor

Current names to change:

```ts
PROFILE = 'omi.jabcode.rs.v0'
encodeToJABCode()
decodeFromJABCode()
renderDebugMatrix()
```

Correct split:

```ts
export const DEBUG_PROFILE = 'omi.jabcode.debug.v0'
export const BSI_PROFILE = 'omi.jabcode.bsi03137.v0'

export function encodeToDebugMatrix(...)
export function decodeFromDebugMatrix(...)

export function encodeToJABCode(...)
export function decodeFromJABCode(...)
```

`encodeToJABCode()` must not return a fake debug SVG.

It must return a BSI-conformant symbol object or call a verified BSI encoder backend.

---

## 15. Minimum Compliant Implementation Boundary

A minimum actual-spec OMI JAB implementation must include:

```text
official color palette order
official side-version sizing
official finder patterns
metadata generation
metadata LDPC
message LDPC
stuffing bits
data interleaving
reserved module placement
data placement
mask application
mask scoring
metadata placement
SVG or raster rendering from final module matrix
```

Until these exist, the carrier must be labeled:

```text
debug
```

not:

```text
JAB Code
```

---

## 16. OMI Authority Boundary

JAB Code is a carrier.

JAB Code does not validate OMI.

JAB Code does not accept OMI.

JAB Code does not prove the payload.

Canonical rule:

```text
JAB locates and carries.
OMI parses and cites.
Tetragrammatron validates.
Receipt accepts.
Metatron projects.
```

A successfully decoded JAB symbol yields:

```text
carrier bytes
```

Those bytes may begin with:

```text
FF 00 1C 1D 1E 1F 20 FF
```

That pre-header recognizes OMI payload.

Still:

```text
JAB decode is not OMI acceptance.
OMI pre-header is not OMI acceptance.
Validation and receipt accept.
```

---

## 17. Final Canon

```text
The old jabcodeCarrier.ts is a debug matrix carrier.

Actual JAB Code must follow BSI TR-03137.

The carrier profile split is:

omi.jabcode.debug.v0
  internal deterministic debug visualization

omi.jabcode.bsi03137.v0
  actual standards-conformant JAB carrier

JAB Code carries the OMI byte stream.
The OMI byte stream begins with the gauge pre-header.
The OMI---IMO frame follows.
The decoded payload becomes a candidate.
Validation judges.
Receipt accepts.
```

---

## 18. One-Sentence Summary

To make OMI JAB Code actual-spec, the current debug matrix must be renamed `omi.jabcode.debug.v0`, while `omi.jabcode.bsi03137.v0` must implement the BSI TR-03137 JAB pipeline: official primary/secondary symbol structure, official color palette order, finder and alignment patterns, metadata, LDPC error correction, interleaving, reserved module placement, data masking, and decode order, with OMI bytes carried as payload and accepted only after OMI validation and receipt.


## 3. Primary Projection Classes

OMI projections fall into several classes.

```text
structural surfaces
symbolic surfaces
digest and witness surfaces
coordinate and mapping surfaces
observation and orientation surfaces
sensory surfaces
portal/world surfaces
logic and chart surfaces
hardware and carrier surfaces
process/runtime surfaces
geometry/configuration surfaces
```

A surface may belong to more than one class.

For example:

```text
Omi-Gnomon is both orientation and geometry.
Omi-Matrix is both observation and witness.
Omi-Jab is both carrier and contextual encoding.
Omi-Shader is both geometry and hardware acceleration.
Omi-Receipt is both witness and lawful resolution proof.
```

---

## 4. Core Projection Surfaces

| Projection | Face Exposed | Primary Role |
|---|---|---|
| **Omi-Form** | Structural | Exposes shape, cells, and boundaries. |
| **Omi-Glyph** | Symbolic | Exposes marks, signs, Unicode characters, SVG marks, and visible notation. |
| **Omi-Hash** | Digest | Exposes compact identity and verification digest. |
| **Omi-Map** | Coordinate | Exposes feature mapping, spatial correspondence, and route placement. |
| **Omi-Image** | Package | Exposes the nine universal edges inside carrier sentinels. |
| **Omi-Matrix** | Observation | Exposes witness fields, matrix relations, and observable structure. |
| **Omi-Gnomon** | Orientation | Exposes directional shadow, axes, right angle, and pointer orientation. |
| **Omi-Mirror** | Chiral | Exposes mirror traversal and `MM → NN` relational reversal. |
| **Omi-Clock** | Timing | Exposes ticks, cadence, replay order, and period inspection. |
| **Omi-Light** | Visual | Exposes color, opacity, illumination, and state by light. |
| **Omi-Sound** | Auditory | Exposes rhythm, tone, vibration, and cadence. |
| **Omi-Portal** | Access | Exposes a doorway into an object, world, or bounded projection. |
| **Omi-World** | Environment | Exposes persistent self-stepping projection. |
| **Omi-Sense** | Sensory | Exposes five-channel routing: visual, sound, touch, motion, comprehension. |

These are not separate objects.

They are canonical faces of one OMI object.
