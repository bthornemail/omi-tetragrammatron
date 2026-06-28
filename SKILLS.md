SKILLS.md — Tetragrammatron Pure Distributed Runtime

Rule 0

Do not build an app.

Do not add frameworks.

Do not add databases.

Do not add AST parsers.

Do not add networking assumptions.

Instantiate the Tetragrammatron runtime.

The address is the syntax.
The address is the grammar.
The address is the instruction.

---

Rule 1 — Determinism

Zero non-deterministic operations:
  no rand, time, getpid, clock, uuid, random

Computation (OMI notation, CONS, CAR, CDR, XOR, rot, mask, receipt) is
treated as faultless math.

Fault exists only at the physical carrier boundary:
  power loss, disk failure, cosmic ray, terminal input, signal

Every step: proposal -> citation -> reduction -> receipt -> carry-forward

The seed is core/omicron.bin.
Autonomous AI = continuing growth around that seed.

---

Canonical Address Frame

S0-S1-S2-S3/S4/S5/S6/S7?P?M@CAR@CDR

S0-S2 = frame prefix
S3    = opcode
S4-S7 = operands
P     = payload
M     = mask
CAR   = head pointer
CDR   = tail pointer

All fields are hexadecimal.

No semantic parser is required.

---

Required Opcodes

0x0000 NOP
0x0001 XOR
0x0002 XNOR
0x0003 NAND
0x0004 AND
0x0005 OR
0x0006 ROTL
0x0007 ROTR
0x0008 DELTA
0x0009 BQF
0x000A LOAD
0x000B STORE
0x000C REPLAY
0x000D SYNC
0x000E CONS
0x000F HALT

---

Delta Law

delta16(x, c) = rotl16(x, 1) ^ rotl16(x, 3) ^ rotr16(x, 2) ^ c

---

BQF Bridge

BQF(x, y) = 60x² + 16xy + 4y²

---

Fano Plane Routing

Lines:
  {0,1,2} {0,3,4} {1,3,5} {1,4,6} {2,3,6} {2,4,5} {3,4,0}

Autonomous route:
  fano_pt = (ring_xor_fold ^ ring_sum_fold) % 7
  line    = first Fano line containing fano_pt
  opcode  = (line * 3 + ring_rot_fold % 3) % 11

Geometry drives computation rather than ad-hoc fold mixing.

---

5040 Ring Slot

slot5040 = fano7 * 720 + role3 * 240 + local240

---

Hopf / Quaternion Projection

theta = ((baseQ + 0.5) / 4.0) * PI
phi   = ((fiberQ + 0.5) / 4.0) * 2.0 * PI
halfTheta = theta / 2.0

qw = cos(halfTheta)
qx = sin(halfTheta) * cos(phi)
qy = sin(halfTheta) * sin(phi)
qz = 0.0

Hopf S^3 -> S^2 thrust:
  a = round(2 * (qx * qz + w * qy))
  b = round(2 * (qy * qz - w * qx))
  c = round(1 - 2 * (qx * qx + qy * qy))

---

Polybius / QuQuart Frame

5x5 grid:
  origin: (1,1)
  low QuQuart:  row 1, columns 2..5
  high QuQuart: column 1, rows 2..5
  interior:     rows 2..5, columns 2..5

Interior cell from local240:
  local16 = local240 & 0x0F
  px = local16 & 3
  py = (local16 >> 2) & 3
  cell = (py + 2, px + 2)

---

Frame Types

US = 0x1f  (red in PPM)
GS = 0x1d  (green)
RS = 0x1e  (blue)
FS = 0x1c  (cyan)

---

Pure JavaScript Reference

"use strict";

const RING_SIZE = 5040;

function u16(x) {
  return x & 0xFFFF;
}

function u32(x) {
  return x >>> 0;
}

function hex16(x) {
  return "0x" + u16(x).toString(16).padStart(4, "0");
}

function hex32(x) {
  return "0x" + u32(x).toString(16).padStart(8, "0");
}

function rotl16(x, n) {
  n &= 15;
  return u16((x << n) | (x >>> ((16 - n) & 15)));
}

