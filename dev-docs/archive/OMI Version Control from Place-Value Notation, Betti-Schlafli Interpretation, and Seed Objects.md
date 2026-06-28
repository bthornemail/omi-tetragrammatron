# OMI Version Control from Place-Value Notation, Betti–Schläfli Interpretation, and Seed Objects

## 1. The Core Claim

OMI version control is not merely a history of text edits.

It is a history of scoped declarations moving through:

```text
place
  ↓
citation
  ↓
validation
  ↓
receipt
  ↓
projection
```

A normal version-control system asks:

```text
What changed in this file?
```

OMI version control asks:

```text
What relation changed?
Where was it scoped?
Which carrier held it?
Which seed interpreted it?
Which topology did it project through?
Which receipt accepted it?
```

This means the version is not just the file state.

The version is the accepted receipt of a scoped transformation.

---

# 2. Place-Value Notation as Version Scope

OMI begins with place value.

The 32 non-printing controls stage 32 address places:

```text
0x00..0x1F → n00..n31
```

These are not readable text yet.

They are the place controllers.

Readable notation appears later.

So every version begins with the question:

```text
Which place changed?
```

Not merely:

```text
Which character changed?
```

A versioned object is therefore a place-value object:

```text
n00 n01 n02 n03
n04 n05 n06 n07
n08 n09 n10 n11
n12 n13 n14 n15
n16 n17 n18 n19
n20 n21 n22 n23
n24 n25 n26 n27
n28 n29 n30 n31
```

These 32 places group into the OMI ruler:

```text
S0-S1-S2-S3/S4/S5/S6/S7
```

So a version can be addressed as:

```text
file changed at the byte level
  ↓
declaration changed at the notation level
  ↓
place-value cascade changed at the OMI level
  ↓
receipt changed at the validation level
```

This gives a deeper version identity than a line diff.

---

# 3. FS/GS/RS/US as Version-Control Topology

The final four ASCII controls form the topology spine:

```text
FS = File Separator
GS = Group Separator
RS = Record Separator
US = Unit Separator
```

In OMI version control, these become scope boundaries:

```text
FS = file version scope
GS = group/module version scope
RS = record/declaration version scope
US = unit/value version scope
```

So instead of only naming files, OMI names where inside the file the version belongs.

```text
fs.o/project
  ↓
gs.o/skills
  ↓
rs.o/base64-seed
  ↓
us.o/alphabet
```

This is CIDR-like because each prefix narrows the valid interpretation.

Example:

```text
fs.o/SKILLS.md
fs.o/SKILLS.md/gs.o/encoding
fs.o/SKILLS.md/gs.o/encoding/rs.o/base64
fs.o/SKILLS.md/gs.o/encoding/rs.o/base64/us.o/alphabet
```

A change at `us.o/alphabet` is smaller than a change at `rs.o/base64`.

A change at `rs.o/base64` is smaller than a change at `gs.o/encoding`.

A change at `fs.o/SKILLS.md` changes the whole file scope.

This lets version control become scoped.

---

# 4. `o.o` as the Neutral Version Seed

The origin object is:

```text
o.o
```

This is the neutral seed.

It means:

```text
origin object
neutral closure
no specialized carrier interpretation yet
```

Every scoped encoding model can specialize `o.o`.

Examples:

```text
o.base64.o
o.hex.o
o.binary.o
o.utf8.o
o.jabcode.o
o.ipv6.o
```

Each one means:

```text
take the neutral origin object
scope it through a named interpretation
close it back into object form
```

So:

```text
o.base64.o
```

means:

```text
the Base64-scoped neutral seed object
```

It is not a payload.

It is not a decoded value.

It is the declared interpretation environment for payloads that will later arrive.

---

# 5. `omi-<base>-<64>-imo` as Citation Route

The matching citation route is:

```text
omi-base-64-imo
```

This says:

```text
OMI citation side
  ↓
base interpretation
  ↓
64-radix carrier parameter
  ↓
IMO carrier side
```

So the relationship is:

```text
o.base64.o
  = neutral seed object

omi-base-64-imo
  = citation route for Base64 carrier interpretation
```

Together:

```lisp
(o.base64.o . omi-base-64-imo)
```

means:

```text
the Base64 seed object is cited through the OMI/IMO Base64 route
```

This is perfect for version control because it separates:

```text
encoding scope
carrier route
payload instance
validation receipt
```

A file version may carry a Base64 string, but the version-control system knows whether that string was interpreted under `o.base64.o`.

---

# 6. Zero Polynomial Function Encoding Schema

The zero-polynomial seed means:

```text
no payload deformation has been accepted yet
only the interpretation scope has been declared
```

For Base64:

```text
o.base64.o
```

can be treated as:

```text
zero polynomial over Base64 carrier scope
```

Meaning:

```text
f₀(x) = 0 under Base64 interpretation context
```

This is not arithmetic pretending to be decoding.

It is a neutral function seed.

It says:

```text
This encoding scope exists.
No payload has yet been accepted through it.
```

Then an actual payload enters:

```text
encoded bytes
  ↓
omi-base-64-imo
  ↓
validate Base64 grammar
  ↓
decode through skill.base64.decode
  ↓
receipt accepts decoded relation
```

The seed is versioned separately from the payload.

This matters because the algorithm may evolve.

Version control must distinguish:

```text
Base64 schema changed
payload changed
decode algorithm changed
receipt changed
projection changed
```

---

# 7. Betti–Schläfli Interpretation

A version is not only textual.

It can also project as topology.

The Betti–Schläfli layer asks:

```text
What kind of shape does this accepted relation project as?
What holes, boundaries, shells, or incidence forms does it imply?
```

Schläfli symbols describe regular geometric structure:

```text
{3,3} tetrahedral relation
{4,3} cubical relation
{3,4} octahedral relation
{5,3} dodecahedral relation
{3,5} icosahedral relation
```

Betti numbers describe topological features:

```text
b0 = connected components
b1 = loops / tunnels
b2 = voids / enclosed shells
```

So a version can carry both:

```text
place-value address
receipt hash
scope prefix
encoding seed
Schläfli projection
Betti signature
```

Example:

```lisp
(version.seed . o.base64.o)
(version.route . omi-base-64-imo)
(version.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(version.schlafli . "{3,3}")
(version.betti . "(1 0 0)")
```

That says:

```text
this version of the Base64 seed is a connected, no-hole, no-void seed relation
```

If a later version introduces a cyclic dependency, it may project differently:

```lisp
(version.betti . "(1 1 0)")
```

meaning:

```text
one connected component
one loop
no enclosed void
```

This gives version control a topology of change.

---

# 8. Why This Is Better Than Plain Git Diff

Git records file snapshots and diffs.

OMI version control records scoped accepted relations.

Git can say:

```text
line 42 changed
```

OMI can say:

```text
the Base64 seed object's alphabet unit changed
under FS/GS/RS/US scope
inside SKILLS.md
using omi-base-64-imo carrier route
validated against skill.base64.alphabet
accepted into receipt slot N
projected with Betti signature (1,0,0)
```

Git tracks text.

OMI tracks relation acceptance.

The systems can work together:

```text
Git commit
  = carrier snapshot

OMI receipt
  = accepted semantic relation inside the snapshot
```

So Git remains useful.

OMI augments it with deterministic semantic versioning.

---

# 9. Version-Control Objects

An OMI version object should include:

```text
version id
parent receipt
scope prefix
seed object
citation route
carrier file
declaration hash
algorithm hash
validation rule
receipt handle
projection signature
Betti numbers
Schläfli symbol
logical cycle
```

Readable declaration:

```lisp
(version.id . receipt-042)
(version.parent . receipt-041)
(version.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64/us.o/alphabet)
(version.seed . o.base64.o)
(version.route . omi-base-64-imo)
(version.carrier . SKILLS.md)
(version.algorithm . skill.base64.decode)
(version.validation . tetragrammatron.validate)
(version.schlafli . "{3,3}")
(version.betti . "(1 0 0)")
(version.result . accepted)
```

This lets a peer reconstruct the meaning of the version without trusting the original machine.

---

# 10. AGENTS.md and SKILLS.md in Version Control

`AGENTS.md` versions the allowed behavior.

`SKILLS.md` versions the algorithms.

`.imo` files carry normalized declarations.

Receipts accept exact versions.

So:

```text
AGENTS.md
  controls resolver behavior

SKILLS.md
  defines reproducible algorithms

*.imo
  carries normalized scoped declarations

receipt ring
  records accepted versions
```

Example:

```lisp
(agent.allowed-skill . skill.base64.decode)
(agent.version-scope . fs.o/AGENTS.md/gs.o/permissions)

(skill.base64.decode.seed . o.base64.o)
(skill.base64.decode.route . omi-base-64-imo)
(skill.base64.decode.effect . pure)
(skill.base64.decode.version . v1)
```

A change to AGENTS.md may affect whether a skill is allowed.

A change to SKILLS.md may affect how the skill computes.

A change to `.imo` may affect the normalized carrier.

A receipt identifies which exact combination was accepted.

---

# 11. Declarative Version Transitions

A version transition is just a declared relation:

```lisp
(version.previous . receipt-041)
(version.next . receipt-042)
(version.delta . changed-base64-alphabet)
```

But it does not become accepted merely because the declaration exists.

It follows the pipeline:

```text
version delta declaration
  ↓
citation
  ↓
scope resolution
  ↓
algorithm validation
  ↓
receipt
  ↓
projection
```

That gives lazy version control.

A proposed version can exist without being accepted.

A branch can exist as candidates.

A merge can exist as a declared relation.

Only validation decides what enters the accepted receipt history.

---

# 12. Branches as Scope Forks

A branch is a scoped fork of receipt lineage.

```lisp
(branch.name . feature/base64-seed)
(branch.from . receipt-041)
(branch.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
```

The branch does not corrupt main history because it is scoped.

```text
main receipt line
  receipt-040 → receipt-041

branch receipt line
  receipt-041 → candidate-042a → receipt-043a
```

A merge becomes:

```lisp
(merge.left . receipt-043a)
(merge.right . receipt-044b)
(merge.scope . fs.o/SKILLS.md/gs.o/encoding)
```

Validation checks whether the merged relation is consistent.

If accepted, the merge gets a receipt.

---

# 13. Merges as Topological Gluing

This is where Betti–Schläfli interpretation becomes powerful.

A merge is not just text conflict resolution.

It is gluing relation surfaces.

If two branches edit disjoint units:

```text
branch A edits us.o/alphabet
branch B edits us.o/padding
```

then the merge may preserve simple topology:

```text
Betti = (1,0,0)
```

If two branches introduce circular dependency:

```text
skill.A depends on skill.B
skill.B depends on skill.A
```

then the merge may produce a loop:

```text
Betti = (1,1,0)
```

If a set of dependencies encloses a missing unresolved declaration, it may produce a void:

```text
Betti = (1,1,1)
```

This lets version control detect not only textual conflict, but structural conflict.

Schläfli projection can label the shape of the accepted relation:

```text
{3,3} minimal seed
{4,3} cubical dependency lattice
{3,4} octahedral reciprocal relation
{5,3}/{3,5} higher semantic shells
```

This is not replacing tests.

It augments tests with topology.

---

# 14. Seed Objects as Versioned Base Models

A seed object like:

```text
o.base64.o
```

acts like a base model.

It defines the neutral interpretation surface.

Other versions specialize it:

```text
o.base64.rfc4648.o
o.base64.urlsafe.o
o.base64.nopad.o
```

Each one can cite a route:

```text
omi-base-64-imo
omi-base-64-url-imo
omi-base-64-nopad-imo
```

Each one can have a skill:

```lisp
(skill.base64.rfc4648.decode . algorithm.base64.rfc4648)
(skill.base64.urlsafe.decode . algorithm.base64.urlsafe)
(skill.base64.nopad.decode . algorithm.base64.nopad)
```

This turns encoding schemas into versioned objects.

The seed says:

```text
what interpretation family is this?
```

The route says:

```text
how does it travel between OMI and IMO?
```

The skill says:

```text
how is it computed exactly?
```

The receipt says:

```text
which instance was accepted?
```

---

# 15. Version IDs as OMI Receipts

A version ID should not be only a hash of file text.

It should be a receipt handle.

Example:

```text
receipt:5040/2820
```

or:

```text
omi-version:fs.o/SKILLS.md/gs.o/encoding/rs.o/base64@receipt-2820
```

This identifies:

