# OMI as a Pseudo-Persistent Open World Model

## Decentralized Receipt Worlds for Phones, ESP32s, Browsers, and Real-World Digital Modeling

## 1. The Goal

The goal of OMI is to build pseudo-persistent open worlds.

A pseudo-persistent open world is a shared digital model of real-world and symbolic objects that does not depend on one central server, one permanent database, or one rendered scene as the source of truth.

Instead, the world persists because peers exchange, validate, record, and replay receipts.

```text
world state = fold(receipt history)
```

The browser may display the world.  
The phone may carry the world.  
The ESP32 may witness the world.  
A file may store a piece of the world.  
A barcode may optically carry part of the world.  
A WebSocket may stream part of the world.  
A DOM element may project part of the world.

But none of those surfaces are the world’s authority.

The accepted object is the receipt.

The open world is the projection of accepted receipts.

---

# 2. The Paradigm

The old model for a digital world is:

```text
server database
  ↓
application state
  ↓
3D scene
  ↓
rendered world
```

OMI reverses that.

```text
physical or symbolic event
  ↓
citation
  ↓
validation
  ↓
receipt
  ↓
local receipt ring
  ↓
projection surfaces
  ↓
open-world view
```

The old model asks:

```text
Where is this object in the server’s world?
```

OMI asks:

```text
What is this object incident with?
What receipt accepts that relation?
Which peer can replay the citation?
Which surface should project it?
```

That changes the definition of an open world.

A world is no longer a huge mutable scene graph owned by a server.

A world is a replayable field of accepted incidence receipts.

---

# 3. Pseudo-Persistence

“Pseudo-persistent” means the world does not have to exist as one permanent object in one place.

It persists because enough receipts survive across peers.

Each peer may only have part of the world:

```text
phone A has receipts 1..400
phone B has receipts 250..900
ESP32 node has receipts 880..920
browser cache has receipts 700..940
file export has receipts 1..300
JABCode scan carries receipt 412
```

When peers meet, they exchange what they know.

They do not need to trust each other’s rendered scenes.

They exchange receipt candidates, hashes, missing receipt requests, and accepted receipt fragments.

```text
peer A:
  I have receipts A, B, C, D.

peer B:
  I have B, C, E.

exchange:
  A sends D.
  B sends E.
  both validate.
  both update local rings.
```

The world persists because receipts are replayable.

The world is open because no peer has to be the permanent owner.

The world is decentralized because authority comes from validation, not from central storage.

---

# 4. Real-World Digital Modeling

OMI’s open world is meant to model real things.

A real-world object may be:

```text
a lamp
a door
a room
a tool
a plant
a robot
a sensor
a switch
a person’s phone
an ESP32 node
a printed marker
a physical location
a document
a rendered geometry
```

But the model does not begin by saying:

```text
object = 3D mesh at coordinate x,y,z
```

It begins by saying:

```text
object = cited relation with receipt history
```

For example:

```text
lamp-east-wall
  is witnessed by esp32-a17
  is in room-kitchen
  has relay state off
  has last accepted receipt R42
  has projection lane 7
  has CSS state accepted
```

The real-world object becomes a digital object because there is a receipt trail connecting physical observation, citation, validation, and projection.

---

# 5. ESP32 as Physical Edge Witness

In this model, the ESP32 is not a miniature central server.

It is a physical edge witness.

Its job is to observe, signal, and actuate.

An ESP32 can:

```text
read GPIO
read sensors
detect button presses
control LEDs
drive relays
drive servos
broadcast local packets
receive accepted commands
bridge serial, BLE, Wi-Fi, ESP-NOW
display or scan small optical carriers
```

But the ESP32 should not be treated as the final authority over the open world.

Its core role is:

```text
physical event → receipt candidate
accepted receipt → physical projection
```

Example:

```text
button pressed on ESP32
  ↓
ESP32 emits event packet
  ↓
phone receives packet
  ↓
phone/browser validates packet shape and citation
  ↓
receipt is recorded
  ↓
browser projects accepted state
  ↓
CSS lights matching DOM object
```

