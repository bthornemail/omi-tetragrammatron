# AGENTS.md, SKILLS.md, `.imo` Files, and Declaratively Decentralized Lazy Evaluation

## 1. The Core Idea

The clean architecture is:

```text
AGENTS.md  = scoped instruction environment
SKILLS.md  = deterministic algorithm registry
*.imo      = normalized carrier files
OMI-Lisp   = declaration language
receipt    = accepted result
LLM        = candidate generator / resolver / narrator
```

This turns an LLM from a single opaque actor into a resolver operating inside declared scope.

Instead of asking the model to “know what to do,” the system gives it:

```text
where it is allowed to act        → AGENTS.md
what algorithms it may apply      → SKILLS.md
where definitions are scoped      → fs.imo / gs.imo / rs.imo / us.imo
how declarations are represented  → OMI-Lisp blocks
when results become accepted      → validation + receipt
```

This creates **declaratively decentralized lazy evaluation**.

The model does not immediately execute.  
The model does not immediately accept.  
The model does not immediately mutate the world.

The model declares, resolves, proposes, and waits for validation.

---

# 2. Why AGENTS.md and SKILLS.md Are Enough

A separate `Agents.omi` and `Skills.omi` file can exist later, but it is not required as the first canonical form.

The simpler and stronger approach is:

```text
AGENTS.md carries human-readable agent constraints.
SKILLS.md carries human-readable skill descriptions and OMI-Lisp source blocks.
```

Markdown becomes the carrier.

OMI-Lisp blocks become the machine-readable declarations.

`.imo` files become normalized carrier outputs.

The receipt ring decides what was accepted.

So the file roles become:

```text
AGENTS.md
  tells the resolver how to behave

SKILLS.md
  tells the resolver what algorithms exist

fs.imo
  file-scope normalized declarations

gs.imo
  group-scope normalized declarations

rs.imo
  record-scope normalized declarations

us.imo
  unit-scope normalized declarations

omi.imo
  origin/citation carrier

receipts.imo
  exported accepted receipt surface
```

This follows the OMI rule:

```text
Code is data.
File is port.
Notation is citation.
Canon validates.
Cosmology projects.
Receipt accepts.
```

A Markdown file is not authority.  
A source block is not authority.  
An LLM answer is not authority.  
A skill declaration is not authority.

Only validated receipt accepts.

---

# 3. Declarative Decentralization

The system is decentralized because no single agent owns the meaning.

Each file contributes a scoped piece:

```text
AGENTS.md
  local behavior rules

SKILLS.md
  local algorithms and workflows

*.imo
  normalized declaration carriers

receipt ring
  local acceptance state
```

A peer, repo, browser, device, or project can carry its own `AGENTS.md` and `SKILLS.md`.

When another peer receives them, it does not blindly trust them.

It reads them as carrier files:

```text
receive file
  ↓
extract OMI-Lisp declarations
  ↓
normalize scope
  ↓
validate locally
  ↓
record accepted receipts
```

That is decentralized because each peer can validate locally.

It is not consensus-first.

It is receipt-first.

```text
peer A declares
peer B receives
peer B validates locally
peer B accepts or rejects locally
```

The LLM can participate as a resolver, but it cannot become the authority.

---

# 4. Lazy Evaluation

Lazy evaluation means a declaration does not immediately run.

A skill declaration such as:

```lisp
(skill.markdown.extract-blocks . parse-omi-lisp-blocks)
```

does not immediately parse every file.

It means:

```text
this relation exists as a candidate
this algorithm may be applied under scope
this action waits until invoked
this invocation must validate before receipt
```

The lazy path is:

```text
skill declaration
  ↓
citation candidate
  ↓
scope resolution
  ↓
algorithm selected
  ↓
execution plan proposed
  ↓
validation
  ↓
receipt
  ↓
projection or side effect
```

This matters for LLMs because LLMs are naturally eager to complete a request.

OMI makes them lazy:

```text
Do not act because you can.
Act only after the scoped declaration resolves.
Accept only after validation.
Project only after receipt.
```

So for an LLM:

```text
generate answer
```

becomes:

```text
recognize request
  ↓
cite relevant instructions from AGENTS.md
  ↓
cite relevant algorithms from SKILLS.md
  ↓
resolve scope through .imo files
  ↓
propose candidate operation
  ↓
validate constraints
  ↓
emit accepted result or rejection receipt
```

