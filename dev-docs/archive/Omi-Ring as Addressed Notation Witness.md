# Omi-Ring as Addressed Notation Witness

## Status

Canonical vocabulary correction for the current OMI protocol.

This document separates the term **Omi-Ring** from the broader and more ambiguous term **receipt**.

The word “receipt” can mean many things in ordinary computing:

```text
database row
audit log
transaction record
hash entry
acknowledgement
proof of payment
network confirmation
storage slot
event record
```

That range is too broad for the current OMI protocol.

OMI needs a term that names the specific protocol object:

```text
a bounded addressed notation citation
with forward/inverse readability
capable of validation, attestation, attribution, replay, and projection
```

That term is:

```text
Omi-Ring
```

---

## Definition

An **Omi-Ring** is the bounded addressed notation witness of an OMI relation.

It is the protocol form that makes a relation uniquely referenceable before, during, and after validation.

Canonical form:

```text
omi---imo
```

Local mnemonic forms:

```text
o---o
/---/
?---?
@---@
```

These are not decorative symbols.

They are palindromic notation surfaces for forward and inverse reading.

The Omi-Ring is therefore not merely a “receipt.”

It is the named protocol object that a receipt-like record points to, stores, replays, or projects.

---

## Why Not Just Say Receipt?

A receipt is a phase.

An Omi-Ring is the addressed object.

Use the distinction:

```text
Omi-Ring
  the bounded addressed notation witness

receipt state
  the accepted validation state of an Omi-Ring

receipt ring
  the storage / replay / coordination surface for accepted Omi-Ring states
```

So the corrected stack is:

```text
omi---imo
  ↓
Omi-Ring addressed notation witness
  ↓
candidate citation
  ↓
validation
  ↓
accepted receipt state
  ↓
receipt ring storage
  ↓
projection / scribing / carrier output
```

---

## Core Rule

```text
The Omi-Ring is the addressed notation witness.
The receipt is the accepted state of that witness.
The receipt ring stores accepted Omi-Ring states.
```

This avoids treating “receipt” as the primitive object.

The primitive protocol object is the addressed relation.

---

## Palindromic Mnemonic

OMI uses palindromic and mirror-like notation because every valid relation must support forward and inverse reading.

```text
omi---imo
```

means:

```text
OMI citation side --- IMO carrier side
```

The relation is readable from citation toward carrier and from carrier back toward citation.

The shorter forms name local relation surfaces:

```text
o---o
  object-to-object relation

/---/
  scope-to-scope relation

?---?
  query-to-witness relation

@---@
  CAR-to-CDR closure relation
```

These are all Omi-Ring forms.

They are not separate objects.

They are readable surfaces of the same addressed notation principle.

---

## Omi-Ring and Authority

An Omi-Ring does not accept itself.

An Omi-Ring makes a relation addressable.

Validation determines whether the relation is accepted.

The accepted state may then be stored, replayed, projected, or transmitted.

Canonical authority boundary:

```text
Recognition is not acceptance.
Citation is not acceptance.
Omi-Ring formation is not acceptance.
Projection is not acceptance.
Validation accepts.
Accepted state may be recorded.
```

---

## Updated Vocabulary

Prefer:

```text
Omi-Ring
Omi-Ring citation
Omi-Ring addressed notation
Omi-Ring witness
accepted Omi-Ring state
Omi-Ring storage ring
```

Avoid using “receipt” alone when the meaning could be confused.

Use “receipt” only when referring to the accepted validation state.

---

## One-Sentence Version

An Omi-Ring is the bounded addressed notation witness of an `omi---imo` relation; a receipt is only the accepted validation state of that Omi-Ring, and the receipt ring is only the storage and replay surface for accepted Omi-Ring states.
