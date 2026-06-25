(function() {
var PROFILE = "omi.jabcode.rs.v0";

var PALETTE = [
  "#000000",
  "#0000ff",
  "#00ff00",
  "#00ffff",
  "#ff0000",
  "#ff00ff",
  "#ffff00",
  "#ffffff"
];

var PRELUDE = "OMI-JAB-RS-v0:";

function parsePointer(text) {
  if (typeof text !== 'string') throw new Error("expected string");
  var s = text.trim();
  var prefix = null;
  if (s.indexOf('omi-') === 0) s = s.substring(4);
  var slashIdx = s.indexOf('/');
  if (slashIdx >= 0) {
    var prefixStr = s.substring(slashIdx + 1);
    prefix = parseInt(prefixStr, 10);
    if (isNaN(prefix)) throw new Error("invalid prefix: " + prefixStr);
    s = s.substring(0, slashIdx);
  }
  var parts = s.split('-');
  if (parts.length !== 8) throw new Error("expected 8 hex segments");
  var S = new Uint16Array(8);
  for (var i = 0; i < 8; i++) {
    var v = parseInt(parts[i], 16);
    if (isNaN(v) || v < 0 || v > 0xffff) throw new Error("invalid segment at " + i);
    S[i] = v;
  }
  return { S: S, prefix: prefix };
}

function formatPointer(S, prefix) {
  if (!S || S.length !== 8) throw new Error("expected uint16[8]");
  var parts = [];
  for (var i = 0; i < 8; i++) {
    var h = Number(S[i]).toString(16);
    while (h.length < 4) h = '0' + h;
    parts.push(h);
  }
  var pfx = (prefix === undefined || prefix === null) ? 48 : prefix;
  return 'omi-' + parts.join('-') + '/' + pfx;
}

function packFrame(S) {
  if (!S || S.length !== 8) throw new Error("expected uint16[8]");
  var out = new Uint8Array(16);
  for (var i = 0; i < 8; i++) {
    var v = Number(S[i]);
    if (!Number.isInteger(v) || v < 0 || v > 0xffff) {
      throw new Error("segment out of uint16 range at " + i);
    }
    out[i * 2] = (v >>> 8) & 0xff;
    out[i * 2 + 1] = v & 0xff;
  }
  return out;
}

function unpackFrame(bytes) {
  if (!bytes || bytes.length !== 16) throw new Error("expected 16 bytes");
  var S = new Uint16Array(8);
  for (var i = 0; i < 8; i++) {
    S[i] = ((bytes[i * 2] << 8) | bytes[i * 2 + 1]) & 0xffff;
  }
  return S;
}

function validateFrameShape(S) {
  var result = { ok: true, errors: [], warnings: [], LL: null, NN: null, MM: null };
  if (!S || S.length !== 8) {
    result.ok = false;
    result.errors.push("expected uint16[8], got " + (S ? S.length : 'null'));
    return result;
  }
  if (S[1] !== 0x03bf) result.errors.push("S1 expected 0x03bf, got 0x" + S[1].toString(16));
  if (S[6] !== 0x039f) result.errors.push("S6 expected 0x039f, got 0x" + S[6].toString(16));
  if ((S[0] & 0xff) !== 0x00) result.warnings.push("S0 low byte expected 0x00");
  if ((S[3] >>> 8) !== 0x2b) result.warnings.push("S3 high byte expected 0x2b");
  if ((S[4] >>> 8) !== 0x2f) result.warnings.push("S4 high byte expected 0x2f");
  if ((S[7] & 0xff) !== 0xff) result.warnings.push("S7 low byte expected 0xff");
  var LL0 = S[0] >>> 8;
  var LL3 = S[3] & 0xff;
  var LL4 = S[4] & 0xff;
  var LL7 = S[7] >>> 8;
  if (!(LL0 === LL3 && LL3 === LL4 && LL4 === LL7)) {
    result.warnings.push("LL inconsistency: " + LL0 + ", " + LL3 + ", " + LL4 + ", " + LL7);
  }
  result.ok = result.errors.length === 0;
  if (result.ok) {
    result.LL = LL0;
    result.NN = S[2];
    result.MM = S[5];
  }
  return result;
}

function extractFrameFields(S) {
  return {
    LL: (S[0] >>> 8) & 0xff,
    NN: S[2],
    MM: S[5],
    openOmicron: S[1],
    closeOmicron: S[6],
    prefix: null
  };
}

function encodePayload(S) {
  return packFrame(S);
}

function decodePayload(bytes) {
  var S = unpackFrame(bytes);
  var shape = validateFrameShape(S);
  return { S: S, shape: shape, bytes: bytes };
}

function bytesToBits(bytes) {
  var bits = [];
  for (var i = 0; i < bytes.length; i++) {
    for (var b = 7; b >= 0; b--) bits.push((bytes[i] >>> b) & 1);
  }
  return bits;
}

function bitsToColorIndices(bits) {
  var indices = [];
  for (var i = 0; i + 2 < bits.length; i += 3) {
    indices.push((bits[i] << 2) | (bits[i + 1] << 1) | bits[i + 2]);
  }
  return indices;
}

function textToBytes(s) {
  var out = new Uint8Array(s.length);
  for (var i = 0; i < s.length; i++) out[i] = s.charCodeAt(i);
  return out;
}

function renderDebugMatrix(S, options) {
  options = options || {};
  var MATRIX_SIZE = 21;
  var FINDER_SIZE = 5;

  var frameBytes = packFrame(S);
  var preludeBytes = textToBytes(PRELUDE);
  var allBytes = new Uint8Array(preludeBytes.length + frameBytes.length);
  allBytes.set(preludeBytes);
  allBytes.set(frameBytes, preludeBytes.length);

  var bits = bytesToBits(allBytes);
  var colorIndices = bitsToColorIndices(bits);

  var cells = new Uint8Array(MATRIX_SIZE * MATRIX_SIZE);
  for (var i = 0; i < cells.length; i++) cells[i] = 255;

  function setCell(x, y, val) { cells[y * MATRIX_SIZE + x] = val; }
  function getCell(x, y) { return cells[y * MATRIX_SIZE + x]; }

  function isFinder(x, y) {
    var inTL = x < FINDER_SIZE && y < FINDER_SIZE;
    var inTR = x >= MATRIX_SIZE - FINDER_SIZE && y < FINDER_SIZE;
    var inBL = x < FINDER_SIZE && y >= MATRIX_SIZE - FINDER_SIZE;
    var inBR = x >= MATRIX_SIZE - FINDER_SIZE && y >= MATRIX_SIZE - FINDER_SIZE;
    return inTL || inTR || inBL || inBR;
  }

  function drawFinder(ox, oy) {
    for (var fy = 0; fy < FINDER_SIZE; fy++) {
      for (var fx = 0; fx < FINDER_SIZE; fx++) {
        var isEdge = fy === 0 || fy === FINDER_SIZE - 1 || fx === 0 || fx === FINDER_SIZE - 1;
        setCell(ox + fx, oy + fy, isEdge ? 0 : 7);
      }
    }
  }

  drawFinder(0, 0);
  drawFinder(MATRIX_SIZE - FINDER_SIZE, 0);
  drawFinder(0, MATRIX_SIZE - FINDER_SIZE);
  drawFinder(MATRIX_SIZE - FINDER_SIZE, MATRIX_SIZE - FINDER_SIZE);

  var ci = 0;
  for (var y = 0; y < MATRIX_SIZE && ci < colorIndices.length; y++) {
    for (var x = 0; x < MATRIX_SIZE && ci < colorIndices.length; x++) {
      if (!isFinder(x, y) && getCell(x, y) === 255) {
        setCell(x, y, colorIndices[ci]);
        ci++;
      }
    }
  }

  var matrix = { width: MATRIX_SIZE, height: MATRIX_SIZE, cells: cells, palette: PALETTE };
  var svg = matrixToSVG(matrix, options);

  return {
    profile: PROFILE,
    kind: "debug-matrix",
    S: S,
    bytes: frameBytes,
    matrix: matrix,
    svg: svg
  };
}

function matrixToSVG(matrix, options) {
  options = options || {};
  var w = matrix.width;
  var h = matrix.height;
  var cells = matrix.cells;
  var palette = matrix.palette;
  var ms = options.moduleSize || 10;
  var pad = options.padding || 2;
  var tw = w * ms + pad * 2;
  var th = h * ms + pad * 2;
  var rects = [];
  for (var y = 0; y < h; y++) {
    for (var x = 0; x < w; x++) {
      var idx = cells[y * w + x];
      if (idx === 255) continue;
      var color = palette[idx % palette.length];
      rects.push('<rect x="' + (pad + x * ms) + '" y="' + (pad + y * ms) + '" width="' + ms + '" height="' + ms + '" fill="' + color + '"/>');
    }
  }
  return '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 ' + tw + ' ' + th + '" data-omi-carrier="jabcode-rs" data-omi-profile="' + PROFILE + '">' + rects.join('') + '</svg>';
}

function encodeToJABCode(S, options) {
  return renderDebugMatrix(S, options);
}

function decodeFromJABCode(input, options) {
  options = options || {};
  var bytes;
  var pointer = null;
  if (input instanceof Uint8Array) {
    bytes = input;
  } else if (typeof input === 'string') {
    var s = input.trim();
    if (s.indexOf('omi-') === 0) {
      var parsed = parsePointer(s);
      bytes = packFrame(parsed.S);
      pointer = s;
    } else if (/^[0-9a-fA-F]{32}$/.test(s)) {
      bytes = new Uint8Array(16);
      for (var i = 0; i < 16; i++) bytes[i] = parseInt(s.substring(i * 2, i * 2 + 2), 16);
    } else {
      throw new Error("unknown input format: expected pointer or 32-char hex");
    }
  } else {
    throw new Error("expected Uint8Array or string");
  }
  var decoded = decodePayload(bytes);
  var S = decoded.S;
  var shape = decoded.shape;
  var fields = shape.ok ? extractFrameFields(S) : null;
  return {
    profile: PROFILE,
    S: S,
    bytes: bytes,
    pointer: pointer || formatPointer(S),
    shape: shape,
    fields: fields,
    requiresAlgebraicValidation: true
  };
}

window.JABCodeCarrier = {
  PROFILE: PROFILE,
  PALETTE: PALETTE,
  parsePointer: parsePointer,
  formatPointer: formatPointer,
  packFrame: packFrame,
  unpackFrame: unpackFrame,
  validateFrameShape: validateFrameShape,
  extractFrameFields: extractFrameFields,
  encodePayload: encodePayload,
  decodePayload: decodePayload,
  encodeToJABCode: encodeToJABCode,
  decodeFromJABCode: decodeFromJABCode,
  renderDebugMatrix: renderDebugMatrix,
  matrixToSVG: matrixToSVG
};
})();
