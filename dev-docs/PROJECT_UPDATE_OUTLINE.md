# Project Update Outline

This file indexes the archived update documents in `dev-docs/archive/` and turns them into an implementation roadmap. The archive is source material, not active build context. Active changes should be pulled forward deliberately into source, tests, and main docs.

## 0. Current Status

- C core scaffold has started in `core/`: `core/omi.h`/`core/omi.c` now expose hardened fixed-width types, arena/gauge/bridge scaffold APIs, bitboard masks, and conservative projection gates.
- Runtime application rename is complete: `core/omicron.c`, `core/omicron.o`, and `core/omicron.bin` replaced the legacy first-version `opencode` names.
- Omicron V0 boot/context/dialect scaffold is implemented in `core/omicron.h` and `core/omicron.c`: config initialization, CLI mode selection, gauge pre-header staging, OMI-Lisp induction guard, system object binding checks, address candidate lowering, Omi-Log candidate parsing, and focused C tests. `core/omicron.c` is still monolithic, but legacy dispatch is boxed behind `OmicronConfig` and `omicron_boot`.
- Metatron Scribe V0 is implemented as deterministic accepted-state-to-notation scribing with `--scribe <surface>`, declaration-only surfaces, and focused C tests.
- Barcode carrier scaffold exists in `portal/src/lib/jabcodeCarrier.ts`, ported from `dev-docs/archive/viewer/public/jabcode-carrier.js`. The canonical carrier family is `omi-barcode`; the custom current form is `omi-jabcode`. The current output is debug-only and must not be treated as an active standards JABCode carrier unless it is replaced with an implementation that follows `archive/JabCode.BSI-TR-03137.pdf`.
- The archive contains broader design targets that are not yet fully implemented: full arena runtime, tokenizer/parser declarations, accepted-state-gated projection, adapter policy, repository role policy, P2P sync, hardware targets, and doctrine/docs consolidation.

## 1. Canon And Authority Model

Purpose: stabilize the language used across root docs, `agent-docs/`, portal docs, and implementation comments before deeper runtime work.

Primary archive sources:
- `archive/The Doctrine of Determinism.md`
- `archive/Reasoning Behind the OMI Protocol.md`
- `archive/OMI Protocol Write-Up.md`
- `archive/OMI as a New Paradigm.md`
- `archive/C core.md`

Core decisions to carry forward:
- Keep the four authorities separate: OMI citation, Tetragrammatron validation, Metatron projection, IMO carrier.
- Preserve the Omi-Ring as the addressed palindromic notation witness and the receipt as the accepted state of that witness.
- Encode determinism only; adapter and I/O behavior may carry nondeterministic world events but must not become deterministic computation.
- Treat recognition, citation, Omi-Ring formation, closure, projection, bridge staging, and scan/render as non-acceptance. Validation plus accepted receipt state is the acceptance boundary.

Deliverables:
- Main `README.md` canon section.
- Root `AGENTS.md` refinement if needed.
- A short authority-boundary note in source-facing docs.
- `agent-docs/REPO.md` consolidation for public LLM-agent bootstrapping.
- Explicit statement that portal/agent-facing docs are separate from root repository authority docs.

Public-interface docs:
- `agent-docs/AGENTS.md`
- `agent-docs/SKILLS.md`
- `agent-docs/ADAPTERS.md`
- `agent-docs/REPO.md`

Agent REPO merge target:
- Merge the archive REPO drafts and REPO addenda into `agent-docs/REPO.md`.
- Keep it scoped to bootstrapping LLM agents and role/repo policy through the published `portal/` web portal.
- Do not merge portal public-interface policy into root `README.md`, root `AGENTS.md`, or root `SKILLS.md`.

## 2. C Runtime Scaffold To Reference Runtime

Purpose: evolve the current scaffold into the fuller reference runtime without breaking existing CLI behavior.

