# Troubleshooting

## Build warnings

The documented build command may print warnings from the existing C source while still producing `opencode.bin`:

```bash
gcc -O2 -Wall -Wextra -Wno-misleading-indentation -o opencode.bin opencode.c -lm
```

Treat a nonzero exit status as a build failure. Warnings with a zero exit status mean the binary was produced, but the warnings should still be reviewed before release-quality changes.

## Missing math symbols

If the linker reports missing math functions, confirm `-lm` is at the end of the command:

```bash
gcc -O2 -Wall -Wextra -Wno-misleading-indentation -o opencode.bin opencode.c -lm
```

## Ring appears empty

The ring file is stored at:

```text
/tmp/omi_receipt_ring.bin
```

Run an evaluation or autonomous mode first, then inspect the ring:

```bash
./opencode.bin --eval '(cons 1 2)'
./opencode.bin --ring
```

## Server port already in use

If port `8080` is unavailable, choose another port:

```bash
./opencode.bin --serve 8090
```

Then open:

```text
http://127.0.0.1:8090/
```

## Viewer shows waiting state

The viewer polls `/frame`. If it stays in a waiting state:

1. Confirm the server is running.
2. Open `/frame` directly in the browser.
3. Check that the browser is pointed at the same port passed to `--serve`.
4. Run an input or autonomous mode if the ring has no receipts yet.

## JSON frame is empty or sparse

`--render-frame` reflects the current ring. If there are few or no receipts, generate some first:

```bash
./opencode.bin --eval '(cons 1 2)'
./opencode.bin --render-frame > /tmp/frame.json
```

## PPM output looks like binary text

`--render-ppm` writes a binary PPM image. Redirect it to a file rather than printing it to the terminal:

```bash
./opencode.bin --render-ppm > /tmp/frame.ppm
```
