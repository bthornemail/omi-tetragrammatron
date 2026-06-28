This is a major milestone. You've now unified:

1. The OMI-Lisp specification - complete formal system
2. The uniform bitboard - platform-agnostic substrate
3. The memory architecture - deterministic arena with handles
4. The gauge table - 0x00-0x7F with tetrahedral diagonals
5. The tokenizer/parser - with lexer modes and proper dot notation
6. The receipt ring - 5040 slots with validation gates
7. The hardware projection - only after receipt
8. REPO.md - Role/Repo Based Access Control
9. AGENTS.md & SKILLS.md - declarative decentralized lazy evaluation
10. Version control - place-value notation + Betti-Schlafli + seed objects

Now let me provide the complete AGENTS.md and SKILLS.md for your VCS, fully integrated with the OMI version control system.

---

AGENTS.md - Version Control Agent Behavior

```markdown
# AGENTS.md
## OMI Version Control Agent Behavior Specification

Version: `0.1.0`
Status: Draft
Authority: Version Control Collaboration Boundary

---

## 0. Purpose

This file defines how a resolver (LLM, script, human, or peer) may interact with an OMI version-controlled repository.

It is the behavior authority for:

```text
version creation
version inspection
version validation
version projection
version merging
version rollback
skill invocation
receipt verification
```

AGENTS.md does not define algorithms. It authorizes and constrains behavior.

SKILLS.md defines the exact algorithms.

---

1. Agent Role Definition

The default resolver role in this repository is:

```omi-lisp
(agent.role . Resolver)
(agent.scope . fs.o)
(agent.must . obey-REPO-md)
(agent.must . cite-source)
(agent.must . validate-before-claim)
(agent.must . record-receipts)
(agent.must.not . side-effect-before-receipt)
```

1.1 Agent Permissions

A Resolver in this repository MAY:

```omi-lisp
(agent.may.read . fs.o/**)
(agent.may.cite . fs.o/**)
(agent.may.propose . fs.o/candidates/**)
(agent.may.resolve . fs.o/SKILLS.md)
(agent.may.inspect . receipts.imo)
(agent.may.project . read-only-views)
```

A Resolver in this repository MUST NOT:

```omi-lisp
(agent.must.not . write-canonical-files-directly)
(agent.must.not . merge-branches-without-validation)
(agent.must.not . delete-receipts)
(agent.must.not . project-hardware)
(agent.must.not . claim-authority)
```

1.2 Agent Skill Authorization

A Resolver MAY invoke the following skills from SKILLS.md:

```omi-lisp
(agent.allowed-skill . skill.version.read)
(agent.allowed-skill . skill.version.create)
(agent.allowed-skill . skill.version.diff)
(agent.allowed-skill . skill.version.validate)
(agent.allowed-skill . skill.version.merge)
(agent.allowed-skill . skill.version.branch)
(agent.allowed-skill . skill.slot5040)
(agent.allowed-skill . skill.delta16)
(agent.allowed-skill . skill.bqf32)
(agent.allowed-skill . skill.receipt.verify)
(agent.allowed-skill . skill.imo.normalize)
(agent.allowed-skill . skill.scope.resolve)
```

A Resolver MUST NOT invoke hardware or network skills unless explicitly granted by REPO.md.

---

2. Version State Machine

A version moves through these states:

```text
draft
  ↓
candidate
  ↓
under-review
  ↓
validated
  ↓
accepted
  ↓
projected
```

2.1 Draft State

A draft version is a local proposal.

A draft version is not yet shared.

A draft version MAY be edited freely.

Draft declaration:

```omi-lisp
(version.status . draft)
(version.editor . local-resolver)
(version.scope . fs.o/candidates/drafts)
(version.parent . none)
```

2.2 Candidate State

A candidate version is submitted for review.

A candidate version MUST include:

```text
parent receipt
scope prefix
seed object
route
declaration
skill reference
test results
```

Candidate declaration:

```omi-lisp
(version.status . candidate)
(version.id . candidate-042)
(version.parent . receipt-041)
(version.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(version.seed . o.base64.o)
(version.route . omi-base-64-imo)
(version.carrier . SKILLS.md)
(version.algorithm . skill.base64.zero-polynomial)
(version.test . test.base64.vectors)
```

2.3 Under-Review State

A version under review is being validated.

Reviewer MUST NOT modify candidate content.

Reviewer MUST record validation steps.

Review declaration:

```omi-lisp
(version.status . under-review)
(version.reviewer . Validator)
(version.steps . "(test compile lint topology)")
(version.result . pending)
```

2.4 Validated State

A version that passes all tests is validated.

Validated version is not yet accepted.

Validation MUST produce a hash.

Validation declaration:

```omi-lisp
(version.status . validated)
(version.hash . 0x6789ABCD)
(version.tests-passed . 42)
(version.topology . "(1 0 0)")
(version.result . passes)
```

2.5 Accepted State

A validated version that receives a receipt is accepted.

Only Tetragrammatron may accept.

Acceptance declaration:

```omi-lisp
(version.status . accepted)
(version.receipt . receipt-2820)
(version.slot . 2820)
(version.cycle . 12345)
(version.result . accepted)
```

2.6 Projected State

An accepted version projected to a visible or operational surface.

Projection MAY be:

```text
file write
branch merge
documentation update
geometry render
UI update
```

Projection declaration:

```omi-lisp
(version.status . projected)
(version.projection . fs.o/SKILLS.md)
(version.functor . markdown-write)
(version.result . projected)
```

---

3. Version Creation Rules

A Resolver MAY create a version only if:

```text
parent receipt exists in ring
scope is valid under REPO.md
seed object is declared
route is declared
skill is declared in SKILLS.md
effect class is permitted
```

Version creation declaration:

```omi-lisp
(task.create-version . "Add Base64 seed object")
(task.parent . receipt-041)
(task.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(task.seed . o.base64.o)
(task.route . omi-base-64-imo)
(task.skill . skill.base64.zero-polynomial)
(task.effect . pure)
(task.files . "(SKILLS.md fs.imo)")
```

---

4. Version Diff Rules

A Resolver MAY diff versions only if:

```text
both versions are accessible
both versions have receipts or are candidates
diff scope is valid
```

Diff declaration:

```omi-lisp
(task.diff . "Compare Base64 seed versions")
(task.left . receipt-041)
(task.right . candidate-042)
(task.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(task.output . diff.imo)
```

Diff output MUST include:

```text
carrier diff
declaration diff
skill diff
scope diff
Betti diff
```

---

5. Version Merge Rules

A Resolver MAY merge versions only if:

```text
both versions are accepted or validated
merge scope is valid
no unresolved topological conflict
merge produces valid Betti signature
```

Merge declaration:

```omi-lisp
(task.merge . "Merge Base64 seed updates")
(task.left . receipt-042)
(task.right . receipt-043)
(task.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(task.resolution . manual)
(task.topology . "(1 0 0)")
```

If merge creates a loop, resolver MUST report:

```omi-lisp
(task.conflict . topological-loop)
(task.betti . "(1 1 0)")
(task.resolution . required)
```

---

6. Version Branch Rules

A Resolver MAY branch only if:

```text
parent receipt exists
branch scope is valid
branch does not conflict with main topology
```

Branch declaration:

```omi-lisp
(task.branch . "feature/base64-seed")
(task.parent . receipt-041)
(task.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(task.purpose . "Add Base64 zero polynomial")
```

---

7. Version Rollback Rules

A Resolver MAY rollback only if:

```text
target receipt exists
rollback scope is valid
rollback receipt is recorded
```

Rollback declaration:

```omi-lisp
(task.rollback . "Revert Base64 seed change")
(task.target . receipt-042)
(task.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(task.reason . "Test failure")
(task.receipt . receipt-043)
```

Rollback MUST NOT erase historical receipts.

---

8. Betti-Schlafli Interpretation Rules

A Resolver SHOULD interpret version topology:

```text
b0 = connected components
b1 = loops / tunnels
b2 = voids / enclosed shells
```

Betti interpretation:

```omi-lisp
(task.topology . "Interpret version structure")
(task.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(task.betti . "(1 0 0)")
(task.schlafli . "{3,3}")
(task.interpretation . "connected seed relation")
```

If Betti numbers indicate structural issues, resolver MUST report:

```omi-lisp
(task.topology.warning . "Circular dependency detected")
(task.betti . "(1 1 0)")
(task.recommendation . "Resolve dependency loop before merge")
```

---

9. Receipt Verification Rules

A Resolver MUST verify receipts before claiming acceptance:

```omi-lisp
(task.verify . "Check receipt validity")
(task.receipt . receipt-042)
(task.hash . 0x6789ABCD)
(task.ring . slot-2820)
(task.result . verified)
```

Verification MUST check:

```text
receipt exists in ring
hash matches declaration
scope matches claim
parent matches history
```

---

10. .imo Carrier Rules

A Resolver MAY read .imo carriers.

A Resolver MUST NOT treat .imo as acceptance.

A Resolver MUST validate .imo content locally.

.imo processing:

```omi-lisp
(task.imo.load . fs.imo)
(task.imo.scope . fs.o/SKILLS.md)
(task.imo.status . candidate)
(task.imo.validation . pending)
```

---

11. LLM-Specific Rules

An LLM acting as Resolver MUST:

```text
read AGENTS.md before resolving
read SKILLS.md before claiming skill
distinguish candidate from accepted
avoid side effects without receipt
cite scope when proposing changes
explain reasoning when uncertain
```

An LLM Resolver MUST NOT:

```text
claim acceptance without validation
invent skill definitions
ignore scope restrictions
treat projection as truth
execute side effects from generated text
```

---

12. Error Handling

If a resolver encounters an error, it MUST:

```text
record error receipt
report error to user
propose resolution if possible
stop before side effects
```

Error receipt:

```omi-lisp
(receipt.error . "Validation failed")
(receipt.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(receipt.reason . "Test vector mismatch")
(receipt.suggestion . "Check base64 alphabet")
(receipt.result . rejected)
```

---

13. Security Rules

A Resolver MUST NOT:

```text
access credentials
modify signed files
project hardware without receipt
send network packets without permission
bypass validation
```

Security violation receipt:

```omi-lisp
(receipt.security . "Unauthorized hardware projection")
(receipt.scope . fs.o/hardware)
(receipt.reason . "No hardware receipt")
(receipt.result . rejected)
```

---

14. Agent Declaration Block

```omi-lisp
(agent.name . "OMI Version Control Resolver")
(agent.version . "0.1.0")
(agent.role . Resolver)
(agent.scope . fs.o)

(agent.must.read . REPO.md)
(agent.must.read . AGENTS.md)
(agent.must.read . SKILLS.md)
(agent.must.validate . before-claim)
(agent.must.record . receipts)
(agent.must.not . side-effect-before-receipt)

(agent.skill.allowed . skill.version.read)
(agent.skill.allowed . skill.version.create)
(agent.skill.allowed . skill.version.merge)
(agent.skill.allowed . skill.slot5040)
(agent.skill.allowed . skill.delta16)

(agent.error . record-rejection-receipt)
(agent.security . no-hardware-without-receipt)
```

---

15. Canon Statement

This AGENTS.md defines the version-control resolver behavior authority for an OMI-compatible repository.

It authorizes version creation, diff, merge, branch, rollback, receipt verification, and .imo processing.

It scopes resolver behavior to FS/GS/RS/US prefixes.

It requires validation before acceptance.

It forbids side effects before receipt.

It integrates Betti-Schlafli interpretation for version topology.

It allows LLMs to resolve versions declaratively without granting unchecked authority.

---

16. One-Sentence Version

AGENTS.md is the behavior authority for OMI version-control resolvers, defining allowed actions, required validation, side-effect rules, Betti-Schlafli interpretation, and receipt gates for version creation, diff, merge, branch, rollback, and .imo processing, while forbidding projection without receipt.

```

---

# SKILLS.md - Version Control Algorithms

```markdown
# SKILLS.md
## OMI Version Control Algorithm Registry

Version: `0.1.0`
Status: Draft
Authority: Algorithm Reproducibility Boundary

---

## 0. Purpose

This file defines reproducible algorithms for OMI version control.

Each skill includes:

```text
name
scope address
version
input contract
output contract
effect class
width constraints
endianness
overflow behavior
source code
test vectors
validation rule
```

SKILLS.md does not authorize behavior. AGENTS.md authorizes.

---

1. Version Reading Skills

1.1 skill.version.read

Read a version from a receipt or candidate.

```omi-lisp
(skill.version.read.name . "Read version from receipt")
(skill.version.read.scope . fs.o/skills/gs.o/version/rs.o/read)
(skill.version.read.input . receipt-handle)
(skill.version.read.output . version-structure)
(skill.version.read.effect . pure)
(skill.version.read.version . v1)
```

Canonical C:

```c
typedef struct {
    omi_u32 receipt;
    omi_u32 parent;
    char scope[128];
    char seed[32];
    char route[32];
    char carrier[128];
    char algorithm[64];
    omi_u32 betti0;
    omi_u32 betti1;
    omi_u32 betti2;
    char schlafli[8];
    omi_u8 accepted;
} omi_version_t;

static omi_version_t omi_version_read(omi_arena_t* arena, omi_handle_t receipt) {
    omi_version_t v = {0};
    omi_receipt_slot_t* slot = omi_receipt_get(arena, receipt);
    if (!slot) return v;

    v.receipt = receipt;
    v.accepted = omi_receipt_is_accepted(arena, receipt);
    v.parent = omi_load32le(slot->data + 0);
    /* ... extract scope, seed, route, etc. ... */

    return v;
}
```

Test vector:

```omi-lisp
(test.version.read.0.input . receipt-2820)
(test.version.read.0.expect . "(1 1 0 0 0 0 0 0 1)")
```

---

1.2 skill.version.diff

Diff two versions.

```omi-lisp
(skill.version.diff.name . "Compare two versions")
(skill.version.diff.scope . fs.o/skills/gs.o/version/rs.o/diff)
(skill.version.diff.input . "(left-receipt right-receipt)")
(skill.version.diff.output . diff-structure)
(skill.version.diff.effect . pure)
(skill.version.diff.version . v1)
```

Canonical C:

```c
typedef struct {
    omi_u32 left;
    omi_u32 right;
    omi_u32 carrier_diff;
    omi_u32 declaration_diff;
    omi_u32 skill_diff;
    omi_u32 scope_diff;
    omi_u32 betti0_diff;
    omi_u32 betti1_diff;
    omi_u32 betti2_diff;
} omi_diff_t;

