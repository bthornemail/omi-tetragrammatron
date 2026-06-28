# REPO.md

# Request for Collaboration and Role/Repo Based Access Control Specification

Version: `0.1.0`  
Status: Draft  
Authority: Repository Collaboration Boundary  
Scope: OMI-compatible repositories, skills, agents, carriers, receipts, and contribution workflows

---

## 0. Purpose

This document defines the repository collaboration authority for an OMI-compatible project.

`REPO.md` is the repository-level Request for Collaboration file. It defines:

```text
who may collaborate
which roles exist
which repository scopes are accessible
which actions are allowed
which actions are forbidden
which skills may be invoked
which files are authoritative
which receipts are required
how contributions become accepted
```

`REPO.md` exists so that collaboration can be declarative, scoped, lazy, deterministic, inspectable, and receipt-bound.

This document defines **Role/Repo Based Access Control**, abbreviated here as **RRBAC**.

RRBAC means:

```text
Role controls what kind of actor is requesting access.
Repo scope controls where that actor may operate.
Skill scope controls what algorithm may be used.
Receipt controls what becomes accepted.
```

---

## 1. RFC 2119 Keyword Policy

The key words:

```text
MUST
MUST NOT
REQUIRED
SHALL
SHALL NOT
SHOULD
SHOULD NOT
RECOMMENDED
MAY
OPTIONAL
```

are to be interpreted as normative requirement levels.

In this document:

```text
MUST / REQUIRED / SHALL
  indicates a mandatory rule for compliant repositories.

MUST NOT / SHALL NOT
  indicates a forbidden behavior.

SHOULD / RECOMMENDED
  indicates a strong default rule that may be overridden only with explicit receipt.

MAY / OPTIONAL
  indicates permitted behavior that is not required.
```

---

## 2. Relationship to Other Repository Files

An OMI-compatible repository SHOULD use the following authority split:

```text
REPO.md
  repository collaboration authority

AGENTS.md
  resolver and agent behavior authority

SKILLS.md
  deterministic algorithm and workflow authority

*.imo
  normalized carrier declarations

receipts.imo
  accepted receipt export

README.md
  public human introduction

LICENSE
  legal permission boundary
```

### 2.1 REPO.md

`REPO.md` MUST define collaboration roles, scopes, access rules, contribution gates, and receipt requirements.

`REPO.md` MUST NOT define the full algorithmic implementation of every skill unless the repository intentionally has no separate `SKILLS.md`.

### 2.2 AGENTS.md

`AGENTS.md` MUST define resolver behavior, model behavior, agent constraints, action rules, side-effect rules, and inspection requirements.

`AGENTS.md` SHOULD answer:

```text
How should an agent act here?
What must it check?
What must it refuse?
What must it validate?
What must it cite?
```

### 2.3 SKILLS.md

`SKILLS.md` MUST define reproducible algorithms, workflows, skill declarations, input/output contracts, test vectors, and exact source blocks when the project requires deterministic skill execution.

`SKILLS.md` SHOULD answer:

```text
How is this computation performed?
What inputs are accepted?
What outputs are produced?
What tests prove reproducibility?
What carrier scope does it belong to?
```

### 2.4 `.imo` Files

`.imo` files SHOULD carry normalized OMI declarations extracted from human-readable source files.

`.imo` files MUST NOT be treated as accepted authority unless their contents validate into receipts.

### 2.5 Receipts

Receipts are the accepted collaboration boundary.

A repository MAY receive declarations, patches, skill definitions, model outputs, or `.imo` files from any actor, but it MUST accept them only through validation and receipt.

---

## 3. Foundational Rule

The repository collaboration rule is:

```text
Request does not accept.
Role does not accept.
Agent does not accept.
Skill does not accept.
File does not accept.
Patch does not accept.
Projection does not accept.

Validation and receipt accept.
```

The repository MUST preserve the production posture:

```text
recognize
  ↓
cite
  ↓
validate
  ↓
record
  ↓
project
  ↓
inspect
```

The repository MUST avoid the trap:

```text
render → believe
```

For LLM-assisted collaboration, the repository MUST also avoid:

```text
generate → believe
```

---

## 4. Scope Model

