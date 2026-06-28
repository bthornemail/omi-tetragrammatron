# Omi Surface Projecton Modules

## Clean parsed rule

```text
Omi-* name = development word
Omi relation = protocol form
Receipt = accepted state
```

So the registry should reduce to:

```text
(name . face)
(face . role)
(role . edge)
(edge . receipt-path)
```

## Core faces

| Name       | Reduced role                |
| ---------- | --------------------------- |
| Omi-Form   | structural face             |
| Omi-Glyph  | symbolic / notation face    |
| Omi-Hash   | digest witness              |
| Omi-Map    | coordinate / route face     |
| Omi-Image  | recoverable package face    |
| Omi-Matrix | observation field           |
| Omi-Gnomon | orientation pointer         |
| Omi-Mirror | chiral / inverse face       |
| Omi-Clock  | timing / cadence face       |
| Omi-Light  | visual emission face        |
| Omi-Sound  | sound / cadence face        |
| Omi-Portal | access / doorway face       |
| Omi-World  | persistent environment face |
| Omi-Sense  | sensory reader              |

## Authority / witness faces

| Name        | Reduced role                             |
| ----------- | ---------------------------------------- |
| Omi-Receipt | lawful resolution witness                |
| Omi-Hash    | compact byte/digest witness              |
| Omi-Gate    | origin / zero-reference portal           |
| Omi-Genesis | first cons / birth relation              |
| Omi-Point   | smallest accepted relation               |
| Omi-Shadow  | secondary projection tied to source rule |

## Scope / tower faces

| Name                  | Reduced role                    |
| --------------------- | ------------------------------- |
| Omi-FS / Imo-FS       | file/frame/spatial anchor       |
| Omi-GS / Imo-GS       | graph/guidance relation layer   |
| Omi-RS / Imo-RS       | record/resolution witness layer |
| Omi-US / Imo-US       | unit/understanding memory layer |
| Omi-Node / Imo-Node   | DOM/SVG-safe registered node    |
| Omi-Frame / Imo-Frame | carrier frame projection        |

## Runtime / process faces

| Name          | Reduced role                      |
| ------------- | --------------------------------- |
| Omi-Automaton | abstract agreement machine        |
| Omi-Actor     | supervised runtime cell           |
| Omi-Observer  | reader / witnessing client        |
| Omi-Bus       | service/process routing surface   |
| Omni-Router   | instruction ingestion / dispatch  |
| Omi-Process   | deterministic process envelope    |
| Omi-Trace     | ordered replay log                |
| Omi-Lisp      | dot-notation computation layer    |
| Omi-Alist     | association-list declaration face |

## Function / computation faces

| Name          | Reduced role                          |
| ------------- | ------------------------------------- |
| Omi-Gauge     | spatial resolver                      |
| Omi-Nomogram  | declarative function-scale selector   |
| Omi-SlideRule | operational scale-walk behavior       |
| Omi-Query     | post-address payload plane            |
| Omi-CONS      | payload binding frame                 |
| Omi-CAR       | source/head payload view              |
| Omi-CDR       | continuation/tail payload view        |
| Omi-CID       | agreement/witness view                |
| Omi-CONS256   | 256-bit symbolic meta-object envelope |
| Omi-Notation  | streamable/loggable tuple notation    |

## Geometry / configuration faces

| Name            | Reduced role                               |
| --------------- | ------------------------------------------ |
| Omi-Ring        | circular / spectral bridge                 |
| Omi-Compass     | agreement orientation face                 |
| Omi-Torus       | Gray-code / Karnaugh logic surface         |
| Omi-Chart       | scalar / radar / Smith / sexagesimal chart |
| Omi-Voxel       | tile-map / extrusion surface               |
| Omi-Dali        | unfolded hypercube / subsurface lookup     |
| Omi-Shader      | GPU projection / acceleration surface      |
| Omi-Mesh        | relation-located field network             |
| Omi-NEAT        | entropy-to-topology layer                  |
| Omi-Psi / Ψ_OMI | unified sensed-mesh field state            |

## Carrier / encoding faces

| Name             | Reduced role                                |
| ---------------- | ------------------------------------------- |
| Omi-Tape         | sequential barcode/script carrier           |
| Omi-JabCode          | polychromatic matrix carrier                |
| Omi-Bidi         | directionality / chirality steering         |
| Omi-Bitboard     | dense compatibility / process state surface |
| Omi-RewriteTable | deterministic transition table              |
| Omi-Macro        | source-level rewrite declaration            |
| Omi-Script       | runtime executable projection               |

## Hardware / sensory / body faces

| Name         | Reduced role                       |
| ------------ | ---------------------------------- |
| Omi-Pyramid  | encoder automaton                  |
| Omi-Amulet   | decoder automaton                  |
| Omi-Dome     | immersive sensory world            |
| Omi-Base     | rotational sync platform           |
| Omi-Radio    | radio / gossip carrier             |
| OMI-GPIO     | voltage/agreement transducer       |
| Omi-Talisman | protective personal meaning mirror |
| Omi-Avatar   | digital body projection            |
| Omi-Skeleton | semantic body/rig structure        |
| SpectrumDOM  | wavelength-field interface matrix  |

## Graph / node process faces

| Name             | Reduced role               |
| ---------------- | -------------------------- |
| Omi-Graph        | class/container            |
| Omi-Node         | pure function node         |
| Omi-Edge         | relation input/output      |
| Omi-NodeGraph    | process topology           |
| Omi-NodeFunction | pure transition over edges |

## Minimal canonical reduction

All of those can collapse to this small protocol grammar:

```text
Omi-Form        → (face . structural)
Omi-Glyph       → (face . symbolic)
Omi-Hash        → (face . digest)
Omi-Receipt     → (face . witness)
Omi-Matrix      → (face . observation)
Omi-Gnomon      → (face . orientation)
Omi-Mirror      → (face . chirality)
Omi-Portal      → (face . access)
Omi-World       → (face . environment)
Omi-CAR         → (payload . head)
Omi-CDR         → (payload . continuation)
Omi-CID         → (payload . agreement)
```

And the full rule:

```text
Named face does not create authority.
Named face selects reading.
Relation gives structure.
Validation judges.
Receipt accepts.
```

## Final parsed canon

```text
These are not modules in the heavy software-package sense.

They are OMI named faces.

During development, we can call them Omi-Form, Omi-Matrix, Omi-Jab, Omi-Gauge, Omi-Bitboard, and so on.

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
