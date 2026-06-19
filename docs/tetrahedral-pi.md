# Tetrahedral π carry

This page records the project interpretation that π is treated as a geometry-of-motion carry inside the OMI/Tetragrammatron model.

The claim here is not that π is historically new mathematics. The project shift is how π is represented operationally: π is documented as an internal transition process derived from tetrahedral traversal rather than as an unexplained external constant.

## Traditional view and OMI view

| Traditional view | OMI/Tetragrammatron view |
| --- | --- |
| π is a universal constant | π is a tetrahedron-derived carry |
| π exists outside the geometry being modeled | π emerges from tetrahedral edge/face structure |
| π is primarily a number | π is also a process: stepping through topology |
| π is passive | π actively bridges discrete and continuous state |
| π is imported into the model | π is derived by the model's geometry of motion |
| π is irrational in ordinary real-number form | π can be carried through sexagesimal-rational approximants inside this address system |

## Carry-forward interpretation

In this interpretation, π is the carry that lets a discrete OMI address transition project into smooth circular or spherical motion.

```text
0x0 -> 0xf
```

The discrete step is finite and address-like. The π carry is the bridge that lets that step become continuous when the tetrahedral edge/face traversal is projected onto a great circle.

```text
OMI address lattice
  -> tetrahedral edge/face traversal
  -> π carry forward
  -> great-circle projection
  -> sphere
  -> Hopf/quaternion projection
```

## OMI to IMO transition

The project language treats π as the transition between OMI and IMO address perspectives:

```text
OMI: discrete address, nibble, receipt, ring slot
IMO: inverted or motion-projected address, smooth path, sphere, fiber
```

π is the carry between those views. It does not replace the discrete address. It carries the address forward into continuous projection.

## Sexagesimal rational carry

A sexagesimal-friendly approximant can be written as:

```text
3;8,30 = 3 + 8/60 + 30/3600 = 377/120
```

This fits the project's preference for exact rational carrier values in base-60 style arithmetic. It is an approximant, not the final real-number identity of π.

## Deterministic role

The π carry must remain deterministic:

- It must be derived from explicit geometry state.
- It must not depend on host time, entropy, process identity, or scheduling.
- It should be expressible as exact integer or rational steps where possible.
- Any floating-point projection should be a presentation of deterministic state, not the authority for the state itself.

## Current implementation note

The current C and viewer implementations still use a literal `TWIN_PI` or host-language `Math.PI` for Hopf projection. This documentation captures the intended model direction: refine that imported projection constant into a deterministic tetrahedral carry process such as `tetra_pi()` when the runtime is updated.

Until that implementation exists, the authoritative runtime behavior is still the checked-in code.

## Relationship to existing geometry

The tetrahedral π carry sits between existing project layers:

```text
ring folds
  -> Fano routing
  -> BQF local bridge
  -> tetrahedral π carry
  -> Polybius / sphere projection
  -> Hopf / quaternion frame
```

The intent is to make π part of the same deterministic pipeline as Fano, BQF, Polybius, and Hopf rather than an external assumption.
