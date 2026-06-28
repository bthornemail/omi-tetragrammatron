# OMI From First Principles

## The Uniform Bitboard, Pre-Language, OMI-Lisp, and Pseudo-Persistent Open World Substrate

## 1. The First Problem

Every computing system has to answer a simple question:

```text
What is this?
```

A normal system answers with separate layers:

```text
text parser
instruction set
memory address
file system
event system
network packet
rendered UI
hardware interrupt
```

Each layer has its own grammar.

That creates a problem: the same thing can appear differently depending on where it enters.

A byte from a keyboard is treated one way.  
A byte from a serial port is treated another way.  
A DOM event is treated another way.  
A file is treated another way.  
A hardware pin is treated another way.  
A Lisp expression is treated another way.  
A rendered shape is treated another way.

OMI begins by refusing that split.

The first principle is:

```text
All surfaces may carry the same citation.
No surface is authority by itself.
```

So OMI asks a deeper question:

```text
Can every byte, event, address, declaration, and projection be reduced to one receipt-ruler form?
```

The answer we are building is yes.

---

# 2. The Core Invariant

The core invariant is:

```text
Code is data.
File is port.
Notation is citation.
Canon validates.
Cosmology projects.
Receipt accepts.
```

This means:

```text
Code does not become true because it executes.
A file does not become true because it exists.
A notation does not become true because it is readable.
A rendered surface does not become true because it appears.
A carrier does not become true because it decodes.
```

Truth, inside the protocol, is not display.

Truth is not execution.

Truth is not arrival.

Truth is not syntax.

The accepted boundary is receipt.

So the central production path is:

```text
recognize
  ↓
cite
  ↓
validate
  ↓
record receipt
  ↓
project
  ↓
inspect
```

The trap we avoid is:

```text
render → believe
```

The correct path is:

```text
validate → receipt → project
```

---

# 3. The Minimal OMI Seed

The smallest symbolic form is:

```text
omi---imo
```

This is the origin boundary.

OMI is the citation side.  
IMO is the carrier side.  
The dash span between them is the relation interval.

The minimal receipt grammar is:

```text
o---o/---/?---?@---@
```

This grammar says:

```text
o---o   open the relation
/---/   carry or return through inverse surface
?---?   hold the unresolved witness
@---@   close the CAR/CDR boundary
```

This is not decoration.

It is the readable mnemonic form of the protocol.

The expanded machine form is:

```text
o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

So the three levels are:

```text
omi---imo
  origin boundary

o---o/---/?---?@---@
  receipt grammar

o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
  expanded address-ruler
```

The crucial insight is:

```text
The receipt is not stored beside the address.
The receipt is the address-ruler after validation.
```

---

# 4. The One-Long-Word Instruction

The OMI address is not just an identifier.

It is a one-long-word instruction substrate.

The grouped form is:

```text
S0-S1-S2-S3/S4/S5/S6/S7
```

Each segment is sixteen bits.

Eight segments give:

```text
8 × 16 bits = 128 bits
```

But the deeper form is:

```text
32 hexadecimal nibbles
```

or:

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

Each nibble is a place-value decision.

So the address is not flat.

It is a cascade.

```text
address = 32 place-value nibbles
```

The hextets are readable grouping.

The nibbles are the actual cascade.

```text
S0 = n00 n01 n02 n03
S1 = n04 n05 n06 n07
S2 = n08 n09 n10 n11
S3 = n12 n13 n14 n15
S4 = n16 n17 n18 n19
S5 = n20 n21 n22 n23
S6 = n24 n25 n26 n27
S7 = n28 n29 n30 n31
```

The same nibble in two different places does not mean the same thing.

```text
0xA at n02 is not the same citation contribution as 0xA at n27.
```

That is why we say:

```text
An OMI address is a cascade of 32 hexadecimal place-value nibbles grouped into eight 16-bit ruler segments.
```

---

# 5. The Framed Environment

An address is not read in emptiness.

It is read inside a frame.

A frame can be:

```text
hardware port
ESP32 device
browser document
DOM subtree
CSS scope
SVG group
WebGL scene
file
receipt ring
peer session
Lisp environment
projection environment
```

The frame asks:

```text
Where is this citation being read?
```

The OMI notation asks:

```text
What is being cited?
```

The nibble cascade asks:

```text
How does this citation descend into place-value control?
```

The Polybius gauge asks:

```text
How does each component interpret the local nibble contribution?
```

So the complete cascade is:

```text
framed environment
  ↓
