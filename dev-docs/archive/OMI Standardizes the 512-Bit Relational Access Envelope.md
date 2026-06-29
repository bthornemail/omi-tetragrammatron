# OMI Standardizes the 512-Bit Relational Access Envelope

## Status

Canonical doctrine.

## Core Rule

OMI standardizes the 512-bit relational access envelope, not the hardware sector.

The OMI boot cell is:

```text
512 bits = 64 bytes
```

This is the minimum complete boot relation for the protocol.

It is not the same thing as a 512-byte storage sector.

A storage medium may expose 512-byte sectors, 4096-byte sectors, erase blocks, flash pages, files, memory pages, packets, or browser storage records. Those are carrier surfaces.

The OMI cell remains 512 bits.

## The 512-Bit Envelope

The OMI 512-bit relational access envelope is divided into two 256-bit halves:

```text
first 256 bits  = prefix / gauge / orientation / offset / recovery half
second 256 bits = OMI---IMO addressed relation frame
```

Canonical layout:

```text
0x00..0x1F = prefix half
0x20..0x3F = bootstrap frame half
```

The first half orients the frame.

The second half cites the relation.

Neither half is a hash, signature, encryption key, digest, or sector checksum.

## Prefix Half

The first 256 bits begin with the OMI gauge pre-header:

```text
FF 00 1C 1D 1E 1F 20 FF
```

Readable form:

```text
GAUGE NUL FS GS RS US SP GAUGE
```

This pre-header declares:

```text
this carrier is attempting OMI interpretation
this gauge row is selected
this pre-language envelope begins here
the FS/GS/RS/US topology is staged
the readable boundary is opened
the gauge is sealed by the matching byte
```

The remaining bytes of the prefix half may carry deterministic orientation, offset, recovery, boot-face selection, or local reference-point material.

A safe canonical partition is:

```text
0x00..0x07  gauge pre-header
0x08..0x0F  offset / orientation rail
0x10..0x17  recovery / gauge guard rail
0x18..0x1F  carrier / boot target rail
```

These rails do not create identity.

They make the following OMI---IMO frame locatable, orientable, and recoverable in a carrier.

## Relation Half

The second 256 bits carry the canonical OMI---IMO relation frame:

```text
o-FS-GS-RS-US/FS/GS/RS/US?REGISTER?STACK@CAR@CDR
```

Machine form:

```text
S0 S1 S2 S3 S4 S5 S6 S7 REGISTER STACK CAR CDR
```

Size:

```text
8 × 16-bit ruler fields = 128 bits
4 × 32-bit relation fields = 128 bits
total = 256 bits
```

This half is the addressed place-value relation.

It is the identity-bearing frame.

The prefix helps find and orient it, but the address remains identity.

## Carrier Blocks Are Not Protocol Identity

A 512-byte carrier block contains:

```text
512 bytes = 4096 bits
```

Since one OMI boot cell is:

```text
512 bits = 64 bytes
```

a 512-byte carrier block can hold:

```text
8 × 512-bit OMI boot cells
```

This gives a natural 8-cell carrier orbit:

```text
cell 0 = pinch anchor / canonical entry cell
cells 1..6 = incidence relation cells
cell 7 = branch / carry-forward / next-frame cell
```

The carrier block may mirror the same grammar as the pre-header:

```text
open
stage relation
carry forward
```

But the carrier block does not create identity.

The OMI address relation remains identity.

## Eight-Cell Boot Block

When a 512-byte carrier block is used, OMI may interpret it as an eight-cell boot block:

```text
4096-bit carrier block
└── 8 × 512-bit OMI boot cells
    ├── cell 0: pinch / opener / canonical anchor
    ├── cells 1..6: incidence relation cells
    └── cell 7: branch / carry-forward / next-frame pointer
```

This is a carrier-compatible orbit.

It is not a cryptographic structure.

It is not a hash table.

It is not a sector identity scheme.

It is a place-value staging pattern for deterministic boot relation recovery.

## Relation to the Pre-Header

The eight-cell carrier block scales the same grammar as the eight-byte pre-header:

```text
G 00 1C 1D 1E 1F 20 G
```

