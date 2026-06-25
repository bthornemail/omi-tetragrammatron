var vm = require('vm');
var fs = require('fs');
var path = require('path');
var assert = require('assert');

var GOLDEN_POINTER = "omi-0100-03bf-7c00-2b01-2f01-1434-039f-01ff/48";
var GOLDEN_S = [0x0100, 0x03bf, 0x7c00, 0x2b01, 0x2f01, 0x1434, 0x039f, 0x01ff];
var GOLDEN_HEX = "010003bf7c002b012f011434039f01ff";

var src = fs.readFileSync(
  path.join(__dirname, '..', 'viewer', 'public', 'jabcode-carrier.js'),
  'utf8'
);

var sandbox = {
  window: {},
  Uint8Array: Uint8Array,
  Uint16Array: Uint16Array,
  Error: Error,
  parseInt: parseInt,
  isNaN: isNaN,
  isFinite: isFinite,
  console: console,
  Number: Number,
  String: String,
  encodeURI: encodeURI,
  decodeURI: decodeURI,
  Array: Array,
  Math: Math,
  RegExp: RegExp,
  setTimeout: setTimeout,
  clearTimeout: clearTimeout,
  setInterval: setInterval,
  clearInterval: clearInterval,
  Date: Date,
  JSON: JSON
};
vm.createContext(sandbox);
vm.runInContext(src, sandbox);

var J = sandbox.window.JABCodeCarrier;

/* ─── Test 1: parsePointer ─── */
(function() {
  var r = J.parsePointer(GOLDEN_POINTER);
  assert.equal(r.prefix, 48, 'prefix should be 48');
  assert.equal(r.S.length, 8, 'S length should be 8');
  for (var i = 0; i < 8; i++) {
    assert.equal(r.S[i], GOLDEN_S[i], 'S[' + i + '] mismatch');
  }
  console.log('PASS 1: parsePointer');
})();

/* ─── Test 2: formatPointer ─── */
(function() {
  var S = new Uint16Array(GOLDEN_S);
  var out = J.formatPointer(S, 48);
  assert.equal(out, GOLDEN_POINTER);
  console.log('PASS 2: formatPointer');
})();

/* ─── Test 3: packFrame ─── */
(function() {
  var S = new Uint16Array(GOLDEN_S);
  var bytes = J.packFrame(S);
  assert.equal(bytes.length, 16);
  var hex = '';
  for (var i = 0; i < 16; i++) {
    var h = bytes[i].toString(16);
    if (h.length === 1) h = '0' + h;
    hex += h;
  }
  assert.equal(hex, GOLDEN_HEX);
  console.log('PASS 3: packFrame');
})();

/* ─── Test 4: unpackFrame ─── */
(function() {
  var bytes = new Uint8Array(16);
  for (var i = 0; i < 16; i++) {
    bytes[i] = parseInt(GOLDEN_HEX.substring(i * 2, i * 2 + 2), 16);
  }
  var S = J.unpackFrame(bytes);
  assert.equal(S.length, 8);
  for (var i = 0; i < 8; i++) {
    assert.equal(S[i], GOLDEN_S[i], 'S[' + i + '] mismatch');
  }
  console.log('PASS 4: unpackFrame');
})();

/* ─── Test 5: validateFrameShape golden passes ─── */
(function() {
  var S = new Uint16Array(GOLDEN_S);
  var shape = J.validateFrameShape(S);
  assert.equal(shape.ok, true);
  assert.equal(shape.errors.length, 0);
  assert.equal(shape.LL, 0x01);
  assert.equal(shape.NN, 0x7c00);
  assert.equal(shape.MM, 0x1434);
  console.log('PASS 5: validateFrameShape golden');
})();

/* ─── Test 6: mutate S[1] fails ─── */
(function() {
  var S = new Uint16Array(GOLDEN_S);
  S[1] = 0xdead;
  var shape = J.validateFrameShape(S);
  assert.equal(shape.ok, false);
  assert.ok(shape.errors.length > 0);
  console.log('PASS 6: mutate S[1] fails');
})();