OMI notation citation
  ↓
32 place-value nibbles
  ↓
8 ruler segments
  ↓
0x0..0xF Polybius gauge
  ↓
component rendering or control
```

The address is therefore a framed notation citation, not merely a number.

---

# 6. The Polybius Gauge

The local hexadecimal gauge is:

```text
0 1 2 3
4 5 6 7
8 9 A B
C D E F
```

The tetrahedral diagonals are:

```text
D+ = {0,5,A,F}
D- = {3,6,9,C}
```

These diagonals are control classes.

They let each nibble contribute to:

```text
chirality
posting or pulling
CAR/CDR bias
component lane
gauge pressure
receipt boundary
projection behavior
```

The diagonal sums close:

```text
0 + 5 + A + F = 0x1E
3 + 6 + 9 + C = 0x1E
```

So:

```text
0x1E = diagonal closure witness
```

And since ASCII `0x1E` is Record Separator, it becomes:

```text
0x1E = record separator / diagonal closure witness
```

The full nibble field also closes by sum:

```text
0 + 1 + 2 + ... + F = 0x78
```

So:

```text
0x78 = full hexadecimal system witness
```

This gives the pre-boot constants their roles.

---

# 7. The Pre-Language

Before readable Lisp, before normal syntax, before declarations, OMI begins with the non-printing control range:

```text
0x00..0x1F
```

There are 32 control characters.

There are also 32 place-value nibbles in the OMI address cascade.

This is the alignment:

```text
0x00 → n00
0x01 → n01
0x02 → n02
...
0x1F → n31
```

The control code is not the nibble value.

The control code is the place controller.

The nibble value is still one of:

```text
0x0..0xF
```

So the distinction is:

```text
0x00..0x1F = place controllers
0x0..0xF   = nibble values
```

This is the derived non-printing pre-language.

It means language does not begin with printable symbols.

It begins with place.

The pre-language establishes where a value may be read before it says what the value means.

---

# 8. The Register Injection Band

After the 32 non-printing controls, the first printable band appears:

```text
0x20..0x2F
```

This is the register injection band.

It contains:

```text
0x20 SPACE
0x21 !
0x22 "
0x23 #
0x24 $
0x25 %
0x26 &
0x27 '
0x28 (
0x29 )
0x2A *
0x2B +
0x2C ,
0x2D -
0x2E .
0x2F /
```

This band injects the visible punctuation that makes relation syntax readable.

It contains:

```text
space
quote
hash
binding
modulus
and
open paren
close paren
star
plus
comma
hyphen
dot
slash
```

This is the first visible register surface.

It gives the hidden control layer a way to become readable.

Most important here is:

```text
0x2E = dot
```

The dot becomes the readable relation operator.

---

# 9. The Kernel Reader Band

The next band is:

```text
0x30..0x3F
```

It contains:

```text
0x30 0
0x31 1
0x32 2
0x33 3
0x34 4
0x35 5
0x36 6
0x37 7
0x38 8
0x39 9
0x3A :
0x3B ;
0x3C <
0x3D =
0x3E >
0x3F ?
```

This is the kernel reader band.

It gives:

```text
digits
namespace/range
comment/separator
declaration opener
equality
declaration closer
question/unresolved witness
```

The standard declaration opener is:

```text
0x3C = <
```

So boot becomes readable at the point where the pre-language can open declarations.

---

# 10. The Boot Bridge Sequence

The complete pre-boot order is:

```text
0x00..0x1F  non-printing place controllers
0x20..0x2F  register injection / punctuation
0x30..0x3F  kernel reader
0x78        full nibble-field witness
0x7F        local ASCII seal
0x7C00      boot page witness
0xAA55      external boot bridge
afterward   declarative OMI-Lisp dot notation
```

Each constant has a role:

```text
0x1E
  record separator / diagonal closure witness

0x78
  full nibble-field system witness

0x7F
  local ASCII seal

0x7C00
  boot page witness

0xAA55
  external boot bridge word