RRBAC uses FS/GS/RS/US scope.

```text
FS = File Scope
GS = Group Scope
RS = Record Scope
US = Unit Scope
```

The reduced object forms are:

```text
fs.o
gs.o
rs.o
us.o
o.o
```

A repository scope SHOULD be expressed as a CIDR-like prefix:

```text
fs.o/<file>
fs.o/<file>/gs.o/<group>
fs.o/<file>/gs.o/<group>/rs.o/<record>
fs.o/<file>/gs.o/<group>/rs.o/<record>/us.o/<unit>
```

Example:

```text
fs.o/SKILLS.md/gs.o/encoding/rs.o/base64/us.o/decode
```

This identifies:

```text
file:   SKILLS.md
group:  encoding
record: base64
unit:   decode
```

### 4.1 Scope Specificity

More specific scopes override less specific scopes only when permitted by `REPO.md`.

Example:

```text
fs.o/SKILLS.md
```

is less specific than:

```text
fs.o/SKILLS.md/gs.o/encoding/rs.o/base64/us.o/decode
```

A role MAY be allowed to read broad file scope while only being allowed to write unit scope.

### 4.2 Scope Isolation

A role granted access to one scope MUST NOT assume access to sibling scopes.

Example:

```text
fs.o/SKILLS.md/gs.o/encoding/rs.o/base64
```

does not imply write access to:

```text
fs.o/SKILLS.md/gs.o/encoding/rs.o/utf8
```

unless explicitly granted.

---

## 5. Role Model

A repository MUST define collaboration roles.

The following roles are RECOMMENDED.

### 5.1 Observer

An Observer MAY read public repository materials.

An Observer MUST NOT modify files, submit executable skill declarations, operate tools, or request side effects.

Recommended scope:

```text
read: public
write: none
execute: none
receipt: none
```

### 5.2 Reader

A Reader MAY inspect files, declarations, receipts, and projections.

A Reader MUST NOT accept receipts.

A Reader MUST NOT project side effects.

Recommended scope:

```text
read: fs.o/**
write: none
execute: read-only skills
receipt: none
```

### 5.3 Resolver

A Resolver MAY interpret requests, select applicable scopes, cite declarations, and propose candidate actions.

A Resolver MUST NOT mutate repository state without a validated receipt.

A Resolver MUST distinguish between:

```text
recognized
candidate
validated
accepted
projected
```

Recommended scope:

```text
read: fs.o/**
write: candidate-only
execute: pure/read-only skills
receipt: candidate receipt only
```

### 5.4 Contributor

A Contributor MAY propose patches, declarations, documentation, tests, skills, and `.imo` carriers.

A Contributor MUST submit changes as candidates.

A Contributor MUST NOT mark their own contribution as accepted unless explicitly granted Maintainer or Validator authority.

Recommended scope:

```text
read: fs.o/**
write: candidate branches
execute: declared tests
receipt: candidate receipt
```

### 5.5 Maintainer

A Maintainer MAY merge accepted changes, edit repository files, and curate scopes.

A Maintainer SHOULD NOT bypass validation.

A Maintainer MUST NOT project unsafe side effects without a receipt.

Recommended scope:

```text
read: fs.o/**
write: approved scopes
execute: tests and pure skills
receipt: merge receipts
```

### 5.6 Validator

A Validator MAY run validation rules and accept or reject candidate declarations.

A Validator MUST record validation results as receipts.

A Validator MUST NOT modify candidate content while validating it, except to annotate rejection reasons.

Recommended scope:

```text
read: candidate + canonical scopes
write: receipt scopes
execute: validation skills
receipt: accepted/rejected
```

### 5.7 Skill Author

A Skill Author MAY define algorithms in `SKILLS.md`.

A Skill Author MUST provide reproducibility metadata for deterministic skills.

A Skill Author SHOULD provide:

```text
name
scope
version
input contract
output contract
effect class
source block
test vectors
algorithm hash
validation rule
```

A Skill Author MUST NOT claim skill acceptance without validation receipt.

### 5.8 Agent Author

An Agent Author MAY edit `AGENTS.md`.

An Agent Author MUST preserve repository side-effect rules.

An Agent Author MUST NOT grant broader permissions than `REPO.md` allows.

