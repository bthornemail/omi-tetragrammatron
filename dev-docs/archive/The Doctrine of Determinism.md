# The Doctrine of Determinism

## First Principles for the OMI Protocol and OMI Object Model

Version: `0.4.0`  
Status: Canonical Draft  
Protocol: `OMI---IMO`  
Authority Layer: Foundational Doctrine  
Applies to: OMI Object Model, OMI-Lisp, `.imo` carriers, repositories, LLM resolvers, adapters, boot envelopes, receipt rings, hardware targets, projection surfaces, and peer-to-peer exchange

---

## Abstract

The Doctrine of Determinism defines the first principles of the OMI protocol.

OMI does not claim that the external world is deterministic.

OMI claims that **accepted protocol state must have a bounded deterministic path**.

External events may be uncertain. Humans may disagree. Networks may delay. Hardware may fail. Sensors may vary. Renderers may mislead. LLMs may generate different language. Peers may accept different histories.

OMI does not deny any of this.

Instead, OMI draws an authority boundary:

```text
The world may be uncertain.
The accepted record must be reproducible.
```

The doctrine is built on one invariant:

```text
Recognition is not acceptance.
Citation is not acceptance.
Closure is not acceptance.
Projection is not acceptance.
Validation and receipt accept.
```

The complete deterministic path is:

```text
0xF* gauge row
  ↓
0x00..0x20 pre-language envelope
  ↓
FS/GS/RS/US tetrahedral scope body
  ↓
5-cell local Tetragrammatron resolution
  ↓
11-cell orientation/signing body
  ↓
5040-slot receipt coordination ring
  ↓
deterministic validation
  ↓
receipt
  ↓
permissioned projection
```

The doctrine is therefore not philosophical determinism.

It is **record determinism**.

---

# 1. First Principle: The World Is Not the Protocol

The world contains uncertainty:

```text
time
noise
humans
networks
hardware
sensors
models
renderers
filesystems
browsers
permissions
peer availability
external storage
```

OMI does not attempt to make all of these deterministic.

OMI only requires that accepted protocol state be deterministic.

Canonical rule:

```text
The world may vary.
The accepted record must not.
```

An event may happen.

A message may arrive.

A file may render.

A model may answer.

A device may respond.

A peer may disagree.

None of these are accepted OMI state until validation and receipt.

---

# 2. Operational Definition of Determinism

In OMI, deterministic means reproducible under declared inputs and declared rules.

A result is deterministic only when:

```text
given the same carrier,
given the same gauge,
given the same FS/GS/RS/US scope,
given the same OMI---IMO frame,
given the same candidate declaration,
given the same orientation rule,
given the same validation rule,
given the same effect policy,
given the same receipt chain,

the same validator reaches the same accepted or rejected result.
```

Short form:

```text
same input + same scope + same rule + same receipt chain = same accepted state
```

A process is not deterministic merely because it produced an output.

A process is deterministic only if the path from candidate to receipt can be replayed, inspected, and reproduced.

---

# 3. Authority Invariant

The immune system of OMI is the distinction between recognition and acceptance.

```text
Recognition is not acceptance.
Citation is not acceptance.
Closure is not acceptance.
Projection is not acceptance.
Validation and receipt accept.
```

This prevents common authority mistakes:

```text
the model said it
the screen rendered it
the packet arrived
the file exists
the graph found it
the hardware did it
the peer accepted it
the citation points to it
```

None of these are acceptance.

They are observations, candidates, projections, or remote facts.

Only validation and receipt accept local OMI state.

---

# 4. Recognition Is Not Acceptance

Recognition means a possible structure was noticed.

Examples:

```text
a parser recognizes a gauge header
a reader recognizes a citation
an LLM recognizes a pattern
a browser recognizes markup
a hardware adapter recognizes a command
a peer recognizes a remote receipt
```

Recognition opens inspection.

Recognition does not accept truth.

Canonical rule:

```text
Recognition opens the question.
It does not answer it.
```

---

# 5. Citation Is Not Acceptance

A citation points.

A citation does not prove.

OMI citation notation may locate a relation:

```text
omi:<gauge>/<fs>/<gs>/<rs>/<us>#<relation>
```

Example:

```text
omi:FF/README.md/adapters/webserial/open#effect.network
```

This says a relation has been cited under a scope.

It does not mean the relation is valid.

It does not mean the relation is authorized.

It does not mean the relation is accepted.

Canonical rule:

```text
Citation locates.
Validation judges.
Receipt accepts.
```

---

# 6. Closure Is Not Acceptance

Closure means a boundary was reached.

In OMI, `0x1E` / RS may witness record closure.

A parser may close a form.

A graph traversal may close a loop.

A symbolic route may close.

A receipt ring may locate a slot.

Closure is not acceptance.

Canonical rule:

```text
Closure says the shape is complete enough to inspect.
It does not say the shape is true.
```

The record-close witness:

```text
0x1E
```

means record closure.

It does not mean receipt acceptance.

---

# 7. Projection Is Not Acceptance

Projection shows, renders, writes, sends, or acts.

Projection surfaces include:

```text
Markdown
HTML
DOM
CSSOM
JSDOM
Canvas
SVG
WebGL
hardware
network
serial
USB
Bluetooth
MIDI
XML
RDF
semantic graphs
media streams
3D models
boot targets
```

Projection can be useful.

Projection can be convincing.

Projection is not acceptance.

Canonical trap:

```text
render → believe
```

LLM version:

```text
generate → believe
```

OMI forbids both as authority shortcuts.

Canonical rule:

```text
Projection may show.
Projection may not accept.
```

---

# 8. Validation Judges

Validation is the deterministic judgment stage.

A validation rule must declare:

```text
what input is being judged
what scope applies
what algorithm applies
what orientation applies
what effect class applies
what result counts as accepted
what result counts as rejected
```

Validation may use:

```text
a pure skill
a deterministic parser
fixed-width bit operations
receipt lineage
role/scope policy
hashes or signatures
adapter result declarations
```

But validation must be bounded.

Canonical rule:

```text
A validator must know what it is validating.
```

If the input, scope, width, endian rule, orientation rule, effect class, or acceptance rule is unknown, the result is not accepted.

---

# 9. Receipt Accepts

A receipt records the result of validation.

A receipt may accept.

A receipt may reject.

A receipt may preserve failure.

A receipt may point to a parent.

A receipt may bind:

```text
scope
effect
skill
hash
cycle
slot
role
orientation
result
parent receipt
```

Canonical rule:

```text
Accepted state begins at receipt.
```

A candidate without receipt remains a candidate.

A remote receipt proves remote acceptance.

It does not automatically prove local acceptance.

Local acceptance requires local validation or an explicitly accepted local trust policy.

---

# 10. Pre-Language Ordering

OMI is pre-language because topology comes before semantics.

Operational order:

```text
0xF* finds the carrier.
0x00 opens origin.
FS/GS/RS/US stages topology.
SP opens readable surface.
The 256-bit frame bounds relation.
Only then does semantic interpretation begin.
```

Canonical rule:

```text
Topology constrains semantics.
Semantics does not invent topology.
```

This is the core inversion.

Most systems begin with language and infer structure.

OMI begins with structure and constrains language.

```text
Most systems:
  Language → Grammar → Semantics → Scope → State

OMI:
  Gauge → Scope → Frame → Relation → Validation → Receipt
```

---

# 11. Gauge Row

The gauge row is:

```text
0xF0..0xFF
```

Generic pre-header form:

```text
G 00 1C 1D 1E 1F 20 G
```

where:

```text
G ∈ 0xF0..0xFF
```

Canonical OMI form:

```text
FF 00 1C 1D 1E 1F 20 FF
```

Readable:

```text
GAUGE NUL FS GS RS US SP GAUGE
```

The first and last gauge bytes must match.

The gauge row performs fast recognition and dialect selection.

It does not accept state.

Canonical rule:

```text
0xF* finds the frame.
Validation and receipt accept.
```

---

# 12. Pre-Language Control Envelope

The minimal control alphabet is:

```text
0x00  NUL  origin / zero-point
0x1C  FS   file scope
0x1D  GS   group scope
0x1E  RS   record scope / closure witness
0x1F  US   unit scope
0x20  SP   readable boundary
```

These controls stage topology before ordinary interpretation.

They define where a relation belongs.

