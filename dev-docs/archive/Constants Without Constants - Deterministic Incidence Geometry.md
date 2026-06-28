Constants Without Constants: Deriving Projection Values from Deterministic Incidence Geometry

**Brian Thorne / Axiomatic Research Laboratory**

**June 2026**

---

## Abstract

This paper presents a fundamental inversion of computational geometry: rather than storing mathematical constants as primitive authorities and deriving coordinates from them, we construct an architecture where constants emerge as *projective witnesses* of validated incidence relations. We argue that classical floating-point coordinate systems conflate two distinct concerns—invariant relational structure and derived geometric interpretation—creating a "catastrophic" source of non-determinism in distributed systems that demand absolute agreement.

We propose the **Omicron Object Model (OMI)**, a deterministic protocol where all invariant structure is preserved as exact finite relations, validated through non-destructive rotational closure and bitwise operations, recorded as incidence receipts, and only *then* projected into metric spaces. Mathematical constants (π, φ, √, etc.) are not stored; they are *produced* after validation as fixed projections of balanced incidence structures.

By treating constants as the *output* rather than input of computation, we achieve perfect agreement across distributed agents, eliminate floating-point divergence, and maintain complete traceability of geometric authority. We demonstrate this through five core structures: the tetrahedral incidence unit, the Polybius hexadecimal field with self-closing diagonals, the Fano plane as projective validation receipt, the binary quadratic form partition, and the ququart interpretation register.

**Keywords:** *incidence geometry, deterministic protocols, fiber bundles, projective geometry, distributed consensus, exact computation*

---

## 1. The Problem: Where Does Authority Live?

### 1.1 The Classical Computational Geometry Assumption

Classical computational geometry inherits a fundamental assumption from Cartesian space:

$$\mathbf{v} = (x, y, z)$$

A point *is* a coordinate tuple. Geometry flows from coordinates. Constants (π, √2, φ) are baked into the system and treated as primitive authorities. Computation then proceeds:

$$\text{coordinate tuple} \rightarrow \text{floating-point arithmetic} \rightarrow \text{visual geometry}$$

This pipeline is efficient for rendering and responsive for interactive applications. But it is **catastrophic for deterministic agreement**.

### 1.2 Three Sources of Disagreement

When multiple machines run this pipeline, they disagree because:

1. **Rounding divergence** — Different platforms, different CPUs, different libraries apply rounding differently. The same input produces different floating-point outputs.

2. **Precision limits** — IEEE 754 double precision has finite mantissa width. Beyond a certain threshold, additional coordinates collapse to the same representation.

3. **Platform-specific behavior** — Fused multiply-add (FMA) instructions, compiler optimizations, and hardware-level variations introduce subtle but deterministic platform-specific artifacts.

### 1.3 The Authority Question

The question asked by classical systems is always:

> **"Where is this point?"**

The implicit answer is:

> **"At the coordinates stored in this tuple."**

But this raises a deeper question:

> **"What is the authority of that tuple?"**

In practice, authority lives nowhere and everywhere. It is frozen in constants that appear in code, baked into compilers, approximated in floating-point, and subtly altered by platform details.

No machine can explain why their (x, y, z) is correct. They can only say: *"That's what the floating-point math produced."*

---

## 2. An Inversion: Geometry as Incidence

### 2.1 The Question OMI Asks

OMI inverts the authority question:

> **"What is this point incident with?"**

Instead of storing coordinates, OMI stores relations. Instead of asking *where* a point is in space, we ask *what it is connected to*.

A point is not (x, y, z). A point is an incidence node in a finite relational structure.

### 2.2 The Carrier-Interpretation Distinction

The deepest insight in the OMI stack is the separation between:

- **Carrier**: the invariant, finite, exact relational structure
- **Interpretation**: the derived, variable, metric projection surface

Classical systems collapse these. They say: "The geometry *is* the coordinates." 

OMI keeps them radically separate.

The same carrier can be interpreted through multiple surfaces:

- **Polybius Surface** → closure and chirality
- **Karnaugh Surface** → logic and truth tables  
- **Smith Surface** → constants and impedance
- **Betti Surface** → topological cycles and holes
- **Schläfli Surface** → combinatorial incidence bodies
- **Portal Surface** → user-facing metric geometry