The ESP32 witnesses the physical world.

It does not need to own the whole world model.

---

# 6. Phone as Local World Peer

A phone is the ideal local peer.

It has:

```text
browser
camera
touch
storage
network
Bluetooth
Wi-Fi
sensors
location, if allowed
WebRTC
Service Worker
IndexedDB
QR/JABCode scanning
```

The phone can act as:

```text
receipt scanner
local validator
receipt cache
projection host
P2P bridge
world editor
physical controller
temporary resolver
```

A phone can bridge local ESP32 networks into browser-based open worlds.

It can scan optical receipts from physical objects.

It can display a world surface without becoming the central authority.

The phone’s job is:

```text
receive candidates
validate locally
store receipts
project declaratively
exchange receipts with peers
```

---

# 7. Browser as Declarative Projection Engine

The browser is not just a UI.

The browser is a declarative projection engine.

It already has powerful deterministic-ish surface machinery:

```text
DOM
CSS selectors
CSS cascade
SVG
Canvas
WebGL
WebRTC
Service Worker
IndexedDB
data-* attributes
custom properties
camera input
local files
```

OMI uses those browser capabilities as projection surfaces.

The browser does not validate canon.

The browser projects accepted or inspectable receipts.

The important browser rule is:

```text
JavaScript projects receipt state.
CSS resolves the cascade.
DOM/SVG/WebGL paint the result.
Validation remains upstream.
```

So instead of imperatively saying:

```text
change element #lamp to yellow
```

OMI says:

```text
accepted receipt projects lane 7, band 3
```

Then the browser resolves declaratively:

```css
:root[data-omi-lane="7"] [data-omi-lane="7"] {
  filter: drop-shadow(0 0 8px gold);
}
```

The world changes because the receipt projection changes, not because random UI code mutates state.

---

# 8. The Open World Object

An OMI open-world object is not only a mesh, sprite, or DOM node.

It is a receipt-addressed object.

A simple object may be represented as:

```html
<div
  id="lamp-east-wall"
  data-omi="omi-0100-03bf-7c00-2b01-2f01-1434-039f-01ff/48"
  data-device="esp32-a17"
  data-omi-lane="7"
  data-omi-band="3"
  data-receipt-state="accepted">
</div>
```

This element is not the authority.

It is a projection target.

The authority is the receipt history that caused those attributes to be applied.

A real-world digital object therefore has layers:

```text
physical object
  ↓
device witness
  ↓
event packet
  ↓
OMI citation
  ↓
validation
  ↓
receipt
  ↓
projection attributes
  ↓
DOM/SVG/WebGL surface
```

The DOM object is just the final visible surface.

---

# 9. Receipt Candidates

Peers should not exchange “truth.”

Peers should exchange receipt candidates.

A candidate says:

```text
I observed this.
I cite this object.
I claim this relation.
Here is my previous receipt.
Here is my local sequence.
Here is the OMI frame.
Validate me.
```

Example candidate:

```json
{
  "kind": "omi.edge.event.v0",
  "device": "esp32-a17",
  "seq": 1042,
  "lane": 7,
  "band": 3,
  "gpio": 12,
  "value": 1,
  "prev": "0xa3a946b5fc9a41d7",
  "omi": "omi-0100-03bf-7c00-2b01-2f01-1434-039f-01ff/48"
}
```

A receipt candidate is not accepted just because it arrived.

It becomes accepted only after validation.

An accepted receipt may look like:

```json
{
  "kind": "omi.receipt.v0",
  "accepted": true,
  "source": "esp32-a17",
  "seq": 1042,
  "hash": "0x...",
  "prev": "0x...",
  "slot5040": 1697,
  "projection": {
    "lane": 7,
    "band": 3
  }
}
```

The peer that receives the candidate must validate it.

An ESP32 saying `accepted: true` is not enough.

Acceptance is local validation plus receipt.

---

# 10. The P2P Exchange Model