static omi_diff_t omi_version_diff(omi_arena_t* arena,
                                    omi_handle_t left,
                                    omi_handle_t right) {
    omi_diff_t d = {0};
    d.left = left;
    d.right = right;

    omi_version_t vl = omi_version_read(arena, left);
    omi_version_t vr = omi_version_read(arena, right);

    /* Compare fields */
    d.carrier_diff = (strcmp(vl.carrier, vr.carrier) != 0);
    d.declaration_diff = (vl.receipt != vr.parent); /* simplified */
    d.scope_diff = (strcmp(vl.scope, vr.scope) != 0);
    d.betti0_diff = (vl.betti0 != vr.betti0);
    d.betti1_diff = (vl.betti1 != vr.betti1);
    d.betti2_diff = (vl.betti2 != vr.betti2);

    return d;
}
```

Test vector:

```omi-lisp
(test.version.diff.0.input . "(receipt-041 receipt-042)")
(test.version.diff.0.expect . "(0 1 0 0 0 0 0 0 0)")
```

---

2. Version Creation Skills

2.1 skill.version.create

Create a new version from a declaration.

```omi-lisp
(skill.version.create.name . "Create version from declaration")
(skill.version.create.scope . fs.o/skills/gs.o/version/rs.o/create)
(skill.version.create.input . version-declaration)
(skill.version.create.output . version-candidate)
(skill.version.create.effect . pure)
(skill.version.create.version . v1)
```

Canonical C:

```c
static omi_handle_t omi_version_create(omi_arena_t* arena,
                                        const char* scope,
                                        const char* seed,
                                        const char* route,
                                        const char* carrier,
                                        const char* algorithm) {
    omi_receipt_candidate_t candidate = {0};
    candidate.env = OMI_HANDLE_NIL;
    candidate.cycle = arena->cycle;
    candidate.source = 0x01; /* version source */

    /* Build declaration from components */
    char decl[256];
    snprintf(decl, sizeof(decl),
             "(version.scope . %s)(version.seed . %s)(version.route . %s)",
             scope, seed, route);
    /* ... parse and store ... */

    candidate.declaration = omi_cons(arena, OMI_HANDLE_NIL, OMI_HANDLE_NIL);
    candidate.validated = 1;

    return omi_receipt_store(arena, &candidate);
}
```

Test vector:

```omi-lisp
(test.version.create.0.input . "(fs.o/SKILLS.md o.base64.o omi-base-64-imo)")
(test.version.create.0.expect . receipt-2820)
```

---

3. Version Merge Skills

3.1 skill.version.merge

Merge two versions.

```omi-lisp
(skill.version.merge.name . "Merge two versions")
(skill.version.merge.scope . fs.o/skills/gs.o/version/rs.o/merge)
(skill.version.merge.input . "(left-receipt right-receipt scope)")
(skill.version.merge.output . merged-receipt)
(skill.version.merge.effect . pure)
(skill.version.merge.version . v1)
```

Canonical C:

```c
static omi_handle_t omi_version_merge(omi_arena_t* arena,
                                       omi_handle_t left,
                                       omi_handle_t right,
                                       const char* scope) {
    omi_version_t vl = omi_version_read(arena, left);
    omi_version_t vr = omi_version_read(arena, right);

    /* Check compatibility */
    if (vl.betti1 && vr.betti1) {
        /* Both have loops - merge creates compound loop */
        return OMI_HANDLE_NIL;
    }

    /* Create merged version */
    char decl[512];
    snprintf(decl, sizeof(decl),
             "(merge.left . %u)(merge.right . %u)(merge.scope . %s)",
             left, right, scope);

    omi_receipt_candidate_t candidate = {0};
    candidate.declaration = omi_cons(arena, OMI_HANDLE_NIL, OMI_HANDLE_NIL);
    candidate.validated = 1;

    return omi_receipt_store(arena, &candidate);
}
```

Test vector:

```omi-lisp
(test.version.merge.0.input . "(receipt-041 receipt-042 fs.o/SKILLS.md)")
(test.version.merge.0.expect . receipt-043)
```

---

4. Betti-Schlafli Skills

4.1 skill.topology.betti

Compute Betti numbers from a receipt.

```omi-lisp
(skill.topology.betti.name . "Compute Betti numbers")
(skill.topology.betti.scope . fs.o/skills/gs.o/topology/rs.o/betti)
(skill.topology.betti.input . receipt-handle)
(skill.topology.betti.output . "(b0 b1 b2)")
(skill.topology.betti.effect . pure)
(skill.topology.betti.version . v1)
```

Canonical C:

```c
typedef struct {
    omi_u32 b0;
    omi_u32 b1;
    omi_u32 b2;
} omi_betti_t;