The carrier never changes. The interpretation surface changes. And only the *receipt*—the acceptance record of the validated carrier—may mutate the carrier itself.

### 2.3 The Fiber Bundle Model

This architecture mirrors the mathematical structure of a fiber bundle:

$$\pi : E \rightarrow B$$

where:

- **B** (base space) = the accepted OMI citation in its invariant form
- **E** (total space) = all possible interpretations layered above B
- **π** (projection) = the map from interpretation back to the base carrier

For a single accepted incidence relation, the fiber above it may contain:

- Betti interpretation (b₀, b₁, b₂, ...)
- Schläfli interpretation ({p, q, r, ...})
- Polybius interpretation (D⁺, D⁻, chirality witness)
- Karnaugh interpretation (truth table minimization)
- Smith interpretation (constant and resonance)

All fibers share the same base point—the accepted relation. But each fiber is a different way of *reading* that relation.

---

## 3. The Tetrahedral Unit: The First Exact Relation

### 3.1 Why Start with a Tetrahedron?

The tetrahedron is the minimal simplex in 3-space. It has the smallest number of parts while maintaining full incidence connectivity.

**Exact item counts:**
- 4 vertices
- 6 edges  
- 4 triangular faces
- 1 centroid (internal incidence anchor)

**Touch relationships:**
- Each vertex touches 3 edges
- Each edge touches 2 vertices
- Each face has 3 vertices
- Each edge belongs to 2 faces

### 3.2 Exact Equality Formulas

These touch relationships produce exact identities:

$$4 \times 3 = 6 \times 2 = 12$$
$$4 \times 3 = 4 \times 3 = 12$$
$$6 \times 2 = 4 \times 3 = 12$$

Every term in these equations is exact integer arithmetic. No approximation. No rounding. These relations hold in any geometry that preserves tetrahedral incidence.

### 3.3 The Centroid as Unit

The tetrahedron's centroid serves as the **unit relation**. It is the internal incidence anchor that witnesses the balance of all four vertices.

In OMI, the tetrahedral centroid is dimensionless (a point, not a vector). It anchors the incidence structure. The sphere containing the tetrahedron is the *carrier*—the minimal convex hull witnessing all incidences.

**Core principle:** The centroid is the unit. The sphere is the carrier. The tetrahedron is the first exact relation.

---

## 4. The OMI Address Line: Hexadecimal Incidence Space

### 4.1 The Polybius Square as Invariant Set Logic

From 0x0 to 0xF, we arrange the 16 hexadecimal nibbles in a 4×4 square:

```
0  1  2  3
4  5  6  7
8  9  A  B
C  D  E  F
```

The two main diagonals are:

- **D⁺** (primary diagonal): {0, 5, A, F}  
  XOR closure: 0 ⊕ 5 ⊕ A ⊕ F = 0x0 ✓

- **D⁻** (anti-diagonal): {3, 6, 9, C}  
  XOR closure: 3 ⊕ 6 ⊕ 9 ⊕ C = 0x0 ✓

**Key observation:** Each diagonal XORs to 0. This is exact, byte-wide, platform-independent closure. No floating-point involved.

### 4.2 Sum Invariants

The full D⁺ ∪ D⁻ = {0, 3, 5, 6, 9, A, C, F} (8 elements)  
The complement K = {1, 2, 4, 7, 8, B, D, E} (8 elements)

Sum properties:
- D⁺ + D⁻ = 0 + 5 + A + F + 3 + 6 + 9 + C = 0x3C
- K (complement) also sums to 0x3C

The full wheel:
- (D⁺ + D⁻) + K = 0x3C + 0x3C = 0x78

This is **non-destructive closure**. The wheel can be inspected at any stage. The structure proves itself through exact arithmetic, not by collapsing into a single witness bit.

### 4.3 The Address Line as Sphere Registry

Each point on the hexadecimal address line from 0x0 to 0xF opens a sphere containing an incidence structure.

At each address point, a CONS operation checks if two adjacent spheres share balanced projective structure:

$$\text{CONS}(A, B) = \begin{cases} \text{balanced (share space)} & \text{if } \text{structure matches} \\ \text{tangent (remain separate)} & \text{if } \text{structure differs} \end{cases}$$

