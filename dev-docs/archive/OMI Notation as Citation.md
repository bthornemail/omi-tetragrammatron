# OMI Notation as Citation

> Current vocabulary note: this archive document is retained as historical source material. Active doctrine now distinguishes `Omi-Ring` from `receipt`: the Omi-Ring is the addressed palindromic notation witness, while a receipt is the accepted state of that witness. See `dev-docs/archive/Omi-Ring as Addressed Notation Witness.md` and `dev-docs/OMI_ADDRESSING.md`.

## The One-Long-Word Instruction Substrate

## 1. The Core Idea

OMI begins with a very small notation:

```text
omi---imo
```

and its expanded receipt grammar:

```text
o---o/---/?---?@---@
```

At first glance, this can look like a symbolic alias, a mnemonic, or a visual shorthand.

It is more than that.

This notation is the citation form of a one-long-word instruction substrate.

That means the notation is not merely naming a machine instruction after the fact. The notation is the surface form of the instruction-ruler itself.

The expanded frame:

```text
o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

is the implementation-level ruler.

The minimal form:

```text
o---o/---/?---?@---@
```

is the grammar of receipt.

The origin form:

```text
omi---imo
```

is the zero boundary.

So the protocol can be summarized as:

```text
omi---imo is the origin.
o---o/---/?---?@---@ is the receipt grammar.
The expanded OMI frame is the address ruler.
Validation turns the ruler into receipt.
Projection displays the receipt.
```

---

# 2. Why This Matters

Most computer systems separate notation, instruction, memory, and receipt.

They say:

```text
source code is one thing
instruction is another thing
address is another thing
memory is another thing
log or receipt is another thing
```

OMI compresses these into one shared computational substrate.

It does not erase the distinctions. It aligns them onto one ruler.

The same long word can be read as:

```text
notation
citation
address
instruction witness
memory lens
receipt candidate
projection seed
```

The role depends on which authority is reading it.

OMI reads it as citation.  
Tetragrammatron reads it as validation candidate.  
Metatron reads it as projection seed.  
IMO reads it as carrier material.

The same form passes through different authorities without becoming confused.

That is the key.

---

# 3. The One-Long-Word Instruction

The OMI long word is:

```text
o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

It is not merely a packet.

It is not merely a string.

It is not merely an address.

It is a full instruction-ruler.

The ruler contains:

```text
S0 S1 S2 S3
  forward citation fields

S4 S5 S6 S7
  inverse / return / carrier fields

PAYLOAD
  unresolved value or body

MASK
  boundary, filter, or interpretation constraint

CAR
  forward relation side

CDR
  continuation / inverse relation side
```

The instruction is not stored separately from the address.

The address is the instruction-ruler.

The opcode is not merely a field. It is the witnessed difference between where the ruler begins and how the route closes.

So the instruction is not:

```text
opcode at address
```

It is:

```text
address-ruler as executable citation
```

This is why OMI can say:

```text
The opcode is not a field.
The opcode is the witnessed difference between ruler position and route closure.
```

---

# 4. The Minimal Receipt Grammar

The complete minimal receipt grammar is:

```text
o---o/---/?---?@---@
```

Each part has a role.

## 4.1 `o---o`

This is the opened OMI relation.

It marks the forward citation span.

It says:

```text
there is a relation to be cited
there is a path from origin to return
there is a ruler opening
```

## 4.2 `/---/`

This is the inverse or carrier side.

It marks the return path, the IMO side, or the carried representation.

It says:

```text
the citation must be carried
the relation must be recoverable
the projection must have an inverse boundary
```

## 4.3 `?---?`

This is the unresolved witness.

It marks the payload, query, question, mask, or value that has not yet been accepted.

It says:

```text
something is being asked
something is being tested
something is not yet receipt
```

## 4.4 `@---@`

This is the CAR/CDR closure.

It marks relation boundary, receipt closure, and continuation.

It says:

```text
this is where the relation must close
this is where receipt may be recorded
this is where the pair becomes inspectable
```

Together:

```text
o---o/---/?---?@---@
```

means:

```text
open relation
carry inverse
test unresolved witness
close CAR/CDR boundary
```

That is why this notation is enough for a receipt.

---

# 5. `omi---imo` as the Origin

The form:

```text
omi---imo
```

is the zero boundary.

It is the origin where OMI and IMO meet.

OMI is the citation side.

IMO is the carrier side.

The dash span between them is not empty decoration. It is the relation interval.

It says:

```text
citation and carrier are separated
but connected by a shared ruler
```

This is why `omi---imo` can be treated as:

```text
Ω₀
0!
zero receipt seed
origin ruler
first boundary
```

