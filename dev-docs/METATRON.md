# METATRON

Metatron is the scribe-transducer of accepted OMI receipts.

It is not the validator.
It is not only the renderer.
It is not carrier I/O.

Metatron reads Tetragrammatron-accepted receipt state and adapts the invariant `omi---imo` relation into alternate notation surfaces.

## Authority Rule

```text
OMI cites.
Tetragrammatron validates.
Metatron scribes.
IMO carries.
Surfaces project.
Receipt accepts.
```

Metatron does not accept state. It does not validate truth. It does not mutate citation identity. It does not own files, network, browser ports, or hardware transport.

## Scribe Role

Scribing means deterministic receipt-to-notation transduction.

An accepted relation may be adapted into:

- CONS / CAR / CDR declaration forms
- OMI-Lisp notation
- geometry
- barcode carrier descriptions
- DOM or portal surfaces
- GPIO or hardware projection declarations
- symbolic notation
- projective surface records

Rendering is one output of scribing. Geometry projection is one surface family. The scribe role is broader: it names the boundary between accepted relation and projected form.

## Current Implementation

The current C core implements Metatron Scribe V0 in `core/metatron.h` and `core/metatron.c`:

- `MetatronSurfaceKind`
- `MetatronScribeRecord`
- deterministic surface name parsing
- deterministic receipt-to-notation scribing
- declaration-only surfaces for barcode, DOM, GPIO, symbolic, and projective targets
- `--scribe <surface>` CLI inspection through `core/omicron.c`

The current C core also implements the projection/composer subset:

- `SHAPE_DB[21]`
- seed solids and incidence data
- Schlaefli metadata
- Hopf/quaternion routing
- Polybius cell projection
- Smith chart projection
- JSON frame, PPM, SVG, OBJ, and glTF renderers
- incidence self-check

This is active V0 code, but it is not the full Omi-Surface registry or full OMI-Lisp declaration runtime.

## V0 Technical Debt

`core/omicron.c` currently uses local bridge declarations for `--scribe` integration because it remains a legacy monolithic source and cannot yet include the modular Metatron headers cleanly without conflicts.

This is acceptable for V0.

Future refactor target:
unify `core/omicron.c` with the modular `core/metatron.h` API so the CLI does not duplicate declarations.

## Future Runtime Contract

The full Metatron runtime should:

- read only accepted receipt state
- select declared notation/surface readings
- emit deterministic symbolic or geometric projection records
- keep validation in Tetragrammatron
- keep carrier I/O in IMO
- deny side-effect projection before receipt acceptance

## Lock

```text
Accepted relation stays invariant.
Metatron adapts the accepted relation into notation surfaces.
Projection does not accept.
Receipt accepts.
```
