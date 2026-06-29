# REPO.md addendum: Emergent P2P and Multimedia Use Cases for OMI

## Purpose

This addendum describes the peer-to-peer and multimedia capabilities that naturally emerge from the OMI protocol architecture.

OMI does not require a central authority. Each peer carries its own repository authority, resolver behavior, deterministic skill registry, normalized carriers, and receipt history.

The core rule is:

```text
Peers exchange carriers, candidates, and receipts.
Peers do not exchange truth.

Each peer validates locally.
Each peer accepts locally.
Each peer records its own receipts.
```

Canonical invariant:

```text
Recognition is not acceptance.
Citation is not acceptance.
Closure is not acceptance.
Projection is not acceptance.
Validation and receipt accept.
```

Network invariant:

```text
Send does not accept.
Receive does not accept.
Validation accepts.
Receipt records.
```

---

## 1. Core P2P Authority Split

OMI-compatible repositories use this authority split:

```text
REPO.md      → role/repo authority: who may act, where, and with what effect
AGENTS.md    → resolver behavior: how agents, LLMs, scripts, and peers must act
SKILLS.md    → algorithm registry: how deterministic computation is performed
*.imo        → normalized carriers: what declarations travel
receipts     → accepted/rejected state: what this peer has validated
```

P2P consequence:

```text
No peer is global authority.
No sender is automatically trusted.
No received carrier is automatically accepted.
Every peer validates under its own REPO.md.
```

---

## 2. Role/Repo Based Access Control in P2P

RRBAC means:

```text
role controls who or what is acting
repo scope controls where action may occur
skill scope controls what computation may be used
effect class controls what risk the action carries
receipt controls what becomes accepted
```

A peer may have different roles in different repositories.

Example:

```text
Peer A may be a Reader in repo-alpha.
Peer A may be a Contributor in repo-beta.
Peer A may be a Validator in repo-gamma.
```

A role is never universal.

A scope is never automatically global.

A receipt is always local unless another peer validates and accepts it.

---

## 3. P2P Use Case: Repository Federation

### Scenario

Multiple repositories collaborate without a central server or central authority.

Peer A sends a candidate declaration to Peer B.

Peer B validates the candidate under Peer B’s own `REPO.md`.

```omi-lisp
(peer.send . candidate)
(peer.receive . candidate)
(peer.validate . local-REPO-md)
(peer.receipt . store-if-accepted)
```

Network rule:

```text
send does not accept
receive does not accept
validation accepts
receipt records
```

### Attributes

```text
federation without blind trust
local validation
independent receipt histories
scope-isolated acceptance
peer-specific role grants
```

Peer B may accept a candidate in:

```text
fs.o/docs
```

while rejecting the same peer’s candidate in:

```text
fs.o/src
```

---

## 4. P2P Use Case: Distributed Skill Execution

### Scenario

A peer requests computation from another peer that has a special runtime, model, GPU, hardware device, dataset, or validated skill.

```omi-lisp
(peer.request . skill.audio.mix)
(peer.scope . fs.o/SKILLS.md/gs.o/audio/rs.o/mixer/us.o/v1)
(peer.effect . pure)
(peer.receipt . required)
```

For hardware or external effects:

```omi-lisp
(peer.request . skill.hardware.project)
(peer.scope . fs.o/hardware/gs.o/gpio)
(peer.effect . hardware)
(peer.receipt . required-before-projection)
```

### Attributes

```text
pure skills can be requested safely
read-only skills can be inspected
hardware/network/security-sensitive skills require explicit authorization
results return as declarations
results require validation before acceptance
```

Hardware result example:

```omi-lisp
((event . hardware-projected) . command-declaration)
```

The hardware command and the hardware result are different relations.

---

## 5. P2P Use Case: Cross-Peer Receipt Verification

### Scenario

A peer receives a receipt from another peer and must decide whether to accept it locally.

```omi-lisp
(skill.receipt.verify.name . "Verify receipt")
(skill.receipt.verify.input . receipt-handle)
(skill.receipt.verify.output . "(valid hash scope result)")
(skill.receipt.verify.effect . pure)
```

Verification SHOULD check:

```text
receipt exists
receipt hash is nonzero
receipt scope matches claim
receipt result is accepted or rejected
receipt parent is consistent when parent is declared
algorithm hash matches declared skill
effect class matches declared permission
```

Important boundary:

```text
A remote receipt proves what the remote peer accepted.
It does not prove what the local peer accepts.
```

Local acceptance requires local validation.

---

## 6. P2P Use Case: Decentralized Version Control

### Scenario

Multiple peers contribute to a shared version history without depending on a central Git server as final authority.

A version is not merely a file state.

A version is an accepted scoped relation over a carrier file.

```omi-lisp
(version.id . receipt-2820)
(version.parent . receipt-2819)
(version.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(version.seed . o.base64.o)
(version.route . omi-base-64-imo)
(version.carrier . SKILLS.md)
(version.algorithm . skill.seed.base64)
(version.effect . pure)
(version.result . accepted)
```

### Attributes

```text
scoped versions
receipt-based lineage
seed-object interpretation
carrier separation
local merge validation
rejection receipts
rollback as new relation, not history erasure
```

---

## 7. P2P Use Case: Skill Discovery and Skill Exchange

### Scenario

Peers discover deterministic skills from other peers and decide whether to import, test, or invoke them.

```omi-lisp
(skill.registry.name . "OMI Version Control Skills")
(skill.delta16.scope . fs.o/SKILLS.md/gs.o/math/rs.o/delta16/us.o/v1)
(skill.bqf32.scope . fs.o/SKILLS.md/gs.o/math/rs.o/bqf32/us.o/v1)
(skill.slot5040.scope . fs.o/SKILLS.md/gs.o/ring/rs.o/slot5040/us.o/v1)
```

Discovery flow:

```omi-lisp
(peer.discover . skill.delta16)
(peer.import . skill.delta16)
(peer.validate . skill.delta16)
(peer.invoke . skill.delta16)
```

A peer SHOULD invoke imported skills only after local validation.

### Attributes

```text
deterministic skills
test vectors
effect classes
versioned scope paths
local acceptance
denied-by-default side effects
```

---

## 8. P2P Use Case: Receipt Ring Synchronization

### Scenario

Peers exchange receipt histories and compare lineage.

OMI uses a 5040-slot receipt ring as a deterministic local receipt structure:

```c
#define OMI_RING_SLOTS 5040
#define OMI_RING_SLOT_SIZE 64
```

The slot rule is:

```text
slot5040 = fano7 * 720 + role3 * 240 + local240
```

where:

```text
fano7 ∈ 0..6
role3 ∈ 0..2
local240 ∈ 0..239
```

This gives:

```text
7 × 3 × 240 = 5040
```

### Boundary

The receipt ring is not global consensus by itself.

It is a local ordering and storage structure.

P2P agreement emerges only when peers compare, validate, and accept compatible receipt chains.

Better term:

```text
receipt ring synchronization
```

rather than:

```text
receipt ring consensus
```

---

## 9. P2P Use Case: Conflict Resolution

### Scenario

Two peers propose conflicting changes to the same scope.

```omi-lisp
(conflict.scope . fs.o/SKILLS.md/gs.o/encoding/rs.o/base64)
(conflict.kind . algorithm-conflict)
(conflict.betti . "(1 1 0)")
(conflict.resolution . proposed)
(conflict.result . rejected)
```

Betti interpretation:

```text
b0 = connected components
b1 = loops / cycles
b2 = enclosed unresolved voids
```

Optional Schläfli projection:

```text
(1 0 0) → {3,3} minimal tetrahedral seed
(1 1 0) → {4,3} cubical loop lattice
(1 0 1) → {5,3} enclosed semantic shell
```

### Attributes

```text
scope-aware conflict detection
topology-aware inspection
merge candidates
rejection receipts
historical preservation
rollback as new receipt
```

---

# 10. Emergent P2P Attributes

## 10.1 Zero-Trust Collaboration

OMI supports zero-trust collaboration by treating all incoming material as candidate material.

```text
identity: role-based and scope-bound
authorization: local REPO.md
verification: local validation
execution: effect-class gated
acceptance: receipt-bound
```

Boundary:

```text
If adversarial trust is required, the protocol SHOULD add cryptographic signatures and cryptographic hashes.
```