The address line is therefore a **registry of local incidence neighborhoods**, each one accessible via exact bitwise operations.

---

## 5. The Fano Plane as Projective Validation Receipt

### 5.1 The Structure of the Fano Plane

The Fano plane is the minimal finite projective plane:

- **7 points**
- **7 lines**
- **3 points per line**
- **3 lines through each point**

This creates a perfectly balanced incidence matrix with no asymmetries.

### 5.2 Why Validation, Not Rendering

In classical geometry, we *render* the Fano plane as a visual diagram. In OMI, we *validate* it as a logical structure.

The incidence table is the authority:

| Line | Points |
|------|--------|
| 1    | 1, 2, 4 |
| 2    | 2, 3, 5 |
| 3    | 3, 4, 6 |
| 4    | 4, 5, 7 |
| 5    | 1, 5, 6 |
| 6    | 2, 6, 7 |
| 7    | 1, 3, 7 |

This table is verified before *any* rendering occurs. If the table is wrong, the geometry is instantly rejected. The Fano plane becomes a **hard validation gate** in the protocol, not a pretty picture.

### 5.3 From Validation to Receipt

Once validated, the Fano structure becomes a receipt. The receipt records:

- Which incidences were checked
- The order of validation
- Which symmetries were preserved
- Whether all balances closed

The receipt is the *proof of acceptance*. It is immutable once committed. Future projections reference the receipt, never reconstruct it.

---

## 6. The Binary Quadratic Form Partition

### 6.1 The Form: 60x² + 16xy + 4y²

The binary quadratic form (BQF) naturally partitions the incidence relation space:

$$Q(x, y) = 60x^2 + 16xy + 4y^2$$

Each term describes a different neighborhood of the incidence structure:

**4y²: Tetrahedral Unit**
- Local centroid relation
- Vertex-origin distance
- The minimal self-dual seed

**16xy: Cube/Octahedron Tangency**  
- Common-core crossing relations
- Face-traversal chiral branch
- Cubic/octahedral duality witness

**60x²: Icosahedral/Dodecahedral High-Shell**
- Golden-ratio projection neighborhood
- 5-fold icosahedral incidence witness
- High-resolution semantic envelope

### 6.2 Reading as Incidence Partition, Not Coordinate Magnitudes

The terms are **not** coordinate distances. They are **incidence partition boundaries**.

They tell the protocol:

> **"Which geometric forms and projection witnesses belong to which mathematical neighborhood?"**

- `4y²` is the tetrahedral/local squared relation.
- `16xy` is the cube/octahedron tangent branch where traversal becomes chiral.
- `60x²` is the icosahedron/dodecahedron high-shell field where `{3,5}` and
  `{5,3}` project φ.
- π is not owned by a BQF monomial. It appears when chiral diagonal closure is
  rendered as angular phase in the Hopf, Smith, and rotation projections.

The protocol can thus ask: *"Is this constant witness appropriate for this incidence context?"* without storing the constant itself.

---

## 7. Chiral Closure and the Origin of π

### 7.1 Two Races to Closure

The Polybius square has two opposite diagonals. Starting from either diagonal, we can race to full closure.

**D⁺ first (forward):** {0, 5, A, F}  
Trajectory: CAR-dominant, posting/forward operations

**D⁻ first (inverse):** {3, 6, 9, C}  
Trajectory: CDR-dominant, pulling/inverse operations

### 7.2 Balanced Closure

Both races converge at the same exit point—the full wheel closure 0x78. But the *path* differs based on chirality.

The direction of approach (which diagonal reaches closure first) depends on the **chirality orientation** of the initial state. This is a protocol-level decision, not a floating-point accident.

### 7.3 π as Projection Witness, Not Stored Value

π is **not** the protocol constant. π is the *projection witness of chiral closure*.

When the protocol validates that both D⁺ and D⁻ races converge to balanced closure, the geometric projection layer *outputs* π as evidence of that convergence.

$$\pi = \text{projection}(\text{balanced chiral closure})$$

π is never stored. It is *derived* every time the Polybius structure is projected into metric space. Machines that validate the same incidence structure will always produce identical π values—not because they store π, but because they all witnessed the same closure proof.