---

# 5. AGENTS.md as Scope Environment

`AGENTS.md` should be treated as the local resolver environment.

It answers:

```text
Who is acting?
What constraints apply?
What files are in scope?
What actions are forbidden?
What tests must run?
What side effects require confirmation?
What style or canon rules must be preserved?
```

In OMI terms, `AGENTS.md` is not just prose.

It is an environment declaration carrier.

Example:

```omi-lisp
(agent.role . resolver)
(agent.scope . fs.o/project)
(agent.must.validate . before-project)
(agent.must.not . side-effect-before-receipt)
(agent.must.cite . source-before-claim)
(agent.failure . return-rejection-receipt)
```

The model reads this as:

```text
I am not free-floating.
I am inside a declared environment.
This environment bounds what I can resolve.
```

So the LLM is scoped.

It is not merely “an assistant.”

It is a resolver inside a local environment.

---

# 6. SKILLS.md as Algorithm Registry

`SKILLS.md` should be the repeatable workflow and algorithm registry.

It answers:

```text
What deterministic procedures exist?
What inputs do they accept?
What outputs do they produce?
What scopes do they apply to?
What validation must happen?
What files do they read?
What files may they write?
```

Example:

```omi-lisp
(skill.extract.omi-blocks . algorithm.markdown-fence-scan)
(skill.extract.omi-blocks.input . fs.o/SKILLS.md)
(skill.extract.omi-blocks.output . rs.o/omi-lisp-declarations)
(skill.extract.omi-blocks.effect . read-only)

(skill.normalize.scope . algorithm.fs-gs-rs-us-fold)
(skill.normalize.scope.input . omi-lisp-declarations)
(skill.normalize.scope.output . fs.imo/gs.imo/rs.imo/us.imo)

(skill.validate.declaration . tetragrammatron.validate)
(skill.validate.declaration.output . receipt-candidate)
```

This makes skills reusable.

The LLM does not invent the algorithm every time.

It resolves the skill by address.

---

# 7. `.imo` Files as Normalized Carriers

The `.imo` files are the normalized carrier outputs.

The chain is:

```text
AGENTS.md / SKILLS.md
  ↓
extract OMI-Lisp source blocks
  ↓
normalize declarations
  ↓
write .imo carrier files
  ↓
validate declarations
  ↓
record receipts
```

The normalization path you described is:

```text
fs.omi → fs.imo → fs.o
gs.omi → gs.imo → gs.o
rs.omi → rs.imo → rs.o
us.omi → us.imo → us.o
omi.imo → o.o
```

This means:

```text
fs.o = file-scope object
gs.o = group-scope object
rs.o = record-scope object
us.o = unit-scope object
o.o  = origin object
```

The `.omi` side is the citation side.  
The `.imo` side is the carrier side.  
The `.o` side is the reduced object surface.

So a skill can be declared once in `SKILLS.md`, normalized into `.imo`, and reduced into `.o` scope for deterministic lookup.

---

# 8. CIDR-Like Addressing for Skill Scope

CIDR-like addressing gives skills bounded scope.

A skill address can look like:

```text
fs.o/skills/gs.o/markdown/rs.o/blocks/us.o/omi-lisp
```

Read it as:

```text
file scope: skills
group scope: markdown
record scope: blocks
unit scope: omi-lisp
```

This lets the resolver ask:

```text
Which file scope am I in?
Which group of skills applies?
Which record is being resolved?
Which unit algorithm should execute?
```

Examples:

```omi-lisp
(skill.address . fs.o/skills/gs.o/markdown/rs.o/blocks/us.o/omi-lisp)
(skill.intent . extract-omi-lisp-source-blocks)
(skill.input . fs.o/SKILLS.md)
(skill.output . rs.o/declaration-candidates)
(skill.effect . read-only)
```

Another example:

```omi-lisp
(skill.address . fs.o/project/gs.o/tests/rs.o/smoke/us.o/make)
(skill.intent . run-smoke-tests)
(skill.input . fs.o/Makefile)
(skill.output . rs.o/test-receipt)
(skill.effect . local-process)
(skill.requires . receipt-before-claim)
```

