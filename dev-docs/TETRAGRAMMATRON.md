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

The next implementation boundary must keep validation and storage separate:

```text
validate candidate -> accepted-state object
store accepted-state -> ring entry
```

Validation may accept or reject a candidate.

Storage records an already accepted state.

Projection still waits for accepted state.

## Future API Direction

A future validation API should be named to preserve the boundary:

```c
int tetragrammatron_validate_citation_candidate(
    const OmiCitationCandidate *candidate,
    TetragrammatronAcceptedState *out
);
```

This function should not write the ring in the first pass.

Ring storage should remain explicit and separate.

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
Receipt accepts.
```
