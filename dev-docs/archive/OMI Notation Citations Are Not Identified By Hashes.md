# OMI Notation Citations Are Not Identified By Hashes

## Status

Canonical doctrine.

## Core Rule

OMI notation citations are not identified by hashes.

The addressed place-value relation is the identity. A hash, checksum, digest, encrypted label, signature, fingerprint, cache key, or compressed surrogate is not an OMI identity and cannot become one inside the deterministic protocol core.

The identity of an OMI notation citation is the resolved addressed relation itself:

```text
omi---imo
```

and its lawful place-value cascade:

```text
(omi---imo . (o---o . (/---/ . (?---? . @---@))))
```

The notation is not a label for data. The notation is the addressable relation by which a framed reference moves from one state to another.

## Identity Is Place-Value, Not Sign-Value

Hashing belongs to a sign-value model of computation: a body of data is transformed into a fixed surrogate sign, and that sign is then used as a label, checksum, cache key, proof handle, or identity substitute.

OMI does not require that substitution.

In OMI, the addressed relation already carries its own place-value identity. The frame, route, witness, and closure are not arbitrary strings requiring a digest to become stable. They are the structured coordinate relation itself.

The protocol identity is therefore not:

```text
hash(citation)
digest(citation)
checksum(citation)
signature(citation)
encrypted(citation)
```

The protocol identity is:

```text
addressed place-value relation
```

A digest may describe that relation from outside the protocol, but it does not identify it from inside the protocol.

## The Cascade Of Addressability

The OMI notation cascade is read as a right-nested continuation:

```text
(omi---imo . (o---o . (/---/ . (?---? . @---@))))
```

This means:

```text
omi---imo  root atomic addressed relation
o---o      object/object compiled relation
/---/      scoped derivation path
?---?      witness/query/resolution relation
@---@      CAR/CDR closure continuation
```

Each wider envelope must remain resolvable back to the root relation:

```text
@---@ resolves through ?---?
?---? resolves through /---/
/---/ resolves through o---o
o---o resolves through omi---imo
```

The wider form does not replace the root. The wider form remains addressable because it carries forward the root.

This is the essential difference between OMI place-value identity and hash-based identity. A hash points away from the relation into a surrogate label. An OMI citation resolves through the relation itself.

## No Hash In The Protocol Core

The OMI protocol core must not require hashes for:

```text
identity
citation construction
validation input
validated transition state
carry-forward proof
ring storage
projection authority
deterministic recovery
```

A hash is not required for compatibility, correctness, recovery, validation, or routing inside the OMI protocol.

If a hash is introduced into the core, it creates a second identity channel beside the address. That is a category error. It makes the digest appear to preserve what the address already preserves directly.

The correct rule is:

```text
Address is identity.
Place-value relation is witness.
Validation judges the relation.
Carry-forward is proof.
Storage records the validated relation.
Projection reads only validated relation state.
```

## Hashing And Encryption Are External Carrier Behavior

External systems may hash, encrypt, sign, compress, index, or checksum an OMI notation citation for private reasons:

```text
transport secrecy
private indexing
legacy cache compatibility
debugging outside the core
external database storage
operator-defined archival policy
foreign protocol adaptation
```

Those transformed values are external carrier artifacts.

They are not:

```text
OMI identity
Omi-Ring witness
validation input
validated transition state
carry-forward proof
receipt state
projection authority
```

An external hash or encrypted value may be carried near an OMI citation, but it must not be confused with the citation itself.

If an operator chooses to hash or encrypt an OMI notation citation, they are operating outside the deterministic identity model of the protocol and do so at their own risk.

## Validation Does Not Accept A Digest

Validation does not judge whether a digest exists.

Validation judges whether an addressed place-value relation resolves under the declared deterministic rules.

The Tetragrammatron does not need a hash to validate. It needs the citation relation: frame, route, witness, closure, gauge, and carry-forward behavior.

The proof of validation is not a stored acceptance label. The proof is deterministic reuse: the validated relation can be carried forward through the meta-circular slide-rule nomogram and still resolves under the same gauge law.

Therefore:

```text
Validation judges relation.
Carry-forward proves validation.
Digest labels prove nothing inside OMI.
```

## Storage Does Not Store A Hash As Identity

A storage surface may store a validated relation state. It must not store a hash as the identity of that state.

The stored unit is not:

```text
hash -> accepted object
```

The stored unit is:

```text
validated place-value relation -> carry-forward state
```

Any compatibility field named `hash`, `digest`, `fingerprint`, or `checksum` must be treated as external metadata only, not as a protocol authority field.

The long-term protocol direction is to remove hash-bearing identity fields from the OMI and Tetragrammatron core.

## Projection Does Not Project A Digest

Metatron must not learn that a digest is the thing being projected.

Projection reads validated relation state and adapts it into notation, geometry, symbolic surfaces, DOM surfaces, barcode surfaces, or other declared projections.

Projection does not accept. Projection does not validate. Projection does not identify.

A digest is not a surface root. A digest is not geometry. A digest is not the Omi-Ring witness. A digest is not the address.

Projection begins only after validation of the addressed relation.

## Deterministic Obfuscation Is Not Hashing

If an operator wants local obscurity, locality, or offset behavior, the protocol should prefer declared deterministic coordinate offsets over hashes or encryption.

A declared offset remains part of the place-value relation if it is visible to the validating scope:

```text
public frame + declared local offset -> resolved coordinate citation
```

That is still deterministic.

A hidden hash or encrypted transform is different:

```text
citation -> hidden surrogate
```

That leaves the protocol core and becomes private carrier behavior.

OMI may support local scoped offsets as declared notation. OMI does not need hidden digest identity.

## Implementation Doctrine

Implementation should converge toward these rules:

```text
Do not derive identity from hashes.
Do not derive validation from hashes.
Do not derive slot selection from hashes.
Do not derive projection from hashes.
Do not use hashes as cache keys inside protocol identity.
Do not use hashes as compatibility checksums inside protocol validation.
```

Instead, derive protocol behavior from addressed place-value fields:

```text
root relation
frame
prefix
path
payload
mask
CAR
CDR
gauge
route
witness
closure
```

Any existing hash field in the codebase is scaffold debt unless it is explicitly quarantined outside protocol authority as carrier-only metadata.

## Canonical Lock

```text
OMI notation citations are not identified by hashes.

The addressed place-value relation is the identity.

Hashing, checksums, encryption, signatures, and digest labels are outside the deterministic identity model of the protocol.

External systems may hash or encrypt an OMI notation citation for private transport, secrecy, indexing, or compatibility.

That transformed value is not an OMI identity, not an Omi-Ring witness, not validation input, and not validated state.

It is an external carrier artifact used at the operator's own risk.
```

## Short Form

```text
Address is identity.
Place-value relation is witness.
Validation judges relation.
Carry-forward proves validation.
Hashing and encryption are outside protocol determinism.
```
