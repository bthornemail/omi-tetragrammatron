# Getting started

## Requirements

- A C compiler such as `gcc`.
- The C math library linked with `-lm`.
- A browser for the optional WebGL viewer.

No package manager step is required for the C runtime.

## Build

From the repository root:

```bash
gcc -O2 -Wall -Wextra -Wno-misleading-indentation -o opencode.bin opencode.c -lm
```

## Smoke test

Run these commands after building:

```bash
./opencode.bin --eval '(cons 1 2)'
./opencode.bin --geom
./opencode.bin --boot
./opencode.bin --auto
./opencode.bin --twin
./opencode.bin --render-frame > /tmp/frame.json
./opencode.bin --render-ppm > /tmp/frame.ppm
```

Expected behavior:

- `--eval` emits a receipt for the input expression.
- `--geom` prints Polybius, QuQuart, Fano, BQF, and slot constants.
- `--boot` runs the boot ROM through the nibble CPU.
- `--auto` generates deterministic receipt cycles until a closure condition.
- `--twin` prints a digital twin universe summary.
- `--render-frame` writes a JSON frame.
- `--render-ppm` writes a binary PPM image.

## Serve the viewer

```bash
./opencode.bin --serve 8080
```

Then open:

```text
http://127.0.0.1:8080/
```

The server exposes `/frame` for JSON twin geometry and `/ring` for receipt ring data.