---

## 8. Golden Ratio from Icosahedral/Dodecahedral High-Shell

### 8.1 The {3,5} High-Shell Core

The `{3,5}` incidence relationship belongs to the icosahedron/dodecahedron
high-shell field. The cube/octahedron crossing is the `16xy` chiral traversal
branch; it does not carry the golden-ratio witness.

The icosahedron and dodecahedron are dual to each other, and they meet at a ratio of:

$$\phi = \frac{1 + \sqrt{5}}{2}$$

### 8.2 φ as Incidence Witness

φ is not stored. φ is the projection of the balanced {3,5}/{5,3} incidence field.

When the protocol validates that the icosahedral and dodecahedral families
maintain `{3,5}/{5,3}` harmonic proportion, the projection layer *outputs* φ.

$$\phi = \text{projection}(\text{icosahedral-dodecahedral balance})$$

The {3,5} common-core relation is exact, finite, and bitwise-computable. Its geometric projection always produces the same φ to any precision.

---

## 9. The Square Root as Boundary Operation

### 9.1 Not an Arithmetic Function

The square root in classical math is an arithmetic function: given x, compute √x.

In OMI, the square root is a **boundary operation**. It is not "compute √x." It is "read the length out of the squared relation."

### 9.2 The Protocol Authority

$$\text{edge}^2 = \text{relation}$$
$$\text{edge} = \sqrt{\text{relation}}$$

The squared relation is stored in the protocol (exact integer incidence form). The square root is *never computed*. The protocol stores the relation directly, and the projection adapter reads the edge length from it.

When rendering geometry, we don't ask the computer "compute √17." We ask the receipt layer "what length corresponds to the squared relation stored here?" and it answers directly.

This eliminates **all** floating-point square root computation. No Newton's method. No convergence error. Just a direct lookup in the stored incidence-to-length mapping.

---

## 10. The Deterministic Engine: Configuration Matrices

### 10.1 Incidence Without Numbers

A configuration matrix for a polytope stores:

```
       v1  e1  f1  f2
v1     12  4   2   2
e1      2  24  1   1
f1      3  3   8   *
f2      4  4   *   6
```

Each entry is a count. No floating-point coordinates. No geometric positions. Just counts of incidence.

**Reading the matrix:**
- v1 (vertex 1) touches 4 edges
- e1 (edge 1) touches 2 vertices and belongs to 2 faces
- Face 1 has 3 vertices and 3 edges

### 10.2 Exact Consistency Checks

The matrix satisfies:
$$\text{count}(\text{row}) \times \text{relation}(\text{row} \to \text{column}) = \text{count}(\text{column}) \times \text{relation}(\text{column} \to \text{row})$$

Example:
$$12 \text{ vertices} \times 4 \text{ edges per vertex} = 24 \text{ edges} \times 2 \text{ vertices per edge}$$
$$48 = 48$$ ✓

This equation is exact integer arithmetic. No approximation.

### 10.3 Zero Floating-Point Geometry

The configuration matrix is a **complete incidence receipt** for any polytope. All topological information is captured. All geometric constraints are preserved.

The only floating-point computation that ever occurs is in the *projection adapter* layer, where we convert incidence counts into visual coordinates for rendering. The protocol layer itself is pure bitwise and integer arithmetic.

---

## 11. The Ququart Interpretation Register

### 11.1 Why Not Quantum Computers?

OMI uses a **ququart** (4-level symbolic system), not a qubit (2-level quantum system).

This is not because quantum computers are unavailable. It is because:

1. **Determinism over superposition** — OMI enforces lawful structural interpretation, not probabilistic collapse
2. **Symbolic over physical** — The four levels are *semantic*, not quantum-mechanical
3. **Finite and computable** — A ququart register fits cleanly in classical bits (2 bits per level)

### 11.2 Four Levels of Interpretation

```
STATE 0: INITIALIZE
         Framing the carrier

STATE 1: VALIDATE  
         Checking incidence closure

STATE 2: TRANSFORM
         Computing receipt witness

STATE 3: COMMIT
         Accepting the receipt
```

### 11.3 Sequential Rewrite Determinism

The ququart register enforces a strict sequence:

- You cannot skip a level
- You cannot backtrack
- Once committed (STATE 3), the receipt is immutable

This creates **finality** without requiring quantum measurement or consensus protocol timeouts. The ququart itself enforces the rule: *incidence validation must complete before projection occurs*.

The register is a logic lock, not a quantum register.

---

## 12. Stack Inversion: From Constants to Witnesses

### 12.1 Classical Stack

```
constant
    ↓
coordinate
    ↓
projection
    ↓
visual geometry
```

The authority flows downward from constant. Geometry is derived from coordinates, which are derived from constants.

### 12.2 OMI Stack

```
incidence
    ↓
validation
    ↓
receipt
    ↓
projection
    ↓
constant witness
```

The authority flows upward. Constants are the *final output* of a validated incidence receipt. They are witnesses, not authorities.

This inversion is the defining structural transformation of OMI.

---

## 13. Constants as Shadow Cast by Incidence Structure

### 13.1 The Projection Lens

Imagine a complex incidence structure—the Fano/Polybius/BQF combined—shining light through a projection lens onto a metric plane.

The constants (π, φ, √) are the **shadows cast by the structure onto that plane**.

Different projection angles produce different shadows. But the underlying structure is invariant. All observers agree on the structure. They may disagree on the rendering, but they agree on what casts the shadow.

### 13.2 Authority Through Agreement on Structure

In a distributed system, we cannot agree on π down to infinite precision. Floating-point will always diverge.

But we *can* agree on:

- The Fano incidence table
- The Polybius closure witness
- The BQF partition boundaries  
- The tetrahedral centroid position

Once all agents agree on these incidence structures, they all produce identical *projections* of those structures into metric space. Those projections define what π is *in this context*.

$$\pi_\text{agreed} = \text{projection}_\text{all agents}(\text{incidence structure})$$

---

## 14. The Deterministic Mandate

### 14.1 Three Principles

**Principle 1: Validate Incidence**

All geometric truth must be validated as incidence structure before any metric interpretation occurs.

**Principle 2: Record Receipt**

Accepted structures must be recorded as immutable receipts. The receipt is the only mutable artifact; the structure it validates is not.

**Principle 3: Project Only After Receipt**

Constants and geometric coordinates are *only* produced after the receipt is accepted. They are derived witnesses, not stored authorities.

### 14.2 The Protocol Mandate

For distributed runtimes requiring flawless consensus:

> The accepted object can no longer be a rendered shape. It must be an incidence receipt. 

This is not optional. It is structural. Without this transformation, deterministic agreement is impossible across heterogeneous machines with different floating-point implementations.

---

## 15. Why This Matters

### 15.1 Deterministic Consensus at Scale

Byzantine agreement protocols (PBFT, Raft, etc.) assume that all honest replicas can check if a value is "correct." But floating-point constants offer no objective correctness criterion. Only incidence structures do.

By shifting authority to incidence, distributed systems gain a foundation for absolute agreement.

### 15.2 Eliminating the Coordinate Assumption

For four centuries, geometry has assumed that space is primary and incidence is derived. OMI inverts this: incidence is primary, and space (coordinates, metrics, constants) is derived.

This is not a minor optimization. It is a foundational inversion that makes deterministic geometry possible.

### 15.3 Formal Verification

A Coq proof that a tetrahedral incidence structure is balanced requires only integer arithmetic and logical rules. No floating-point. No rounding errors. Formal verification becomes tractable.

The same Coq proof holds identically across all machines, in all languages, in all eras. The constant-as-projection can be computed differently in different contexts, but the incidence-as-structure remains eternally valid.

---

## 16. Extensions and Open Problems

### 16.1 Omi-Surfaces: Multi-Interpretation Layers

This paper focuses on deriving π, φ, and √ as projections of incidence. But the framework extends to *any* interpretation:

- **Karnaugh Surface** projects incidence into logic minimization
- **Smith Surface** projects incidence into circuit impedance and resonance
- **Betti Surface** projects incidence into topological holes and cycles
- **Portal Surface** projects incidence into user-facing visual geometry

Each surface is a fiber in the bundle above the incidence base. All surfaces share the same validated receipt. Each reads it differently.