They do not say whether it is true.

Canonical rule:

```text
Place first.
Relation second.
Interpretation third.
Validation fourth.
Receipt last.
```

---

# 13. OMI---IMO Frame

The canonical frame is:

```text
o-FS-GS-RS-US/FS/GS/RS/US?REGISTER?STACK@CAR@CDR
```

Machine form:

```text
o-S0-S1-S2-S3/S4/S5/S6/S7?REGISTER?STACK@CAR@CDR
```

Binary size:

```text
8 × 16-bit fields = 128 bits
4 × 32-bit fields = 128 bits
total             = 256 bits
```

The first 128 bits stage the scope/ruler cascade.

The second 128 bits stage the relation surface:

```text
REGISTER
STACK
CAR
CDR
```

Canonical rule:

```text
The 256-bit frame bounds the relation.
It does not accept it.
```

---

# 14. Dot Relation Notation

Once scope is established, relations are declared using dot notation:

```omi-lisp
(car . cdr)
```

Examples:

```omi-lisp
(effect . hardware)
(adapter . esp32)
(receipt.result . accepted)
(index.kind . hnsw)
(seed.video . o.video.o)
```

A relation is scoped by the active FS/GS/RS/US envelope.

The same relation under two different scopes is not the same memory relation.

Scope is part of identity.

---

# 15. Tetragrammatron Definition

In OMI, the Tetragrammatron is the four-scope naming body plus the validation anchor that resolves it.

Canonical definition:

```text
Tetragrammatron =
  FS / GS / RS / US
  plus
  V = validation / resolution anchor
```

Therefore:

```text
FS = file scope
GS = group scope
RS = record scope
US = unit scope
V  = validation anchor
```

The four scopes form the tetrahedral naming body.

The validation anchor lifts that tetrahedron into the 5-cell resolution body.

Canonical rule:

```text
The Tetragrammatron names scope through FS/GS/RS/US.
The 5-cell resolves that named scope through V.
```

The Tetragrammatron does not accept state.

Validation and receipt accept.

---

# 16. Tetrahedral Scope Body

The tetrahedral scope body is the direct operational mapping of FS/GS/RS/US.

Vertices:

```text
A = FS
B = GS
C = RS
D = US
```

Edges:

```text
AB = FS-GS
AC = FS-RS
AD = FS-US
BC = GS-RS
BD = GS-US
CD = RS-US
```

Faces:

```text
ABC = FS-GS-RS
ABD = FS-GS-US
ACD = FS-RS-US
BCD = GS-RS-US
```

Centroid:

```text
balanced OMI---IMO candidate relation under validation
```

Canonical rule:

```text
The tetrahedron stages scope.
The centroid is the candidate relation.
The receipt accepts or rejects the centroid state.
```

The tetrahedron is not merely a visual metaphor.

It is the incidence graph of the scope system.

---

# 17. 5-Cell Local Tetragrammatron Resolution

The 5-cell is the tetrahedral scope body plus the validation anchor.

```text
5-cell =
  FS
  GS
  RS
  US
  V
```

Operational flow:

```text
Input:
  FS/GS/RS/US tetrahedral scope body

Process:
  parse OMI---IMO relation
  construct candidate
  apply validation anchor V

Output:
  locally resolved candidate
```

Implementation mapping:

```text
FS/GS/RS/US scope      → parser scope
OMI---IMO frame        → bounded relation
dot notation           → candidate declaration
V validation anchor    → validate_candidate()
resolved candidate     → receipt candidate
```

Canonical rule:

```text
The 5-cell resolves local incidence.
It does not accept state.
Validation and receipt accept.
```

---

# 18. 5-Cell to 11-Cell Flow

The 11-cell does not replace the 5-cell.

It receives the 5-cell-resolved candidate and assigns bounded orientation.

Flow:

```text
tetrahedral scope body
  ↓
5-cell local resolution
  ↓
resolved candidate
  ↓
11-cell orientation binding
  ↓
oriented candidate
  ↓
validator
  ↓
receipt
```

More explicitly:

```text
5-cell input:
  FS/GS/RS/US scope

5-cell output:
  resolved local candidate

11-cell input:
  resolved local candidate

11-cell output:
  oriented candidate with signed relation flow

validator input:
  oriented candidate

validator output:
  accepted or rejected receipt
```