### 5.9 Projector

A Projector MAY render accepted receipts into documentation, UI, diagrams, geometry, JSON, SVG, DOM, or other projection surfaces.

A Projector MUST NOT treat projection as acceptance.

A Projector MUST project only accepted receipts unless clearly marked as candidate, preview, or rejected.

### 5.10 Administrator

An Administrator MAY alter repository roles, scope grants, and access policy.

An Administrator SHOULD record role changes as receipts.

An Administrator MUST NOT erase receipt history except under an explicitly declared archival, privacy, or security policy.

---

## 6. Access Classes

A repository SHOULD classify actions as follows.

### 6.1 Read

Read access permits inspection.

Read access MUST NOT imply write, execute, validate, or project access.

### 6.2 Cite

Cite access permits an actor to create a citation candidate.

Cite access MUST NOT imply receipt acceptance.

### 6.3 Propose

Propose access permits an actor to submit candidate changes.

Candidate changes MUST be marked as candidate until validated.

### 6.4 Validate

Validate access permits an actor or skill to test a candidate.

Validation MUST produce an accepted or rejected receipt.

### 6.5 Record

Record access permits writing to the receipt surface.

Record access MUST be restricted.

### 6.6 Project

Project access permits rendering accepted receipts to a visible or operational surface.

Project access MUST NOT imply acceptance authority.

### 6.7 Execute

Execute access permits running a skill or tool.

Execution MUST be classified by effect:

```text
pure
read-only
local-write
repo-write
network
hardware
security-sensitive
```

### 6.8 Side Effect

Side-effect access includes:

```text
file write
git commit
git push
network send
email send
package publish
hardware actuation
credential use
deployment
destructive operation
```

Side effects MUST require explicit permission and accepted receipt.

---

## 7. Effect Classes

Every skill or action SHOULD declare an effect class.

### 7.1 Pure

A pure skill reads inputs and produces output without side effects.

Pure skills MAY run during candidate resolution.

### 7.2 Read-Only

A read-only skill may inspect repository files, logs, or metadata.

Read-only skills MUST NOT modify state.

### 7.3 Local-Write

A local-write skill may write temporary files, generated artifacts, or local build outputs.

Local-write skills SHOULD be restricted to declared output paths.

### 7.4 Repo-Write

A repo-write skill may modify repository-tracked files.

Repo-write skills MUST require role authorization and validation.

### 7.5 Network

A network skill may send or receive data across network boundaries.

Network skills MUST be explicitly authorized.

### 7.6 Hardware

A hardware skill may actuate physical devices.

Hardware skills MUST require accepted receipt before projection.

### 7.7 Security-Sensitive

A security-sensitive skill may access secrets, credentials, deployment systems, signing keys, or protected infrastructure.

Security-sensitive skills MUST require Administrator or explicitly delegated authority.

---

## 8. Skill Authorization

A role MAY use only skills allowed by `REPO.md` and constrained by `AGENTS.md`.

A skill declaration SHOULD be shaped as:

```omi-lisp
(skill.name . skill.example)
(skill.scope . fs.o/SKILLS.md/gs.o/example/rs.o/algorithm/us.o/v1)
(skill.effect . pure)
(skill.input . declared-input-type)
(skill.output . declared-output-type)
(skill.validation . required-test-vector)
```

A role grant SHOULD be shaped as:

```omi-lisp
(role.resolver.allowed-skill . skill.example)
(role.resolver.scope . fs.o/SKILLS.md/gs.o/example)
(role.resolver.effect-limit . read-only)
```

A repository MUST reject skill execution when:

```text
the role is not authorized
the scope does not match
the effect class exceeds the role
the required validation is missing
the carrier is malformed
the skill version is unaccepted
```

---

## 9. Request for Collaboration Flow

A Request for Collaboration, abbreviated **RFC** in this repository context, is a scoped invitation to propose, validate, project, or maintain a relation.

This repository RFC is not an internet standard RFC. It is a repository-local collaboration request.

### 9.1 RFC States

A collaboration request SHOULD move through these states:

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

Rejected path:

```text
candidate
  ↓
under-review
  ↓
rejected
  ↓
receipt recorded
```

### 9.2 RFC Declaration