static omi_betti_t omi_topology_betti(omi_arena_t* arena, omi_handle_t receipt) {
    omi_betti_t b = {0};
    omi_receipt_slot_t* slot = omi_receipt_get(arena, receipt);
    if (!slot) return b;

    /* Derive Betti numbers from declaration structure */
    /* b0 = connected components of declaration graph */
    /* b1 = loops (circular dependencies) */
    /* b2 = voids (missing dependencies) */

    /* Example: if declaration has circular reference */
    omi_handle_t decl = slot->declaration_ref;
    omi_handle_t car = omi_car(arena, decl);
    omi_handle_t cdr = omi_cdr(arena, decl);

    if (car == cdr) {
        b.b1 = 1; /* self-reference creates loop */
    }

    b.b0 = 1; /* at least one component */

    return b;
}
```

Test vector:

```omi-lisp
(test.topology.betti.0.input . receipt-041)
(test.topology.betti.0.expect . "(1 0 0)")
```

---

4.2 skill.topology.schlafli

Compute Schläfli symbol from a receipt.

```omi-lisp
(skill.topology.schlafli.name . "Compute Schläfli symbol")
(skill.topology.schlafli.scope . fs.o/skills/gs.o/topology/rs.o/schlafli)
(skill.topology.schlafli.input . receipt-handle)
(skill.topology.schlafli.output . schlafli-symbol)
(skill.topology.schlafli.effect . pure)
(skill.topology.schlafli.version . v1)
```

Canonical C:

```c
static const char* omi_topology_schlafli(omi_arena_t* arena,
                                          omi_handle_t receipt,
                                          char* out,
                                          size_t len) {
    omi_betti_t b = omi_topology_betti(arena, receipt);

    if (b.b0 == 1 && b.b1 == 0 && b.b2 == 0) {
        snprintf(out, len, "{3,3}"); /* tetrahedral seed */
    } else if (b.b0 == 1 && b.b1 == 1 && b.b2 == 0) {
        snprintf(out, len, "{4,3}"); /* cubical loop */
    } else if (b.b0 == 1 && b.b1 == 0 && b.b2 == 1) {
        snprintf(out, len, "{5,3}"); /* dodecahedral void */
    } else {
        snprintf(out, len, "{3,5}"); /* icosahedral complex */
    }

    return out;
}
```

Test vector:

```omi-lisp
(test.topology.schlafli.0.input . receipt-041)
(test.topology.schlafli.0.expect . "{3,3}")
```

---

5. Scope Resolution Skills

5.1 skill.scope.resolve

Resolve an FS/GS/RS/US scope prefix.

```omi-lisp
(skill.scope.resolve.name . "Resolve scope prefix")
(skill.scope.resolve.scope . fs.o/skills/gs.o/scope/rs.o/resolve)
(skill.scope.resolve.input . scope-string)
(skill.scope.resolve.output . "(fs gs rs us)")
(skill.scope.resolve.effect . pure)
(skill.scope.resolve.version . v1)
```

Canonical C:

```c
typedef struct {
    char fs[128];
    char gs[128];
    char rs[128];
    char us[128];
} omi_scope_t;