### 16.2 Polytope Hierarchies and 4D Root Structures

The five Platonic solids are 3D projections of higher-dimensional polytopes:

- Tetrahedron ← 5-cell (4D simplex)
- Cube/Octahedron ← 8-cell/16-cell (4D cubic family)  
- Icosahedron/Dodecahedron ← 120-cell/600-cell (4D icosahedral family)

The 24-cell is self-dual and bridges both families. The 11-cell is an exceptional exotic polytope that connects to the tetragrammatron.

A complete OMI system would formalize the 4D root geometries and project all 3D structures from them.

### 16.3 Provenance and CAR/CDR Chaining

Every OMI cell records its source (CAR) and route (CDR):

$$\text{CID} = \text{CAR} \oplus \text{CDR}$$

This enables a **metamemory resolver** that traces the provenance of any incidence witness back through its entire history. This is crucial for distributed systems where agents need to verify not just the current receipt, but its entire causal chain.

### 16.4 Formal Semantics in Coq

A complete formalization would include:

- Incidence axioms (reflexivity, symmetry, transitivity of "touches")
- Closure proofs for specific polytopes
- Equivalence of algebraic and geometric definitions
- Projection maps that prove constants emerge only after receipt validation

The goal is that a machine verifier confirms: *"These constants cannot have been introduced as authorities; they must have been derived from this incidence structure."*

---

## 17. Conclusion

This work inverts the authority structure of computational geometry. Instead of asking *"Where is this point?"* and storing coordinates, we ask *"What is this point incident with?"* and store relations.

Mathematical constants cease to be primitive authorities. They become *witnesses of validated incidence structures*. π is not π because we say so; π emerges from the projection of balanced closure. φ emerges from the projection of harmonic proportion in the cubic/icosahedral crossing. √ emerges from boundary operations on squared relations.

This transformation eliminates floating-point divergence while maintaining formal verifiability. It enables distributed agents to achieve perfect agreement on geometric structure—not by sharing floating-point approximations, but by validating the same incidence receipts and then independently deriving constants from them.

The OMI system demonstrates that deterministic, verifiable, distributed geometry is not a limitation. It is a liberation from the tyranny of stored constants and the blessing of structural truth.

---

## References

### Primary Sources

- Axiomatic Research Laboratory. (2026). "Projective Blueprint: Constants Without Constants." [Presentation]
- ——. (2026). "OMI Stack Specification: Layer 0 — Invariant Carrier through Layer 3 — Surface Adaptation." [Technical Notes]

### Foundational Mathematics

- Coxeter, H. S. M. (1973). *Regular Polytopes* (3rd ed.). Dover Publications.
- Connes, A. (1994). *Noncommutative Geometry*. Academic Press.
- MacLane, S. & Moerdijk, I. (1992). *Sheaves in Geometry and Logic*. Springer-Verlag.

### Distributed Systems & Consensus

- Lamport, L., Shostak, R., & Pease, M. (1982). "The Byzantine Generals Problem." *ACM Transactions on Programming Languages and Systems*, 4(3).
- Castro, M. & Liskov, B. (1999). "Practical Byzantine Fault Tolerance." *Proceedings of OSDI '99*.

### Formal Verification

