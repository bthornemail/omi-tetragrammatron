# OMI Tetragrammatron

A deterministic autonomous runtime built on four independent authorities
joined by a single invariant: the **receipt**.

```text
                    Metatron
             (Projection Authority)
                     ▲
                     │
OMI ◄──────────── Receipt ────────────► IMO
(Citation)                               (Carrier)
                     │
                     ▼
             Tetragrammatron
           (Validation Authority)
```

---

## The Four Authorities

### OMI — Citation

```
Authority:  Citation
Preserves:  Identity
Never:      Interpretation
```

Owns addresses, registers, CONS/CAR/CDR, CID, 16 opcodes, nibble CPU,
address parsing and formatting.

Creates invariant citations. Does not validate, project, or transport.

---

### Tetragrammatron — Validation

```
Authority:  Validation
Preserves:  Truth
Never:      Identity
```

Owns DeltaC, Polybius, Diagonal Law, Rotation, Governor, 5040 ring,
acceptance logic, three folds.

Reads citations. Produces accepted receipts. Does not render, serialize,
or parse.

---

### Metatron — Projection

```
Authority:  Projection
Preserves:  Interpretation
Never:      Truth
```

Owns geometry (seed crystal tables, SHAPE_DB[21], vertex coordinates,
quaternion/Hopf projection, Schläfli symbols, Betti numbers), all renderers
(JSON frame, PPM, SVG, OBJ, glTF, Smith chart), incidence self-check.

Reads accepted receipts. Produces surfaces. Does not validate, transport,
or mutate citations.

---

### IMO — Carrier

```
Authority:  Carrier
Preserves:  Representation
Never:      Identity
```

Owns ring serialization (file I/O), S-expression parser/compiler/reducer,
HTTP/SSE/WebSocket server, SHA-1, BOOT_ROM[21], signal handling, xmalloc.

Carries surfaces into runtime representations. Does not validate, compose
geometry, or change identity.

---

## Two Orthogonal Axes

### Horizontal — Representation

```
OMI ◄──────────────────────────────► IMO
Citation                              Carrier
```

Answers *How is something carried?*

OMI creates an invariant citation. IMO adapts that invariant into whatever
representation is needed (HTTP, file, GPIO, DOM, flash). Neither decides
truth. They preserve identity across representation changes.

### Vertical — Interpretation

```
Metatron
   ▲
   │
   │
   │
Tetragrammatron
```

Answers *What does an accepted citation become?*

Tetragrammatron decides whether something is accepted. Metatron decides
how an accepted receipt is projected into geometry, symbolics, or any
other surface. Neither changes the underlying identity.

---

## The Receipt

The receipt is the only object every authority may share.

```text
Citation → Validation → Receipt → Projection → Carrier → Surface
```

It is simultaneously:

- protocol witness
- provenance record
- replay unit
- synchronization unit
- geometric seed
- symbolic seed
- transport payload

Each authority may read the receipt. Everything else is private to its module.

---

## Architecture Flow

```
User → IMO → OMI → Tetragrammatron → Receipt → Metatron → IMO → Surface
```

1. IMO receives input (S-expression, OMI address, file, HTTP request)
2. IMO calls OMI to parse and produce a citation
3. OMI passes citation to Tetragrammatron for validation
4. Tetragrammatron produces an accepted receipt
5. Metatron reads the receipt and projects a surface
6. IMO carries the surface to its destination (stdout, file, HTTP response)

---

## Modules

| File                  | Authority   | Lines | Role |
|-----------------------|-------------|-------|------|
| `omi.h` / `omi.c`    | Citation    | ~180  | OMI addresses, CPU, FNV-1a |
| `tetragrammatron.h/.c`| Validation  | ~180  | Ring, folds, acceptance |
| `metatron.h` / `.c`   | Projection  | ~830  | Geometry, renderers, incidence |
| `imo.h` / `imo.c`     | Carrier     | ~850  | HTTP, parser, serialization |
| `opencode.c`          | Orchestration | ~400  | Thin main/CLI dispatch |

---

## Build

Requires a C compiler and the math library.

```bash
make              # build binary + Coq proof
make coq          # build Coq proof only
make smoke        # full smoke test (binary + JS tests)
make test-js      # run JS surface tests (JABCode + receipt cascade)
```