OMI P2P networking should exchange receipts, not rendered state.

Peers can exchange:

```text
known receipt hashes
missing receipt requests
receipt candidates
accepted receipts
object citations
carrier frames
projection hints
device presence
capability declarations
```

A simple peer sync can look like:

```text
peer A → peer B:
  I have hashes [h1,h2,h3,h4]

peer B → peer A:
  I have [h1,h2,h5]
  send me h3,h4
  I will send h5

both:
  validate received receipts
  insert accepted receipts into local ring
  update projections
```

This makes the world pseudo-persistent.

No peer needs full global history.

Peers only need enough validated receipt history to reconstruct the local world they are participating in.

---

# 11. Decentralized World Layers

A practical OMI open-world stack has layers:

```text
Layer 0: Physical world
  sensors, switches, lamps, rooms, robots, people, objects

Layer 1: Edge witnesses
  ESP32, phones, cameras, serial devices, optical markers

Layer 2: Carrier packets
  BLE, Wi-Fi, ESP-NOW, serial, HTTP, WebSocket, WebRTC, file, JABCode

Layer 3: OMI citation
  address, object reference, lane, CAR/CDR, relation frame

Layer 4: Validation
  shape check, deterministic rule, incidence closure, receipt boundary

Layer 5: Receipt ring
  local pseudo-persistent accepted history

Layer 6: Projection
  gnomonic, Smith, mediant, Genaille, CSS, DOM, SVG, WebGL

Layer 7: User interaction
  inspect, edit, accept, reject, export, relay, actuate
```

Each layer has a job.

No layer is allowed to silently become another layer.

---

# 12. The Role of the C Core

The C core is the deterministic heart of the model.

```text
omi.c
  citation, address frame, delta law, BQF, nibble CPU

tetragrammatron.c
  validation, Fano/Polybius, chirality, 5040 ring, receipt folds

metatron.c
  projection, geometry, Smith, Hopf, quaternion, OBJ, glTF, SVG, PPM

imo.c
  carrier, files, HTTP, S-expression parser, serialization, boot ROM
```

This maps directly onto the open-world architecture:

```text
IMO carries.
OMI cites.
Tetragrammatron validates.
Receipt records.
Metatron projects.
IMO carries the projection out.
```

The C core prevents the open world from becoming just UI state.

It gives the world deterministic citation, validation, receipt, and projection boundaries.

---

# 13. ReceiptSurfaceCascade in the Open World

`ReceiptSurfaceCascade` is the browser-side projection adapter.

It takes receipts and receipt pairs and projects them into:

```text
lane
band
mediant
determinant
mean pressure
CSS variables
root data attributes
Genaille SVG state
```

This matters for open worlds because the browser can declaratively decide which objects respond.

A single receipt gives position.

A receipt pair gives motion or relation.

```text
Receipt[n-1] + Receipt[n]
  ↓
positive rational slopes
  ↓
mediant coordinate
  ↓
determinant boundary witness
  ↓
HM ≤ GM ≤ AM ≤ QM pressure ladder
  ↓
lane / band
  ↓
CSS variables + data attributes
  ↓
browser cascade
```

So a moving physical sensor, a phone gesture, a scanned marker, or a peer event can become a projected world state without imperative mutation of every object.

---

# 14. GnomonicSurface in the Open World

`GnomonicSurface` projects receipt pairs through a sphere onto a tangent plane.

Its core purpose is:

```text
cosmological receipt route
  ↓
gnomonic projection
  ↓
straight inspectable browser line
```

The useful property is:

```text
great-circle route → straight projected line
```

In an open world, this means routes between receipts can be displayed as inspectable lines.

For example:

```text
ESP32 A reports door closed
ESP32 B reports lamp on
phone receives both
receipt pair projects to gnomonic route
minimap shows relation path
CSS highlights affected objects
```

The gnomonic surface does not validate.

It lets people inspect the receipt path.

---

# 15. JABCode and Optical Receipts

Optical carriers matter because open worlds need physical handoff.