A scaffold hash such as FNV-1a is not sufficient for adversarial trust.

---

## 10.2 Local Sovereignty

Each peer validates under its own `REPO.md`.

A peer may:

```text
accept a candidate another peer rejects
reject a candidate another peer accepts
grant different roles
define different scopes
maintain independent receipt histories
support different skill versions
```

This is not inconsistency.

This is local sovereignty.

---

## 10.3 Verifiable Provenance

Every accepted change SHOULD have a receipt chain:

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
rejection receipt
```

Each receipt SHOULD identify:

```text
parent receipt
scope
skill or algorithm used
effect class
validation result
logical cycle
result
```

---

## 10.4 Graceful Degradation

If a peer is offline:

```text
local candidates can be drafted
local validation can continue
local receipts can be stored
.imo carriers can be prepared
sync can happen later
```

OMI does not require constant connectivity.

---

## 10.5 Evolutionary Protocol

Skills are versioned by scope.

```omi-lisp
(skill.delta16.version . v1)
(skill.delta16.scope . fs.o/SKILLS.md/gs.o/math/rs.o/delta16/us.o/v1)
(skill.delta16.input . "(x:uint16 c:uint16)")
(skill.delta16.output . "uint16")
(skill.delta16.formula . "rotl16(x,1) xor rotl16(x,3) xor rotr16(x,2) xor c")
```

Peers MAY support multiple versions simultaneously:

```text
us.o/v1
us.o/v2
us.o/v3
```

A peer MUST NOT assume that another peer supports the same skill version.

---

# 11. Multimedia P2P

OMI naturally extends to multimedia because media is treated as carrier material, not authority.

```text
video is a carrier
audio is a carrier
image is a carrier
3D model is a carrier
texture is a carrier
thumbnail is a carrier
```

A media file is not accepted merely because it renders.

Media must still pass validation and receipt.

---

## 11.1 Multimedia Seeds

Media interpretation can be declared with seed objects:

```omi-lisp
(seed.video . o.video.o)
(seed.audio . o.audio.o)
(seed.image . o.image.o)
(seed.model3d . o.model3d.o)
```

Example:

```omi-lisp
(seed.video . o.video.o)
(seed.video.codec . h264)
(seed.video.container . mp4)
(seed.video.route . omi-video-imo)

(seed.audio . o.audio.o)
(seed.audio.codec . aac)
(seed.audio.sample-rate . 48000)
(seed.audio.route . omi-audio-imo)

(seed.image . o.image.o)
(seed.image.codec . jpeg)
(seed.image.route . omi-image-imo)

(seed.model3d . o.model3d.o)
(seed.model3d.format . gltf)
(seed.model3d.route . omi-model3d-imo)
```

Seed rule:

```text
A seed declares interpretation scope.
A seed does not imply payload acceptance.
```

---

## 11.2 Multimedia Effect Classes

The base effect classes are:

```text
pure
read-only
local-write
repo-write
network
hardware
security-sensitive
```

Multimedia operations SHOULD initially be represented as declared effect subtypes rather than new core enum values.

Example:

```omi-lisp
(effect . network)
(effect.subtype . streaming)

(effect . hardware)
(effect.subtype . render)

(effect . local-write)
(effect.subtype . transcode)