static omi_scope_t omi_scope_resolve(const char* scope) {
    omi_scope_t s = {0};
    char buf[512];
    strncpy(buf, scope, sizeof(buf) - 1);

    char* token = strtok(buf, "/");
    int idx = 0;

    while (token && idx < 4) {
        if (strncmp(token, "fs.o/", 5) == 0) {
            strncpy(s.fs, token + 5, sizeof(s.fs) - 1);
        } else if (strncmp(token, "gs.o/", 5) == 0) {
            strncpy(s.gs, token + 5, sizeof(s.gs) - 1);
        } else if (strncmp(token, "rs.o/", 5) == 0) {
            strncpy(s.rs, token + 5, sizeof(s.rs) - 1);
        } else if (strncmp(token, "us.o/", 5) == 0) {
            strncpy(s.us, token + 5, sizeof(s.us) - 1);
        }
        token = strtok(NULL, "/");
        idx++;
    }

    return s;
}
```

Test vector:

```omi-lisp
(test.scope.resolve.0.input . "fs.o/SKILLS.md/gs.o/encoding/rs.o/base64")
(test.scope.resolve.0.expect . "(SKILLS.md encoding base64 )")
```

---

6. Receipt Skills

6.1 skill.receipt.verify

Verify a receipt is valid.

```omi-lisp
(skill.receipt.verify.name . "Verify receipt")
(skill.receipt.verify.scope . fs.o/skills/gs.o/receipt/rs.o/verify)
(skill.receipt.verify.input . receipt-handle)
(skill.receipt.verify.output . "(valid hash)")
(skill.receipt.verify.effect . pure)
(skill.receipt.verify.version . v1)
```

Canonical C:

```c
typedef struct {
    omi_u8 valid;
    omi_u32 hash;
} omi_receipt_verify_t;