A printed marker, screen, sticker, or device display can carry an OMI frame.

JABCode can act as an optical receipt carrier.

Current status:

```text
OMI-JAB-RS Profile v0
debug matrix carrier only
not full BSI TR 03137 compliant yet
```

The role is still useful:

```text
scan visual carrier
  ↓
recover frame
  ↓
shape check
  ↓
validate
  ↓
record receipt
  ↓
project object into local world
```

A sticker on a real object can become a world portal.

But the scan is not acceptance.

The scan is carrier recovery.

Validation still decides.

---

# 16. Files as Ports

In this model, files are not just storage.

Files are ports.

A file can carry:

```text
receipt logs
object definitions
projection surfaces
HTML pages
JABCode frames
S-expression input
boot ROM material
world exports
peer sync snapshots
```

Loading a file does not mean accepting its contents.

It means opening a port.

The file’s contents must still become citation, validation, and receipt before entering canon.

This lets open worlds be exported and imported safely:

```text
export world fragment
  ↓
file carries receipts
  ↓
other peer imports file
  ↓
peer validates receipts locally
  ↓
accepted subset joins local world
```

---

# 17. Code as Data

In a declarative browser open world, code should not be trusted merely because it can execute.

Code is carried as data first.

A worker script, S-expression, browser module, or device program can be treated as a cited object.

```text
code text
  ↓
citation
  ↓
validation
  ↓
receipt
  ↓
execution or projection
```

This protects the world from confusing execution with truth.

An open world should not say:

```text
this script ran, so it is valid
```

It should say:

```text
this script was cited, validated, recorded, and then projected or executed under receipt boundary
```

---

# 18. Notation as Citation

OMI notation is not magic.

An OMI string is a citation candidate.

```text
omi-0100-03bf-7c00-2b01-2f01-1434-039f-01ff/48
```

This notation names a structure.

It does not automatically accept the structure.

The protocol must resolve it.

```text
notation
  ↓
citation
  ↓
validation
  ↓
receipt
```

In an open world, this lets objects be named without letting names become truth.

A room, device, marker, avatar, file, or surface can be cited.

Only receipts make accepted world state.

---

# 19. Open-World Editing

Editing an OMI open world is not just changing DOM.

Editing is proposing a new receipt candidate.

For example, a user drags a projected lamp icon from one wall to another.

The browser may show a preview.

But the edit should be recorded as:

```text
old citation
new citation candidate
relation change
previous receipt hash
editor peer
validation path
```

Then:

```text
preview
  ↓
candidate receipt
  ↓
validation
  ↓
accepted receipt
  ↓
projection update
```

This allows collaborative open-world editing without a central database.

Peers exchange edits as receipt candidates.

Each peer validates and records accepted edits.

---

# 20. Physical Actuation

The same rule applies to physical action.

A browser should not randomly command a device because UI state changed.

It should send an accepted receipt or receipt-derived command.

```text
user action
  ↓
candidate receipt
  ↓
validation
  ↓
accepted command receipt
  ↓
ESP32 receives
  ↓
actuates relay / LED / servo
  ↓
ESP32 reports result
  ↓
new receipt records physical outcome
```

This makes physical control auditable.

A device action becomes part of the world history.

---

# 21. Pseudo-Persistent Identity

Object identity should be receipt-based, not server-row-based.

A pseudo-persistent object can survive across:

```text
phone offline
ESP32 reboot
browser refresh
file export
JABCode scan
peer reconnect
local cache deletion
```

because its identity is recoverable from citation and receipt history.

Object identity is not:

```text
row id in one database
```

It is:

```text
stable citation + accepted receipt path
```

That is why the OMI address line matters.

The address is a ruler for reopening the object’s relation-space.

---

# 22. Conflict Handling

In decentralized worlds, peers will disagree.

OMI should not hide that.

Conflicts are receipt relationships.

Examples:

```text
two peers report different lamp states
two devices claim same object id
two edits change same relation
a receipt candidate fails validation
a carrier decodes but shape check fails
```