```

The bridge word `0xAA55` is not a byte in the 0x00..0x7F gauge table.

It is a 16-bit external bridge word.

It enters through a bridge resolver, not through the byte gauge.

This keeps the protocol clean:

```text
byte gauge is 0x00..0x7F
bridge resolver handles 16-bit bridge words
```

---

# 11. The Uniform Bitboard

The uniform bitboard is the platform-agnostic active witness.

It does not replace the full OMI address.

It records the current gauge state of the cascade.

A practical 64-bit bitboard can hold:

```text
bits 0..6    last gauge byte index 0x00..0x7F
bits 7..11   D+ count
bits 12..16  D- count
bit 17       0x1E record witness seen
bit 18       0x78 system witness seen
bit 19       0x7F local seal seen
bit 20       0x7C00 boot page seen
bit 21       0xAA55 external bridge seen
bits 22..31  active address place n00..n31
bits 32..47  last OMI result or witness
bits 48..63  frame fold / local hash / reserved
```

The complete uniform board includes:

```text
uint64_t board
uint8_t  nibble[32]
uint16_t ruler[8]
uint32_t payload
uint32_t mask
uint32_t car
uint32_t cdr
```

The relationship is:

```text
nibble[32] = address cascade
ruler[8]   = grouped hextet fields
board      = active witness
payload/mask/car/cdr = receipt grammar closure
```

The bitboard allows any platform to reduce its local event into the same shape.

An ESP32, phone, browser, file, serial packet, DOM node, or optical scan can all update the same kind of witness board.

That is the platform-agnostic self-hosting layer.

---

# 12. The Deterministic Arena

To make the bitboard portable, memory must be deterministic.

That means:

```text
fixed offsets
fixed widths
canonical byte order
no host pointers in canonical state
no C bitfields for protocol layout
no wall-clock time in deterministic computation
no random values in deterministic computation
```

The arena is a byte-addressed memory geography.

Example regions:

```text
Boot ROM
control place staging
register injection staging
kernel reader staging
gauge table
uniform bitboard
ring window
stack
heap
symbol table
CONS table
I/O buffer
hardware registers
projection buffer
boot bridge
receipt ring storage
```

The important rule is:

```text
Arena bytes are canonical.
Host structs are views.
```

A platform may use C structs for convenience, but the protocol itself must be defined by fixed offsets and explicit load/store helpers.

Canonical byte order must be fixed.

Pointer addresses are not identity.

Handles are identity.

---

# 13. Handles Instead of Host Pointers

OMI cannot store host pointers as canonical values.

A pointer from one machine is meaningless on another machine.

So OMI uses fixed-width handles.

Example:

```text
0x0100002A = CONS handle index 0x2A
0x02000005 = SYMBOL handle index 0x05
0x06000010 = RECEIPT handle index 0x10
```

The top bits identify the handle type.

The low bits identify the index.

This makes references portable across:

```text
ESP32
x86
ARM
RISC-V
browser WASM
phone runtime
file export
peer network
```

The canon rule is:

```text
Pointer addresses are carrier details.
OMI references are deterministic handles.
```

---

# 14. Derived Non-Printing OMI-Lisp

Now the key missing layer appears:

OMI-Lisp is not introduced at the visible text layer.

It is derived from the non-printing control layer.

The 32 controls stage the address places.

The 0x20..0x2F band injects visible punctuation.

The 0x30..0x3F band creates the kernel reader.

Then the system can read declarative OMI-Lisp.

So the order is:

```text
non-printing control structure
  ↓
address-place cascade
  ↓
OMI ruler
  ↓
receipt grammar
  ↓
