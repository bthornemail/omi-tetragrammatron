# Runtime data model

This page names the major runtime structures so readers can navigate `opencode.c` without first understanding every mode.

## Parser and normalized input

The parser converts an input string into nodes:

```text
atom
string
list
pair
```

The compiler then builds a normalized representation, detects canonical OMI or alist-style input, collects lane events, and creates a byte tape for hashing.

## OMI instruction

An OMI instruction contains eight 16-bit segments plus 32-bit payload, mask, CAR, and CDR fields:

```text
s0 s1 s2 s3
s4 s5 s6 s7
payload mask car cdr
```

This is the shared representation used by address parsing, operation execution, CPU execution, seed writing, and autonomous frame construction.

## CPU state

The nibble CPU state has register-like fields, a fixed memory array, and a fixed stack:

```text
pc
car_reg
cdr_reg
payload
mask
flags
halted
epoch
memory
stack
stack_ptr
```

The CPU is deterministic because its next state is derived only from the current state and the instruction nibbles it accepts.

## Ring slot

The ring is an array of 5040 fixed slots. Each slot stores:

```text
cycle
receipt string
hash
result
```

The ring is saved to `/tmp/omi_receipt_ring.bin`. It is intentionally bounded: later writes reuse ring positions instead of growing a log.

## Twin geometry

The digital twin projection is derived from cycle, folds, opcode, result, and geometric indices. The resulting frame contains:

```text
chart11
baseQ
fiberQ
fano7
role3
qxy
local240
slot5040
quaternion components
Hopf direction
Polybius cell
frame type
ring folds
```

The projection is a deterministic view of receipt state. It is not a replacement for the receipt ring.

## Data flow

```text
input string
  -> parse nodes
  -> compile normalized form and event tape
  -> reduce or execute operation
  -> emit receipt
  -> store ring slot
  -> fold ring
  -> derive geometry
  -> render text, JSON, PPM, or viewer state
```

Autonomous mode loops through the same flow, except it derives the next OMI frame from ring folds and Fano routing rather than from a new external expression.