/* ─── Test 7: mutate S[6] fails ─── */
(function() {
  var S = new Uint16Array(GOLDEN_S);
  S[6] = 0xbeef;
  var shape = J.validateFrameShape(S);
  assert.equal(shape.ok, false);
  assert.ok(shape.errors.length > 0);
  console.log('PASS 7: mutate S[6] fails');
})();

/* ─── Test 8: mutate S[7] low byte warns ─── */
(function() {
  var S = new Uint16Array(GOLDEN_S);
  S[7] = 0x0100;
  var shape = J.validateFrameShape(S);
  assert.ok(shape.warnings.length > 0);
  console.log('PASS 8: mutate S[7] low byte warns');
})();

/* ─── Test 9: decodePayload roundtrip ─── */
(function() {
  var S = new Uint16Array(GOLDEN_S);
  var bytes = J.packFrame(S);
  var decoded = J.decodePayload(bytes);
  assert.equal(decoded.S.length, 8);
  for (var i = 0; i < 8; i++) {
    assert.equal(decoded.S[i], GOLDEN_S[i], 'roundtrip S[' + i + '] mismatch');
  }
  assert.equal(decoded.shape.ok, true);
  console.log('PASS 9: decodePayload roundtrip');
})();

/* ─── Test 10: encodeToJABCode returns debug-matrix with SVG ─── */
(function() {
  var S = new Uint16Array(GOLDEN_S);
  var result = J.encodeToJABCode(S);
  assert.equal(result.kind, 'debug-matrix');
  assert.equal(result.profile, 'omi.jabcode.rs.v0');
  assert.ok(result.svg.indexOf('data-omi-profile="omi.jabcode.rs.v0"') >= 0);
  assert.ok(result.matrix.width === 21);
  assert.ok(result.matrix.height === 21);
  console.log('PASS 10: encodeToJABCode debug matrix');
})();

/* ─── Test 11: parsePointer lower case ─── */
(function() {
  var r = J.parsePointer(GOLDEN_POINTER.toLowerCase());
  assert.equal(r.prefix, 48);
  for (var i = 0; i < 8; i++) {
    assert.equal(r.S[i], GOLDEN_S[i]);
  }
  console.log('PASS 11: parsePointer lowercase');
})();

/* ─── Test 12: parsePointer without omi- prefix ─── */
(function() {
  var noPrefix = GOLDEN_POINTER.substring(4);
  var r = J.parsePointer(noPrefix);
  assert.equal(r.prefix, 48);
  for (var i = 0; i < 8; i++) {
    assert.equal(r.S[i], GOLDEN_S[i]);
  }
  console.log('PASS 12: parsePointer without omi-');
})();

/* ─── Test 13: decodeFromJABCode accepts hex string ─── */
(function() {
  var result = J.decodeFromJABCode(GOLDEN_HEX);
  assert.equal(result.profile, 'omi.jabcode.rs.v0');
  assert.equal(result.shape.ok, true);
  assert.equal(result.requiresAlgebraicValidation, true);
  console.log('PASS 13: decodeFromJABCode hex');
})();

/* ─── Test 14: decodeFromJABCode accepts pointer ─── */
(function() {
  var result = J.decodeFromJABCode(GOLDEN_POINTER);
  assert.equal(result.shape.ok, true);
  assert.equal(result.pointer, GOLDEN_POINTER);
  console.log('PASS 14: decodeFromJABCode pointer');
})();

/* ─── Test 15: packFrame throws on bad length ─── */
(function() {
  var threw = false;
  try { J.packFrame(new Uint16Array(4)); } catch (e) { threw = true; }
  assert.ok(threw, 'should throw on wrong length');
  console.log('PASS 15: packFrame throws on bad length');
})();

/* ─── Test 16: unpackFrame throws on bad length ─── */
(function() {
  var threw = false;
  try { J.unpackFrame(new Uint8Array(4)); } catch (e) { threw = true; }
  assert.ok(threw, 'should throw on wrong length');
  console.log('PASS 16: unpackFrame throws on bad length');
})();

/* ─── Test 17: parsePointer throws on malformed ─── */
(function() {
  var threw = false;
  try { J.parsePointer('bad'); } catch (e) { threw = true; }
  assert.ok(threw, 'should throw on malformed');
  console.log('PASS 17: parsePointer throws on malformed');
})();

console.log('\nAll tests passed.');
