# S-expressions and OMI frames

The runtime accepts two closely related input families: symbolic S-expressions and OMI address frames. Both are reduced into receipts so the accepted input, normalized shape, result, and hash can be replayed or inspected later.

## S-expression inputs

Use `--eval` for one expression:

```bash
./opencode.bin --eval '(cons 1 2)'
```

Use `--repl` for interactive evaluation:

```bash
./opencode.bin --repl
```

The reducer supports ordinary atom/list/pair shapes and recognizes OMI declarations when the expression uses canonical OMI structure or alist-style lane pairs.

## Canonical OMI declarations

Canonical declarations use `omi` as the root form. The runtime treats section names as GS events and recursively collects nested content into the event tape.

A minimal canonical shape is:

```scheme
(omi (section value))
```

Canonical collection is useful when the input should carry explicit structure instead of only being treated as a plain list.

## Alist lane declarations

Alist-style declarations use pairs whose head names an OMI lane. The lane names map to four frame classes:

```text
FS  0x1c
GS  0x1d
RS  0x1e
US  0x1f
```

When a pair head names one of these lanes, the runtime records the lane and the canonicalized value in the event tape.

## Receipts

A receipt is emitted for accepted input. It records enough observable state to inspect what was accepted and how it reduced.

Receipt content includes:

```text
cycle
source or normalized input
shape
result
hash
```

Accepted receipts are stored in the fixed ring. Rejected inputs emit rejection receipts but do not become deterministic computation authority beyond the reported failure.

## OMI address frame

The address frame shape is:

```text
S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

Example:

```text
0x0001-0x0000-0x0000-0x0001/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000
```

Field meanings:

```text
S0..S7   16-bit frame segments
PAYLOAD  32-bit payload
MASK     32-bit mask
CAR      32-bit source/head side
CDR      32-bit route/tail side
```

`S3` selects the operation when executing an OMI frame. The frame can also be passed through the nibble CPU with `--cpu`.

## Operation surface

```text
0x0000  zero / NOP
0x0001  XOR
0x0002  XNOR
0x0003  NAND
0x0004  AND
0x0005  OR
0x0006  ROTL
0x0007  ROTR
0x0008  DELTA
0x0009  BQF
0x000A  pass S4
0x000B  pass S4
0x000C  pass S4
0x000D  S4 XOR S5 XOR S6
0x000E  CAR XOR CDR
0x000F  zero / terminal-like operation
default fold across all frame fields
```

## Nibble CPU execution

The nibble CPU extracts 32 nibbles from `S0..S7`. Each 16-bit segment contributes four 4-bit opcodes. CPU state includes:

```text
pc
car_reg
cdr_reg
payload
mask
flags
halted
epoch
memory[65536]
stack[512]
```

The CPU halts when it sees nibble `0xF`.