Primary archive sources:
- `archive/The OMI Uniform Bitboard.md`
- `archive/The Uniform Bitboard, Pre-Language, OMI-Lisp, and Pseudo-Persistent Open World Substrate.md`
- `archive/OMI Pre-Language Control Envelope Authority - Replacing LL-MM-NN with FS-GS-RS-US.md`
- `archive/OMI Gauge Pre-Header Authority.md`
- `archive/OMI Gauge Header and Orbit Authority.md`
- `archive/OMI eMMC Boot Target Example.md`

Depreciated implementation sources:
- `depreciated/Reference-Final Implementation - Complete Files.md`
- `depreciated/Master Header - (omi.h) - Hardened Implementation.md`
- `depreciated/Normative Errata.md`

Implementation phases:
- Phase 2.1: finish scaffold cleanup with warning-safe helper functions and tests.
- Phase 2.2: completed rename of the legacy application target from `opencode` to `omicron`: `opencode.c` -> `omicron.c`, `opencode.o` -> `omicron.o`, `opencode.bin` -> `omicron.bin`, with `Makefile`, smoke tests, docs, and CLI references updated.
- Phase 2.3: keep the runtime implementation split across `core/omi.c`, `core/imo.c`, `core/tetragrammatron.c`, `core/metatron.c`, and the thin orchestration file `core/omicron.c`.
- Phase 2.4: implemented Omicron boot/context/dialect scaffold with `OmicronConfig`, `OmicronMode`, `OmicronDialect`, deterministic pre-header staging, object binding checks, address candidate lowering, Omi-Log candidate parsing, and C tests.
- Phase 2.5: add full gauge table behavior, bitboard folds, bridge slots, and explicit boot mode handling.
- Phase 2.6: reduce `core/omicron.c` in place so `main()` uses `OmicronConfig` and dispatches authority modules without changing CLI behavior. Legacy dispatch is currently boxed.
- Phase 2.7: started parser candidate construction with explicit input buffers and Omi-Log declaration heads, without treating bytes as acceptance.
- Phase 2.8: add fuller parser form handling for declarations, dot relations, quote forms, and candidate construction.
- Phase 2.9: connect Omi-Ring validation candidates to Tetragrammatron-owned accepted-state storage without moving carrier I/O into OMI.
- Phase 2.10: add projection gates for accepted state, declared effect, role/scope permission, and bridge authorization.

Acceptance tests:
- Struct sizes and endian helpers.
- Gauge table active entries and 0x1E/0x78/0x7F behavior.
- Bridge staging for 0x001E, 0x0078, 0x7C00, 0x007F, 0xAA55.
- Tokenizer/parser fixture tests.
- Projection denial before accepted receipt state.
- Existing `make smoke` must keep passing before and after the `omicron` rename.

## 3. Pre-Language, Notation, And OMI-Lisp

Purpose: formalize FS/GS/RS/US, the 256-bit OMI-IMO frame, dot notation, and derived readable syntax.

Primary archive sources:
- `archive/OMI Pre-Language Control Envelope Authority - Replacing LL-MM-NN with FS-GS-RS-US.md`
- `archive/OMI Citation Notation and Gauge Pre-Header.md`
- `archive/OMI Gauge Pre-Header Authority.md`
- `archive/OMI Gauge Header and Orbit Authority.md`
- `archive/OMI Notation as Citation.md`
- `archive/Derived Non-Printing OMI-Lisp.md`
- `archive/OMI-Lisp_Complete_Specification.md`

Implementation targets:
- Canonical pre-header recognition: `0xF* 00 1C 1D 1E 1F 20 0xF*`.
- Fast-fail parser helpers for raw OMI gauge carriers.
- Round-trip display form for OMI-IMO frames.
- Dot relation parser that maps to CAR/CDR closure.
- Clear separation between byte witness, Omi-Ring candidate, validation, accepted receipt state, and projection.

Non-goals for first pass:
- No accepting declarations directly from notation.
- No hidden side effects from parser output.
- No replacing the existing S-expression evaluator until fixtures prove compatibility.

## 4. Receipts, Versioning, And Repository Policy

Purpose: define how updates, skills, agents, and collaboration requests become scoped Omi-Ring candidates and accepted receipt states.