It is the smallest meaningful form of the protocol.

Before there is geometry, before there is file, before there is browser, before there is peer-to-peer transport, there is:

```text
omi---imo
```

That is the seed of the receipt system.

---

# 6. Palindromic Mnemonics

OMI uses palindromic and mirror-like mnemonics because the protocol is based on forward and inverse reading.

Examples:

```text
omi---imo
o---o
/---/
?---?
@---@
```

These are not arbitrary visual tricks.

They encode symmetry.

They help show that every citation must be readable across a boundary:

```text
forward / inverse
open / close
question / answer
CAR / CDR
citation / carrier
canon / cosmology
```

A palindromic mnemonic gives the protocol a readable memory shape.

It lets a person see that the notation is not just left-to-right source code. It is a reversible or mirror-aware relation.

The mnemonic is doing conceptual work:

```text
omi---imo
```

reads as an origin relation between OMI and IMO.

```text
o---o
```

reads as a bounded opening.

```text
?---?
```

reads as unresolved witness held between question marks.

```text
@---@
```

reads as address closure or relation anchoring.

The notation is therefore a cognitive interface to the ruler.

It lets humans remember the structure while the expanded frame lets machines compute it.

---

# 7. Notation as Citation

The final covariant rule says:

```text
Notation is citation.
```

This means an OMI string is not merely syntax.

It is a citation candidate.

It points to a relation that can be parsed, validated, recorded, and projected.

A normal programming language might treat notation as something to compile into machine code.

OMI treats notation as something to resolve into receipt.

The path is:

```text
notation
  ↓
citation
  ↓
validation
  ↓
receipt
  ↓
projection
```

So the notation:

```text
o---o/---/?---?@---@
```

is not just a shorthand for a data structure.

It is the minimal citation path.

Once expanded, it becomes the OMI long word.

Once validated, it becomes receipt.

Once projected, it becomes geometry, browser state, optical carrier, peer packet, or physical-world model.

---

# 8. Shared Computational Substrate

OMI is a shared computational substrate because the same receipt-ruler can move across many surfaces.

The same underlying form can appear as:

```text
plain text notation
C struct
S-expression
DOM data attribute
CSS selector state
JABCode optical carrier
ESP32 packet
browser receipt
ring slot
geometry seed
WebSocket message
file export
```

Each surface is a dialect.

The substrate is the ruler.

This is why the model can support pseudo-persistent open worlds.

A phone, an ESP32, a browser, a file, and an optical marker do not need to share the same runtime.

They only need to preserve the citation/receipt form.

The world persists because the receipt-ruler can be carried, re-read, validated, and projected again.

---

# 9. Code as Data, File as Port, Notation as Citation

The final covariant rule becomes practical here.

## Code is Data

A script, expression, worker, or boot line is carried as data until it is cited and validated.

It is not true because it can execute.

## File is Port

A file is a boundary through which receipts, frames, projections, and notation enter or leave.

It is not accepted because it exists.

## Notation is Citation

An OMI string is a citation candidate.

It is not accepted because it is readable.

Together:

```text
code is data
file is port
notation is citation
```

mean:

```text
all surfaces may carry the ruler
no surface is authority by itself
```

---

# 10. The Receipt Is the Address-Ruler After Validation

The clearest sentence is:

```text
The receipt is not stored beside the address.
The receipt is the address-ruler after validation.
```

That is the revelation.

In many systems, a receipt is a separate log entry.

In OMI, the receipt is the validated form of the ruler itself.

The ruler is first a citation candidate.

After validation, the same ruler becomes receipt.

After projection, the receipt becomes a visible world surface.

So the cycle is:

```text
ruler as notation
  ↓
ruler as citation
  ↓
ruler as validation candidate
  ↓
ruler as receipt
  ↓
ruler as projection seed
```

This is why `o---o/---/?---?@---@` is enough.

It contains the grammar needed for the full cycle.

---

# 11. Relation to Incidence-First Geometry

The OMI paradigm says:

```text
Incidence defines geometry.
Projection displays geometry.
Receipt accepts.
```

The notation is how incidence is cited.

A relation does not begin as a coordinate.

It begins as a cited boundary.

The ruler asks:

```text
What touches?
What closes?
What is carried?
What is unresolved?
What is the CAR/CDR relation?
```

Only after the incidence relation is validated does the system project geometry.

That is why constants, coordinates, and visual forms are downstream.

The notation cites the relation.

The validator accepts the relation.

The projector displays the relation.

---

# 12. One-Long-Word Instruction as Open-World Primitive