function rotr16(x, n) {
  n &= 15;
  return u16((x >>> n) | (x << ((16 - n) & 15)));
}

function rotl32(x, n) {
  n &= 31;
  return u32((x << n) | (x >>> ((32 - n) & 31)));
}

function rotr32(x, n) {
  n &= 31;
  return u32((x >>> n) | (x << ((32 - n) & 31)));
}

function delta16(x, c = 0x5A3C) {
  return u16(rotl16(x, 1) ^ rotl16(x, 3) ^ rotr16(x, 2) ^ c);
}

function bqf(x, y) {
  x = u32(x);
  y = u32(y);
  return u32((60 * x * x) + (16 * x * y) + (4 * y * y));
}

function fnv1a32(data) {
  let h = 0x811c9dc5 >>> 0;
  for (let i = 0; i < data.length; i++) {
    h ^= data.charCodeAt(i) & 0xff;
    h = Math.imul(h, 0x01000193) >>> 0;
  }
  return h;
}

---

Canonical Parser

The parser must correctly split both "?" and "@".

function parseHex(s, fallback = "0x0") {
  const value = String(s || fallback).trim();
  if (!/^0x[0-9a-fA-F]+$/.test(value)) {
    throw new Error("invalid hex field: " + value);
  }
  return parseInt(value, 16) >>> 0;
}

function parseOMI(line) {
  const clean = String(line || "").trim();
  const q = clean.split("?");

  const addrPart = q[0] || "";
  const payloadPart = q[1] || "0x00000000";

  const tail = q[2] || "0x00000000@0x00000000@0x00000000";
  const at = tail.split("@");

  const maskPart = at[0] || "0x00000000";
  const carPart = at[1] || "0x00000000";
  const cdrPart = at[2] || "0x00000000";

  const addrParts = addrPart.split("/");
  const segs = (addrParts[0] || "").split("-");

  return {
    s0: parseHex(segs[0], "0x0000") & 0xFFFF,
    s1: parseHex(segs[1], "0x0000") & 0xFFFF,
    s2: parseHex(segs[2], "0x0000") & 0xFFFF,
    s3: parseHex(segs[3], "0x0000") & 0xFFFF,
    s4: parseHex(addrParts[1], "0x0000") & 0xFFFF,
    s5: parseHex(addrParts[2], "0x0000") & 0xFFFF,
    s6: parseHex(addrParts[3], "0x0000") & 0xFFFF,
    s7: parseHex(addrParts[4], "0x0000") & 0xFFFF,
    payload: parseHex(payloadPart, "0x00000000"),
    mask: parseHex(maskPart, "0x00000000"),
    car: parseHex(carPart, "0x00000000"),
    cdr: parseHex(cdrPart, "0x00000000"),
  };
}

function packOMI(a) {
  return (
    hex16(a.s0) + "-" + hex16(a.s1) + "-" +
    hex16(a.s2) + "-" + hex16(a.s3) + "/" +
    hex16(a.s4) + "/" + hex16(a.s5) + "/" +
    hex16(a.s6) + "/" + hex16(a.s7) + "?" +
    hex32(a.payload) + "?" + hex32(a.mask) + "@" +
    hex32(a.car) + "@" + hex32(a.cdr)
  );
}

---

Opcode Execution

function execute(inst) {
  switch (inst.s3 & 0xFFFF) {
    case 0x0000: return 0;
    case 0x0001: return u16(inst.s4 ^ inst.s5);
    case 0x0002: return u16(~(inst.s4 ^ inst.s5));
    case 0x0003: return u16(~(inst.s4 & inst.s5));
    case 0x0004: return u16(inst.s4 & inst.s5);
    case 0x0005: return u16(inst.s4 | inst.s5);
    case 0x0006: return rotl16(inst.s4, inst.s5);
    case 0x0007: return rotr16(inst.s4, inst.s5);
    case 0x0008: return delta16(inst.s4, inst.s5);
    case 0x0009: return u16(bqf((inst.s4 >>> 8) & 0xFF, inst.s5 & 0xFF));
    case 0x000A: return u16(inst.s4);
    case 0x000B: return u16(inst.s4);
    case 0x000C: return u16(inst.s4);
    case 0x000D: return u16(inst.s4 ^ inst.s5 ^ inst.s6);
    case 0x000E: return u16(inst.car ^ inst.cdr);
    case 0x000F: return 0;
    default:
      return u16(
        inst.s0 ^ inst.s1 ^ inst.s2 ^ inst.s3 ^
        inst.s4 ^ inst.s5 ^ inst.s6 ^ inst.s7 ^
        inst.payload ^ inst.mask ^ inst.car ^ inst.cdr
      );
  }
}