visible OMI-Lisp dot notation
```

Readable OMI-Lisp is not the root.

Readable OMI-Lisp is the first human-readable declarative layer after the receipt-ruler has been staged.

This is why it is pre-Lisp.

Lisp begins with symbolic expressions.

OMI begins with non-printing place controllers.

---

# 15. Dot Notation

In ordinary Lisp:

```lisp
(a . b)
```

means a cons pair.

In OMI-Lisp, dot notation is deeper.

The dot is the visible projection of CAR/CDR receipt closure.

It descends from:

```text
@---@
```

in the minimal receipt grammar.

So:

```lisp
(a . b)
```

means:

```text
a is cited on the CAR side
b is cited on the CDR side
the relation is declared
the declaration becomes a receipt candidate
validation decides whether it is accepted
```

The dot is not just borrowed Lisp syntax.

It is the readable surface of the non-printing receipt relation.

This is why the same dot can be used across every layer:

```text
hardware.port . value
event.source . observed-state
actor.intent . requested-relation
object . declaration
car . cdr
citation . receipt
```

The common shape is:

```text
left . right
```

or:

```text
CAR . CDR
```

Dot notation is the universal relation operator.

---

# 16. Hardware, Event, and Intent Declarations

Because dot notation is the visible CAR/CDR relation operator, it works from hardware to intent.

Hardware declaration:

```lisp
(gpio12 . high)
```

Event declaration:

```lisp
(button0 . pressed)
```

Intent declaration:

```lisp
(user.intent . open-door)
```

Object declaration:

```lisp
(lamp-east-wall . relay-off)
```

Receipt declaration:

```lisp
(citation-042 . accepted)
```

Each one means:

```text
declare the relation
cite the relation
validate the relation
record the receipt
project only after receipt
```

This lets OMI use one notation for hardware events, user intents, memory pairs, and world declarations.

---

# 17. Lazy Evaluation Down to Hardware

In normal systems, hardware commands often cause immediate side effects.

OMI refuses that.

A declaration such as:

```lisp
(relay2 . on)
```

does not mean:

```text
turn relay on immediately
```

It means:

```text
declare relay2/on relation
cite it through the OMI ruler
validate it
record receipt
then project to hardware if accepted
```

The hardware path is:

```text
declaration
  ↓
citation
  ↓
receipt candidate
  ↓
validation
  ↓
accepted receipt
  ↓
hardware projection
  ↓
result receipt
```

This is lazy evaluation down to hardware.

Nothing acts merely because syntax appeared.

The hardware acts only from an accepted receipt.

---

# 18. Bridge Recognition Is Not Receipt Acceptance

The external boot bridge `0xAA55` is important, but it is not authority by itself.

It means:

```text
external boot convention recognized
```

It does not mean:

```text
receipt accepted
```

So the gates are separate:

```text
bridge recognized
  ↓
declaration parsed
  ↓
candidate created
  ↓
candidate validated
  ↓
receipt stored
  ↓
projection allowed
```

The key rule:

```text
0xAA55 allows the hardware bridge to exist.
Receipt allows hardware action.
```

This preserves the whole architecture.

---

# 19. The Receipt Ring

The receipt ring records accepted states.

The ring has 5040 slots.

The slot formula is:

```text
slot5040 = fano7 × 720 + role3 × 240 + local240
```

where:

```text
local240 = Q(x,y) mod 240
Q(x,y) = 60x² + 16xy + 4y²
```

The ring gives the system a finite cyclic receipt surface.

It lets the system say:

```text
this declaration was accepted at this cycle
this receipt has this hash
this slot belongs to this Fano/role/local fold
this projection may be replayed
```

The ring window may be small.

The full ring storage is larger.

This distinction matters:

```text
ring window = cursor/header/folds
ring storage = full 5040 receipt slots
```

The ring records receipts.

It does not make invalid declarations valid.

---

# 20. Self-Hosting

The system is self-hosting because it can use its own notation to read, carry, validate, and regenerate itself.

The loop is:

```text
pre-language gauge
  ↓
address cascade
  ↓
OMI ruler
  ↓
validation
  ↓
receipt
  ↓
OMI-Lisp declaration
  ↓
new citation
  ↓
same gauge
```

But self-hosting does not mean self-believing.

Every loop must pass through validation again.

The system can read itself.

It can carry itself.

It can project itself.

It can compile itself.

But it cannot accept itself without receipt.

That is the safe meta-circular boundary.

---

# 21. Ubiquitous Platform-Agnostic Model

The same structure can run on different platforms because the substrate is fixed.

On an ESP32:

```text
GPIO event
  ↓
gauge byte
  ↓
uniform bitboard
  ↓
OMI citation packet
  ↓
receipt candidate
```

On a phone:

```text
BLE/Wi-Fi/camera input
  ↓
decode carrier
  ↓
update bitboard
  ↓
validate receipt
  ↓
project browser world
```

In a browser:

```text
DOM event / scanned code / WebSocket packet
  ↓
