# OMI eMMC Boot Target Example

## 512-Bit Boot Envelope, First 256-Bit Frame, and Four-Face Storage Tetrahedron

Version: `0.1.0`
Status: Target Example
Authority Layer: Hardware Carrier / Flashable Boot Envelope
Depends on: OMI Gauge Pre-Header, OMI-IMO 256-bit Frame, Receipt Ring, Adapter Policy

---

## 0. Purpose

This document defines eMMC as a target example for making OMI flashable.

The purpose is not to make eMMC the only hardware target.

The purpose is to show how OMI can map onto a real flash-storage layout using a bounded boot envelope.

Canonical hardware target idea:

```text
512-bit OMI boot envelope
 =
256-bit prefix/gauge half
 +
256-bit bootstrap OMI---IMO frame
```

In bytes:

```text
512 bits = 64 bytes
256 bits = 32 bytes
```

Therefore:

```text
bytes 0x00..0x1F = OMI prefix/gauge half
bytes 0x20..0x3F = first OMI---IMO bootstrap frame
```

The first half finds and orients the frame.

The second half stages the first frame.

Validation and receipt accept.

---

## 1. eMMC as a Four-Face Carrier

An eMMC-like target naturally suggests four storage faces:

```text
BOOT0
BOOT1
SECURE / RPMB-like authenticated storage
USER data area
```

OMI maps these as a tetrahedral carrier:

```text
face 0 = BOOT0
face 1 = BOOT1
face 2 = secure receipt / authenticated storage
face 3 = user carrier / ordinary storage
```

Canonical interpretation:

```text
BOOT0 = primary boot candidate face
BOOT1 = alternate / recovery boot candidate face
SECURE = receipt, authentication, rollback, monotonic witness face
USER = carrier, document, media, repo, and .imo data face
```

This gives OMI a hardware tetrahedron:

```text
 SECURE
 /\
 / \
 BOOT0----BOOT1
 \ /
 \/
 USER
```

The tetrahedron is not mystical.

It is a storage authority map:

```text
two boot candidates
one secure receipt authority surface
one user carrier surface
```

---

## 2. The 512-Bit OMI Boot Envelope

The OMI boot envelope is exactly 64 bytes.

```text
OMI_BOOT_ENVELOPE_SIZE = 64 bytes
```

Layout:

```text
0x00..0x1F prefix half 256 bits
0x20..0x3F bootstrap half 256 bits
```

### 2.1 Prefix Half

The prefix half begins with the canonical OMI gauge pre-header:

```text
FF 00 1C 1D 1E 1F 20 FF
```

Readable:

```text
GAUGE NUL FS GS RS US SP GAUGE
```

The remaining bytes in the first 256-bit half may carry prefix extension data:

```text
0x00..0x07 = gauge pre-header
0x08..0x0F = dialect / orbit / version fields
0x10..0x17 = local240 / slot5040 / orientation fields
0x18..0x1F = checksum / reserved / receipt pointer
```

The prefix half does not accept state.

It identifies and orients the carrier.

### 2.2 Bootstrap Half

The bootstrap half is the first canonical OMI---IMO frame:

```text
S0 S1 S2 S3 S4 S5 S6 S7 REGISTER STACK CAR CDR
```

Binary size:

```text
8 × 16-bit fields = 128 bits
4 × 32-bit fields = 128 bits
total = 256 bits
```

Expanded:

```text
S0 = FS
S1 = GS
S2 = RS
S3 = US
S4 = FS'
S5 = GS'
S6 = RS'
S7 = US'
REGISTER
STACK
CAR
CDR
```

Visible ruler form:

```text
o-FS-GS-RS-US/FS/GS/RS/US?REGISTER?STACK@CAR@CDR
```

The bootstrap half stages the first candidate frame.

It does not accept state.

---

## 3. First Frame Delineation

The eMMC target uses the first 512 bits to delineate the first OMI frame.

Canonical rule:

```text
The first 256 bits orient the carrier.
The second 256 bits stage the first OMI---IMO frame.
```