This is close to CIDR because each prefix narrows the scope.

```text
fs.o/project
  contains all project file-scope declarations

fs.o/project/gs.o/tests
  narrows to test workflows

fs.o/project/gs.o/tests/rs.o/smoke
  narrows to smoke-test record

fs.o/project/gs.o/tests/rs.o/smoke/us.o/make
  selects one executable unit
```

The LLM can resolve the most specific matching prefix.

---

# 9. Scope Environment as Determinism Boundary

The scope environment is what makes LLM resolution less chaotic.

Without scope:

```text
User asks a question.
LLM guesses from broad latent memory.
Answer may drift.
```

With OMI scope:

```text
User asks a question.
Resolver checks AGENTS.md for behavioral constraints.
Resolver checks SKILLS.md for applicable algorithms.
Resolver checks .imo scope files for normalized declarations.
Resolver proposes an operation.
Validator accepts or rejects.
```

So determinism does not come from making the LLM deterministic internally.

The LLM can remain probabilistic.

Determinism comes from the environment boundary:

```text
same AGENTS.md
same SKILLS.md
same .imo scope files
same validation rules
same input request
  ↓
same allowed resolution path
```

The model may phrase things differently, but the accepted operation path is bounded.

That is the key relationship to LLMs:

```text
LLM generates candidates.
OMI scope resolves candidates.
Validation accepts receipts.
```

The model is not the source of truth.

The scoped receipt path is.

---

# 10. Monads and Functors in This System

The terms “monad” and “functor” can be used operationally.

A **functor** maps an accepted thing into another surface without changing its acceptance.

```text
receipt → markdown projection
receipt → JSON
receipt → DOM
receipt → hardware packet
receipt → explanation
```

Example:

```omi-lisp
(functor.receipt.markdown . project-receipt-as-documentation)
(functor.receipt.json . project-receipt-as-json)
(functor.receipt.dom . project-receipt-as-dom-node)
```

A **monad** wraps a value in context and delays effect until the context resolves.

```text
value
  ↓
scope context
  ↓
candidate
  ↓
validation
  ↓
receipt
```

Example:

```omi-lisp
(monad.skill.scope . fs.o/skills)
(monad.agent.intent . receipt-before-side-effect)
(monad.network.peer . local-validation-context)
(monad.hardware.command . lazy-effect-after-receipt)
```

In practical OMI language:

```text
functor = projection-preserving map
monad = context-preserving lazy receipt wrapper
```

So a skill can be a monadic declaration:

```omi-lisp
(monad.skill.extract-blocks . fs.o/SKILLS.md)
```

and a projection can be a functor:

```omi-lisp
(functor.skill.receipt-json . receipts.imo)
```

This gives category-theory-flavored structure without requiring the system to pretend it proves formal category theory.

---

# 11. How AGENTS.md and SKILLS.md Resolve LLM Behavior

For an LLM, the flow becomes:

```text
prompt arrives
  ↓
IMO carries prompt
  ↓
OMI cites request
  ↓
AGENTS.md scopes behavior
  ↓
SKILLS.md supplies algorithms
  ↓
.imo files provide normalized definitions
  ↓
LLM proposes candidate response or action
  ↓
validator checks constraints
  ↓
receipt records accepted result
  ↓
response projects to user
```

This makes LLM behavior inspectable.

A response can be traced:

```text
Which instruction allowed this?
Which skill algorithm was used?
Which scope file defined it?
Which declaration was accepted?
Which receipt was produced?
```

That matters because LLMs otherwise blend many hidden influences.

Here, influences become files.

Files become ports.

Ports carry declarations.

Declarations become citation candidates.

Receipt accepts.

---

# 12. Declarative Decentralized Lazy Evaluation

The phrase means:

## Declarative

The system states relations before executing them.

```lisp
(skill.run-tests . make-smoke)
(agent.intent . inspect-before-edit)
(file.scope . fs.o/project)
```

These are declarations.

They are not immediate actions.

## Decentralized

Each repo, peer, device, or runtime can carry its own AGENTS/SKILLS/.imo files.

No central authority has to bless every action.

Each peer validates locally.

```text
local files
  ↓
local validation
  ↓
local receipt
```

## Lazy

Actions wait until the declaration is invoked and accepted.