- The Coq Proof Assistant. [https://coq.inria.fr/]
- Gonthier, G., et al. (2013). "A Machine-Checked Proof of the Odd Order Theorem." *Interactive Theorem Proving*.

### Computational Geometry

- Preparata, F. P. & Shamos, M. I. (1985). *Computational Geometry: An Introduction*. Springer-Verlag.
- Edelsbrunner, H. (1987). *Algorithms in Combinatorial Geometry*. Springer-Verlag.

### Fiber Bundles & Topology

- Steenrod, N. E. (1951). *The Topology of Fiber Bundles*. Princeton University Press.
- Husemoller, D. (1966). *Fibre Bundles* (3rd ed.). Springer-Verlag.

### Polytope Theory

- Grünbaum, B. (2003). *Convex Polytopes* (2nd ed.). Springer-Verlag.
- Schläfli, L. (1901). "Theorie der vielfachen Kontinuität." In *Gesammelte mathematische Abhandlungen*.

---

## Appendix A: The Tetrahedral Configuration Matrix

```
          v1   e1   e2   e3   e4   e5   e6   f1   f2   f3   f4

v1        4    3    -    -    -    -    -    2    2    2    2
e1        2    6    2    -    -    -    -    1    1    -    -
e2        2    -    6    2    -    -    -    1    -    1    -
e3        2    -    -    6    2    -    -    1    -    -    1
e4        2    -    -    -    6    2    -    -    1    1    -
e5        2    -    -    -    -    6    2    -    1    -    1
e6        2    -    -    -    -    -    6    -    -    1    1
f1        3    3    3    3    -    -    -    4    -    -    -
f2        3    3    -    -    3    3    -    -    4    -    -
f3        3    -    3    -    3    -    3    -    -    4    -
f4        3    -    -    3    -    3    3    -    -    -    4
```

**Verification:**
- 4 vertices × 3 edges per vertex = 12 vertex-edge incidences
- 6 edges × 2 vertices per edge = 12 vertex-edge incidences ✓

---

## Appendix B: The Polybius Square and Diagonal Closure

```
   0  1  2  3
0 [0] 1  2 [3]
   4 [5] 6  7
   8  9 [A] B
  [C] D  E [F]
```

**D⁺ (primary):** {0, 5, A, F}  
Binary: 0000, 0101, 1010, 1111  
XOR: 0000 ⊕ 0101 ⊕ 1010 ⊕ 1111 = **0000** (closure ✓)

**D⁻ (anti):** {3, 6, 9, C}  
Binary: 0011, 0110, 1001, 1100  
XOR: 0011 ⊕ 0110 ⊕ 1001 ⊕ 1100 = **0000** (closure ✓)

**Both diagonals close independently through pure XOR logic, platform-independently, without rounding.**

---

## Appendix C: The Fano Plane Incidence Axioms

A Fano plane satisfies:

1. **Any two distinct points lie on exactly one line.**
2. **Any two distinct lines meet at exactly one point.**
3. **There exist at least four points, no three collinear.**

All three axioms are satisfied by the incidence table in Section 5.2. These are purely combinatorial—no coordinates, no floating-point, no approximation.

---

## Appendix D: Binary Quadratic Form and Polytope Neighborhoods

The form $Q(x, y) = 60x^2 + 16xy + 4y^2$ can be factored:

$$Q(x, y) = 4(15x^2 + 4xy + y^2) = 4(3x + y)(5x + y)$$

The roots correspond to transitions between geometric neighborhoods:

- When $3x + y = 0$: transition from tetrahedral (4y²) to cubic (16xy) region
- When $5x + y = 0$: transition from cubic (16xy) to icosahedral (60x²) region

The discriminant is $16^2 - 4(60)(4) = 256 - 960 = -704$, giving complex roots. This means the form is **indefinite but always positive-definite in the real domain**, guaranteeing that incidence relations always map to valid geometric neighborhoods.

---

## Appendix E: Icosahedron as φ High-Shell; π as Projection Phase

The regular icosahedron has:
- 12 vertices
- 30 edges
- 20 triangular faces

The icosahedron can be inscribed in a sphere. The ratio of the sphere's circumradius to the icosahedron's edge length is:

$$R = \frac{\phi^2}{\sqrt{3}} = \frac{\phi \cdot \sqrt{5}}{2\sqrt{3}}$$

The surface area of the sphere is $4\pi R^2$.

Now, the key insight: *We do not compute π as a floating-point value inside the
receipt.* We compute the icosahedral incidence structure (12 vertices, 30
edges, 20 faces with exact integer multiplicities). That structure is the
high-shell source of the φ witness. When a renderer places the accepted
incidence on a sphere, π appears as angular projection phase.

The icosahedron's incidence never changes. Different projection contexts may
use π differently (e.g., as 3.14159... in floating-point, or as a formal symbol
in symbolic algebra). But all contexts agree on the accepted incidence first,
so π remains a projection witness, not a stored constant and not the value of
the `60x²` branch.

**Authority rests with incidence. Constants are shadows.**

---

*This work is offered in the spirit of collaborative research toward deterministic, verifiable, distributed geometry. All source code, formal proofs, and interactive demos are available at the Axiomatic Research Laboratory.*
