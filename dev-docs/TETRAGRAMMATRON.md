# Tetragrammatron

Status: active validation guardrail

Tetragrammatron is the validation authority. It judges whether an OMI citation candidate becomes validated carry-forward state.

```text
Omi-Log declares.
Omicron normalizes and lowers.
OMI constructs citation candidates.
Tetragrammatron validates later.
```

## Authority Boundary

Tetragrammatron may:

- validate OMI citation candidates
- produce validated Omi-Ring transition state
- write validated-state ring entries
- export and import validated-state ring memory
- compute deterministic folds over validated-state ring memory

Tetragrammatron must not:

- parse Omi-Log source
- own Omicron dialect lowering
- own OMI citation construction
- project surfaces
- serialize carrier files
- perform browser, network, filesystem, or hardware carrier I/O

## Candidate Is Not Validation

These are not validation:

```text
OmiCitationCandidate
Omi-Log source block
Omicron lowered_candidate
Omi-Log normalized candidate head
carrier decode
projection request
```

Validation is the boundary between candidate state and validated carry-forward state.

Hashes, checksums, fingerprints, signatures, encrypted labels, and digest labels are not OMI identity, not Omi-Ring witnesses, not validation input, and not validated state.

## Validation And Storage Split

The implementation keeps validation and storage separate:

```text
validate candidate -> validated carry-forward state
store validated-state -> ring entry
```

Validation may validate or reject a candidate.

Storage records an already validated state.

Projection still waits for validated carry-forward state.

## V0 Validation API

The V0 validation-only API is:

```c
int tetragrammatron_validate_citation_candidate(
    const OmiCitationCandidate *candidate,
    TetragrammatronValidatedState *out
);
```

It constructs:

```c
typedef struct {
    uint16_t result;
    uint16_t relation_fold;
    uint32_t slot5040;
    uint8_t validated;
} TetragrammatronValidatedState;
```

This function does not write the ring.

## V0 Storage API

The V0 explicit storage API is:

```c
int tetragrammatron_store_validated_state(
    const TetragrammatronValidatedState *state
);
```

This function records an already validated state in the in-memory validated-state ring.

It does not validate candidates, parse source, project surfaces, serialize files, or perform carrier I/O.

Forbidden names for candidate-stage APIs:

```text
receipt_from_omilog
accept_omilog
project_omilog
proof_from_omilog
store_candidate_as_receipt
hash_citation_identity
```

Allowed direction:

```text
tetragrammatron_validate_citation_candidate
tetragrammatron_store_validated_state
```

## Lock

```text
OMI citation candidates are not validated state.
Hashing is not citation identity.
Tetragrammatron validates.
Validated carry-forward state may then be stored.
IMO serializes later.
Metatron projects later.
```