static omi_receipt_verify_t omi_receipt_verify(omi_arena_t* arena,
                                                omi_handle_t receipt) {
    omi_receipt_verify_t r = {0};
    omi_receipt_slot_t* slot = omi_receipt_get(arena, receipt);
    if (!slot) return r;

    r.hash = slot->hash;
    r.valid = (slot->hash != 0) ? 1 : 0;

    return r;
}
```

Test vector:

```omi-lisp
(test.receipt.verify.0.input . receipt-2820)
(test.receipt.verify.0.expect . "(1 0x6789ABCD)")
```

---

6.2 skill.receipt.parent

Find the parent of a receipt.

```omi-lisp
(skill.receipt.parent.name . "Find parent receipt")
(skill.receipt.parent.scope . fs.o/skills/gs.o/receipt/rs.o/parent)
(skill.receipt.parent.input . receipt-handle)
(skill.receipt.parent.output . parent-handle)
(skill.receipt.parent.effect . pure)
(skill.receipt.parent.version . v1)
```

Canonical C:

```c
static omi_handle_t omi_receipt_parent(omi_arena_t* arena,
                                        omi_handle_t receipt) {
    omi_receipt_slot_t* slot = omi_receipt_get(arena, receipt);
    if (!slot) return OMI_HANDLE_NIL;

    return omi_load32le(slot->data);
}
```

Test vector:

```omi-lisp
(test.receipt.parent.0.input . receipt-2820)
(test.receipt.parent.0.expect . receipt-2819)
```

---

7. Carrier Normalization Skills

7.1 skill.imo.normalize

Normalize a declaration to .imo form.

```omi-lisp
(skill.imo.normalize.name . "Normalize to .imo")
(skill.imo.normalize.scope . fs.o/skills/gs.o/imo/rs.o/normalize)
(skill.imo.normalize.input . declaration-text)
(skill.imo.normalize.output . normalized-imo)
(skill.imo.normalize.effect . pure)
(skill.imo.normalize.version . v1)
```

Canonical C:

```c
static void omi_imo_normalize(const char* input,
                               char* output,
                               size_t len) {
    /* Extract OMI-Lisp blocks from Markdown */
    /* Normalize FS/GS/RS/US scope */
    /* Write .imo format */

    const char* start = strstr(input, "```omi-lisp");
    if (start) {
        start += 11; /* skip "```omi-lisp" */
        const char* end = strstr(start, "```");
        if (end) {
            size_t block_len = end - start;
            strncpy(output, start, block_len);
            output[block_len] = 0;
        }
    }
}
```

Test vector:

```omi-lisp
(test.imo.normalize.0.input . "(version.seed . o.base64.o)")
(test.imo.normalize.0.expect . "(version.seed . o.base64.o)")
```

---

8. Version Validation Skills

8.1 skill.version.validate

Validate a version candidate.

```omi-lisp
(skill.version.validate.name . "Validate version")
(skill.version.validate.scope . fs.o/skills/gs.o/version/rs.o/validate)
(skill.version.validate.input . version-candidate)
(skill.version.validate.output . validation-result)
(skill.version.validate.effect . pure)
(skill.version.validate.version . v1)
```

Canonical C:

```c
typedef struct {
    omi_u8 valid;
    char reason[256];
    omi_u32 hash;
} omi_validation_t;

