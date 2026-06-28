# The OMI Uniform Bitboard

## A Platform-Agnostic Self-Hosting Substrate for Ubiquitous Pseudo-Persistent Worlds

## 1. Purpose

The OMI uniform bitboard is the pre-boot, pre-language, platform-agnostic substrate that lets any machine enter the same OMI receipt protocol.

It is designed so that:

```text
a phone
an ESP32
a browser
a file
a DOM element
a CSS selector
a serial packet
a JABCode scan
a boot string
a local C runtime
```

can all reduce their local events into the same shared structure:

```text
framed environment
  ↓
OMI notation citation
  ↓
32-nibble address cascade
  ↓
Polybius 0x0..0xF gauge
  ↓
uniform bitboard witness
  ↓
validation
  ↓
receipt
  ↓
projection
```

The bitboard is not the protocol by itself.

It is the **uniform pre-boot board** that makes the protocol portable.

It gives every platform a common way to say:

```text
I saw this byte.
I placed it in this nibble position.
I folded it through this gauge.
I produced this ruler witness.
Now validation may decide whether it becomes receipt.
```

---

# 2. Central Claim

An OMI address is a cascade of 32 hexadecimal place-value nibbles.

```text
32 hex characters = 32 nibbles = 128 bits
```

The human-readable grouped form is:

```text
S0-S1-S2-S3/S4/S5/S6/S7
```

where each `S` segment is 16 bits, or four hex nibbles.

But the deeper structure is:

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

The address is not flat.

The address is a cascade.

```text
0xA at n02 is not the same citation contribution as 0xA at n27.
```

Position matters.

The OMI uniform bitboard exists to hold that cascade in a platform-neutral way.

---

# 3. The Minimal Receipt Grammar

The whole address system descends from the minimal OMI receipt grammar:

```text
omi---imo
```

and:

```text
o---o/---/?---?@---@
```

The expanded machine-readable ruler is:

```text
o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
```

These are not separate ideas.

They are three resolutions of the same substrate:

```text
omi---imo
  origin boundary

o---o/---/?---?@---@
  minimal receipt grammar

o-S0-S1-S2-S3/S4/S5/S6/S7?PAYLOAD?MASK@CAR@CDR
  expanded address-ruler
```

The bitboard is the compact computational surface for this ruler.

---

# 4. Why a Bitboard

A bitboard is useful because it gives every platform the same small state object.

Different devices have different capabilities:

```text
ESP32 has small memory and physical pins.
Phones have cameras, browsers, storage, radios.
Browsers have DOM, CSS, SVG, WebGL, IndexedDB.
Servers have files, sockets, and larger memory.
Microcontrollers have GPIO, UART, SPI, I2C.
```

The bitboard lets each platform reduce its local world into a common deterministic witness.

The platform does not need to understand every projection.

It only needs to preserve the bitboard rules.

```text
local event
  ↓
uniform bitboard update
  ↓
OMI ruler citation
  ↓
validation candidate
```

This is how self-hosting becomes possible.

The runtime can rebuild its own reading conditions from the same gauge board it uses to read external events.

---

# 5. Pre-Language: 0x00..0x1F

The pre-language begins before printable text.

The 32 ASCII control codes:

```text
0x00..0x1F
```

are the non-printing gauge alphabet for the 32 place-value nibbles of the OMI address cascade.

They act as place controllers.

```text
0x00 → n00
0x01 → n01
0x02 → n02
...
0x1F → n31
```

This does not mean the control code is the nibble value.

It means the control code selects the address place.

The nibble value is still:

```text
0x0..0xF
```

So the pre-language distinction is:

```text
0x00..0x1F = place controllers
0x0..0xF   = nibble values
```

The 32 non-printing characters prepare the address-ruler before ordinary syntax begins.

This is why OMI is pre-Lisp.

Lisp starts when symbols and lists are readable.

OMI starts one layer earlier, by defining the hidden place-control layer that makes reading lawful.

---

# 6. Register Primer: 0x20..0x2F

After the control alphabet, the first printable band appears:

```text
0x20..0x2F
```

This is the visible register injection band.

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

This band primes the register with punctuation.

It gives the hidden control substrate a readable surface:

```text
space
quote
hash
binding
modulus
conjunction
list open
list close
star
plus
comma
hyphen
dot
slash
```

Most importantly, this band contains the visible symbols needed for relation:

```text
( )
.
/
-
```

That makes it the bridge from pre-language to readable relation syntax.

---

# 7. Kernel Reader: 0x30..0x3F

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

It supplies:

```text
digits
namespace/range
comment/separator
declaration opener
equality
closure
question / unresolved witness
```

The standard boot opener is:

```text
0x3C = '<'
```

This is the point where the system can begin readable declaration syntax.