The browser can project conflicts declaratively:

```html
<div data-receipt-state="conflict"></div>
```

```css
[data-receipt-state="conflict"] {
  outline: 1px dashed red;
}
```

The conflict is not a crash.

It is an inspectable state.

Resolution means choosing or deriving a validated receipt path.

---

# 23. World Persistence Without Central Servers

A central server can still exist as a peer, cache, or relay.

But it should not be the only authority.

The decentralized world should work with:

```text
local phone only
ESP32 mesh only
phone + ESP32
browser + local file
browser + optical scan
browser + WebRTC peers
local C runtime + browser
```

A server may help distribute receipts.

But the server does not make them true.

The receiving peer validates.

That is the decentralized rule:

```text
relay does not accept
receiver validates
receipt records
```

---

# 24. First Demonstration Path

The first practical demonstration should be small.

## Demo 1: ESP32 Button to Browser Receipt

```text
ESP32 button press
  ↓
event packet
  ↓
phone/browser
  ↓
receipt candidate
  ↓
validation
  ↓
accepted receipt
  ↓
DOM object lights up
```

This proves:

```text
physical event → receipt → declarative projection
```

## Demo 2: Two ESP32s and a Mediant Path

```text
ESP32 A reports event A
ESP32 B reports event B
  ↓
two receipts
  ↓
ReceiptSurfaceCascade computes mediant
  ↓
GnomonicSurface draws projected path
  ↓
CSS highlights related objects
```

This proves:

```text
receipt pair → relation path → open-world projection
```

## Demo 3: Optical Object Import

```text
scan JABCode sticker
  ↓
recover OMI frame
  ↓
validate
  ↓
record object receipt
  ↓
object appears in local browser world
```

This proves:

```text
physical marker → receipt object → pseudo-persistent world entity
```

## Demo 4: P2P Receipt Sync

```text
phone A has object receipts
phone B enters range
peers exchange receipt hashes
missing receipts are requested
both validate
both project same object state
```

This proves:

```text
open world without central server
```

---

# 25. Why This Works as a Narrative

The narrative is:

```text
We are building a decentralized open-world protocol where physical devices, phones, files, browsers, and optical markers are all ports into the same receipt space.

ESP32s witness the physical world.
Phones bridge local receipt histories.
Browsers project accepted receipts declaratively.
Files carry world fragments.
JABCode carries optical frames.
CSS resolves visible state.
DOM/SVG/WebGL paint the world.
The C core keeps citation, validation, receipt, and projection separate.

The world persists because receipts persist.
The world opens because peers can exchange receipts.
The world stays deterministic because rendered state is never authority.
```

That is the complete model.

---

# 26. The One-Sentence Goal

OMI is a pseudo-persistent decentralized open-world protocol where ESP32s and phones witness, carry, validate, and exchange receipt candidates, while browsers declaratively project accepted receipts into DOM, SVG, CSS, and WebGL surfaces.

---

# 27. The Short Canon

```text
Code is data.
File is port.
Notation is citation.
ESP32 is witness.
Phone is peer.
Browser is projection.
Receipt is persistence.
Validation is authority.
```

---

# 28. Final Model

```text
physical world
  ↓
ESP32 witness
  ↓
carrier packet
  ↓
phone/browser peer
  ↓
OMI citation
  ↓
Tetragrammatron validation
  ↓
receipt ring
  ↓
ReceiptSurfaceCascade
  ↓
Gnomonic / Smith / Genaille / CSS
  ↓
DOM / SVG / WebGL
  ↓
open-world projection
  ↓
new user or device event
  ↓
next receipt candidate
```

This is the pseudo-persistent open world.

It does not require one global server.

It does not require rendered geometry to be trusted.

It does not require ESP32s to be powerful world engines.

It only requires peers to exchange receipt candidates, validate them deterministically, record accepted receipts, and project them declaratively.

The accepted world is receipt history.

The visible world is projection.

The physical world is witness.

The network is carrier.

The browser is surface.

The protocol is OMI.