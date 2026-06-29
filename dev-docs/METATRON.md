# METATRON

Metatron is the scribe-transducer of validated Omi-Ring carry-forward states.

It is not the validator.
It is not only the renderer.
It is not carrier I/O.

Metatron reads Tetragrammatron-validated state of an Omi-Ring witness and adapts the invariant `omi---imo` relation into alternate notation surfaces.

## Authority Rule

```text
OMI cites.
Tetragrammatron validates.
Metatron scribes.
IMO carries.
Surfaces project.
Validation carries forward.
```

Metatron does not accept state. It does not validate truth. It does not mutate citation identity. It does not own files, network, browser ports, or hardware transport.

## Scribe Role

Scribing means deterministic validated-state-to-notation transduction.

A validated relation may be adapted into:

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
- `metatron_scribe_validated_slot`
- deterministic surface name parsing
- deterministic validated-state-to-notation scribing
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

## V0 Projection Gate

Metatron Scribe V0 reads stored validated-state ring entries.

The gate is intentionally stricter than "candidate exists":

```text
validated-state object -> ring entry -> Metatron scribe record
```

Metatron must reject candidate-only objects. A `RingSlot` must contain a
stored validated-state record before Metatron marks it validated or scribable.
Legacy metadata-only slots are not projection inputs.

Metatron does not validate.
Metatron does not store.
Metatron does not serialize.
Metatron does not perform browser, network, hardware, or filesystem effects.

## V0 Technical Debt

`core/omicron.c` currently uses local bridge declarations for `--scribe` integration because it remains a legacy monolithic source and cannot yet include the modular Metatron headers cleanly without conflicts.

This is acceptable for V0.

Future refactor target:
unify `core/omicron.c` with the modular `core/metatron.h` API so the CLI does not duplicate declarations.

## Future Runtime Contract

The full Metatron runtime should:

- read only validated Omi-Ring state
- read stored validated-state ring entries before projection/scribing
- select declared notation/surface readings
- emit deterministic symbolic or geometric projection records
- keep validation in Tetragrammatron
- keep carrier I/O in IMO
- deny side-effect projection before validated carry-forward state

## Lock

```text
Validated relation stays invariant.
Metatron adapts the validated relation into notation surfaces.
Projection does not accept.
Metatron reads stored validated state.
IMO serializes later.
Validation carries forward.
```

Compatibility note: runtime names such as `metatron_scribe_receipt` and
`MetatronScribeRecord` remain stable API names. In current doctrine, they mean
validated Omi-Ring state records, not primitive protocol objects.
`metatron_scribe_receipt` is the compatibility name; the explicit V0 gate name
is `metatron_scribe_validated_slot`.
