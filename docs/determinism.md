# Determinism guide

The runtime is organized around a hard boundary:

```text
encode determinism
do not encode nondeterminism
```

This page explains how to apply that boundary when reading, testing, or extending the project.

## Deterministic computation

Computation must be derived from accepted input and deterministic state transitions. Current deterministic operations include:

- S-expression parsing and normalization.
- OMI address parsing.
- Lane event tape construction.
- FNV-1a hashing.
- XOR, masking, and wrapping arithmetic.
- 16-bit rotations.
- `delta16` reduction.
- OMI operation execution.
- Nibble CPU execution.
- Ring slot selection from `g_cycle % 5040`.
- Ring folds.
- Fano routing.
- BQF calculation.
- 5040-slot geometry mapping.
- Polybius cell mapping.
- Hopf/quaternion projection.
- Receipt string generation.

## Carrier behavior

Carrier behavior is permitted when it only moves bytes, stores state, or presents output. Examples:

- Reading terminal input.
- Writing receipt ring files.
- Handling signals to stop and save.
- Serving HTTP responses.
- Browser rendering.
- Redirecting JSON or PPM output to files.

Carrier behavior can fail because of the host environment. That failure should not become a hidden source of computation.

## Nondeterministic sources to avoid

Do not introduce computation that depends on:

```text
rand
random
srand
time
clock
getpid
uuid
/dev/urandom
```

The same rule applies to any equivalent entropy source, timestamp source, process identity source, or host scheduling behavior.

## Review questions for changes

Before merging a runtime change, ask:

1. Does the same accepted input produce the same receipt?
2. Does the same ring state fold to the same values?
3. Does geometry depend only on deterministic fields?
4. Does an HTTP or file operation only transport output, rather than select computation?
5. Are errors reported explicitly rather than silently changing reduction behavior?

## Documentation expectation

If deterministic behavior changes, update the relevant documentation page in the same change. Examples:

- Input grammar changes belong in `sexpr-and-omi.md`.
- Ring changes belong in `geometry-and-ring.md` and `runtime-data-model.md`.
- Mode changes belong in `cli.md` and `smoke-test-matrix.md`.
- Endpoint changes belong in `http-and-render-api.md` and `viewer.md`.