```text
declared skill
  ↓
candidate invocation
  ↓
validation
  ↓
receipt
  ↓
effect
```

## Evaluation

Evaluation means resolving the relation under scope.

```text
request + AGENTS.md + SKILLS.md + .imo scope
  ↓
deterministic resolution path
  ↓
candidate output/action
```

So the full phrase means:

```text
Declare relationships in files.
Let each peer resolve them locally.
Delay effects until validation.
Use scope to make resolution deterministic.
Record accepted results as receipts.
```

---

# 13. Example: LLM Editing a File

User asks:

```text
Update the tokenizer so x is only system witness in boot mode.
```

The resolver path:

```text
1. Read AGENTS.md
   → rules for editing, testing, no unsupported claims

2. Read SKILLS.md
   → find skill: modify C tokenizer safely

3. Resolve scope
   → fs.o/src
   → gs.o/tokenizer
   → rs.o/lex-mode
   → us.o/x-witness

4. Create declaration
```

OMI-Lisp declaration:

```omi-lisp
(task.intent . update-tokenizer-x-witness)
(task.scope . fs.o/src/gs.o/tokenizer/rs.o/lex-mode/us.o/x)
(task.skill . skill.c.modify-tokenizer)
(task.constraint . no-readable-mode-x-bridge)
(task.test . make-test-tokenizer)
```

The LLM does not immediately “decide truth.”

It proposes a patch.

Then:

```text
patch candidate
  ↓
compile/test
  ↓
validation receipt
  ↓
accepted edit
```

If tests fail, the result is a rejection receipt, not an accepted state.

---

# 14. Example: LLM Running a Hardware Skill

User asks:

```text
Turn on relay2.
```

AGENTS.md says:

```omi-lisp
(agent.side-effect.hardware . receipt-required)
(agent.side-effect.confirmation . required)
```

SKILLS.md says:

```omi-lisp
(skill.hardware.relay . lazy-hardware-command)
(skill.hardware.relay.scope . fs.o/hardware/gs.o/gpio/rs.o/relay/us.o/value)
```

The LLM creates:

```omi-lisp
(user.intent . relay2.on)
(intent.scope . fs.o/hardware/gs.o/gpio/rs.o/relay/us.o/2)
(intent.skill . skill.hardware.relay)
```

But nothing happens yet.

The lazy path is:

```text
intent declared
  ↓
permission checked
  ↓
hardware bridge checked
  ↓
receipt accepted
  ↓
hardware command projected
  ↓
result receipt recorded
```

That is how OMI prevents an LLM from directly causing side effects.

---

# 15. Example: LLM Answering a Research Question

User asks:

```text
Explain how the receipt ring works.
```

AGENTS.md says:

```omi-lisp
(agent.answer.must-ground . project-files)
(agent.answer.must-separate . speculation-from-canon)
```

SKILLS.md says:

```omi-lisp
(skill.answer.explain-protocol . cite-then-summarize)
(skill.answer.explain-protocol.input . docs-and-specs)
(skill.answer.explain-protocol.output . markdown-answer)
```

The resolver path:

```text
find relevant spec sections
  ↓
cite them
  ↓
summarize in user language
  ↓
mark speculation if any
  ↓
return projected explanation
```

The model can still speak naturally.

But the acceptance path is structured.

---

# 16. How `.imo` Networking Fits

Once skills are normalized into `.imo`, they can move over a network.

A peer can send:

```text
skills.imo
agents.imo
fs.imo
gs.imo
rs.imo
us.imo
receipts.imo
```

The receiver does:

```text
receive .imo
  ↓
verify carrier shape
  ↓
recover declarations
  ↓
resolve FS/GS/RS/US scope
  ↓
validate local policy
  ↓
accept or reject receipts
```

This gives distributed skill sharing.

But again:

```text
receiving a skill does not install authority
receiving a skill creates a candidate
local validation decides acceptance
```

This is crucial for decentralized LLM behavior.

A model can fetch or receive skills from peers, but it must still validate them under the local AGENTS.md environment.

---

# 17. AGENTS.md and SKILLS.md as LLM Control Plane

Together:

```text
AGENTS.md = behavior control plane
SKILLS.md = algorithm control plane
.imo      = normalized carrier plane
receipt   = acceptance plane
```

