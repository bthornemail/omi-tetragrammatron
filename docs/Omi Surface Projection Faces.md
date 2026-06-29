# Omi Surface Projection Faces

## Clean Parsed Rule

```text
Omi-* name = development word
Omi relation = protocol form
Receipt = accepted state
```

Named faces do not multiply modules.

Named faces do not create authority.

They are readable surface names for one accepted OMI object.

The registry reduces to:

```text
(name . face)
(face . role)
(role . edge)
(edge . receipt-path)
```

Only validation plus receipt accepts the resolved relation.

A name may appear in multiple classes.

That does not create multiple modules.

It means one named face has multiple readable roles or can be carried forward from one group to another by relation.

## Core Faces

| Name | Reduced role |
| --- | --- |
| Omi-Form | structural face |
| Omi-Glyph | symbolic / notation face |
| Omi-Hash | digest witness |
| Omi-Map | coordinate / route face |
| Omi-Image | recoverable package face |
| Omi-Matrix | observation field |
| Omi-Gnomon | orientation pointer |
| Omi-Mirror | chiral / inverse face |
| Omi-Clock | timing / cadence face |
| Omi-Light | visual emission face |
| Omi-Sound | sound / cadence face |
| Omi-Portal | access / doorway face |
| Omi-World | persistent environment face |
| Omi-Sense | sensory reader |

## Authority / Witness Faces

| Name | Reduced role |
| --- | --- |
| Omi-Receipt | lawful resolution witness |
| Omi-Hash | compact byte/digest witness |
| Omi-Gate | origin / zero-reference portal |
| Omi-Genesis | first cons / birth relation |
| Omi-Point | smallest accepted relation |
| Omi-Shadow | secondary projection tied to source rule |

## Scope / Tower Faces

| Name | Reduced role |
| --- | --- |
| Omi-FS / Imo-FS | file/frame/spatial anchor |
| Omi-GS / Imo-GS | graph/guidance relation layer |
| Omi-RS / Imo-RS | record/resolution witness layer |
| Omi-US / Imo-US | unit/understanding memory layer |
| Omi-Node / Imo-Node | node face in DOM/SVG context |
| Omi-Frame / Imo-Frame | carrier frame projection |

The `Omi-*` side names source-side or citation-side readings.

The `Imo-*` side names runtime-side or carrier-side readings.

`Omi-Node` is a node face.

Context relations disambiguate its reading:

```text
(node.context . dom)
(node.role . registered-element)
```

## Runtime / Process Faces

| Name | Reduced role |
| --- | --- |
| Omi-Automaton | abstract agreement machine |
| Omi-Actor | supervised runtime cell |
| Omi-Observer | reader / witnessing client |
| Omi-Bus | service/process routing surface |
| Omni-Router | instruction ingestion / dispatch |
| Omi-Process | deterministic process envelope |
| Omi-Trace | ordered replay log |
| Omi-Lisp | dot-notation computation layer |
| Omi-Alist | association-list declaration face |

## Function / Computation Faces

| Name | Reduced role |
| --- | --- |
| Omi-Gauge | spatial resolver |
| Omi-Nomogram | declarative function-scale selector |
| Omi-SlideRule | operational scale-walk behavior |
| Omi-Query | post-address payload plane |
| Omi-CONS | payload binding frame |
| Omi-CAR | source/head payload view |
| Omi-CDR | continuation/tail payload view |
| Omi-CID | agreement/witness view |
| Omi-CONS256 | 256-bit symbolic meta-object envelope |
| Omi-Notation | streamable/loggable tuple notation |

## Geometry / Configuration Faces

| Name | Reduced role |
| --- | --- |
| Omi-Ring | circular / spectral bridge |
| Omi-Compass | agreement orientation face |
| Omi-Torus | Gray-code / Karnaugh logic surface |
| Omi-Chart | scalar / radar / Smith / sexagesimal chart |
| Omi-Voxel | tile-map / extrusion surface |
| Omi-Dali | unfolded hypercube / subsurface lookup |
| Omi-Shader | GPU projection / acceleration surface |
| Omi-Mesh | relation-located field network |
| Omi-NEAT | entropy-to-topology layer |
| Omi-Psi / Psi_OMI | unified sensed-mesh field state |