Before `0x3C`, the runtime is preparing the gauge.

At `0x3C`, the runtime can begin standard boot declaration.

---

# 8. System Witness: 0x78

The value:

```text
0x78
```

is the full hexadecimal nibble-field sum:

```text
0 + 1 + 2 + 3 + 4 + 5 + 6 + 7
+ 8 + 9 + A + B + C + D + E + F
= 0x78
```

So `0x78` means:

```text
the full local hexadecimal field has been traversed
```

It is the system witness.

It does not come from x86 folklore.

It comes from the complete 0x0..0xF nibble sweep.

Canon role:

```text
0x78 = full nibble field witness / system sweep
```

---

# 9. Record Witness: 0x1E

The value:

```text
0x1E
```

is the diagonal closure witness.

For the two tetrahedral diagonals:

```text
D+ = {0,5,A,F}
D- = {3,6,9,C}
```

both diagonal sums close to `0x1E`:

```text
0 + 5 + A + F = 0x1E
3 + 6 + 9 + C = 0x1E
```

And in ASCII:

```text
0x1E = RS = Record Separator
```

So its role is extremely clean:

```text
0x1E = record separator / diagonal closure witness
```

It should not be treated as simply D+ or D-.

It is balanced closure.

---

# 10. Local Seal: 0x7F

The value:

```text
0x7F
```

is the full local ASCII seal.

ASCII `0x00..0x7F` gives the 128-entry pre-boot gauge table.

So `0x7F` marks the end of that local declaration range.

Canon role:

```text
0x7F = local ASCII seal / DEL boundary
```

The known boot derivation is:

```text
0x78 → full nibble field witness
0x7F → local seal
0x7F XOR 0x03 = 0x7C
0x7C << 8 = 0x7C00
```

So `0x7C00` becomes the boot-page witness.

---

# 11. External Boot Bridge: 0xAA55

The value:

```text
0xAA55
```

is not an ASCII gauge entry.

It is a 16-bit bridge word.

It must not be indexed as if it were inside a 128-entry table.

Correct role:

```text
0xAA55 = external boot bridge witness
```

It can be associated with `0x7F` as a seal condition, or processed by a bridge resolver after the local ASCII gauge has completed.

The correct order is:

```text
0x00..0x1F  control place layer
0x20..0x2F  register injection
0x30..0x3F  kernel reader
0x78        system field witness
0x7F        local seal
0x7C00      boot-page witness
0xAA55      external boot bridge
```

Only after this does declarative OMI-Lisp dot notation become lawful.

---

# 12. Declarative OMI-Lisp After the Bridge

After the gauge, system witness, local seal, boot page, and external bridge have been staged, OMI-Lisp dot notation becomes readable.

Ordinary Lisp dot notation:

```lisp
(a . b)
```

means a cons pair.

In OMI-Lisp, this sits on top of the receipt grammar:

```text
o---o/---/?---?@---@
```

The dot becomes the readable surface of CAR/CDR relation closure.

```text
A . B
```

means:

```text
A is cited as the CAR-side relation.
B is cited as the CDR-side continuation.
The pair remains lazy until forced by validation, projection, or hardware demand.
```

So declarative OMI-Lisp does not create the receipt-ruler.

It becomes readable after the receipt-ruler has been staged.

Canon line:

```text
OMI-Lisp does not create the receipt-ruler.
OMI-Lisp is the first readable declarative layer after the receipt-ruler has been staged.
```

---

# 13. Lazy Evaluation Down to Hardware

In this model, laziness is not only a high-level language feature.

It is a hardware-facing protocol rule.

A declaration does not immediately perform a side effect.

A declaration first becomes a citation.

```text
declaration
  ↓
citation
  ↓
validation candidate
  ↓
receipt
  ↓
projection or hardware actuation
```

For an ESP32:

```text
relay.on
```

should not mean:

```text
immediately toggle GPIO
```

It should mean:

```text
declare relation
cite device/object/action
validate command receipt
then actuate hardware
then record result receipt
```

This makes hardware evaluation lazy and receipt-bound.

The hardware acts only after the citation path has been staged and accepted.

---

# 14. The Polybius 0x0..0xF Gauge

The local nibble field is the 4×4 hexadecimal Polybius gauge:

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

These diagonals provide the chiral control classes:

```text
D+ first → posting / CAR-dominant
D- first → pulling / CDR-dominant
balanced → simultaneous or unresolved closure
```

Every nibble in the 32-place cascade passes through this gauge.

Thus every address contributes to:

```text
chirality
CAR/CDR bias
component lane
gauge pressure
receipt boundary
projection control
```

The Polybius gauge is how the 32-nibble address becomes component-readable.

---

# 15. The Framed Environment

The OMI address does not begin at the nibble.

It begins inside a framed environment.