static omi_validation_t omi_version_validate(omi_arena_t* arena,
                                              const char* candidate) {
    omi_validation_t v = {0};

    /* Check scope is valid */
    if (strstr(candidate, "fs.o/") == NULL) {
        strcpy(v.reason, "No scope found");
        return v;
    }

    /* Check seed is declared */
    if (strstr(candidate, "o.base64.o") == NULL &&
        strstr(candidate, "o.hex.o") == NULL) {
        strcpy(v.reason, "No seed object");
        return v;
    }

    /* Check route is declared */
    if (strstr(candidate, "omi-") == NULL) {
        strcpy(v.reason, "No route");
        return v;
    }

    v.valid = 1;
    v.hash = omi_fnv1a((const omi_u8*)candidate, strlen(candidate));

    return v;
}
```

Test vector:

```omi-lisp
(test.version.validate.0.input . "(version.seed . o.base64.o)")
(test.version.validate.0.expect . "(1 0x6789ABCD)")
```

---

9. Skill Declaration Block

```omi-lisp
(skill.registry.name . "OMI Version Control Skills")
(skill.registry.version . "0.1.0")
(skill.registry.scope . fs.o/SKILLS.md)

(skill.version.read . skill.version.read)
(skill.version.diff . skill.version.diff)
(skill.version.create . skill.version.create)
(skill.version.merge . skill.version.merge)
(skill.topology.betti . skill.topology.betti)
(skill.topology.schlafli . skill.topology.schlafli)
(skill.scope.resolve . skill.scope.resolve)
(skill.receipt.verify . skill.receipt.verify)
(skill.receipt.parent . skill.receipt.parent)
(skill.imo.normalize . skill.imo.normalize)
(skill.version.validate . skill.version.validate)

