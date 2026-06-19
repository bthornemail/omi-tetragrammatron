# Deterministic geometry validation

The deterministic geometry update adds a self-checkable incidence layer around the geometry runtime. The goal is to make shape data, Fano routing, and solid lookup tables auditable from deterministic finite data rather than trusted by inspection.

## Check mode

Run the full deterministic geometry self-check suite with:

```bash
./opencode.bin --check
```

The mode reports four validation sections:

```text
Fano plane consistency
CONFIG_MATRIX / SHAPE_DB mapping
SHAPE_DB edge and vertex integrity
solid_lookup reachability
```

A passing check means the compiled geometry tables agree with the invariants encoded by the runtime.

## Correct Fano plane incidence

The Fano plane must contain seven unique lines, each with three points, where every point appears on three lines and every pair of points appears on exactly one line.

The corrected line set is:

```text
{0,1,2}
{0,3,4}
{1,3,5}
{1,4,6}
{2,3,6}
{2,4,5}
{0,5,6}
```

The previous duplicate line `{3,4,0}` is not valid because it repeats the same point set as `{0,3,4}`. The corrected `{0,5,6}` restores proper incidence.

`FANO_PT_LINES` is the dual lookup for point-to-line incidence. It must agree with `FANO_LINES`: each point maps to exactly three containing lines, and each listed line must contain the point it claims.

## Shape relation edge model

The geometry relation edge type is named `OmiRelEdge`. It stores endpoint citations rather than raw integer endpoints:

```text
OmiRelEdge
  a: Citation
  b: Citation
```

`ShapeDef.edges` uses this edge type. The rename from `E2` to `OmiRelEdge` documents intent without changing geometry behavior.

## Shape integrity checks

The edge-incidence checker validates each shape with finite graph and polyhedral invariants:

```text
sum(vertex_degrees) = 2 * edge_count
Euler characteristic: V - E + F = 2
vertex degrees remain inside expected range
edge endpoints reference valid vertices
```

The Euler check applies to ordinary convex polyhedra. Star compounds such as the stellated tetrahedron are exempted when the topology is intentionally not a simple convex polyhedron.

## Dodecahedron edge regeneration

The dodecahedron edge table is expected to have:

```text
20 vertices
30 edges
12 faces
```

The deterministic geometry update regenerates the dodecahedron edges by testing all 20-vertex pairs against the golden-ratio adjacency criterion:

```text
dot(vertex_a, vertex_b) = ψ + φ
```

This removes duplicate or incorrect edge entries and restores the expected 30-edge dodecahedral graph.

## Golden ratio from incidence

`tetra_golden_ratio()` derives φ from the recurrence:

```text
x = 1 + 1 / x
```

The implementation uses 48 iterations, giving the same double-precision result as the closed form while avoiding an imported `sqrt(5)` expression in the geometry path.

This recurrence is the algebraic signature of `{3,5}` / `{5,3}` icosahedral incidence, so φ is treated as a deterministic geometry-derived value rather than an external square-root constant.

## Deterministic review rule

Geometry tables should be accepted only when they pass their incidence checks. If a new solid, relation edge, or lookup table is added, update the check mode and this documentation in the same change.
