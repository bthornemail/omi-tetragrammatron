var vm = require('vm');
var fs = require('fs');
var path = require('path');
var assert = require('assert');

var src = fs.readFileSync(
  path.join(__dirname, '..', 'viewer', 'public', 'receipt-surface-cascade.js'),
  'utf8'
);

var doc = {
  documentElement: {
    style: { setProperty: function() {} },
    dataset: {}
  },
  getElementById: function() { return null; }
};

var sandbox = {
  window: {},
  document: doc,
  Math: Math,
  parseFloat: parseFloat,
  console: console,
  Error: Error,
  JSON: JSON,
  Array: Array,
  String: String,
  Number: Number,
  isNaN: isNaN,
  isFinite: isFinite
};
vm.createContext(sandbox);
vm.runInContext(src, sandbox);

var C = sandbox.window.ReceiptSurfaceCascade;

/* ─── Test 1: receiptToRational returns positive integers ─── */
(function() {
  var r = C.receiptToRational({ angle: '90', centroid: { stop_metric: 5 } });
  assert.equal(typeof r.a, 'number');
  assert.equal(typeof r.b, 'number');
  assert.ok(r.a > 0, 'a should be positive');
  assert.ok(r.b > 0, 'b should be positive');
  assert.ok(r.value > 0);
  console.log('PASS 1: receiptToRational returns positive integers');
})();

/* ─── Test 2: mediant of 1/2 and 1/3 is 2/5 ─── */
(function() {
  var left  = { a: 1, b: 2, value: 0.5 };
  var right = { a: 1, b: 3, value: 1 / 3 };
  var m = C.mediant(left, right);
  assert.equal(m.a, 2);
  assert.equal(m.b, 5);
  assert.ok(Math.abs(m.value - 0.4) < 1e-9);
  console.log('PASS 2: mediant({1/2},{1/3}) = 2/5');
})();

/* ─── Test 3: mediant of equal slopes ─── */
(function() {
  var left  = { a: 2, b: 4, value: 0.5 };
  var right = { a: 3, b: 6, value: 0.5 };
  var m = C.mediant(left, right);
  assert.equal(m.a, 5);
  assert.equal(m.b, 10);
  assert.ok(Math.abs(m.value - 0.5) < 1e-9);
  console.log('PASS 3: mediant of equal slopes');
})();

/* ─── Test 4: determinant sign and magnitude stable ─── */
(function() {
  var left  = { a: 1, b: 2 };
  var right = { a: 2, b: 3 };
  var det = C.determinant(left, right);
  /* b*c - a*d = 2*2 - 1*3 = 4 - 3 = 1 */
  assert.equal(det, 1);
  console.log('PASS 4: determinant(1/2, 2/3) = 1');
})();

/* ─── Test 5: determinant is anti-symmetric ─── */
(function() {
  var left  = { a: 1, b: 2 };
  var right = { a: 2, b: 3 };
  var fwd = C.determinant(left, right);
  var rev = C.determinant(right, left);
  assert.equal(fwd, -rev);
  console.log('PASS 5: determinant is anti-symmetric');
})();

/* ─── Test 6: determinantClass(0) = equivalent ─── */
(function() {
  assert.equal(C.determinantClass(0), 'equivalent');
  console.log('PASS 6: determinantClass(0) = equivalent');
})();

/* ─── Test 7: determinantClass(1) = adjacent ─── */
(function() {
  assert.equal(C.determinantClass(1), 'adjacent');
  assert.equal(C.determinantClass(-1), 'adjacent');
  console.log('PASS 7: determinantClass(±1) = adjacent');
})();

/* ─── Test 8: determinantClass(2) = wide ─── */
(function() {
  assert.equal(C.determinantClass(2), 'wide');
  assert.equal(C.determinantClass(100), 'wide');
  assert.equal(C.determinantClass(-5), 'wide');
  console.log('PASS 8: determinantClass(|det|≥2) = wide');
})();

/* ─── Test 9: meanLadder obeys HM ≤ GM ≤ AM ≤ QM ─── */
(function() {
  var testPairs = [
    [1, 2], [3, 7], [0.5, 0.8], [10, 20], [1, 100], [0.01, 0.03]
  ];
  for (var i = 0; i < testPairs.length; i++) {
    var x = testPairs[i][0], y = testPairs[i][1];
    var m = C.meanLadder(x, y);
    assert.ok(m.hm <= m.gm + 1e-12, 'HM ≤ GM for (' + x + ',' + y + ')');
    assert.ok(m.gm <= m.am + 1e-12, 'GM ≤ AM for (' + x + ',' + y + ')');
    assert.ok(m.am <= m.qm + 1e-12, 'AM ≤ QM for (' + x + ',' + y + ')');
  }
  console.log('PASS 9: meanLadder obeys HM ≤ GM ≤ AM ≤ QM');
})();

/* ─── Test 10: meanLadder clamps near-zero inputs ─── */
(function() {
  var m = C.meanLadder(0, 0);
  assert.ok(m.hm > 0);
  assert.ok(m.gm > 0);
  assert.ok(m.am > 0);
  assert.ok(m.qm > 0);
  console.log('PASS 10: meanLadder clamps near-zero');
})();

/* ─── Test 11: receiptPairToSurface returns lane/band in 0..9 ─── */
(function() {
  var leftR  = { angle: '0',   centroid: { stop_metric: 1 } };
  var rightR = { angle: '180', centroid: { stop_metric: 5 } };
  var s = C.receiptPairToSurface(leftR, rightR);
  assert.ok(s.lane >= 0 && s.lane <= 9, 'lane in 0..9');
  assert.ok(s.band >= 0 && s.band <= 9, 'band in 0..9');
  assert.ok(typeof s.det === 'number');
  assert.ok(['equivalent','adjacent','wide'].indexOf(s.detClass) >= 0);
  console.log('PASS 11: receiptPairToSurface lane/band in range');
})();

/* ─── Test 12: receiptPairToSurface with same angle → equivalent ─── */
(function() {
  var r = { angle: '45', centroid: { stop_metric: 2 } };
  var s = C.receiptPairToSurface(r, r);
  assert.equal(s.detClass, 'equivalent');
  assert.equal(s.det, 0);
  console.log('PASS 12: same receipt pair → equivalent');
})();

/* ─── Test 13: receiptToRational handles missing fields ─── */
(function() {
  var r = C.receiptToRational({});
  assert.ok(r.a > 0);
  assert.ok(r.b > 0);
  assert.ok(r.value > 0);
  console.log('PASS 13: receiptToRational handles empty receipt');
})();

/* ─── Test 14: receiptToRational normalizes angle ─── */
(function() {
  var r = C.receiptToRational({ angle: '540', centroid: { stop_metric: 3 } });
  /* 540 % 360 = 180 */
  assert.ok(r.angleDeg >= 0 && r.angleDeg < 360);
  console.log('PASS 14: receiptToRational normalizes angle');
})();

console.log('\nAll tests passed.');