## Carrier / Encoding Faces

| Name | Reduced role |
| --- | --- |
| Omi-Tape | sequential barcode/script carrier |
| Omi-Barcode | canonical barcode carrier family |
| Omi-JabCode | polychromatic matrix barcode face |
| Omi-Bidi | directionality / chirality steering |
| Omi-Bitboard | dense compatibility / process state surface |
| Omi-RewriteTable | deterministic transition table |
| Omi-Macro | source-level rewrite declaration |
| Omi-Script | runtime executable projection |

`Omi-Barcode` is the canonical barcode family.

`Omi-JabCode` is the polychromatic matrix barcode face under that family.

Relations say whether a barcode face is custom, BSI-compatible, debug, image, binary, or carrier:

```text
(face . carrier)
(carrier.family . barcode)
(carrier.surface . polychrome-matrix)
(carrier.standard . jabcode)
(carrier.status . candidate)
```

Barcode differences are represented through OMI/IMO addressing, receipt context, payload relation, file relation, code relation, data relation, and port relation.

The barcode surface carries or recovers representation.

It does not validate, accept, or create authority.

## Hardware / Sensory / Body Faces

| Name | Reduced role |
| --- | --- |
| Omi-Pyramid | encoder automaton |
| Omi-Amulet | decoder automaton |
| Omi-Dome | immersive sensory world |
| Omi-Base | rotational sync platform |
| Omi-Radio | radio / gossip carrier |
| OMI-GPIO | voltage/agreement transducer |
| Omi-Talisman | protective personal meaning mirror |
| Omi-Avatar | digital body projection |
| Omi-Skeleton | semantic body/rig structure |
| SpectrumDOM | wavelength-field interface matrix |

## Graph / Node Process Faces

| Name | Reduced role |
| --- | --- |
| Omi-Graph | class/container |
| Omi-Node | node face in graph context |
| Omi-Edge | relation input/output |
| Omi-NodeGraph | process topology |
| Omi-NodeFunction | pure transition over edges |

`Omi-Node` keeps the same name as the DOM/SVG node face.

The graph reading is selected by relation:

```text
(node.context . graph)
(node.role . pure-function)
```

## Cross-Class Examples

| Face | Belongs to |
| --- | --- |
| Omi-Gnomon | orientation and geometry |
| Omi-Matrix | observation and witness |
| Omi-JabCode | barcode carrier and contextual encoding |
| Omi-Shader | geometry and hardware acceleration |
| Omi-Receipt | witness and lawful resolution proof |
| Omi-Ring | spectral bridge and circular measurement |
| Omi-Compass | agreement and orientation |

## Minimal Canonical Reduction

All named faces collapse to relation grammar.

| Named face | Protocol reduction |
| --- | --- |
| Omi-Form | `(face . structural)` |
| Omi-Glyph | `(face . symbolic)` |
| Omi-Hash | `(face . digest)` |
| Omi-Receipt | `(face . witness)` |
| Omi-Matrix | `(face . observation)` |
| Omi-Gnomon | `(face . orientation)` |
| Omi-Mirror | `(face . chirality)` |
| Omi-Portal | `(face . access)` |
| Omi-World | `(face . environment)` |
| Omi-Barcode | `(face . carrier)` |
| Omi-JabCode | `(carrier . barcode-polychrome)` |
| Omi-CAR | `(payload . head)` |
| Omi-CDR | `(payload . continuation)` |
| Omi-CID | `(payload . agreement)` |

And the full rule:

```text
Named face selects reading.
Relation gives structure.
Validation judges.
Receipt accepts.
```

## Final Parsed Canon

```text
These are not modules in the heavy software-package sense.

They are OMI named faces.

At protocol level, they reduce to relations:

(face . structural)
(face . symbolic)
(face . carrier)
(face . witness)
(face . orientation)
(face . runtime)
(face . sensory)
(face . geometry)
(face . process)

Receipt accepts the resolved relation.
```

Short lock:

```text
Do not multiply modules.
Parse them as faces.
Compile faces to relations.
Accept only by receipt.
```