A collaboration request SHOULD be declared as:

```omi-lisp
(rfc.id . rfc-0001)
(rfc.title . "Add Base64 seed object")
(rfc.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(rfc.role.requested . SkillAuthor)
(rfc.effect . pure)
(rfc.skill . skill.base64.zero-polynomial)
(rfc.status . candidate)
```

### 9.3 Required RFC Fields

A repository RFC MUST include:

```text
id
title
requester
role requested
scope requested
effect class
files touched
skills invoked
validation required
acceptance criteria
rollback/rejection rule
```

### 9.4 RFC Acceptance

A repository RFC MUST NOT be accepted unless:

```text
the requested role is valid
the requested scope is valid
the requested effect class is permitted
required tests pass
required validations pass
a receipt is recorded
```

---

## 10. Version-Control Authority

A version is not merely a file state.

A version is an accepted scoped relation over a carrier file.

A version SHOULD be declared as:

```omi-lisp
(version.id . receipt-2820)
(version.parent . receipt-2819)
(version.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64/us.o/schema)
(version.seed . o.base64.o)
(version.route . omi-base-64-imo)
(version.carrier . SKILLS.md)
(version.algorithm . skill.base64.zero-polynomial)
(version.effect . pure)
(version.result . accepted)
```

A repository MUST distinguish:

```text
file diff
relation delta
algorithm delta
scope delta
receipt delta
projection delta
```

A commit MAY carry many candidate declarations.

Only accepted receipts define accepted version state.

---

## 11. Seed Object Policy

A repository MAY define seed objects such as:

```text
o.o
o.base64.o
o.hex.o
o.binary.o
o.utf8.o
o.jabcode.o
o.ipv6.o
```

The neutral seed is:

```text
o.o
```

A scoped seed such as:

```text
o.base64.o
```

SHOULD mean:

```text
Base64-scoped neutral seed object
```

A seed object MUST NOT imply payload acceptance.

A seed object declares an interpretation scope.

A payload instance MUST still be validated.

Example:

```omi-lisp
(seed.base64 . o.base64.o)
(seed.base64.route . omi-base-64-imo)
(seed.base64.effect . pure)
(seed.base64.status . declared)
```

---

## 12. AGENTS.md Policy

`AGENTS.md` MUST NOT override `REPO.md`.

`AGENTS.md` MAY narrow permissions granted by `REPO.md`.

`AGENTS.md` MUST NOT broaden permissions beyond `REPO.md`.

If `AGENTS.md` and `REPO.md` conflict, `REPO.md` SHALL control.

`AGENTS.md` SHOULD include:

```text
resolver behavior
tool-use policy
testing policy
citation policy
side-effect policy
error handling
style/canon rules
```

Example:

```omi-lisp
(agent.must.cite . before-claim)
(agent.must.validate . before-project)
(agent.must.not . side-effect-before-receipt)
(agent.allowed-skill . skill.extract.omi-blocks)
```

---

## 13. SKILLS.md Policy

`SKILLS.md` MUST define reproducible algorithms for deterministic skills.

A skill that affects repository state MUST include:

```text
name
scope
version
effect class
input contract
output contract
algorithm source
test vectors
validation rule
```

A skill SHOULD include exact source blocks when reproducibility matters.

Example:

```omi-lisp
(skill.slot5040.name . "Fano-role-local ring slot")
(skill.slot5040.scope . fs.o/SKILLS.md/gs.o/ring/rs.o/slot5040/us.o/v1)
(skill.slot5040.input . "(fano7 role3 local240)")
(skill.slot5040.output . "slot in 0..5039")
(skill.slot5040.formula . "fano7*720 + role3*240 + local240")
(skill.slot5040.effect . pure)
```

A skill MUST NOT be executed if its effect class exceeds the caller's role grant.

---

## 14. `.imo` Carrier Policy

`.imo` files SHOULD be generated from accepted or candidate OMI-Lisp declarations.

`.imo` files MUST be treated as carriers, not authorities.

A `.imo` file MAY contain:

```text
normalized declarations
scope maps
skill references
seed references
receipt exports
carrier hashes
```

A `.imo` file MUST NOT create access by itself.