OMI gauge update
  ↓
root data attributes / CSS variables
  ↓
declarative projection
```

In a file:

```text
file opened
  ↓
file acts as port
  ↓
bytes enter gauge
  ↓
citations validate into receipts
```

In P2P networking:

```text
peer packet
  ↓
receipt candidate
  ↓
bitboard witness
  ↓
local validation
  ↓
receipt ring
  ↓
projection update
```

Every platform differs in carrier.

Every platform shares the same ruler.

---

# 22. Pseudo-Persistent Open Worlds

This is the larger goal.

A pseudo-persistent open world is not a central database.

It is a world reconstructed from receipts.

```text
world state = fold(receipt history)
```

A real-world object becomes digital when it has a receipt trail.

Example:

```text
physical lamp
  ↓
ESP32 witness
  ↓
OMI citation
  ↓
uniform bitboard
  ↓
receipt
  ↓
browser projection
```

The world persists because receipts can be replayed.

The world is open because peers can exchange receipt candidates.

The world is decentralized because no rendered surface is authority.

The world is platform-agnostic because phones, ESP32s, browsers, files, and optical carriers all reduce into the same receipt-ruler.

---

# 23. Browser Projection

The browser is a projection engine.

It should not be canon.

It can project receipts through:

```text
DOM
CSS
SVG
Canvas
WebGL
IndexedDB
Service Worker
WebRTC
data-* attributes
CSS variables
```

The browser path is:

```text
receipt
  ↓
projection variables
  ↓
CSS selectors
  ↓
DOM/SVG/WebGL paint
```

The browser may display the world.

The browser does not accept the world.

Receipt accepts.

Projection displays.

---

# 24. P2P Networking

P2P peers should exchange receipt candidates, not rendered truth.

Peers exchange:

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

A peer does not trust another peer’s screen.

It validates received receipts locally.

```text
peer sends candidate
receiver validates
accepted receipt enters local ring
projection updates
```

This gives decentralized pseudo-persistence.

---

# 25. What We Are Bringing

We are bringing a uniform substrate where:

```text
ASCII controls become place controllers.
Hex nibbles become address cascade values.
The address becomes a one-long-word ruler.
The ruler becomes citation.
Citation becomes receipt candidate.
Validation accepts receipt.
Receipt projects hardware, browser, file, optical, and P2P worlds.
```

We are bringing a pre-language where non-printing structure comes before visible syntax.

We are bringing a Lisp-like notation where dot is not borrowed decoration, but the readable operator for CAR/CDR receipt closure.

We are bringing a hardware model where events and intents use the same relation grammar.

We are bringing a boot model where external conventions like `0xAA55` are bridges, not origins.

We are bringing a memory model where arena bytes are canonical and host pointers are not identity.

We are bringing a world model where persistence comes from receipts, not servers.

We are bringing a browser model where CSS, DOM, SVG, and WebGL are projections, not authority.

We are bringing a P2P model where peers exchange receipt candidates and validate locally.

---

# 26. The Complete Canon

```text
OMI begins before language.

0x00..0x1F stages the 32 non-printing place controllers.
0x20..0x2F injects visible register punctuation.
0x30..0x3F forms the kernel reader.
0x78 witnesses the full hexadecimal field.
0x7F seals the local ASCII gauge.
0x7C00 opens the boot page.
0xAA55 bridges external boot convention.

After that, OMI-Lisp dot notation becomes readable.

Dot notation is the visible CAR/CDR relation operator.

Hardware events, user intents, declarations, memory pairs, and receipts all use the same relation form.

The OMI address is a 32-nibble cascade grouped into an eight-segment ruler.

The ruler is citation until validated.

The validated ruler is receipt.

Receipt is pseudo-persistence.

Projection is display.

No carrier, file, code, browser, or hardware event is authority before receipt.
```

---

# 27. One-Sentence Version

OMI is a platform-agnostic self-hosting receipt substrate where the 32 non-printing ASCII controls stage the 32-place address cascade, the uniform bitboard witnesses the OMI ruler, OMI-Lisp dot notation declares CAR/CDR relations across hardware, events, intents, and memory, and only validated receipts may project into physical devices, browsers, files, optical carriers, or decentralized open-world peers.