Primary archive sources:
- `archive/REPO.md - First Draft.md`
- `archive/REPO.md - OMI Request for Collaboration and Role-Repo Based Access Control.md`
- `archive/AGENTS.md`
- `archive/SKILLS.md`
- `archive/OMI Version Control from Place-Value Notation, Betti-Schlafli Interpretation, and Seed Objects.md`
- `archive/REPO.md Addendum - Bootstrapping an OMI-Compatible LLM Agent.md`
- `archive/REPO.md Addendum - Emergent P2P and Multimedia Use Cases for OMI.md`

Implementation targets:
- Treat root `README.md`, root `AGENTS.md`, and root `SKILLS.md` as repository/runtime authority docs.
- Treat `agent-docs/AGENTS.md`, `agent-docs/SKILLS.md`, `agent-docs/ADAPTERS.md`, and `agent-docs/REPO.md` as public web-portal interface docs for bootstrapping external/LLM agents through the published portal.
- Define role/effect classes in a small machine-readable block.
- Add pure skill registry examples for `delta16`, `bqf32`, `slot5040`, receipt verification, and scope resolution.
- Build a minimal deterministic version-candidate format before implementing merge/rollback.
- Keep portal public-interface policies separate and distinct from root repository policies unless an explicit bridge document says otherwise.

Acceptance criteria:
- Pure/read-only/repo-write/network/hardware/security-sensitive effects are explicit.
- Side effects require accepted receipt state and policy allowance.
- LLM/agent behavior remains advisory unless validated and recorded.

## 5. Carriers, Adapters, And Browser Surfaces

Purpose: keep carriers and projections outside validation while making browser and device workflows usable.

Primary archive sources:
- `archive/ADAPTERS.md`
- `archive/2026-06-28-Project_Architecture_Check.md`
- `archive/OMI as a Pseudo-Persistent Open World Model.md`
- `archive/REPO.md Addendum - Emergent P2P and Multimedia Use Cases for OMI.md`
- `archive/JabCode.BSI-TR-03137.pdf`

Current state:
- `omi-barcode` / `omi-jabcode` debug carrier is implemented as deterministic debug matrix, not full BSI TR 03137 compliance.
- Portal TypeScript surfaces and module tests exist for barcode debug carrier, receipt surface cascade, and gnomonic surface.

Implementation targets:
- Do not promote JABCode as an active carrier unless implementation follows the official specification in `archive/JabCode.BSI-TR-03137.pdf`.
- If official JABCode compliance is not implemented, leave JABCode out of the active carrier roadmap and rely on OMI citation notation.
- Use OMI citation notation as the sufficient canonical relation description: `omi---imo`, `o---o`, `/---/`, `?---?`, and `@---@`.
- Document any carrier as carrier recovery only, never acceptance.
- Add adapter declarations for browser, serial, DOM/CSSOM/Canvas/WebGL, and ESP32.
- Add projection gate calls before any hardware/network/browser side-effect adapter.

Future optional work:
- Official-spec JABCode encoder/decoder behind the existing API, using `archive/JabCode.BSI-TR-03137.pdf` as the contract.
- Media bridge registry for stream/render/capture/transcode constants.
- Browser UI panels for carrier status and validation status as separate states.

## 6. Metatron Scribe, Geometry, And Surface Model

Purpose: align geometry surfaces with accepted-state projection while preserving Metatron as the scribe-transducer of accepted Omi-Ring states. Rendering is one output of scribing, not the whole role.

Primary archive sources:
- `archive/Reasoning Behind the OMI Protocol.md`
- `archive/OMI as a New Paradigm.md`

Depreciated research notes:
- `depreciated/Metatron Scribe.md`
- `depreciated/Clippings.md`

Active canon:
- `METATRON.md`

Implementation targets:
- Keep existing geometry renderers and incidence checks stable.
- Promote Metatron as accepted-state-to-notation transducer without making it validation authority.
- Clarify Omi-surfaces as projection fibers over accepted Omi-Ring states.
- Expand surface vocabulary only after accepted-state/projection gates are in place.
- Avoid turning visual surfaces into validation or acceptance authorities.

