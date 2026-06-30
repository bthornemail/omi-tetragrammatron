# Constants as Projection Witnesses

OMI does not store mathematical constants as protocol literals.

The protocol order is:

```text
Finite incidence / relation first.
Projection boundary second.
Classical constant equality third.
```

A constant may appear only after a validated finite relation reaches a
projection boundary. The constant is therefore a witness of projection, not an
authority inside the protocol. Classical equality is a bridge theorem after
projection, not protocol authority.

Current checked lanes:

```text
π   = limit witness from chiral carry.
√3  = length witness from squared relation.
φ   = fixed-point witness with checked recurrence convergence.
```

## π Lane

π is derived as the real projection limit of accumulator-derived D+/D-
Polybius chiral carry.

```text
D+/D- diagonal closure
-> chiral accumulator phase
-> alternating projection stream
-> checked real limit
-> classical PI
```

## √3 Lane

√3 is derived as the projection name of a validated tetrahedral squared
relation.

```text
tetrahedral centroid-to-vertex squared relation = 3
-> projection boundary
-> sqrt 3 as rendered length witness
```

The protocol stores the squared relation, not the square root.

## φ Lane

φ is derived from validated `{3,5}/{5,3}` dual incidence and the rectified
`r{3,5}` common core.

```text
{3,5}/{5,3} incidence
-> r{3,5} common core
-> positive fixed point
-> recurrence x[n+1] = 1 + 1/x[n]
-> Fibonacci-ratio identity
-> checked convergence
-> classical phi
```

The recurrence is a projection witness. The incidence field remains protocol
authority.

## Rule For Future Constants

New constants must follow the same pattern:

```text
finite relation
-> projection witness
-> classical equality
```

They must not be introduced as protocol literals, trusted runtime constants,
hidden lookup values, hashes, or renderer assumptions.

Canonical short lock:

```text
No stored constants.
No admitted convergence.
No protocol pollution.

Finite relation first.
Projection witness second.
Classical equality third.
```