At byte scale:

```text
first G = opener / pinch
middle six bytes = staged control cascade
last G = closer / branch
```

At boot-cell scale:

```text
first cell = opener / pinch
middle six cells = staged incidence cascade
last cell = closer / branch / carry-forward
```

The same open/interior/close grammar appears at both scales.

## Power-Of-Two Scaling

OMI uses compatible power-of-two scaling laws.

Linear doubling gives the address-width ladder:

```text
4 bits    = nibble
8 bits    = byte
16 bits   = local guard field
32 bits   = relation field unit
64 bits   = boot-cell subrail
128 bits  = half-frame
256 bits  = full OMI---IMO frame
512 bits  = boot envelope
4096 bits = eight-cell carrier block
```

Square scaling gives relation surfaces:

```text
16 × 16 = 256
64 × 64 = 4096
```

Cubic scaling gives boot-cell and carrier volume:

```text
8 × 8 × 8 = 512
16 × 16 × 16 = 4096
```

Quartic scaling gives closure frames:

```text
4 × 4 × 4 × 4 = 256
8 × 8 × 8 × 8 = 4096
```

These are not competing models.

They are compatible views of the same place-value cascade.

## Tetragrammatron Resolution Ladder

The Tetragrammatron may use an exponent-like resolution ladder analogous to the generalized mean ladder:

```text
-1  inverse / rollback / CDR-side recovery
 0  origin / neutral / zero-point
 1  linear address step
 2  quadratic fold
 3  cubic boot-cell volume
```

This does not make OMI a system of numerical averages.

The ladder is used only as a resolution-order analogy.

The Tetragrammatron governs deterministic transition resolution, not statistical averaging.

## Not A Cryptographic Key-Pair

The 512-bit OMI envelope may be compared to a key-pair only by shape and coordination use.

It is not a cryptographic key-pair.

The public side is the canonical OMI---IMO reference relation.

The private or local side is a declared offset, orientation, carrier face, or reference point used for scoped obfuscational access and deterministic resolution.

The pair does not encrypt, decrypt, sign, verify, or create identity inside the protocol.

Cryptography may be added by an external carrier layer between projected surfaces.

Cryptographic material is never protocol identity, never validation input, and never the Omi-Ring witness.

## Boundary With Hardware Sectors

Storage media may expose:

```text
512-byte sectors
4096-byte sectors
flash pages
erase blocks
memory pages
filesystem blocks
network packet payloads
browser storage records
WASM memory regions
```

Those are carrier allocation surfaces.

They may contain, align, pad, repeat, mirror, or transport OMI boot cells.

They do not define OMI identity.

Canonical distinction:

```text
512-bit OMI cell = protocol boot relation
512-byte sector = carrier allocation surface
4096-byte sector = carrier allocation surface
```

Carrier alignment may change.

The protocol cell does not.

## No Hash, No Digest, No Sector Authority

The 512-bit envelope is not a hash envelope.

The prefix half does not hash the relation half.

The carrier sector does not become the identity of the OMI cell.

The protocol must not derive identity, validation, carry-forward proof, or projection authority from:

```text
hash
checksum
digest
signature
encrypted label
sector number
block address
filesystem inode
database key
```

Those may exist outside the protocol.

They do not identify the OMI relation.

## Canonical Lock

```text
OMI standardizes the 512-bit relational access envelope, not the hardware sector.

The first 256 bits orient, offset, and recover.

The second 256 bits carry the OMI---IMO relation.

Storage media may expose 512-byte, 4KiB, or other allocation blocks.

Those are carrier surfaces only.

The OMI cell remains 512 bits.

The address remains identity.

Validation judges carry-forward.

Cryptography, hashing, and sector policy remain outside the core protocol.
```

## One-Sentence Summary

The OMI 512-bit relational access envelope is the minimum complete boot relation of the protocol, made from a 256-bit prefix/orientation/recovery half plus a 256-bit OMI---IMO address frame, and may be packed into larger carrier blocks such as 512-byte or 4KiB sectors without making sector layout, cryptography, hashing, or storage policy part of OMI identity.