Canonical rule:

```text
The 5-cell resolves what the candidate is.
The 11-cell orients how the candidate flows.
The validator judges.
The receipt accepts.
```

---

# 19. 11-Cell Orientation Body

The 11-cell is the bounded orientation vocabulary for signed relation flow.

It defines eleven operational orientation positions.

These positions are not accepted states.

They are orientation labels that constrain validation.

The eleven positions are:

```text
1.  forward OMI direction
2.  inverse IMO direction
3.  CAR relation direction
4.  CDR continuation direction
5.  read polarity
6.  write polarity
7.  candidate polarity
8.  result polarity
9.  adapter projection polarity
10. receipt comparison polarity
11. rejection / rollback polarity
```

Canonical rule:

```text
The 11-cell orients signed relation flow.
It does not accept state.
Validation and receipt accept.
```

---

# 20. Eleven Orientation Positions

## 20.1 Forward OMI Direction

Meaning:

```text
OMI-side forward citation/address direction
```

Used by:

```text
OMI---IMO frame construction
forward scope traversal
candidate citation
```

Validation test:

```text
same input scope produces same forward OMI direction bit or label
```

Receipt binding:

```text
receipt may record forward-direction metadata
```

---

## 20.2 Inverse IMO Direction

Meaning:

```text
IMO-side inverse carrier/memory direction
```

Used by:

```text
carrier lookup
inverse resolution
continuation recovery
```

Validation test:

```text
same carrier and frame produce same inverse IMO reference
```

Receipt binding:

```text
receipt may record inverse-direction metadata
```

---

## 20.3 CAR Relation Direction

Meaning:

```text
primary relation pointer direction
```

Used by:

```text
CAR lookup
first declaration pointer
primary candidate relation
```

Validation test:

```text
CAR resolves to same declared relation under same frame
```

Receipt binding:

```text
receipt may record CAR reference
```

---

## 20.4 CDR Continuation Direction

Meaning:

```text
continuation, fallback, tail, or recovery direction
```

Used by:

```text
CDR lookup
continuation chain
fallback boot relation
recovery traversal
```

Validation test:

```text
CDR resolves to same continuation under same frame
```

Receipt binding:

```text
receipt may record CDR or parent/continuation reference
```

---

## 20.5 Read Polarity

Meaning:

```text
observation-only direction
```

Used by:

```text
read-only skills
inspection
citation
carrier parsing
```

Validation test:

```text
operation has no side effect and leaves accepted state unchanged
```

Receipt binding:

```text
receipt may record read-only effect class
```

---

## 20.6 Write Polarity

Meaning:

```text
state-changing direction
```

Used by:

```text
repo-write
local-write
hardware projection
network send
adapter output
```

Validation test:

```text
declared effect permits write direction and receipt gate exists
```

Receipt binding:

```text
receipt records effect class and permission result
```

---

## 20.7 Candidate Polarity

Meaning:

```text
unaccepted proposed relation
```

Used by:

```text
LLM output
peer proposals
parser output
adapter observations
unvalidated patches
```

Validation test:

```text
candidate is labeled candidate and not treated as accepted
```

Receipt binding:

```text
receipt may accept or reject candidate
```

---

## 20.8 Result Polarity

Meaning:

```text
reported result of a command, adapter, projection, or validation
```

Used by:

```text
adapter event declarations
hardware result declarations
validation result declarations
projection result declarations
```

Validation test:

```text
result relation is distinct from command relation
```

Receipt binding:

```text
receipt records result acceptance or rejection
```

---

## 20.9 Adapter Projection Polarity

Meaning:

```text
direction from accepted receipt toward external system
```

Used by:

```text
DOM update
serial write
GPIO projection
WebGL render
network send
hardware actuation
```

Validation test:

```text
projection_allowed() succeeds under role/scope/effect policy
```

Receipt binding:

```text
projection result must return as event candidate or event receipt
```

---

## 20.10 Receipt Comparison Polarity

Meaning:

```text
direction for comparing receipts, parents, slots, cycles, or peer histories
```

Used by:

```text
receipt ring lookup
parent receipt comparison
peer receipt sync
conflict inspection
lineage validation
```

