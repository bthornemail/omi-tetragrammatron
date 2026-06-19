AGENTS.md — instructions for coding agents working on this project

## Core constraint

encode determinism
do not encode nondeterminism

Never use: rand, time, getpid, clock, uuid, random, srand, /dev/urandom
or any source of non-determinism in computation.

I/O (terminal input, file writes, signals, network) is carrier behavior,
not computation. It is permitted but must not affect deterministic output.

## Source layout

- opencode.c — single-file merged runtime (all modes, geometry, CPU, ring)
- viewer/ — WebGL frontend (Three.js)
- SKILLS.md — full OMI runtime specification
- AGENTS.md — this file

## Build

gcc -O2 -Wall -Wextra -o opencode.bin opencode.c -lm

## Smoke test

./opencode.bin --eval '(cons 1 2)'
./opencode.bin --geom
./opencode.bin --boot
./opencode.bin --auto
./opencode.bin --twin
./opencode.bin --render-frame > /tmp/frame.json
./opencode.bin --render-ppm > /tmp/frame.ppm
./opencode.bin --serve 8080 &
curl http://127.0.0.1:8080/frame
curl http://127.0.0.1:8080/ring

## Code style

- No comments (unless absolutely necessary)
- Single-letter variable names in hot paths
- Run-on if/for statements without braces for simple guards
- FNV-1a for all hashing
- u16/u32 macros for wrapping
- Deterministic math only (cos/sin from -lm is allowed — pure function)

## Git conventions

- Commit source + binary together
- Commit message: "Add/Update <feature> — <reason>"
- Do not commit other agent's work (lib/, public/, SKILLS.md originally)
  unless explicitly asked

## Architecture

Computation layer (deterministic):
  opencode.c -> opencode.bin
  OMI reduction, geometry pipeline, ring memory
  No rand/time/clock

Visualization layer (read-only):
  viewer/ -> WebGL / Three.js
  Reads ring state from --serve HTTP endpoint
  Renders geometry, no computation

## Modes in opencode.c

--eval <s>      evaluate one S-expression
--repl          interactive REPL
--auto          autonomous ring mode (verbose)
--boot          run 21-stage boot ROM on nibble CPU
--run <file>    process OMI addresses from file
--cpu <addr>    execute single OMI address on nibble CPU
--geom          print geometry constants
--seed <path>   write packed boot ROM binary
--ring          dump receipt ring
--watch         live ring monitor (polls file)
--twin          digital twin universe summary
--render-frame  JSON geometry frame to stdout
--render-ppm    PPM image to stdout
--serve [port]  HTTP server for WebGL viewer (default 8080)

## Key formulas

delta16(x, c) = rotl16(x,1) ^ rotl16(x,3) ^ rotr16(x,2) ^ c
BQF(x, y) = 60x^2 + 16xy + 4y^2
slot5040 = fano7 * 720 + role3 * 240 + local240
Hopf: quaternion from theta/phi, thrust from quat components

## Fano routing (autonomous mode)

fano_pt = (xor_fold ^ sum_fold) % 7
line    = first Fano line containing fano_pt
opcode  = (line * 3 + rot_fold % 3) % 11 -> opcodes[opcode]

## Ring

Fixed 5040 slots, 4096 bytes each, file at /tmp/omi_receipt_ring.bin
Indexed by g_cycle % RING_SIZE
Three folds: XOR, SUM, ROT

## When in doubt

Ask the user before:
- Adding dependencies
- Changing deterministic guarantees
- Committing other agent's work