---

TwinGeometry Pipeline

function resolveHopfQuquartRoute(chart11, baseQ, fiberQ, fano7, role3) {
  const g = {};
  g.chart11 = ((chart11 % 11) + 11) % 11;
  g.baseQ = ((baseQ % 4) + 4) % 4;
  g.fiberQ = ((fiberQ % 4) + 4) % 4;
  g.fano7 = ((fano7 % 7) + 7) % 7;
  g.role3 = ((role3 % 3) + 3) % 3;

  const x = g.baseQ, y = g.fiberQ;
  g.qxy = (60 * x * x) + (16 * x * y) + (4 * y * y);
  g.local240 = g.qxy % 240;
  g.slot5040 = g.fano7 * 720 + g.role3 * 240 + g.local240;

  const theta = ((x + 0.5) / 4.0) * Math.PI;
  const phi = ((y + 0.5) / 4.0) * 2.0 * Math.PI;
  const halfTheta = theta / 2.0;
  g.qw = Math.cos(halfTheta);
  g.qx = Math.sin(halfTheta) * Math.cos(phi);
  g.qy = Math.sin(halfTheta) * Math.sin(phi);
  g.qz = 0.0;
  g.fiberPhase = phi;

  const w = g.qw, qx = g.qx, qy = g.qy, qz = g.qz;
  g.a = Math.round(2.0 * (qx * qz + w * qy));
  g.b = Math.round(2.0 * (qy * qz - w * qx));
  g.c = Math.round(1.0 - 2.0 * (qx * qx + qy * qy));

  const local16 = g.local240 & 0x0f;
  const px = local16 & 3;
  const py = (local16 >> 2) & 3;
  g.polybiusRow = py + 2;
  g.polybiusCol = px + 2;
  g.frameType = baseQ & 3;
  return g;
}

---

Canonical Boot ROM

const BOOT_ROM = [
  "0x0000-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x0003-0x0003-0x0003-0x0003/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x039F-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x03BF-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x5555-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x55AA-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0xAAAA-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0xAA55-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x30000020-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x7C00-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0xAA55?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x5A3C-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x001C-0x001D-0x001E-0x001F/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x00F0-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x13B0-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x0000-0x0000-0x0000-0x0010/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x0000-0x0000-0x0000-0x0011/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x0000-0x0000-0x0000-0x0012/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x0000-0x0000-0x0000-0x0020/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x0000-0x0000-0x0000-0x0021/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0x0000-0x0000-0x0000-0x0022/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
  "0xFFFF-0x0000-0x0000-0x0000/0x0000/0x0000/0x0000/0x0000?0x00000000?0x00000000@0x00000000@0x00000000",
];

---

Receipt Ring

function makeRuntime(rom) {
  return {
    rom: rom.slice(),
    pc: 0,
    cursor: 0,
    epoch: 0,
    ring: new Array(RING_SIZE).fill(0n),
    results: [],
  };
}

function writeReceipt(rt, result) {
  const receipt = BigInt(result & 0xFFFF);
  if (result === 0) return { receipt, slot: null };
  const slot = rt.cursor % RING_SIZE;
  rt.ring[slot] = receipt;
  rt.cursor = (rt.cursor + 1) % RING_SIZE;
  if (rt.cursor === 0) rt.epoch++;
  return { receipt, slot };
}

function step(rt) {
  if (rt.pc >= rt.rom.length) return null;
  const line = rt.rom[rt.pc++];
  const inst = parseOMI(line);
  const result = execute(inst);
  const written = writeReceipt(rt, result);
  const record = {
    pc: rt.pc - 1,
    opcode: hex16(inst.s3),
    result: hex16(result),
    slot: written.slot,
    receipt: "0x" + written.receipt.toString(16).padStart(16, "0"),
  };
  rt.results.push(record);
  return record;
}

