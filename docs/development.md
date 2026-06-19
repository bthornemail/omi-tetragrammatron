# Development notes

## Primary invariant

Keep computation deterministic. Do not introduce computation that depends on nondeterministic sources such as entropy devices, clocks, process identifiers, UUID generation, or pseudo-random generators.

Carrier behavior is allowed when it only moves input or output bytes. Examples include files, sockets, terminal input, signals, and browser rendering.

## Safe documentation workflow

When updating documentation:

1. Keep the runtime source as the authority.
2. Prefer exact command examples that can be copied from the repository root.
3. Keep formulas in plain text blocks so they render predictably on GitHub Pages.
4. Document carrier behavior separately from deterministic computation.
5. Add new pages to the documentation map in `docs/index.md`.

## Runtime extension checklist

Before changing `opencode.c`, identify whether the change affects:

- Accepted input grammar.
- Receipt shape.
- Hashing or normalization.
- Ring size, slot format, or persistence path.
- Autonomous stop conditions.
- Fano routing.
- Geometry formulas.
- JSON or PPM output shape.
- HTTP endpoints.

If any of these change, update the matching documentation page in `docs/`.

## GitHub Pages notes

The documentation is designed for GitHub Pages from the `docs/` folder. In repository settings, set Pages to deploy from the current branch and `/docs` folder.

`docs/_config.yml` selects a GitHub Pages theme and site metadata. The Markdown files remain readable directly on GitHub even before Pages is enabled.

## Suggested page ownership

```text
docs/getting-started.md       build and first run
docs/architecture.md          system boundaries and layout
docs/cli.md                   command modes
docs/sexpr-and-omi.md         input grammar and operations
docs/runtime-data-model.md    structs and data flow
docs/geometry-and-ring.md     ring folds and projection formulas
docs/viewer.md                browser visualization
docs/examples.md              copy-paste workflows
docs/troubleshooting.md       common problems
```