A frame may be:

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
Lisp evaluation environment
```

The frame says:

```text
where is this citation being read?
```

The OMI address says:

```text
what is being cited?
```

The nibble cascade says:

```text
how does the citation descend into place-value control?
```

The Polybius gauge says:

```text
how do components interpret the local nibble contribution?
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
component rendering / control
```

---

# 16. The Uniform Bitboard Layout

The bitboard should be explicit, portable, and endian-safe.

Do not rely on C bitfields for protocol layout.

Use integer masks and shifts.

A 64-bit bitboard can hold the active pre-boot witness:

```text
bits 0..6    last gauge byte index 0x00..0x7F
bits 7..11   D+ count
bits 12..16  D- count
bit 17       bridge 0x1E seen
bit 18       bridge 0x78 seen
bit 19       seal 0x7F seen
bit 20       boot page 0x7C00 seen
bit 21       bridge 0xAA55 seen
bits 22..31  active address place n00..n31
bits 32..47  last OMI result / witness
bits 48..63  frame fold / local hash / reserved
```

This is the live gauge board.

It does not need to carry the entire address.

It carries the current witness state of the address cascade.

The full 128-bit address cascade remains:

```text
uint16_t S[8]
```

or:

```text
uint8_t nibble[32]
```

A complete uniform runtime may therefore hold:

```c
typedef struct {
    uint64_t board;
    uint8_t  nibble[32];
    uint16_t s[8];
    uint32_t payload;
    uint32_t mask;
    uint32_t car;
    uint32_t cdr;
} OmiUniformBoard;
```

The 64-bit board is the active witness.

The 32 nibbles are the address cascade.

The eight `S` fields are the grouped ruler.

The payload/mask/CAR/CDR fields complete the receipt grammar.

---

# 17. Portable Bitboard Masks

A platform-agnostic implementation should use explicit masks:

```c
#define OMI_BB_GAUGE_MASK       0x000000000000007FULL
#define OMI_BB_DPLUS_SHIFT      7
#define OMI_BB_DMINUS_SHIFT     12
#define OMI_BB_BRIDGE_1E_BIT    17
#define OMI_BB_BRIDGE_78_BIT    18
#define OMI_BB_SEAL_7F_BIT      19
#define OMI_BB_BOOT_7C00_BIT    20
#define OMI_BB_BRIDGE_AA55_BIT  21
#define OMI_BB_PLACE_SHIFT      22
#define OMI_BB_RESULT_SHIFT     32
#define OMI_BB_FOLD_SHIFT       48
```

Use explicit setters:

```c
static inline uint64_t omi_bb_set_gauge(uint64_t b, uint8_t code) {
    return (b & ~OMI_BB_GAUGE_MASK) | (uint64_t)(code & 0x7F);
}

static inline uint64_t omi_bb_set_flag(uint64_t b, unsigned bit) {
    return b | (1ULL << bit);
}
```

Do not use compiler-dependent bitfields for canonical state.

Bitfields may be useful internally, but not for the protocol surface.

---

# 18. Gauge Table Role

The 128-entry gauge table maps each ASCII code to a declarative gauge entry.

Each entry should contain:

```text
code
class
diagonal class
gauge action
S0..S7
PAYLOAD
MASK
CAR
CDR
bridge role
active flag
```

It should not directly become authority.

Canon rule:

```text
Gauge selects.
Ruler cites.
Bitboard witnesses.
Validation accepts.
Receipt records.
Projection displays.
```

A gauge entry says:

```text
this byte selects this ruler contribution
```

It does not say:

```text
this byte is accepted truth
```

---

# 19. Correct Gauge Entry Structure

A portable declarative structure:

```c
typedef enum {
    GAUGE_CLASS_CONTROL,
    GAUGE_CLASS_PRINTABLE,
    GAUGE_CLASS_DEL
} GaugeClass;

typedef enum {
    DIAG_NONE,
    DIAG_PLUS,
    DIAG_MINUS,
    DIAG_BALANCED
} DiagClass;

typedef enum {
    GAUGE_ACTION_NONE,
    GAUGE_ACTION_PLACE,
    GAUGE_ACTION_REGISTER_INJECT,
    GAUGE_ACTION_KERNEL_READ,
    GAUGE_ACTION_RECORD_CLOSE,
    GAUGE_ACTION_SYSTEM_WITNESS,
    GAUGE_ACTION_SEAL,
    GAUGE_ACTION_BOOT_PAGE,
    GAUGE_ACTION_EXTERNAL_BRIDGE
} GaugeAction;

