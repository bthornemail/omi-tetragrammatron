# OMI-Lisp Boot Sequence Normalization

OMI-Lisp is the lazy declaration language for OMI relations.

It is not the validation engine. It is not the receipt authority. It is the
readable surface that becomes available only after the pre-language control
stream reaches the readable boundary.

Normalized target:

```text
OMI-Lisp = lazy declaration surface.
omicron.c = boot/lowering entry point.
omi-tetragrammatron = deterministic validation/runtime kernel.
```

Core lock:

```text
Dot notation is not primitive at byte zero.
Dot notation is earned after SP.
OMI-Lisp is not allowed to accept state.
Only validation plus receipt accepts state.
```

## 1. Seed

```lisp
(NULL . NULL)
```

The seed is the first paired axis.

It is not yet syntax, memory, a variable, a function, or accepted state.

## 2. Pair

```lisp
(NULL . NULL)
```

The pair is the first structure.

```text
NULL          = the axis, the position from which all positions are measured
(NULL . NULL) = paired axis, a line, a direction
```

This pair is the seed. From this seed, the ladder unfolds.

## 3. Unary Control Stream

The non-printing range:

```text
0x00..0x1F
```

is the pre-language control stream.

It stages control, reference, pointer, and scope positions.

It does not contain dot notation.

The stream is:

```text
NUL SOH STX ETX EOT ENQ ACK BEL BS HT LF VT FF CR SO SI
DLE DC1 DC2 DC3 DC4 NAK SYN ETB CAN EM SUB ESC FS GS RS US
```

The first 16 symbols are references. The second 16 symbols are pointers.

## 4. SP Rupture

```text
0x20 SP
```

SP is the readable boundary.

Before SP, the stream is control-only.

After SP, readable distinction becomes possible.

Canonical rule:

```text
The dot is earned at SP.
```

The dot is not given at byte zero. SP creates the first empty readable position:
the distinction between something and nothing.

## 5. Structure Characters

The range:

```text
0x21..0x3F
```

introduces structure characters:

```text
! " # $ % & ' ( ) * + , - . /
0 1 2 3 4 5 6 7 8 9 : ; < = > ?
```

At this phase, OMI-Lisp may express declarations:

```lisp
(a . b)
(key . value)
(name . face)
(face . role)
(role . edge)
(edge . receipt-path)
```

These are declarations only.

They are not accepted state.

| Character | Hex | Name | Role |
|-----------|-----|------|------|
| SP | 0x20 | Space | Readable boundary |
| ! | 0x21 | Exclamation | Not |
| " | 0x22 | Quote | String delimiter |
| # | 0x23 | Number | Macro / candidate marker |
| $ | 0x24 | Dollar | Variable |
| % | 0x25 | Percent | Modulo |
| & | 0x26 | Ampersand | And |
| ' | 0x27 | Apostrophe | Quote |
| ( | 0x28 | Left paren | List open |
| ) | 0x29 | Right paren | List close |
| * | 0x2A | Asterisk | Multiply |
| + | 0x2B | Plus | Add |
| , | 0x2C | Comma | CONS surface |
| - | 0x2D | Hyphen | Subtract / address separator |
| . | 0x2E | Dot | Pair constructor |
| / | 0x2F | Slash | Derivation boundary |
| 0-9 | 0x30-0x39 | Digits | Numbers |
| : | 0x3A | Colon | Label |
| ; | 0x3B | Semicolon | Comment |
| < | 0x3C | Less than | Comparison |
| = | 0x3D | Equals | Equality |
| > | 0x3E | Greater than | Comparison |
| ? | 0x3F | Question | Predicate / witness query |

## 6. Function Closure Surface

The range:

```text
0x40..0x7F
```

introduces function names, variables, arrays, blocks, meta operators, and REPL
surface syntax.