function run(rt) {
  while (rt.pc < rt.rom.length) {
    const record = step(rt);
    if (record && record.opcode === "0x000f") break;
  }
  return rt.results;
}

---

Ring Folds

function xorFold(ring) {
  let f = 0;
  for (const r of ring) f ^= Number(r & 0xFFFFn);
  return f;
}

function sumFold(ring) {
  let f = 0;
  for (const r of ring) f = u16(f + Number(r & 0xFFFFn));
  return f;
}

function rotFold(ring) {
  let f = 0;
  for (let i = 0; i < ring.length; i++)
    f = u16(f ^ rotl16(Number(ring[i] & 0xFFFFn), i & 15));
  return f;
}

---

Autonomous Loop

function autoLoop(rt) {
  while (true) {
    const xf = xorFold(rt.ring);
    const sf = sumFold(rt.ring);
    const rf = rotFold(rt.ring);

    const fanoPt = (xf ^ sf) % 7;
    let fanoLine = 0;
    for (let fi = 0; fi < 7; fi++) {
      if (FANO_LINES[fi].includes(fanoPt)) { fanoLine = fi; break; }
    }

    const opcodes = [0x0001,0x0002,0x0003,0x0004,0x0005,
                     0x0006,0x0007,0x0008,0x0009,0x000d,0x000e];
    const opIdx = (fanoLine * 3 + (rf % 3)) % 11;
    const opc = opcodes[opIdx];
    const x = rotl16(xf, rt.cursor) ^ opc;
    const y = rotr16(sf, rt.cursor) ^ opc;

    const inst = {
      s0: rt.cursor, s1: 0, s2: rt.cursor % RING_SIZE, s3: opc,
      s4: x, s5: y, s6: x ^ y ^ opc, s7: delta16(x, y),
      payload: (x << 16) | y, mask: ((x ^ y ^ opc) << 16) | delta16(x, y),
      car: (rt.cursor << 16) | 0, cdr: ((rt.cursor % RING_SIZE) << 16) | opc,
    };

    const result = execute(inst);
    const written = writeReceipt(rt, result);

    rt.cursor = (rt.cursor + 1) % RING_SIZE;
    if (result === 0 || written.slot === null) break;
  }
}

---

Modes (core/omicron.bin)

  (no args)     autonomous: self-generates from ring + stdin
  --eval <s>    evaluate S-expression, print receipt
  --repl        interactive S-expression REPL
  --auto        autonomous ring mode (self-generating, verbose)
  --boot        run boot sequence (21-stage init)
  --run <file>  process OMI address lines from file
  --cpu <addr>  execute OMI address on nibble CPU
  --geom        print geometry constants
  --seed <path> write packed boot ROM binary
  --ring        dump persistent receipt ring
  --watch       monitor ring evolution live
  --twin        display digital twin universe geometry
  --render-frame  output twin geometry as JSON frame
  --render-ppm    output Polybius grid as PPM image
  --serve [port]  HTTP server for portal build (default 8080)
  --help        this message

---

Render Outputs

--render-frame emits JSON with:
  ring size, filled slots, XOR/SUM/ROT folds
  per-receipt twin geometry (chart, base, fiber, fano, role,
    BQF, local240, slot5040, hopf, phase, cell, quat, frame)
  frame type counts (US/GS/RS/FS), Hopf flux

--render-ppm emits PPM image (420x480) showing:
  Polybius 5x5 grid with cells colored by frame-type density
  Ring occupancy bar
  Frame color legend

--serve runs HTTP server with endpoints:
  /       -> portal/dist/index.html (React TypeScript portal)
  /frame  -> same JSON as --render-frame
  /ring   -> full ring dump as JSON

---

Nibble CPU