More explicitly:

```text
prefix half:
 find OMI
 check gauge
 check dialect
 stage topology
 stage orbit orientation

bootstrap half:
 load first FS/GS/RS/US ruler
 load first REGISTER/STACK/CAR/CDR relation
 stage boot candidate
```

The first OMI---IMO frame SHOULD point to the next boot object, receipt object, or user-carrier object.

Example first-frame meaning:

```text
FS = boot face
GS = boot profile
RS = boot record
US = first unit

REGISTER = boot flags
STACK = boot stack/candidate pointer
CAR = primary declaration pointer
CDR = continuation / recovery pointer
```

---

## 4. Mapping to BOOT0 and BOOT1

The two eMMC boot faces map naturally to OMI boot candidates.

```text
BOOT0 = primary boot candidate
BOOT1 = alternate boot candidate
```

BOOT0 SHOULD contain:

```text
canonical gauge pre-header
primary OMI boot envelope
primary first OMI---IMO frame
primary boot candidate declarations
```

BOOT1 SHOULD contain:

```text
canonical or recovery gauge pre-header
alternate OMI boot envelope
recovery first OMI---IMO frame
fallback declarations
```

Canonical rule:

```text
BOOT0 proposes primary boot.
BOOT1 proposes fallback boot.
Neither accepts boot state.
Validation and receipt accept.
```

If BOOT0 fails validation, a resolver MAY inspect BOOT1.

If BOOT1 validates, BOOT1 may become the accepted boot candidate by receipt.

---

## 5. Mapping to Secure Storage

Secure storage, such as an RPMB-like area, maps to receipt and rollback authority.

It SHOULD contain or reference:

```text
accepted boot receipt
rejected boot receipts
rollback counters
monotonic witness values
secure hash material
signature material, if supported
receipt parent pointers
```

Canonical secure-face rule:

```text
Secure storage stores acceptance witnesses.
It does not make unvalidated content true.
```

A secure receipt may accept that a boot candidate passed validation.

It does not make arbitrary user data trusted.

---

## 6. Mapping to User Storage

The user area maps to ordinary OMI carriers.

It MAY contain:

```text
REPO.md
AGENTS.md
SKILLS.md
ADAPTERS.md
*.imo carrier files
documents
media
indexes
HNSW graph data
semantic graphs
receipts export
projection artifacts
```

User storage is carrier space.

Canonical rule:

```text
USER data carries candidates.
USER data does not accept itself.
```

The user area can carry huge data.

The boot envelope only needs to point into it safely.

---

## 7. Four-Face Authority Map

The eMMC target forms this OMI tetrahedron:

| Face | Storage Area | OMI Role | Authority Boundary |
|---|---|---|---|
| Face 0 | BOOT0 | primary boot candidate | proposes primary boot |
| Face 1 | BOOT1 | fallback boot candidate | proposes recovery boot |
| Face 2 | SECURE / RPMB-like | receipt / rollback witness | stores accepted/rejected boot receipts |
| Face 3 | USER | carrier / repo / media / index | carries declarations and data |

Canonical statement:

```text
BOOT0 and BOOT1 stage boot candidates.
SECURE stores boot receipts and rollback witnesses.
USER carries ordinary OMI data.
Validation and receipt accept.
```

---

## 8. Flashable Image Layout

A flashable OMI eMMC image MAY be organized as:

```text
BOOT0:
 [512-bit OMI boot envelope]
 [primary bootstrap code or declarations]
 [primary receipt pointer]

BOOT1:
 [512-bit OMI boot envelope]
 [recovery bootstrap code or declarations]
 [fallback receipt pointer]

SECURE:
 [accepted boot receipt]
 [rollback counter]
 [receipt parent]
 [secure hash/signature material]

USER:
 [REPO.md]
 [AGENTS.md]
 [SKILLS.md]
 [ADAPTERS.md]
 [*.imo carriers]
 [documents/media/indexes]
 [receipt exports]
```

The exact placement is device-specific.