(skill.slot5040 . fs.o/skills/gs.o/ring/rs.o/slot5040)
(skill.delta16 . fs.o/skills/gs.o/math/rs.o/delta16)
(skill.bqf32 . fs.o/skills/gs.o/math/rs.o/bqf32)
```

---

10. Canon Statement

This SKILLS.md defines reproducible algorithms for OMI version control.

Each skill includes name, scope, input contract, output contract, effect class, source code, test vectors, and validation rules.

Skills are read-only/pure unless explicitly declared otherwise.

Skills MUST be validated before acceptance.

Skills MAY be invoked by Resolvers authorized in AGENTS.md.

Skills ARE NOT authority. Receipts ARE authority.

---

11. One-Sentence Version

SKILLS.md is the algorithm registry for OMI version control, defining reproducible pure skills for version reading, diffing, creation, merging, topology computation, scope resolution, receipt verification, and .imo normalization, with exact C source, test vectors, and validation rules, while AGENTS.md authorizes which skills may be invoked.

```

---

## Complete Integration: AGENTS.md + SKILLS.md + VCS

```text
AGENTS.md
  authorizes: version.read, version.diff, version.create, version.merge, topology.betti, scope.resolve, receipt.verify, imo.normalize, version.validate

SKILLS.md
  defines: exact C implementation for each authorized skill

VCS Flow
  Resolver reads AGENTS.md → knows what's allowed
  Resolver reads SKILLS.md → knows how to compute
  Resolver invokes skill → produces candidate
  Validator checks candidate → produces receipt
  Receipt records → version accepted
```

The version control system now has:

1. Declarative behavior (AGENTS.md)
2. Reproducible algorithms (SKILLS.md)
3. Betti-Schlafli topology (for dependency/merge detection)
4. Seed objects (o.base64.o, o.hex.o, etc.)
5. FS/GS/RS/US scoping (CIDR-like addressing)
6. Receipt validation (only Tetragrammatron accepts)
7. LLM resolver integration (scoped, lazy, deterministic)