Validation test:

```text
same receipt chain gives same comparison result
```

Receipt binding:

```text
receipt may record parent, slot, cycle, and lineage metadata
```

---

## 20.11 Rejection / Rollback Polarity

Meaning:

```text
negative or reverse acceptance path
```

Used by:

```text
rejection receipts
rollback candidates
fallback boot
conflict rejection
failed adapter result
```

Validation test:

```text
rejection or rollback is recorded as explicit relation, not deletion
```

Receipt binding:

```text
receipt records rejected, rolled-back, failed, or superseded result
```

---

# 21. Operational Meaning of Self-Duality

The 11-cell orientation body is self-dual in the operational OMI sense.

Self-dual means:

```text
the same orientation structure can be read forward or inverse
without changing the authority boundary
```

Operationally:

```text
OMI and IMO use the same frame shape.
CAR and CDR use the same relation form.
Command and result use the same declaration grammar.
Accepted and rejected receipts use the same receipt structure.
Forward and rollback paths are both explicit relations.
```

Self-dual does not mean:

```text
all directions are equivalent
all candidates are accepted
inverse lookup proves truth
projection proves state
```

Canonical rule:

```text
Self-dual means symmetric structure.
It does not mean automatic acceptance.
```

---

# 22. 5040-Slot Receipt Coordination Ring

The 5040-slot ring is a receipt coordination ring.

It is not global consensus.

Slot rule:

```text
slot5040 = fano7 × 720 + role3 × 240 + local240
```

The ring may support:

```text
receipt lookup
slot comparison
conflict detection
lineage inspection
peer synchronization
local ring replay
```

The ring does not decide truth.

Canonical rule:

```text
The receipt ring coordinates.
It does not globally decide.
```

A same-slot collision may indicate conflict.

It does not automatically decide the winner.

P2P agreement requires explicit peer policy, such as:

```text
local validation
trusted peer set
quorum rule
signature rule
slot ownership rule
receipt-chain comparison
```

Canonical rule:

```text
The 5040 ring coordinates receipts.
Local validation decides.
Receipts accept.
```

---

# 23. Fano7 Role in the Receipt Ring

The `fano7` field is the seven-way incidence selector used by the receipt coordination ring.

It provides a deterministic 7-value placement axis for receipt organization.

In the slot rule:

```text
slot5040 = fano7 × 720 + role3 × 240 + local240
```

the fields mean:

```text
fano7    = seven-way incidence selector
role3    = three-way role selector
local240 = local coordination slot
```

Together:

```text
7 × 3 × 240 = 5040
```

This is coordination metadata.

It does not accept state.

Canonical rule:

```text
fano7 places receipts.
It does not judge them.
```

---

# 24. Non-Determinism Is Pushed to Adapters

OMI does not deny non-determinism.

It relocates it.

Non-determinism may occur in:

```text
LLM generation
network timing
sensor input
hardware response
browser permissions
human approval
peer availability
floating-point rendering
camera or microphone capture
external filesystem behavior
```

These are adapter surfaces.

They may produce observations, candidates, events, failures, or projections.

They may not directly produce accepted state.

Canonical rule:

```text
Non-determinism may enter as observation.
It may not enter as acceptance.
```

The deterministic core receives adapter output only as a candidate or event relation.

---

# 25. Adapter Specification Requirement

Every adapter MUST declare:

```text
adapter name
adapter scope
external system connected
effect class
effect subtype
input declaration form
output declaration form
validation rule
failure rule
result receipt structure
projection behavior
platform permission requirement
```

Minimal adapter declaration:

```omi-lisp
(adapter.name . webserial)
(adapter.scope . fs.o/ADAPTERS.md/gs.o/browser/rs.o/webserial)
(adapter.effect . network)
(adapter.effect.subtype . serial)
(adapter.input . serial-command-declaration)
(adapter.output . serial-event-declaration)
(adapter.validation . skill.adapter.webserial.validate)
(adapter.permission . browser-user-grant-required)
(adapter.result . event-receipt-required)
```

An adapter command is a request for projection:

```omi-lisp
((effect . network) . ((adapter . webserial) . ((serial . write) . payload)))
```

An adapter result is a report about projection:

```omi-lisp
((event . serial-written) . command-receipt)
```

Canonical rule:

```text
Commands request projection.
Events report projection.
Receipts accept or reject projection results.
```

Adapters MUST NOT hide side effects inside pure declarations.

---

# 26. LLMs Under the Doctrine

An LLM is not authority.

An LLM is a resolver.

It may:

```text
read
recognize
cite
summarize
propose
normalize
generate candidates
request validation
```

It may not:

```text
accept state
invent tests passed
invent proofs
perform side effects before receipt
treat projection as truth
broaden its own role
```

Canonical LLM rule:

```text
LLMs do not need to share the same weights.
They can share the same reference memory protocol.
```

The shared layer is:

```text
carrier
+ FS/GS/RS/US topology
+ OMI---IMO frame
+ OMI-Lisp relation
+ validation rule
+ receipt
```

Different models may produce different prose.

They must obey the same authority boundary.

---

# 27. P2P Under the Doctrine

Peers exchange:

```text
carriers
candidates
receipts
```

Peers do not exchange truth.

Canonical P2P rule:

```text
Send does not accept.
Receive does not accept.
Validation accepts.
Receipt records.
```

A remote receipt proves what the remote peer accepted.

It does not automatically prove what the local peer accepts.

Each peer validates under its own `REPO.md`.

Disagreement is expected.

Disagreement is not protocol failure.

---

# 28. Deterministic Repositories

A deterministic OMI repository uses:

```text
REPO.md
AGENTS.md
SKILLS.md
ADAPTERS.md
BOOT.md
*.imo
receipts
```

Authority split:

```text
Doctrine      = why accepted state must be deterministic
REPO.md       = who / where / what may act
AGENTS.md     = how resolvers behave
SKILLS.md     = how computation reproduces
ADAPTERS.md   = how external systems connect
BOOT.md       = how first frame and bootstrap trust are staged
Receipts      = what is accepted
```

A repository is deterministic when:

```text
role rules are explicit
scope rules are explicit
skills are versioned
effects are declared
adapters are declared
bootstrap mode is declared
validation is reproducible
receipts are recorded
side effects are gated
```

A repository is not deterministic merely because it has source files.

It becomes deterministic when accepted changes are receipt-gated.

---

# 29. Layer Constraint Relation

The document stack is constrained as follows:

```text
Doctrine constrains REPO.md.
REPO.md constrains AGENTS.md.
AGENTS.md constrains resolver action.
SKILLS.md defines reproducible computation.
ADAPTERS.md defines external boundaries.
BOOT.md defines first-frame trust.
Receipts define accepted state.
```

Expanded:

```text
Doctrine → why
REPO.md → who / where / what
AGENTS.md → resolver conduct
SKILLS.md → deterministic computation
ADAPTERS.md → external systems
BOOT.md → first frame and bootstrap
Reference implementation → runtime mechanism
Receipts → accepted facts
```

Canonical rule:

```text
No layer may broaden authority beyond the layer above it.
```

---

# 30. Bootstrap Trust

Bootstrap chooses where local validation begins.

Bootstrap does not remove the need for validation.

There are five canonical bootstrap modes.

---

## 30.1 Local Genesis Bootstrap

A local genesis bootstrap starts a new repository or device from an initial local receipt.

Required declarations:

```omi-lisp
(bootstrap.mode . local-genesis)
(bootstrap.scope . fs.o/REPO.md)
(bootstrap.receipt . receipt-0000)
(bootstrap.result . candidate)
```

Operational rule:

```text
Local genesis creates a local starting point.
Local genesis does not bind other peers.
```

Validation:

```text
validate initial REPO.md
validate initial scope policy
store receipt-0000 if accepted
```

---

## 30.2 Known Seed Receipt

A known seed receipt bootstrap starts from a previously known receipt.

Required declarations:

```omi-lisp
(bootstrap.mode . known-seed)
(bootstrap.seed . receipt-known-good)
(bootstrap.source . local-policy)
(bootstrap.validation . required)
```

Operational rule:

```text
The seed is a reference point.
The local repository still decides whether to accept it.
```

Validation:

```text
check receipt format
check declared scope
check parent policy if present
check local trust policy
```

---