(effect . security-sensitive)
(effect.subtype . capture)
```

This avoids changing the core ABI for every media operation.

Media effect mapping:

```text
streaming  → network
rendering  → hardware
transcode  → local-write
capture    → security-sensitive
inference  → hardware or security-sensitive, depending on model/data
```

---

## 11.3 Multimedia Bridge Policy

The base external bridge is:

```text
0xAA55 = external bridge authorization
```

Media-specific bridge words MAY be introduced later, but they SHOULD be treated as extension words, not required core protocol.

Candidate extension words:

```c
#define OMI_BRIDGE_MEDIA_STREAM     0x1A55u
#define OMI_BRIDGE_MEDIA_RENDER     0x1B55u
#define OMI_BRIDGE_MEDIA_CAPTURE    0x1C55u
#define OMI_BRIDGE_MEDIA_TRANSCODE  0x1D55u
```

Extension rule:

```text
Media bridge words MUST NOT bypass effect permission.
Media bridge words MUST NOT bypass validation.
Media bridge words MUST NOT write receipts directly.
```

---

# 12. Multimedia Use Case: Decentralized Video Streaming

### Scenario

Peers stream video chunks without a central CDN.

```omi-lisp
(media.video.id . video-abc123)
(media.video.codec . h264)
(media.video.resolution . 1920x1080)
(media.video.framerate . 30)
(media.video.bitrate . 5000)
(media.video.seed . o.video.o)
(media.video.effect . network)
```

Chunk receipt:

```omi-lisp
(receipt.accepts . video-chunk-001)
(receipt.scope . fs.o/media/gs.o/video/rs.o/chunks/us.o/001)
(receipt.effect . network)
(receipt.hash . chunk-hash)
(receipt.peer . peer-address-123)
(receipt.result . accepted)
```

Flow:

```text
Peer A
  creates chunk candidate
  validates chunk
  stores local receipt
  sends chunk carrier

Peer B
  receives chunk carrier
  validates locally
  stores accepted/rejected receipt
  requests next chunk if accepted
```

Streaming is a network effect.

Network effects require explicit authorization.

---

# 13. Multimedia Use Case: Collaborative Editing

### Scenario

Multiple peers edit a video, audio, image, or scene project.

Timeline declaration:

```omi-lisp
(media.timeline.id . timeline-456)
(media.timeline.scope . fs.o/media/gs.o/video/rs.o/timeline)
(media.timeline.tracks . "(video-001 audio-001 subtitles-001)")
(media.timeline.effects . "(fade-in keyframe transition)")
(media.timeline.effect . repo-write)
```

Edit candidate:

```omi-lisp
(change.status . candidate)
(change.scope . fs.o/media/gs.o/video/rs.o/timeline/us.o/track-001)
(change.operation . insert-frame-at-30s)
(change.carrier . frame-data.imo)
(change.validation . required)
```

Conflict example:

```omi-lisp
(conflict.scope . fs.o/media/gs.o/video/rs.o/timeline/us.o/track-001)
(conflict.kind . temporal-overlap)
(conflict.betti . "(1 1 0)")
(conflict.resolution . required)
(conflict.result . rejected)
```

Attributes:

```text
scoped edits
candidate changes
receipt history
rollback as new relation
topology-assisted conflict inspection
```

---

# 14. Multimedia Use Case: Distributed 3D Model Sharing

### Scenario

Peers share 3D models, textures, animations, and scenes.

```omi-lisp
(model3d.id . model-mesh-789)
(model3d.seed . o.model3d.o)
(model3d.format . gltf)
(model3d.vertices . 50000)
(model3d.texture . texture-001)
(model3d.animation . walk-cycle)
(model3d.effect . network)
```

Model receipt:

```omi-lisp
(receipt.accepts . model-mesh-789)
(receipt.scope . fs.o/models/gs.o/3d/rs.o/meshes)
(receipt.hash . model-hash)
(receipt.projection . WebGL)
(receipt.result . accepted)
```

Projection pipeline:

```text
accepted model receipt
  ↓
projection_allowed()
  ↓
WebGL projection surface
  ↓