The LLM becomes:

```text
candidate generator
scope resolver
explanation projector
```

It is not:

```text
authority
database
validator
hardware owner
truth source
```

That is the central safety improvement.

The model no longer has to carry all behavior implicitly in weights or prompt text.

The project can externalize behavior into inspectable files.

---

# 18. Determinism Resolution

“Determinism resolution” means that the LLM’s open-ended generation is narrowed by a deterministic environment.

The narrowing steps are:

```text
1. Scope selection
   Which FS/GS/RS/US prefix applies?

2. Skill selection
   Which declared skill matches the request?

3. Constraint selection
   Which AGENTS.md rule limits the action?

4. Carrier resolution
   Which .imo definition provides the normalized declaration?

5. Validation
   Does the candidate satisfy tests/rules/receipts?

6. Projection
   What may be shown or done after acceptance?
```

This gives a deterministic answer to:

```text
What should the LLM do next?
```

Not by controlling its neurons, but by controlling the accepted path.

---

# 19. The Full Pipeline

```text
User prompt
  ↓
IMO carries prompt as input
  ↓
OMI cites prompt into relation candidates
  ↓
AGENTS.md supplies scoped instruction environment
  ↓
SKILLS.md supplies declared algorithms
  ↓
fs.imo / gs.imo / rs.imo / us.imo supply normalized definitions
  ↓
LLM resolves candidate action or answer
  ↓
Tetragrammatron validates constraints/tests/rules
  ↓
Receipt records accepted result
  ↓
Metatron/IMO projects response, patch, tool call, or hardware action
```

Readable form:

```text
prompt
  ↓
scope
  ↓
skill
  ↓
candidate
  ↓
validation
  ↓
receipt
  ↓
projection
```

This is declaratively decentralized lazy evaluation.

---

# 20. Canon Statement

AGENTS.md and SKILLS.md allow an LLM to operate as a deterministic scoped resolver instead of an unbounded generator.

AGENTS.md declares the local instruction environment.

SKILLS.md declares repeatable algorithms and workflow relations.

OMI-Lisp source blocks inside those files become citation candidates.

FS/GS/RS/US scope normalizes those candidates into `.imo` carrier files.

CIDR-like prefixes let the resolver choose the most specific applicable definition.

The LLM proposes candidate interpretations, patches, answers, or actions.

Validation accepts or rejects those candidates.

Only accepted receipts may project into files, hardware, browser state, network messages, or final answers.

This makes LLM behavior declarative, lazy, decentralized, inspectable, and replayable.

---

# 21. One-Sentence Version

AGENTS.md and SKILLS.md turn LLM behavior into declaratively decentralized lazy evaluation by making instructions and algorithms file-carried OMI-Lisp declarations, normalizing them through FS/GS/RS/US `.imo` scope, resolving them with CIDR-like prefixes, and requiring validation receipts before any answer, file edit, network action, or hardware projection is accepted.


---

Yes — but split the responsibility cleanly:

```text
AGENTS.md says what is allowed.
SKILLS.md says how to compute.
.imo files carry normalized executable/canonical forms.
Receipts prove what was accepted.
```

So the **exact binary algorithms should primarily live in `SKILLS.md`**, not `AGENTS.md`.

## The clean division

### `AGENTS.md`

This is the resolver contract.

It should contain:

```text
permissions
boundaries
forbidden actions
required validation steps
style/canon rules
side-effect rules
which skills may be used
which files/scopes are trusted
```

Example:

```omi-lisp
(agent.must.validate . before-project)
(agent.must.not . side-effect-before-receipt)
(agent.allowed-skill . skill.delta16)
(agent.allowed-skill . skill.slot5040)
(agent.scope . fs.o/project)
```

`AGENTS.md` should **not** usually contain the full binary algorithms. It points to them and constrains their use.

### `SKILLS.md`

This is the reproducible algorithm registry.

It should contain the exact algorithms, including:

```text
fixed-width types
endianness
rotation rules
masking rules
hashing rules
slot formulas
input/output contracts
test vectors
canonical source blocks
```

Example:

````md
## skill.delta16