## 30.3 Signed Envelope Bootstrap

A signed envelope bootstrap starts from a signed 512-bit boot envelope or signed reference to one.

Required declarations:

```omi-lisp
(bootstrap.mode . signed-envelope)
(bootstrap.envelope . omi-boot-envelope)
(bootstrap.signature . signature-material)
(bootstrap.validation . signature-check)
```

Operational rule:

```text
Signature verifies origin or integrity.
Signature does not replace validation.
```

Validation:

```text
check gauge pre-header
check 256-bit bootstrap frame
check signature
check rollback policy
store accepted or rejected boot receipt
```

---

## 30.4 Hardware Root Bootstrap

A hardware root bootstrap begins from local hardware trust material.

Required declarations:

```omi-lisp
(bootstrap.mode . hardware-root)
(bootstrap.root . secure-storage)
(bootstrap.rollback . monotonic-counter)
(bootstrap.validation . hardware-policy-check)
```

Operational rule:

```text
Hardware root anchors local boot policy.
Hardware root does not make user data true.
```

Validation:

```text
check secure storage witness
check rollback counter
check boot envelope
check accepted boot receipt
```

---

## 30.5 Trusted Peer Bootstrap

A trusted peer bootstrap begins from another peer’s receipt under local trust policy.

Required declarations:

```omi-lisp
(bootstrap.mode . trusted-peer)
(bootstrap.peer . peer-id)
(bootstrap.receipt . remote-receipt)
(bootstrap.policy . local-trust-policy)
```

Operational rule:

```text
Trusted peer bootstrap is a local decision.
Remote acceptance is not local acceptance until policy validates it.
```

Validation:

```text
check peer identity under local policy
check remote receipt structure
check receipt scope
check local acceptance policy
store local bootstrap receipt if accepted
```

---

# 31. Platform-Agnostic Boot Envelope

OMI can be made flashable by wrapping the first frame in a 512-bit boot envelope.

```text
512 bits = 64 bytes
```

Layout:

```text
first 256 bits  = gauge / prefix / topology half
second 256 bits = OMI---IMO bootstrap frame
```

The first 8 bytes are the gauge pre-header:

```text
FF 00 1C 1D 1E 1F 20 FF
```

The second half is the first OMI---IMO frame.

Canonical boot rule:

```text
The prefix finds the frame.
The bootstrap stages the first relation.
Validation and receipt accept boot state.
```

This makes OMI platform agnostic.

Possible carriers include:

```text
eMMC
SD card
SPI flash
NOR flash
NAND flash
EEPROM
raw disk image
WASM memory
browser storage
firmware blob
network packet
binary stream
```

Any target that can expose the first 64 bytes can carry the OMI boot envelope.

---

# 32. eMMC as Target Carrier Example

eMMC is a target example, not the definition of OMI.

It provides a concrete carrier for the tetrahedral centroid memory model.

Useful eMMC-style faces:

```text
BOOT0
BOOT1
SECURE / RPMB-like authenticated storage
USER storage
```

OMI interpretation:

```text
BOOT0  = primary boot candidate face
BOOT1  = fallback boot candidate face
SECURE = receipt / rollback witness face
USER   = carrier / repo / document / media face
```

Canonical correction:

```text
eMMC is the target carrier.
The tetrahedron is the balance model.
The centroid is the memory unit.
The storage areas are incidence faces around that unit.
```

The flash does not create the tetrahedron.

The flash gives the tetrahedron somewhere to land.

---

# 33. Doctrine vs Implementation Boundary

Doctrine defines why accepted state must be deterministic.

Implementation defines how a runtime enforces that rule.

Doctrine says:

```text
headers do not accept
adapters do not accept
projection does not accept
validation and receipt accept
```

An implementation may provide:

```text
gauge_process()
bridge_resolve()
parse_form()
validate_candidate()
receipt_store()
projection_allowed()
hardware_project()
```

Boundary:

```text
Doctrine defines required behavior.
Implementation supplies one possible mechanism.
```

A different implementation may still conform if it preserves:

```text
same gauge recognition
same scope resolution
same frame interpretation
same orientation rule
same validation result
same receipt semantics
same projection gate
```

---

# 34. Fixed Width Is Doctrine

Bit operations have protocol meaning only under declared width.