rendered browser/VR view
```

Rendering is projection.

Rendering is not acceptance.

---

# 15. Multimedia Use Case: Decentralized Audio Production

### Scenario

Musicians collaborate on tracks, mixes, stems, and plugins.

```omi-lisp
(audio.project.id . project-123)
(audio.project.seed . o.audio.o)
(audio.project.scope . fs.o/audio/gs.o/music/rs.o/project)
(audio.project.tracks . "(drums bass guitar vocals)")
(audio.project.bpm . 120)
(audio.project.key . "C Major")
```

Pure audio skill:

```omi-lisp
(skill.audio.mix.name . "Multitrack audio mixer")
(skill.audio.mix.scope . fs.o/SKILLS.md/gs.o/audio/rs.o/mixer/us.o/v1)
(skill.audio.mix.effect . pure)
(skill.audio.mix.input . "(tracks gains pan)")
(skill.audio.mix.output . "PCM-audio-stream")
(skill.audio.mix.validation . test.audio.mix)
```

Attributes:

```text
track isolation
pure mixing skills
versioned plugins
receipt-based edit history
local validation
```

---

# 16. Multimedia Use Case: Distributed Media Inference

### Scenario

Peers process images, video, or audio using local AI models.

```omi-lisp
(skill.vision.detect.name . "Object detection")
(skill.vision.detect.scope . fs.o/SKILLS.md/gs.o/vision/rs.o/detect/us.o/v1)
(skill.vision.detect.effect . hardware)
(skill.vision.detect.input . image-data)
(skill.vision.detect.output . "(objects boxes confidence)")
(skill.vision.detect.model . model-yolo-v8)
(skill.vision.detect.validation . test.vision.detect)
```

Boundary:

```text
Model output is not acceptance.
Inference output is a candidate.
Validation and receipt accept.
```

Attributes:

```text
GPU-aware execution
model versioning
local privacy
receipt-based inference results
effect-gated hardware use
```

---

# 17. Multimedia Use Case: P2P Media Discovery

### Scenario

Peers discover media through metadata carriers and read-only search skills.

```omi-lisp
(media.metadata.id . meta-001)
(media.metadata.type . video)
(media.metadata.title . "Collaborative Documentary")
(media.metadata.tags . "(documentary collaboration p2p)")
(media.metadata.creator . peer-address-456)
(media.metadata.thumbnail . thumbnail.imo)
```

Discovery skill:

```omi-lisp
(skill.discovery.query.name . "Search media registry")
(skill.discovery.query.scope . fs.o/SKILLS.md/gs.o/discovery/rs.o/query)
(skill.discovery.query.effect . read-only)
(skill.discovery.query.input . search-query)
(skill.discovery.query.output . matching-receipts)
```

Attributes:

```text
metadata as carrier
search as read-only skill
provenance through receipts
thumbnail carriers
local acceptance
```

---

# 18. Reference P2P Flow

```text
Peer A                                  Peer B
  |                                       |
  | create candidate                      |
  | validate locally                      |
  | store local receipt                   |
  |                                       |
  | send .imo carrier -----------------> |
  |                                       |
  |                                  receive carrier
  |                                  validate locally
  |                                  store accepted/rejected receipt
  |                                  project only if allowed
  |                                       |
  | receive ack <----------------------- |
  |                                       |
  | compare receipt chains                |
```

The message is not truth.

The carrier is not truth.

The receipt from Peer A is not automatically Peer B’s truth.

Peer B validates locally.

---

# 19. P2P Use Case Matrix

|Use Case|OMI Feature|P2P Attribute|
|---|---|---|
|Repository federation|`REPO.md` per peer|local sovereignty|
|Distributed skills|effect classes + skills|zero-trust execution|
|Receipt verification|`skill.receipt.verify`|provenance checking|
|Decentralized version control|FS/GS/RS/US scopes|scoped histories|
|Skill exchange|`SKILLS.md` + test vectors|deterministic execution|
|Receipt synchronization|5040-slot ring + cycles|local receipt lineage|
|Conflict resolution|Betti + Schläfli|topology-assisted inspection|
|Video streaming|chunk receipts|validated media delivery|
|Collaborative editing|scoped edit candidates|mergeable media histories|
|3D sharing|WebGL projection|receipt-gated rendering|
|Audio production|pure media skills|deterministic mixing|
|Media inference|effect-gated models|validated AI output|
|Media discovery|metadata carriers|read-only search|

---

# 20. Canonical P2P and Multimedia Invariant

```text
Recognition is not acceptance.
Citation is not acceptance.
Closure is not acceptance.
Projection is not acceptance.
Validation and receipt accept.

Send does not accept.
Receive does not accept.
Validation accepts.
Receipt records.

Media is not acceptance.
A video is a carrier.
An audio file is a carrier.
An image is a carrier.
A 3D model is a carrier.
A texture is a carrier.
A thumbnail is a carrier.

Streaming is a network effect.
Rendering is a hardware effect.
Transcoding is a local-write effect.
Capture is a security-sensitive effect.
Inference is an effect-gated candidate process.

Each effect requires explicit authorization.
Each peer validates under its own REPO.md.
Each peer stores its own receipts.
Peers exchange carriers, candidates, and receipts, not truth.
```