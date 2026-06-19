# Examples

These examples assume the runtime has been built from the repository root:

```bash
gcc -O2 -Wall -Wextra -Wno-misleading-indentation -o opencode.bin opencode.c -lm
```

## Evaluate a pair

```bash
./opencode.bin --eval '(cons 1 2)'
```

Use this as the smallest end-to-end receipt test.

## Evaluate canonical OMI input

```bash
./opencode.bin --eval '(omi (frame (US . alpha) (GS . beta)))'
```

This exercises the OMI declaration path and lane event collection.

## Execute an OMI CPU frame

```bash
./opencode.bin --cpu '0x0001-0x0000-0x0000-0x0001/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000'
```

This parses one address frame and runs its nibbles through the CPU.

## Run a file of inputs

Create an input file:

```bash
cat > /tmp/omi-inputs.txt <<'EOF_INPUT'
(cons 1 2)
(omi (frame (US . alpha)))
0x0001-0x0000-0x0000-0x0001/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000
EOF_INPUT
```

Run it:

```bash
./opencode.bin --run /tmp/omi-inputs.txt
```

## Inspect geometry

```bash
./opencode.bin --geom
```

This prints the Polybius frame, QuQuart rails, Fano lines, BQF samples, frame constant, delta sample, QuQuart phases, and example 5040-slot calculation.

## Produce render artifacts

```bash
./opencode.bin --render-frame > /tmp/frame.json
./opencode.bin --render-ppm > /tmp/frame.ppm
```

`frame.json` is suitable for inspection and web consumption. `frame.ppm` is a binary image using no external image library.

## Serve and inspect HTTP endpoints

Start the server:

```bash
./opencode.bin --serve 8080
```

In another terminal:

```bash
curl http://127.0.0.1:8080/frame
curl http://127.0.0.1:8080/ring
```

Open the viewer at:

```text
http://127.0.0.1:8080/
```