The OMI rule is the logical map, not a claim that every device exposes identical offsets.

---

## 9. Boot Validation Flow

```text
power-on / boot read
 ↓
read BOOT0 envelope
 ↓
check gauge pre-header
 ↓
parse first 256-bit OMI---IMO frame
 ↓
validate boot candidate
 ↓
check secure receipt / rollback witness
 ↓
if accepted: project boot
 ↓
if rejected or invalid: inspect BOOT1
 ↓
validate fallback candidate
 ↓
record accepted/rejected boot receipt
```

Canonical boot invariant:

```text
Boot read is not acceptance.
Header match is not acceptance.
Frame parse is not acceptance.
Secure storage reference is not acceptance.
Validation and receipt accept.
```

---

## 10. First 512 Bits as Hardware Shebang

The OMI eMMC target treats the first 512 bits as a hardware shebang.

```text
[0x00..0x3F] = OMI boot shebang envelope
```

This gives hardware and tools a bounded first read:

```text
read 64 bytes
check OMI gauge
load first frame
decide whether to continue parsing
```

The fast-fail path is simple:

```text
if header != FF 00 1C 1D 1E 1F 20 FF:
 not canonical OMI boot envelope
```

The validated path is:

```text
if header matches:
 parse bootstrap OMI---IMO frame
 validate candidate
 check receipt
```

---

## 11. Optional Prefix Half Detail

The first 256-bit prefix half may be defined as:

```text
offset size meaning
0x00 8 gauge pre-header
0x08 1 version major
0x09 1 version minor
0x0A 1 dialect byte
0x0B 1 flags
0x0C 2 local240
0x0E 2 slot5040
0x10 4 cycle
0x14 4 orientation11 low word
0x18 4 prefix hash/check
0x1C 4 receipt pointer or reserved
```

This is a candidate layout.

Canonical requirements are only:

```text
first 8 bytes = gauge pre-header
prefix half = 256 bits
bootstrap half = 256 bits
```

The exact subfields may be validated by `SKILLS.md`.

---

## 12. Optional Bootstrap Half Detail

The second 256-bit half is fixed by the OMI---IMO frame:

```text
offset size meaning
0x20 2 S0 / FS
0x22 2 S1 / GS
0x24 2 S2 / RS
0x26 2 S3 / US
0x28 2 S4 / FS'
0x2A 2 S5 / GS'
0x2C 2 S6 / RS'
0x2E 2 S7 / US'
0x30 4 REGISTER
0x34 4 STACK
0x38 4 CAR
0x3C 4 CDR
```

This is the first frame.

The CDR may point to continuation.

The CAR may point to first declaration.

REGISTER and STACK may stage boot flags and boot candidate context.

---

## 13. Canonical eMMC Target Summary

```text
eMMC gives OMI a flashable target example.

BOOT0 = primary boot candidate
BOOT1 = fallback boot candidate
SECURE = receipt / rollback / authenticated witness face
USER = carrier / repo / document / media / index face

The first 512 bits define the OMI boot envelope.

First 256 bits:
 gauge prefix and orbit orientation

Second 256 bits:
 first OMI---IMO bootstrap frame

The tetrahedron is:
 two boot faces
 one secure receipt face
 one user carrier face
```

---

## 14. One-Sentence Summary

The OMI eMMC target uses the first 512 bits as a flashable boot envelope, with a 256-bit gauge/prefix half followed by a 256-bit OMI---IMO bootstrap frame, naturally mapping eMMC-style BOOT0, BOOT1, secure authenticated storage, and user storage into a four-face OMI tetrahedron where boot sectors stage candidates, secure storage records receipts, user storage carries data, and only validation and receipt accept boot state.
:::

That locks the hardware-facing story:

```text
512 bits gives the device the envelope.
BOOT0 and BOOT1 give two boot faces.
SECURE gives the receipt face.
USER gives the carrier face.
```

And the safest short canon is:

```text
The flash target does not make OMI true.
It gives OMI a bounded place to begin.
```