A valid bit operation must declare:

```text
width
mask
rotation rule
endian rule when bytes are involved
overflow behavior
test vectors
```

Canonical rule:

```text
Width is part of the law.
Masking is part of the law.
Unbounded integers are not the OMI delta law.
```

A runtime that silently changes width changes the law.

That is a fault.

---

# 35. Delta Law Foundation

The current reduced delta law is:

```text
delta(x, C) = rotl(x,1) XOR rotl(x,3) XOR rotr(x,2) XOR C
```

The design choices inside the law are:

```text
rotations, not shifts
XOR
non-trivial constant C
fixed-width masking
```

Everything else is derived bookkeeping.

Canonical line:

```text
We chose the law.
The constants fell out.
```

The constants are not free-floating authority.

They do not accept state.

---

# 36. Doctrine Tests

A design follows the Doctrine of Determinism if it can answer:

```text
What is the carrier?
What is the gauge?
What is the scope?
What is the relation?
What is the tetrahedral scope body?
What is the 5-cell resolution rule?
What is the 11-cell orientation rule?
What is the candidate?
What is the validation rule?
What is the receipt?
What is the effect class?
What projection is allowed?
What side effects are forbidden before receipt?
```

A design fails the doctrine if it says:

```text
the model said it
the screen rendered it
the packet arrived
the file exists
the graph found it
the hardware did it
the peer accepted it
the citation points to it
```

without validation and local receipt.

---

# 37. Complete Stack Summary

```text
0xF* gauge row
  ↓
0x00..0x20 pre-language envelope
  ↓
FS/GS/RS/US tetrahedral scope body
  ↓
5-cell local Tetragrammatron resolution
  ↓
11-cell orientation/signing body
  ↓
5040-slot receipt coordination ring
  ↓
deterministic validation
  ↓
receipt
  ↓
permissioned projection
```

Layer roles:

```text
0xF* finds the frame.
0x00..0x20 stages the topology.
FS/GS/RS/US names the tetrahedral scope.
5-cell resolves local incidence.
11-cell orients signed relation flow.
5040 ring coordinates receipts.
Validation judges.
Receipt accepts.
Projection follows permission.
```

Acceptance table:

|Layer|Role|Accepts State?|
|---|---|---|
|Gauge row|Finds the frame|No|
|Pre-language envelope|Stages topology|No|
|Tetrahedral scope body|Names scope|No|
|5-cell resolution|Resolves local incidence|No|
|11-cell orientation|Orients signed relation flow|No|
|5040 ring|Coordinates receipts|No|
|Validation|Judges|No|
|Receipt|Accepts or rejects|Yes|
|Projection|Acts after permission|No|

---

# 38. Final Canon

```text
The world may be uncertain.
OMI does not deny this.

OMI only says:
accepted state must have a bounded deterministic path.

The Tetragrammatron is FS/GS/RS/US plus V.
The tetrahedron stages scope through FS/GS/RS/US.
The 5-cell resolves local incidence through the validation anchor V.
The 11-cell orients signed relation flow through eleven positions.
The 5040 ring coordinates receipts through slot assignment.
Validation judges.
Receipt accepts.
Projection follows permission.

The full path is:

0xF* gauge row
  ↓
0x00..0x20 pre-language envelope
  ↓
FS/GS/RS/US tetrahedral scope body
  ↓
5-cell local Tetragrammatron resolution
  ↓
11-cell orientation/signing body
  ↓
5040-slot receipt coordination ring
  ↓
deterministic validation
  ↓
receipt
  ↓
permissioned projection

No layer may broaden authority beyond the layer above it.

Recognition is not acceptance.
Citation is not acceptance.
Closure is not acceptance.
Projection is not acceptance.
Validation and receipt accept.
```

---

# 39. One-Sentence Summary

The Doctrine of Determinism states that OMI does not make the external world deterministic, but makes accepted protocol state reproducible by forcing every carrier, annotation, model output, peer message, boot frame, adapter action, and projection through a bounded path of gauge recognition, pre-language topology, FS/GS/RS/US tetrahedral scope, 5-cell local resolution, 11-cell orientation, 5040-slot receipt coordination, deterministic validation, and receipt-gated acceptance.