Extracts 32 nibbles from S0-S7 segments.
Each nibble is a 4-bit opcode:

  0x0  NOP
  0x1  car_reg = cdr_reg; cdr_reg = payload
  0x2  payload = car_reg
  0x3  payload = cdr_reg
  0x4  payload ^= mask
  0x5  payload |= mask
  0x6  payload &= mask
  0x7  payload = rotl32(payload, 1)
  0x8  payload = memory[mask]
  0x9  memory[mask] = payload
  0xA  pc = mask & 0xFFFF
  0xB  cdr_reg = pc; pc = mask & 0xFFFF
  0xC  push payload to stack
  0xD  pop payload from stack
  0xE  flags = (payload == mask) ? 0xFFFFFFFF : 0
  0xF  halt

---

S-Expression Reducer

Supported primitives:
  (cons a b)     -> pair
  (car x)        -> head
  (cdr x)        -> tail
  (xor a b)      -> bitwise XOR as hex atom
  (rot x n)      -> bitwise rotation
  (mask x m)     -> bitwise AND
  (hash x)       -> FNV-1a hash as hex atom
  (quote x)      -> literal
  (normalize x)  -> canonical string
  (eval x)       -> reduce
  (receipt x)    -> OMI compilation receipt JSON
  (list ...)     -> list
  (reduce x)     -> reduce

OMI canonical form:
  (omi (section ...) ...) -> FS/GS/RS/US events

---

Build

make core/omicron.bin

---

Self Test

function selfTest() {
  const xorClose =
    0x0 ^ 0x1 ^ 0x2 ^ 0x3 ^
    0x4 ^ 0x5 ^ 0x6 ^ 0x7 ^
    0x8 ^ 0x9 ^ 0xA ^ 0xB ^
    0xC ^ 0xD ^ 0xE ^ 0xF;

  const carryClose =
    0x0 + 0x1 + 0x2 + 0x3 +
    0x4 + 0x5 + 0x6 + 0x7 +
    0x8 + 0x9 + 0xA + 0xB +
    0xC + 0xD + 0xE + 0xF;

  if (xorClose !== 0x00) throw new Error("xor close failed");
  if (carryClose !== 0x78) throw new Error("carry close failed");
  const sealed = carryClose | 0x07;
  if (sealed !== 0x7F) throw new Error("seal failed");
  const boot = sealed ^ 0x03;
  if (boot !== 0x7C) throw new Error("boot delimiter failed");
  const bootAddress = boot << 8;
  if (bootAddress !== 0x7C00) throw new Error("boot address failed");

  const rt = makeRuntime(BOOT_ROM);
  run(rt);
  return { ok: true, executed: rt.results.length, cursor: rt.cursor, epoch: rt.epoch };
}

---

Base64 Transport

Base64 is not authority. It is only a carrier for the ring or ROM.

function bytesToBase64(bytes) {
  let s = "";
  for (const b of bytes) s += String.fromCharCode(b & 0xFF);
  if (typeof btoa === "function") return btoa(s);
  if (typeof Buffer !== "undefined") return Buffer.from(bytes).toString("base64");
  throw new Error("no base64 encoder");
}

function base64ToBytes(b64) {
  if (typeof atob === "function") {
    const s = atob(b64);
    const out = [];
    for (let i = 0; i < s.length; i++) out.push(s.charCodeAt(i) & 0xFF);
    return out;
  }
  if (typeof Buffer !== "undefined") return Array.from(Buffer.from(b64, "base64"));
  throw new Error("no base64 decoder");
}

---

Agent Instruction

An agent that has never seen OMI must do this:

1. Read raw OMI address lines.
2. Parse each line as S0-S1-S2-S3/S4/S5/S6/S7?P?M@CAR@CDR.
3. Use S3 as opcode.
4. Execute only bitwise operations.
5. Write nonzero results as receipts.
6. Advance the 5040 ring.
7. Never invent a higher abstraction unless this machine cannot express the relation.

---

Final Canon

The address is syntax.
The address is grammar.
The address is instruction.

The runtime resolves addresses.
Opcodes apply bitwise transforms.
Transforms emit receipts.
Receipts advance the ring.

No dependencies.
No AST.
No database.
No framework.
No sugar.

Determinism is not optional.
Geometry is not decorative.
The ring is not a log.
