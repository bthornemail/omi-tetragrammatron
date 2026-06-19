# Glossary

## Address frame

An OMI input shape containing eight 16-bit segments plus payload, mask, CAR, and CDR:

```text
S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

## Autonomous mode

A mode where the runtime folds the receipt ring, derives a new OMI frame through Fano routing and deterministic formulas, executes it, stores a receipt, and repeats until a closure condition.

## BQF

Binary quadratic form used as a bridge value:

```text
60x² + 16xy + 4y²
```

## CAR and CDR

32-bit frame fields that carry source/head and route/tail values. They are also familiar names from pair/list operations.

## Carrier behavior

I/O and presentation behavior that transports or stores bytes without defining deterministic computation. Examples include terminal input, file writes, sockets, signals, and browser rendering.

## Delta law

The 16-bit rolling reduction:

```text
delta16(x, c) = rotl16(x, 1) ^ rotl16(x, 3) ^ rotr16(x, 2) ^ c
```

## Digital twin

A deterministic projection of receipt and ring state into geometry fields such as Fano point, role, local slot, Polybius cell, frame type, quaternion candidate, and Hopf direction.

## Fano routing

Autonomous operation selection using ring folds and Fano plane incidence.

## Frame class

One of the four OMI frame classes:

```text
US
GS
RS
FS
```

## Hopf projection

A deterministic quaternion-derived projection used to produce a compact direction from base and fiber quadrants.

## IMO

A project term for the motion-projected or inverted perspective of an OMI address, where a discrete address is carried into smooth geometric projection.

## Nibble CPU

A small CPU that interprets nibbles extracted from OMI address segments as 4-bit opcodes.

## OMI

The project notation and runtime frame system used for symbolic input, address frames, receipts, ring folding, and geometric projection.

## Polybius frame

A 5×5 geometry frame with origin at `(1,1)`, QuQuart rails on the top row and left column, and a 4×4 interior.

## QuQuart

A four-phase quadrant/rail concept used by the geometry projection and frame classes.

## Receipt

An observable record emitted by the runtime for accepted input. Receipts are stored in the fixed ring and folded into future autonomous state.

## Ring

A fixed 5040-slot receipt compactor backed by `/tmp/omi_receipt_ring.bin`.

## Slot5040

A geometry slot derived from Fano point, role, and local BQF value:

```text
slot5040 = fano7 * 720 + role3 * 240 + local240
```


## Tetrahedral π carry

The project interpretation of π as a deterministic carry-forward process derived from tetrahedral edge/face traversal, bridging discrete OMI address states and smooth circular or spherical projection.