---

## Coq Proof

```bash
cd proof && coqc phi_proof.v
```

Proves:

- **phi_sq_eq** — φ² = φ + 1
- **icosa_forces_phi** — {3,5} edge-length equality forces φ
- **pi_gt_three** / **pi_lt_four** — rational bounds on π via Leibniz series

---

## Deterministic Core

```c
delta16(x, c) = rotl16(x, 1) ^ rotl16(x, 3) ^ rotr16(x, 2) ^ c
```

```text
BQF(x, y) = 60x² + 16xy + 4y²

slot5040 = fano7 × 720 + role3 × 240 + local240
```

The receipt ring: 5040 fixed slots, 4096 bytes each, circular.

Three folds across the ring: XOR, SUM, ROT.

---

## OMI Address Frame

```text
S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

16 fields × uint16. The address is the citation body.

---

## Modes

| Mode | Description |
|------|-------------|
| `--eval <s>` | evaluate one S-expression |
| `--repl` | interactive REPL |
| `--auto` | autonomous ring mode |
| `--boot` | run 21-stage boot ROM on nibble CPU |
| `--run <file>` | process addresses/expressions from file |
| `--cpu <addr>` | execute one OMI address on nibble CPU |
| `--geom` | print geometry constants |
| `--ring` | dump receipt ring |
| `--twin` | digital twin universe summary |
| `--watch` | live ring monitor |
| `--render-frame` | JSON geometry frame to stdout |
| `--render-ppm` | PPM image to stdout |
| `--render-obj` | OBJ mesh to stdout |
| `--render-gltf` | glTF JSON to stdout |
| `--smith` | Smith SVG to stdout |
| `--check` | incidence self-check |
| `--serve [port]` | HTTP server for WebGL viewer (default 8080) |

---

## Deeper Picture

The architecture mirrors a Riemannian bundle.

OMI notation is the **base space**. Each accepted receipt is a point in
that space. Metatron attaches a **fiber** to every receipt — a Smith chart,
a Platonic solid, a Schläfli interpretation, a GPIO layout, an SVG, an
ASCII projection. Changing the projection does not change the underlying
receipt.

The two dual pairs:

```text
{ OMI, IMO }             { Tetragrammatron, Metatron }
  Citation / Carrier        Validation / Projection
  Identity ↔ Representation Truth ↔ Interpretation
```

These are independent. Each pair preserves one invariant and passes the
receipt unchanged across its axis.

---

## Browser Surface View

The viewer at `/` is a cosmological surface, not a protocol authority.

### Projection Surfaces

All browser views derive from the same receipt. Each surface projects the receipt differently; none validates, accepts, or mutates canon.

```text
Receipt pair
  ↓
cosmological sphere / orientation surface
  ↓
gnomonic projection — flattens sphere to plane; great circles become straight lines
  ↓
Smith chart — continuous coordinate / impedance view
  ↓
mediant / determinant — rational route resolver; boundary witness
  ↓
mean ladder (HM ≤ GM ≤ AM ≤ QM) — surface pressure ordering
  ↓
Genaille ruler — discrete lane/band quantizer
  ↓
CSS cascade — declarative selector-resolution view
  ↓
DOM / SVG / WebGL paint
```

```text
GnomonicSurface
  normalizes sphere points
  projects onto tangent plane
  draws straight geodesic lines in minimap SVG
  does not validate, accept, or store state across receipts

ReceiptSurfaceCascade
  converts receipt to rational slope
  computes mediant of receipt pairs
  witnesses determinant boundary
  orders mean pressure ladder
  resolves lane/band for Genaille and CSS
  writes :root data attributes + CSS variables
  does not style elements — CSS selectors do that
```

### Final Covariant Rule

```text
Code is data.
File is port.
Notation is citation.
Canon validates.
Cosmology projects.
Receipt accepts.
```

### Practical Invariants

```text
Do not let rendered cosmology write canon.
Do not let file IO imply acceptance.
Do not let code execution imply truth.
Do not let notation imply receipt.
All browser views are cosmology.
All source files are ports.
All code emitted through the page is data until validated.
All OMI strings are notation until resolved as citations.
Only receipt validation promotes them into canon.
```
