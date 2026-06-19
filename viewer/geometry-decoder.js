/* Pure JS TwinGeometry decoder — deterministic, mirrors opencode.c */

const FANO_LINES = [
  [0,1,2],[0,3,4],[1,3,5],[1,4,6],[2,3,6],[2,4,5],[3,4,0]
];

const QUQUART_PHASE = [0x1f, 0x1d, 0x1e, 0x1c];
const FRAME_NAMES = ['US','GS','RS','FS'];

function fnv1a32(data) {
  let h = 0x811c9dc5 >>> 0;
  for (let i = 0; i < data.length; i++) {
    h ^= data.charCodeAt(i) & 0xff;
    h = Math.imul(h, 0x01000193) >>> 0;
  }
  return h;
}

function modPos(x, m) { return ((x % m) + m) % m; }

function bqf32(x, y) {
  return (60 * x * x) + (16 * x * y) + (4 * y * y);
}

function polybiusGetInterior(x, y) {
  if (x < 0 || x > 3 || y < 0 || y > 3) return null;
  return { row: y + 2, col: x + 2 };
}

function resolveHopfQuquartRoute(chart11, baseQ, fiberQ, fano7, role3) {
  const g = {};
  g.chart11 = modPos(chart11, 11);
  g.baseQ = modPos(baseQ, 4);
  g.fiberQ = modPos(fiberQ, 4);
  g.fano7 = modPos(fano7, 7);
  g.role3 = modPos(role3, 3);

  const x = g.baseQ, y = g.fiberQ;
  g.qxy = bqf32(x, y);
  g.local240 = g.qxy % 240;
  g.slot5040 = g.fano7 * 720 + g.role3 * 240 + g.local240;

  const TWIN_PI = 3.14159265358979323846;
  const theta = ((x + 0.5) / 4.0) * TWIN_PI;
  const phi = ((y + 0.5) / 4.0) * 2.0 * TWIN_PI;
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
  const cell = polybiusGetInterior(px, py);
  if (cell) {
    g.polybiusRow = cell.row;
    g.polybiusCol = cell.col;
  } else {
    g.polybiusRow = 2;
    g.polybiusCol = 2;
  }

  g.frameType = baseQ & 3;
  return g;
}

/* Total time: 0.037s */