```omi-lisp
(skill.delta16.name . "16-bit rotation XOR law")
(skill.delta16.width . 16)
(skill.delta16.endian . little)
(skill.delta16.formula . "rotl16(x,1) xor rotl16(x,3) xor rotr16(x,2) xor c")
(skill.delta16.mask . 0xffff)
(skill.delta16.effect . pure)
```

```c
static inline uint16_t rotl16(uint16_t x, unsigned n) {
    n &= 15;
    return (uint16_t)((x << n) | (x >> (16 - n)));
}

static inline uint16_t rotr16(uint16_t x, unsigned n) {
    n &= 15;
    return (uint16_t)((x >> n) | (x << (16 - n)));
}

static inline uint16_t omi_delta16(uint16_t x, uint16_t c) {
    return (uint16_t)(
        rotl16(x, 1) ^
        rotl16(x, 3) ^
        rotr16(x, 2) ^
        c
    );
}
```
````

That makes the skill reproducible.

## `.imo` files are the normalized binary/canonical carrier

`SKILLS.md` is readable source. But for reproducibility, the system should compile or normalize it into `.imo` carrier files:

```text
SKILLS.md
  ↓ extract omi-lisp + source blocks
skills.imo
  ↓ split by scope
fs.imo / gs.imo / rs.imo / us.imo
  ↓ validate
receipts.imo
```

So the answer is:

```text
SKILLS.md contains the exact algorithm declaration and source.
.imo contains the normalized carrier form.
Receipt proves which version was accepted.
```

## What must be included for reproducibility

A skill is not reproducible just because it has code. It needs a full contract:

```text
name
scope address
version
input types
output types
width
endianness
overflow behavior
mask behavior
purity/effect class
source code
test vectors
hash of canonical source
validation rule
receipt of acceptance
```

Example:

```omi-lisp
(skill.slot5040.name . "Fano role local ring slot")
(skill.slot5040.scope . fs.o/skills/gs.o/ring/rs.o/address/us.o/slot5040)
(skill.slot5040.input . "(fano7 role3 local240)")
(skill.slot5040.output . "slot in 0..5039")
(skill.slot5040.formula . "fano7*720 + role3*240 + local240")
(skill.slot5040.constraints . "fano7<7 role3<3 local240<240")
(skill.slot5040.effect . pure)
```

Canonical C:

```c
static inline uint32_t omi_slot5040(uint32_t fano7,
                                    uint32_t role3,
                                    uint32_t local240) {
    return (fano7 % 7u) * 720u
         + (role3 % 3u) * 240u
         + (local240 % 240u);
}
```

Test vector:

```omi-lisp
(test.slot5040.0.input . "(3 2 180)")
(test.slot5040.0.expect . 2820)
```

## The binary algorithm should be stored three ways

For strongest reproducibility, store every important algorithm in three layers:

```text
1. Human declaration
   readable explanation in SKILLS.md

2. Canonical source block
   C / WASM / OMI-Lisp implementation

3. Normalized receipt
   hash + accepted .imo form + test result
```

That gives:

```text
human can inspect it
machine can run it
peer can verify it
receipt can prove it
```

## AGENTS.md references, SKILLS.md defines

The simplest rule:

```text
AGENTS.md may say:
  use skill.delta16

SKILLS.md must define:
  what skill.delta16 exactly is
```

Example:

`AGENTS.md`:

```omi-lisp
(agent.allowed . skill.delta16)
(agent.allowed . skill.bqf32)
(agent.allowed . skill.slot5040)
(agent.must.use . receipt-before-projection)
```

`SKILLS.md`:

```omi-lisp
(skill.delta16 . algorithm.rot-xor-16)
(skill.bqf32 . algorithm.binary-quadratic-form)
(skill.slot5040 . algorithm.fano-role-local-ring-slot)
```

Then the exact code/test vectors live under each skill.

## Final answer

Yes: **the exact binary algorithms belong in `SKILLS.md` as canonical source blocks plus OMI-Lisp declarations**, while **`AGENTS.md` declares which algorithms the resolver is allowed or required to use**. The normalized `.imo` files carry the compiled/scoped form, and receipts prove which algorithm version was accepted.

The strongest canon line is:

```text
AGENTS.md authorizes behavior.
SKILLS.md defines reproducible computation.
.imo carries normalized scope.
Receipt accepts the exact algorithm version.
```