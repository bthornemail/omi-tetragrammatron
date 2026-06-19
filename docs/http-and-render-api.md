# HTTP and render API

`opencode.bin` can expose the same deterministic twin state through terminal render modes and a small HTTP server. The HTTP server is carrier behavior: it transports the current runtime state to a browser or client, but it does not define the computation.

## Start the server

```bash
./opencode.bin --serve 8080
```

Use another port when needed:

```bash
./opencode.bin --serve 8090
```

## Routes

| Route | Purpose | Consumer |
| --- | --- | --- |
| `/` | Serves `viewer/index.html` | Browser |
| `/frame` | Serves the digital twin frame as JSON | Viewer, scripts, inspection tools |
| `/ring` | Serves receipt ring state as JSON | Viewer, scripts, inspection tools |
| Viewer asset paths | Serve files under `viewer/` | Browser module and script loading |

## `/frame` JSON

The frame endpoint mirrors `--render-frame`. It summarizes ring occupancy, folds, per-receipt twin geometry, frame counts, and Hopf flux.

High-level shape:

```json
{
  "ring_size": 5040,
  "filled": 0,
  "folds": {
    "xor": "0x0000",
    "sum": "0x0000",
    "rot": "0x0000"
  },
  "receipts": [],
  "frame_counts": {
    "US": 0,
    "GS": 0,
    "RS": 0,
    "FS": 0
  },
  "hopf_flux": [0, 0, 0]
}
```

Receipt geometry entries include cycle, hash-derived operation/result data, slot geometry, Polybius cell, frame type, and Hopf/quaternion projection fields.

## `/ring` JSON

The ring endpoint returns receipt slots that are currently filled. It is useful when a client needs receipt strings and raw hashes rather than only projected twin geometry.

Typical fields per filled slot:

```text
slot
cycle
hash
result
receipt
```

## Terminal render modes

Render a JSON frame without running the server:

```bash
./opencode.bin --render-frame > /tmp/frame.json
```

Render a binary PPM image:

```bash
./opencode.bin --render-ppm > /tmp/frame.ppm
```

The PPM renderer writes a direct image file. It should be redirected to a file rather than printed to the terminal.

## Client checks

After starting the server, inspect endpoints with:

```bash
curl http://127.0.0.1:8080/frame
curl http://127.0.0.1:8080/ring
```

If `/frame` returns valid JSON but the browser viewer does not update, inspect browser console output and confirm the page is loaded from the same host and port as the server.