```text
@ A B C D E F G H I J K L M N O
P Q R S T U V W X Y Z [ \ ] ^ _
` a b c d e f g h i j k l m n o
p q r s t u v w x y z { | } ~ DEL
```

This layer may express computation, but computation remains candidate-only
until validation and receipt.

| Character | Hex | Name | Role |
|-----------|-----|------|------|
| @ | 0x40 | At | Meta operator / continuation surface |
| A-Z | 0x41-0x5A | Uppercase | Constants / function names |
| [ | 0x5B | Left bracket | Array open |
| \ | 0x5C | Backslash | Escape |
| ] | 0x5D | Right bracket | Array close |
| ^ | 0x5E | Caret | XOR |
| _ | 0x5F | Underscore | Word join |
| ` | 0x60 | Grave | Quote |
| a-z | 0x61-0x7A | Lowercase | Variables |
| { | 0x7B | Left brace | Block open |
| \| | 0x7C | Pipe | Pipe / or |
| } | 0x7D | Right brace | Block close |
| ~ | 0x7E | Tilde | Negation |
| DEL | 0x7F | Delete | Delete |

## 7. Omicron Lowering

`omicron.c` is the entry point for lowering OMI-Lisp declarations into
omi-tetragrammatron candidates.

Conceptual entry sequence:

```text
stage pre-header
-> induce readable boundary
-> earn dot operator
-> parse OMI-Lisp declaration
-> lower to OMI frame candidate
-> submit to Tetragrammatron
-> record receipt
-> project/carry
```

Canonical implementation shape:

```c
omicron_config_init(&cfg);
omicron_config_from_cli(&cfg, argc, argv);

omicron_stage_preheader(cfg.dialect, cfg.preheader);
cfg.flags |= OMICRON_FLAG_PREHEADER_STAGED;

omicron_induce_omi_lisp(&cfg);
cfg.flags |= OMICRON_FLAG_PRELANGUAGE_INDUCED;

omicron_load_system_objects(&cfg);
cfg.flags |= OMICRON_FLAG_OBJECTS_BOUND;
```

`omicron.c` is the boot/lowering interface, not a fifth authority.

## 8. Minimal OMI-Lisp Core

The normalized OMI-Lisp core is intentionally small:

```lisp
(NULL . NULL)

(name . value)

(scope . FS)
(scope . GS)
(scope . RS)
(scope . US)

(face . role)
(role . edge)
(edge . receipt-path)

(intent . candidate)
(candidate . relation)
(relation . frame)
```

Userspace declarations grow from the same surface:

```lisp
(Omi-Portal . face)
(face . access)
(role . doorway)
(edge . receipt-path)
```

Surface routing is still declarative:

```lisp
(surface . DOM)
(surface.scope . FS)
(surface.node . Omi-Node)
(surface.route . omi---imo)
(surface.intent . candidate)
```

Still lazy. Still declarative. No side effects yet.

## 9. Bridge Contract

```text
OMI-Lisp may declare unresolved relations.

An unresolved declaration is not an error.
It is an unfinished path.

Omicron lowers the path into a candidate frame.

Tetragrammatron decides whether the candidate validates.

The receipt records the result.

Projection may only display or actuate after receipt.
```

## 10. Authority Split

```text
OMI-Lisp
  declares candidates lazily

Omicron
  stages boot, earns readable syntax, lowers declarations

OMI
  owns citation/address relation

Tetragrammatron
  validates deterministic carry-forward

Receipt ring
  records accepted/rejected state

Metatron
  projects accepted state

IMO
  carries bytes/events/files/surfaces
```

Canonical wording:

```text
Omicron may induce OMI-Lisp.
Omicron may lower declarations.
Omicron may route candidates.

Omicron may not validate truth.
Omicron may not accept state.
Omicron may not turn projection witnesses into protocol literals.
```

## 11. Bootloader Analogy

| Stage | ASCII Range | OMI Phase | Boot Analogy |
|-------|-------------|-----------|--------------|
| 0x00-0x1F | Control codes | Unary stream | CPU reset, bare metal |
| 0x20 | SP | Readable boundary | Bootloader rupture |
| 0x21-0x3F | Structure chars | Pairs, lists, declarations | Kernel initializes |
| 0x40-0x7F | Printing chars | Closures, variables, REPL surface | Userspace |

## Summary

```text
OMI-Lisp declares.
Omicron lowers.
OMI cites.
Tetragrammatron validates.
Receipt accepts.
Metatron projects.
IMO carries.
```

This is the normalized ladder from seed to readable declaration surface.