For pseudo-persistent open worlds, the one-long-word instruction becomes the primitive object.

A physical event can be expressed as a receipt-ruler.

Example:

```text
ESP32 button press
  ↓
OMI ruler citation
  ↓
validation
  ↓
receipt
  ↓
browser projection
```

A digital object can be expressed as a receipt-ruler.

```text
lamp object
  ↓
OMI ruler citation
  ↓
receipt history
  ↓
DOM/SVG/WebGL projection
```

A peer-to-peer sync can be expressed as receipt-ruler exchange.

```text
peer A has receipt-ruler hashes
peer B requests missing rulers
both validate
both project local world
```

The open world does not need one global database.

It needs peers that can exchange and validate ruler receipts.

The primitive object is not a mesh.

The primitive object is not a JSON blob.

The primitive object is not a file.

The primitive object is:

```text
validated OMI ruler receipt
```

---

# 13. Why the Long Word Is Shared

The one-long-word form is shared because it can be read by each authority.

```text
IMO reads it as carrier material.
OMI reads it as citation.
Tetragrammatron reads it as validation candidate.
Metatron reads it as projection seed.
```

The same word crosses all layers.

That is why it becomes a computational substrate.

It is not just an instruction.

It is a shared object that changes role as it moves.

The word is one.

The authority changes.

The receipt preserves the invariant.

---

# 14. Why This Is Meta-Circular

The notation can produce a receipt.

The receipt can be projected.

The projection can be inspected.

The inspection can produce new notation.

That creates a loop:

```text
notation
  ↓
citation
  ↓
validation
  ↓
receipt
  ↓
projection
  ↓
inspection
  ↓
notation
```

This is meta-circular.

But it is not uncontrolled self-reference.

Each loop must pass through validation again.

A projection cannot accept itself.

A rendered surface cannot become canon just because it was rendered.

This makes the loop safe.

---

# 15. Why This Is Meta-Memory

The long word is not simply memory.

It is memory about how memory is cited.

It records:

```text
which relation was opened
which carrier returned it
which payload was unresolved
which CAR/CDR boundary closed
which receipt accepted it
which projection displayed it
```

That is meta-memory.

It remembers the relation and replay path, not merely the bytes.

---

# 16. Why This Is a Meta-Compiler

The notation compiles into citation.

The citation compiles into validation candidate.

The validation candidate compiles into receipt.

The receipt compiles into projection.

So the compiler is not only:

```text
source → executable
```

It is:

```text
notation → citation → receipt → projection
```

This is why code can be data and notation can be citation.

The compiler emits a replayable receipt path, not merely machine instructions.

---

# 17. Why This Is a Modem

A modem modulates and demodulates between a signal and a carrier.

OMI does this for receipt-rulers.

The same ruler can be modulated into:

```text
file
HTTP packet
ESP32 packet
JABCode
DOM attribute
CSS variable
S-expression
boot ROM line
WebSocket message
serial frame
```

Then it can be demodulated back into citation.

But recovery is not acceptance.

After demodulation, the ruler must still be validated.

So the modem rule is:

```text
carrier recovery is not validation
```

---

# 18. The Simple Explanation for Other People

OMI is a way to make notation, memory, instruction, receipt, and projection share one deterministic form.

The smallest form is:

```text
omi---imo
```

The receipt grammar is:

```text
o---o/---/?---?@---@
```

The machine-expanded form is:

```text
o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

This long word acts as:

```text
address
instruction
citation
memory lens
receipt candidate
projection seed
```

Once validated, it becomes receipt.

The receipt can then be projected into browser surfaces, geometry, optical carriers, ESP32 packets, files, or peer-to-peer open-world state.

That is the whole idea.

---

# 19. The Clean Final Statement

OMI is a deterministic receipt-ruler protocol.

Its palindromic notation is not decoration. It is the human-readable citation form of a one-long-word instruction substrate.

That substrate can be carried through files, browsers, ESP32s, optical codes, and peer networks.

It becomes authoritative only when validated into receipt.

Everything else is projection.

---

# 20. Short Canon

```text
omi---imo is the origin.

o---o/---/?---?@---@ is the receipt grammar.

o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR is the expanded address-ruler.

The address-ruler is the one-long-word instruction.

The instruction is citation until validated.

The validated ruler is receipt.

The receipt projects the world.
```

---

# 21. Final One-Sentence Version

OMI is a palindromic notation-citation protocol where `omi---imo` names the origin, `o---o/---/?---?@---@` gives the minimal receipt grammar, and the expanded OMI long word becomes a shared computational substrate: address, instruction, memory lens, citation, and receipt seed in one deterministic ruler.