typedef struct {
    uint8_t code;
    GaugeClass cls;
    DiagClass diag;
    GaugeAction action;

    uint16_t s[8];
    uint32_t payload;
    uint32_t mask;
    uint32_t car;
    uint32_t cdr;

    uint32_t bridge;
    uint8_t active;
} OmiGaugeEntry;
```

Do not place arbitrary function pointers in the canonical gauge table.

Handlers can exist in the runtime, but the gauge table should be declarative.

This preserves self-hosting determinism.

---

# 20. Platform-Agnostic Rules

The uniform bitboard must obey the following rules:

```text
1. Fixed widths only.
2. Explicit masks and shifts only.
3. No compiler-dependent bitfield layout.
4. No random values in deterministic computation.
5. No wall-clock time in deterministic computation.
6. No pointer addresses as canonical identity.
7. No file IO as acceptance.
8. No rendered state as acceptance.
9. No external boot signature as origin.
10. Receipt only after validation.
```

This is how the same bitboard can run across:

```text
ESP32
x86
ARM
RISC-V
browser WASM
native C runtime
phone bridge
local server
```

The platform may differ.

The board rule must not.

---

# 21. Self-Hosting Meaning

Self-hosting means the system can use its own notation and gauge to read, carry, validate, project, and regenerate itself.

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

Self-hosting does not mean the system believes itself automatically.

Every loop must pass through validation again.

A self-hosting OMI runtime can read its own boot notation, compile its own declarative OMI-Lisp forms, project its own browser surfaces, and exchange its own receipts over P2P networks.

But it cannot accept itself without validation.

---

# 22. Ubiquitous Platform Model

The same bitboard can appear differently on different platforms.

## ESP32

```text
GPIO event
  ↓
gauge byte
  ↓
bitboard update
  ↓
OMI citation packet
  ↓
receipt candidate
```

## Phone

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

## Browser

```text
DOM event / scanned code / WebSocket packet
  ↓
OMI gauge update
  ↓
root data attributes / CSS variables
  ↓
declarative projection
```

## File

```text
file opened
  ↓
file acts as port
  ↓
bytes enter gauge
  ↓
citations validate into receipts
```

## P2P Network

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

All of these are different carriers over the same board.

---

# 23. Pseudo-Persistent Open Worlds

The bitboard is the base of pseudo-persistent open worlds.

A world object is not only a mesh or DOM node.

It is a receipt-addressed relation.

```text
physical object
  ↓
device witness
  ↓
OMI citation
  ↓
uniform bitboard
  ↓
validation
  ↓
receipt
  ↓
projection
```

The world persists because the receipt can be replayed.

The world is open because different peers can carry the same board state through different surfaces.

The world is platform-agnostic because the board does not depend on one CPU, browser, or device type.

---

# 24. Correct Narrative

The OMI uniform bitboard is the deterministic pre-boot surface that makes the one-long-word notation portable.

The 32 non-printing ASCII controls address the 32 place-value nibbles.

The 0x20..0x2F band injects visible register punctuation.

The 0x30..0x3F band forms the kernel reader.

The full nibble field witnesses itself at 0x78.

The local ASCII range seals at 0x7F.

The boot page emerges at 0x7C00.

The external boot bridge 0xAA55 is recognized only after the OMI gauge has already staged its own ruler.

After that, OMI-Lisp dot notation becomes the declarative lazy surface over CAR/CDR relation closure.

The bitboard makes this process portable across hardware, browser, phone, file, optical carrier, and peer network.

---

# 25. Final Canon

```text
An OMI address is a framed notation citation.

The address cascades through 32 hexadecimal place-value nibbles.

The 32 ASCII controls 0x00..0x1F act as non-printing place controllers for those nibbles.

The 0x20..0x2F band injects visible register punctuation.

The 0x30..0x3F band forms the kernel reader.

The 0x0..0xF Polybius gauge classifies each nibble through D+, D-, neutral, or balanced closure.

The uniform bitboard records the active gauge witness.

The expanded ruler groups the cascade into S0..S7 ? PAYLOAD ? MASK @ CAR @ CDR.

Validation accepts the ruler into receipt.

Receipt enables pseudo-persistent projection across platforms.
```

---

# 26. Short Version

```text
0x00..0x1F controls the places.
0x20..0x2F primes the register.
0x30..0x3F forms the kernel reader.
0x78 witnesses the full system field.
0x7F seals the local table.
0x7C00 opens the boot page.
0xAA55 bridges external boot convention.
omi-lisp dot notation becomes declarative after the ruler is staged.
The uniform bitboard makes this portable everywhere.
```

---

# 27. One-Sentence Version

The OMI uniform bitboard is a platform-agnostic self-hosting surface where a framed notation citation cascades through 32 place-value nibbles, the 0x00..0x1F control alphabet assigns those places, the 0x0..0xF Polybius gauge classifies each nibble, and the resulting one-long-word ruler is validated into receipt before being projected into hardware, browser, file, optical, or P2P open-world surfaces.