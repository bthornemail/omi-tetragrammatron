# Tetragrammatron

Status: active validation guardrail

Tetragrammatron is the validation authority. It is the only authority that may judge whether an OMI citation candidate becomes an accepted Omi-Ring state.

```text
Omi-Log declares.
Omicron normalizes and lowers.
OMI constructs citation candidates.
Tetragrammatron validates later.
```

## Authority Boundary

Tetragrammatron may:

- validate OMI citation candidates
- produce accepted Omi-Ring state
- write accepted-state ring entries
- export and import accepted-state ring memory
- compute deterministic folds over accepted-state ring memory

Tetragrammatron must not:

- parse Omi-Log source
- own Omicron dialect lowering
- own OMI citation construction
- project surfaces
- serialize carrier files
- perform browser, network, filesystem, or hardware carrier I/O

## Candidate Is Not Acceptance

These are not acceptance:

```text
OMI citation hash
OmiCitationCandidate
Omi-Log source block
Omicron lowered_candidate
Omi-Log normalized candidate head
carrier decode
projection request
```

Validation is the boundary between candidate state and accepted state.

## Validation And Storage Split

The implementation keeps validation and storage separate:

```text
validate candidate -> accepted-state object
store accepted-state -> ring entry
```

Validation may accept or reject a candidate.

Storage records an already accepted state.

Projection still waits for accepted state.

## V0 Validation API

The V0 validation-only API is:

```c
int tetragrammatron_validate_citation_candidate(
    const OmiCitationCandidate *candidate,
    TetragrammatronAcceptedState *out
);
```

It constructs:

```c
typedef struct {
    uint64_t citation_hash;
    uint16_t result;
    uint32_t slot5040;
    uint8_t accepted;
} TetragrammatronAcceptedState;
```

This function does not write the ring.

## V0 Storage API

The V0 explicit storage API is:

```c
int tetragrammatron_store_accepted_state(
    const TetragrammatronAcceptedState *state
);
```

This function records an already accepted state in the in-memory accepted-state ring.

It does not validate candidates, parse source, project surfaces, serialize files, or perform carrier I/O.

Forbidden names for candidate-stage APIs:

```text
receipt_from_omilog
accept_omilog
project_omilog
proof_from_omilog
store_candidate_as_receipt
```

Allowed direction:

```text
tetragrammatron_validate_citation_candidate
tetragrammatron_store_accepted_state
```

## Lock

```text
OMI citation candidates are not accepted state.
Tetragrammatron validates.
Accepted state may then be stored.
IMO serializes later.
Metatron projects later.
Receipt accepts.
```