Access is granted only by `REPO.md` policy and accepted receipts.

---

## 15. Receipt Policy

A receipt MUST record the accepted or rejected result of a validation process.

A receipt SHOULD include:

```text
receipt id
parent receipt
scope
role
requester
skill
effect class
input hash
output hash
validation result
logical cycle
result
```

A rejection receipt SHOULD include a reason.

Example:

```omi-lisp
(receipt.id . receipt-2820)
(receipt.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(receipt.role . SkillAuthor)
(receipt.skill . skill.base64.zero-polynomial)
(receipt.result . accepted)
```

A repository MUST NOT claim acceptance without a receipt.

---

## 16. Conflict Policy

A conflict occurs when two or more candidate declarations attempt to modify incompatible relations in the same scope.

Conflicts SHOULD be classified as:

```text
text conflict
scope conflict
role conflict
skill conflict
algorithm conflict
receipt conflict
topology conflict
```

A conflict MUST be resolved before acceptance.

A merge MUST produce a receipt.

If a merge introduces a dependency loop, the repository SHOULD mark the topology as cyclic.

If the repository supports Betti signatures, it MAY record:

```text
b0 = connected components
b1 = loops
b2 = enclosed voids
```

Example:

```omi-lisp
(conflict.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(conflict.kind . algorithm-conflict)
(conflict.betti . "(1 1 0)")
(conflict.result . rejected)
```

---

## 17. Delegation Policy

A role MAY delegate a subset of its authority only if `REPO.md` permits delegation.

Delegation MUST be narrower than or equal to the delegator's authority.

Delegation MUST specify:

```text
delegate
role
scope
effect class
duration or expiration
revocation rule
```

Example:

```omi-lisp
(delegate.from . Maintainer)
(delegate.to . Contributor)
(delegate.scope . fs.o/SKILLS.md/gs.o/docs)
(delegate.effect . repo-write)
(delegate.expires . receipt-cycle-3000)
```

Delegation SHOULD be recorded as a receipt.

---

## 18. Revocation Policy

A repository MUST support revocation of role grants.

Revocation MUST NOT erase historical receipts.

Revocation SHOULD affect future actions only.

Example:

```omi-lisp
(revoke.role . Contributor)
(revoke.scope . fs.o/SKILLS.md/gs.o/encoding)
(revoke.reason . superseded-permission)
```

---

## 19. LLM Resolver Policy

An LLM acting in this repository MUST be treated as a Resolver unless explicitly granted another role.

An LLM Resolver MUST:

```text
read REPO.md before assuming access
obey AGENTS.md within REPO.md limits
use SKILLS.md for reproducible algorithms
distinguish candidate from accepted
avoid side effects before receipt
cite scope when proposing changes
```

An LLM Resolver MUST NOT:

```text
invent authority
claim acceptance without validation
execute side effects from generated text
treat projection as truth
broaden its own role
ignore scope restrictions
```

An LLM MAY propose patches, explanations, tests, declarations, and candidate receipts if its role allows.

---

## 20. Network Collaboration Policy

A repository MAY receive collaboration requests from peers.

Peer-provided declarations MUST be treated as candidates.

Peer-provided `.imo` files MUST be validated locally.

Peer-provided receipts MAY be inspected, but they MUST NOT be accepted blindly.

A receiving repository MUST validate under local `REPO.md` policy.

Network rule:

```text
send does not accept
receive does not accept
validation accepts
receipt records
```

---

## 21. Minimal Compliant REPO.md Declaration Block

A repository SHOULD include a machine-readable declaration block.

```omi-lisp
(repo.name . "omi-core")
(repo.protocol . "OMI-RRBAC")
(repo.version . "0.1.0")
(repo.authority . REPO.md)

(repo.scope.root . fs.o)
(repo.scope.group . gs.o)
(repo.scope.record . rs.o)
(repo.scope.unit . us.o)

(repo.file.agents . AGENTS.md)
(repo.file.skills . SKILLS.md)
(repo.file.receipts . receipts.imo)

(role.default . Observer)
(role.llm.default . Resolver)

(policy.acceptance . receipt-required)
(policy.side-effect . forbidden-before-receipt)
(policy.projection . accepted-receipts-only)
(policy.conflict . receipt-required)
```

