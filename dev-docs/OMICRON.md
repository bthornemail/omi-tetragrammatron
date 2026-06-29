# Omicron

Status: active V0 scaffold

Omicron is the boot, context, and dialect induction layer for the runtime. It is not a fifth authority. It prepares execution context, stages the gauge pre-header, binds system object declarations, induces the readable OMI-Lisp surface, and then dispatches to the four authorities.

```text
Omicron boots and configures.
OMI cites.
Tetragrammatron validates.
Metatron scribes and projects.
IMO carries.
Receipt accepts.
```

## Authority Boundary

Omicron owns:
- boot mode selection
- dialect selection
- pre-header staging
- pre-language induction
- system object binding
- CLI and embedded entrypoint configuration
- bitboard bootstrap context

Omicron does not own:
- OMI address law
- validation
- receipt storage
- projection meaning
- carrier I/O except wrapper dispatch
- hardware, browser, network, or filesystem side effects

## V0 API

The V0 scaffold is declared in `core/omicron.h` and implemented in `core/omicron.c`.

Public types:
- `OmicronMode`
- `OmicronDialect`
- `OmicronConfig`

Public functions:
- `omicron_config_init`
- `omicron_config_from_cli`
- `omicron_boot`
- `omicron_stage_preheader`
- `omicron_induce_omi_lisp`
- `omicron_load_system_objects`
- `omicron_mode_name`
- `omicron_dialect_name`

## Dialects

Current dialect bytes:

```text
0xF0 = Omicron bootstrap dialect
0xFF = canonical OMI dialect
```

Reserved dialect bytes:

```text
0xF1..0xFE
```

## Gauge Pre-Header

Omicron stages the deterministic eight-byte gauge pre-header:

```text
Fdelta 00 1C 1D 1E 1F 20 Fdelta
```

Canonical OMI:

```text
FF 00 1C 1D 1E 1F 20 FF
```

Omicron bootstrap:

```text
F0 00 1C 1D 1E 1F 20 F0
```

The first and last dialect bytes must match. This is staging and recognition only. It is not acceptance.

## Boot Order

V0 records the intended construction method:

1. Select boot mode.
2. Select gauge dialect.
3. Stage pre-header.
4. Stage pre-language controls.
5. Open readable boundary at `0x20`.
6. Stage punctuation/register band.
7. Stage kernel reader band.
8. Bind system object declarations.
9. Load readable `.omi` declarations.
10. Load `.imo` carrier declarations.
11. Induce OMI-Lisp dialect.
12. Dispatch authorities.

## Object Surfaces

Object declarations are context surfaces:

```text
omi.o
imo.o
tetragrammatron.o
metatron.o
```

V0 only checks deterministic pointer/length pair integrity for these bindings. It does not parse, validate, accept, serialize, or project them.

## Technical Debt

`core/omicron.c` is still a legacy monolithic runtime file. The V0 API has been added in place, but `main()` has not yet been reduced to an `OmicronConfig` plus authority dispatch flow.

Future reduction target:

```c
OmicronConfig cfg;
omicron_config_init(&cfg);
omicron_config_from_cli(&cfg, argc, argv);
return omicron_boot(&cfg);
```

That refactor must preserve existing CLI behavior.

## Lock

Omicron stages context.
Omicron induces dialect.
Omicron dispatches authorities.
Omicron does not accept.
