# Geometry and ring

## Receipt ring

The receipt ring is fixed-size and file-backed:

```text
5040 slots
4096 bytes per slot
/tmp/omi_receipt_ring.bin
```

The ring index is:

```text
g_cycle % 5040
```

This makes the ring a bounded spatial compactor rather than an append-only log.

Each stored receipt carries a cycle, receipt string, hash, and result. Ring folds summarize the current receipt field as XOR, SUM, and ROT values.

## Delta law

```text
delta16(x, c) = rotl16(x, 1) ^ rotl16(x, 3) ^ rotr16(x, 2) ^ c
```

## OMI address frame

```text
S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

Fields:

```text
S0..S7   16-bit frame segments
PAYLOAD  32-bit payload
MASK     32-bit mask
CAR      32-bit source/head side
CDR      32-bit route/tail side
```

## Fano routing

Autonomous mode derives the next operation from ring folds through Fano incidence:

```text
fano_point = (ring_xor_fold ^ ring_sum_fold) % 7
line       = first Fano line containing fano_point
opcode     = (line * 3 + ring_rot_fold % 3) % 11
```

The Fano lines are:

```text
{0,1,2}
{0,3,4}
{1,3,5}
{1,4,6}
{2,3,6}
{2,4,5}
{3,4,0}
```

## BQF bridge

```text
BQF(x, y) = 60x² + 16xy + 4y²
```

The local BQF value feeds the 5040-slot geometry:

```text
local240 = BQF(baseQ, fiberQ) % 240
slot5040 = fano7 * 720 + role3 * 240 + local240
```

## Polybius and QuQuart frame

The 5×5 frame uses `(1,1)` as origin. The low QuQuart rail is row `1`, columns `2..5`. The high QuQuart rail is column `1`, rows `2..5`. The interior is rows `2..5`, columns `2..5`.

Interior cell selection:

```text
local16 = local240 & 0x0F
px = local16 & 3
py = (local16 >> 2) & 3
cell = (py + 2, px + 2)
```

## Hopf projection

```text
theta = ((baseQ + 0.5) / 4) * pi
phi   = ((fiberQ + 0.5) / 4) * 2pi
qw = cos(theta / 2)
qx = sin(theta / 2) * cos(phi)
qy = sin(theta / 2) * sin(phi)
qz = 0
```

The Hopf direction is rounded from the quaternion candidate:

```text
a = round(2(qxqz + qwqy))
b = round(2(qyqz - qwqx))
c = round(1 - 2(qx² + qy²))
```