---

## 22. Example Role Grants

```omi-lisp
(role.Observer.read . fs.o/README.md)
(role.Observer.write . none)
(role.Observer.execute . none)

(role.Resolver.read . fs.o/**)
(role.Resolver.write . candidate-only)
(role.Resolver.execute . pure)
(role.Resolver.must . cite-scope)

(role.Contributor.read . fs.o/**)
(role.Contributor.write . candidate-branch)
(role.Contributor.execute . pure/read-only)
(role.Contributor.must . submit-rfc)

(role.Validator.read . candidate/**)
(role.Validator.write . receipts.imo)
(role.Validator.execute . validation-skills)
(role.Validator.must . record-result)

(role.Maintainer.read . fs.o/**)
(role.Maintainer.write . approved-scopes)
(role.Maintainer.execute . tests)
(role.Maintainer.must . preserve-receipts)
```

---

## 23. Example Skill Grant

```omi-lisp
(skill.allowed . skill.extract.omi-blocks)
(skill.allowed.scope . fs.o/SKILLS.md/gs.o/parser)
(skill.allowed.effect . read-only)

(skill.allowed . skill.slot5040)
(skill.allowed.scope . fs.o/SKILLS.md/gs.o/ring/rs.o/slot5040)
(skill.allowed.effect . pure)

(skill.denied . skill.hardware.project)
(skill.denied.reason . requires-explicit-hardware-receipt)
```

---

## 24. Example Request for Collaboration

```omi-lisp
(rfc.id . rfc-0002)
(rfc.title . "Add Base64 seed object")
(rfc.requester . contributor.local)
(rfc.role.requested . SkillAuthor)
(rfc.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(rfc.effect . pure)
(rfc.files . "(SKILLS.md fs.imo)")
(rfc.skill . skill.base64.zero-polynomial)
(rfc.validation . test.base64.vectors)
(rfc.status . candidate)
```

Acceptance receipt:

```omi-lisp
(receipt.accepts . rfc-0002)
(receipt.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(receipt.result . accepted)
(receipt.effect . pure)
```

---

## 25. Security Requirements

A compliant repository MUST protect:

```text
credentials
signing keys
deployment tokens
network write paths
hardware actuation paths
package publishing paths
receipt history
role grants
```

A repository MUST NOT allow an LLM Resolver to access secrets unless explicitly granted by role and scope.

A repository MUST NOT allow candidate declarations to trigger network, file, or hardware side effects before receipt.

A repository SHOULD record rejected attempts as rejection receipts when security policy is involved.

---

## 26. Compliance Levels

### Level 0: Informational

Repository has a `REPO.md`, but no machine-readable declarations.

### Level 1: Declarative

Repository defines roles, scopes, and policies in `REPO.md`.

### Level 2: Scoped

Repository uses FS/GS/RS/US scope prefixes.

### Level 3: Skill-Gated

Repository integrates `SKILLS.md` skill declarations.

### Level 4: Receipt-Gated

Repository requires receipts for accepted changes.

### Level 5: Network-Collaborative

Repository can receive peer declarations and validate locally.

### Level 6: Side-Effect Safe

Repository enforces receipt-before-side-effect for tools, hardware, deployment, and network actions.

A repository claiming OMI-RRBAC compliance SHOULD declare its level:

```omi-lisp
(repo.compliance . OMI-RRBAC-Level-4)
```

---

## 27. Final Canon

`REPO.md` is the collaboration authority.

`AGENTS.md` is the resolver behavior authority.

`SKILLS.md` is the reproducible algorithm authority.

`.imo` files are normalized carriers.

Receipts are accepted authority.

Roles define who may act.

Scopes define where action may occur.

Skills define how computation is performed.

Effects define what risk the action carries.

Validation defines whether a candidate is accepted.

No actor, agent, file, model, skill, branch, projection, or peer may accept repository state without receipt.

---

## 28. One-Sentence Summary

`REPO.md` defines Role/Repo Based Access Control for OMI collaboration by binding roles, scopes, skills, effects, files, `.imo` carriers, and receipt gates into a repository-local Request for Collaboration protocol where every contribution remains a candidate until validated and recorded as an accepted receipt.