```text
scope
carrier
relation
receipt
```

A peer can resolve:

```text
Which file?
Which group?
Which record?
Which unit?
Which accepted receipt?
```

This is better than a plain hash because it includes the scoped interpretation path.

---

# 16. Networking Version Control

When a peer sends a version, it should send:

```text
carrier file diff
normalized .imo declaration
receipt candidate
parent receipt
skill references
algorithm hashes
projection signature
```

Receiver path:

```text
receive version packet
  ↓
verify carrier hash
  ↓
resolve FS/GS/RS/US scope
  ↓
load required skill definitions
  ↓
validate declaration
  ↓
compare parent receipt
  ↓
accept or reject locally
```

The receiver does not trust the sender.

The receiver validates.

This gives decentralized version control.

```text
send does not accept
receive does not accept
validation accepts
receipt records
```

---

# 17. LLM Version-Control Resolution

For LLMs, this is powerful.

An LLM often edits files by pattern.

OMI version control forces the edit into a scoped relation.

Example prompt:

```text
Update the Base64 seed object.
```

Resolver path:

```text
read AGENTS.md
  ↓
find allowed edit behavior

read SKILLS.md
  ↓
find base64 seed algorithm

resolve scope
  ↓
fs.o/SKILLS.md/gs.o/encoding/rs.o/base64

create version delta
  ↓
candidate relation

validate
  ↓
tests + scope + algorithm hash

receipt
  ↓
accepted version
```

The model does not simply “change code.”

It proposes a scoped version transition.

The accepted transition becomes receipt.

This makes LLM file editing reproducible.

---

# 18. The Perfect Version-Control Rule

```text
A version is not a file state.
A version is an accepted scoped relation over a carrier file.
```

Therefore:

```text
file diff = carrier delta
OMI declaration = relation delta
skill algorithm = computation delta
Betti/Schläfli = topology delta
receipt = accepted version delta
```

This lets the system track:

```text
what changed physically
what changed semantically
what changed algorithmically
what changed topologically
what was accepted canonically
```

---

# 19. Minimal Version-Control Declaration Format

A minimal OMI version declaration:

```lisp
(version . receipt-2820)
(version.parent . receipt-2819)
(version.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64/us.o/schema)
(version.seed . o.base64.o)
(version.route . omi-base-64-imo)
(version.carrier . SKILLS.md)
(version.algorithm . skill.base64.zero-polynomial)
(version.schlafli . "{3,3}")
(version.betti . "(1 0 0)")
(version.effect . pure)
(version.result . accepted)
```

A proposed change:

```lisp
(delta.intent . update-base64-schema)
(delta.parent . receipt-2819)
(delta.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64/us.o/schema)
(delta.seed . o.base64.o)
(delta.route . omi-base-64-imo)
(delta.effect . pure)
(delta.status . candidate)
```

Accepted change:

```lisp
(receipt.delta . receipt-2820)
(receipt.parent . receipt-2819)
(receipt.accepts . delta.intent)
(receipt.result . accepted)
```

---

# 20. Canon Statement

OMI version control treats every version as a scoped, validated relation.

`o.o` is the neutral origin object.

Objects such as `o.base64.o` are scoped seed models that define neutral encoding interpretation surfaces.

Routes such as `omi-base-64-imo` declare how the seed moves between citation and carrier.

FS/GS/RS/US provide the place-value scope hierarchy.

CIDR-like prefixes locate versioned definitions inside files and `.imo` carriers.

SKILLS.md defines the exact algorithms that make the relation reproducible.

AGENTS.md defines which version transitions are allowed.

Betti numbers describe the topology of the accepted relation.

Schläfli symbols describe the projected geometric class.

Receipts accept version transitions.

Therefore a version is not merely a file snapshot.

A version is an accepted scoped relation over a carrier file, interpreted through a seed object, validated by deterministic skills, and projected through topology.

---

# 21. One-Sentence Version

OMI version control uses `o.o` seed objects such as `o.base64.o`, citation routes such as `omi-base-64-imo`, FS/GS/RS/US place-value scopes, deterministic SKILLS.md algorithms, AGENTS.md permission environments, and Betti–Schläfli projection signatures so that every version becomes an accepted receipt of a scoped relation rather than merely a text snapshot.