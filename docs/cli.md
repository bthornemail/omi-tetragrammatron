# Command reference

Build first:

```bash
gcc -O2 -Wall -Wextra -Wno-misleading-indentation -o opencode.bin opencode.c -lm
```

## General help

```bash
./opencode.bin --help
```

## Evaluate one S-expression

```bash
./opencode.bin --eval '(cons 1 2)'
```

Prints a receipt containing the cycle, source expression, reduced result, and hash.

## REPL

```bash
./opencode.bin --repl
```

Special REPL commands:

```text
:ring
:quit
```

## Run autonomous mode

```bash
./opencode.bin --auto
```

Autonomous mode boots the CPU, folds the ring, derives geometry/routing values, constructs OMI frames, executes them, stores receipts, and stops on zero, repeat, or epoch closure.

## Boot the nibble CPU

```bash
./opencode.bin --boot
```

Runs the built-in boot ROM on the nibble CPU and prints CPU state.

## Process an input file

```bash
./opencode.bin --run path/to/input.txt
```

Processes lines as OMI addresses or expressions according to runtime parsing rules.

## Execute one OMI address on the CPU

```bash
./opencode.bin --cpu '0x0001-0x0000-0x0000-0x0001/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000'
```

## Geometry constants

```bash
./opencode.bin --geom
```

Prints the Polybius frame, QuQuart rails, Fano lines, BQF samples, and ring-slot structure.

## Ring commands

Dump the receipt ring:

```bash
./opencode.bin --ring
```

Watch ring evolution:

```bash
./opencode.bin --watch
```

## Digital twin outputs

Display text summary:

```bash
./opencode.bin --twin
```

Render JSON frame:

```bash
./opencode.bin --render-frame > /tmp/frame.json
```

Render deterministic PPM image:

```bash
./opencode.bin --render-ppm > /tmp/frame.ppm
```

## Seed and server

Write packed boot ROM seed:

```bash
./opencode.bin --seed opencode.seed
```

Serve the browser viewer and JSON endpoints:

```bash
./opencode.bin --serve 8080
```


## Deterministic geometry checks

```bash
./opencode.bin --check
```

Runs finite self-checks for Fano incidence, CONFIG_MATRIX / SHAPE_DB mapping, SHAPE_DB edge and vertex integrity, and solid_lookup reachability.