Potential tests:
- Existing `--check`, `--render-frame`, `--render-ppm`, OBJ/glTF output smoke.
- Browser surface tests for deterministic same-input same-output rendering.

## 7. Hardware, eMMC, ESP32, And P2P Worlds

Purpose: plan physical and distributed carrier behavior without compromising determinism.

Primary archive sources:
- `archive/OMI eMMC Target as Tetrahedral Centroid Memory Model.md`
- `archive/OMI as a Pseudo-Persistent Open World Model.md`
- `archive/Emergent P2P Use Cases & Attributes for OMI Protocol.md`
- `archive/REPO.md Addendum - Emergent P2P and Multimedia Use Cases for OMI.md`
- `archive/ADAPTERS.md`

Implementation targets:
- Hardware root bootstrap modes: local genesis, known seed receipt, signed envelope, hardware root, trusted peer.
- eMMC 512-bit boot envelope as carrier, not authority.
- ESP32 adapter declarations and receipt-gated hardware projection.
- P2P receipt exchange that imports Omi-Ring candidates, validates locally, and records local accepted states.

Non-goals until policy gates exist:
- No direct hardware actuation from parsed declarations.
- No network trust without local validation.
- No peer message accepted as canon solely by signature or transport.

## 8. Documentation Consolidation Plan

Purpose: reduce duplicate and overlapping archive material into a small active documentation set.

Suggested active docs:
- `README.md`: project purpose, build, modes, core authority model, current capabilities.
- `AGENTS.md`: coding constraints, determinism, authority boundaries, build/test rules.
- `SKILLS.md`: runtime specification and deterministic skill registry.
- `agent-docs/REPO.md`: public web-portal role/repo context produced by merging the REPO drafts and REPO addenda.
- `agent-docs/AGENTS.md`, `agent-docs/SKILLS.md`, `agent-docs/ADAPTERS.md`: public-interface agent/bootstrap policy docs for published portal usage.
- `dev-docs/PROJECT_UPDATE_OUTLINE.md`: this roadmap.
- `dev-docs/PROJECT_SPECIFICATON.md`: active standard project specification.
- `dev-docs/PROJECT_OUTLINE.md`: active standard project outline.
- `dev-docs/OMICRON.md`: active Omicron boot/context/dialect scaffold note.
- Optional future `dev-docs/IMPLEMENTATION_NOTES.md`: narrow notes for current phase only.

Archive handling:
- Leave `dev-docs/archive/` as historical/source context.
- Pull forward only resolved decisions.
- Do not treat contradictory archive drafts as active requirements until reconciled here or in root docs.

## 9. Recommended Next Work Order

1. Reconcile current C scaffold with `Normative Errata` and finish warning-safe tests.
2. Rename `opencode` runtime application files and build targets to `omicron`. Completed.
3. Merge REPO addenda and role-repo policy drafts into `agent-docs/REPO.md`, keeping portal policy separate from root policy. Completed.
4. Add pre-header and frame parser helpers with pure round-trip tests. Completed for the V0 Omicron pre-header/address candidate scaffold.
5. Add tokenizer/parser candidate construction for declarations. Started with V0 Omi-Log candidate parsing.
6. Connect Omi-Ring candidate validation to Tetragrammatron-owned accepted-state storage.
7. Implement projection gate enforcement for adapters.
8. Consolidate root README/AGENTS/SKILLS active docs separately from portal/agent docs.
9. Decide whether to implement official JABCode compliance or leave JABCode out of active carriers.
10. Add P2P/hardware demos only after gates are enforced.

## 10. Guardrails

- Do not add dependencies without approval.
- Do not introduce nondeterministic computation: no random, time, UUID, process ID, clocks, or `/dev/urandom`.
- Do not move file/network/hardware I/O into OMI, Tetragrammatron, or Metatron.
- Do not treat carrier decode, bridge recognition, parser closure, model output, or projection as acceptance.
- Keep existing CLI modes and smoke tests passing after each phase.
