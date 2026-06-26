var vm = require('vm');
var fs = require('fs');
var path = require('path');
var assert = require('assert');

var src = fs.readFileSync(
  path.join(__dirname, '..', 'viewer', 'public', 'gnomonic-surface.js'),
  'utf8'
);

var mockDoc = {
  getElementById: function() { return null; }
};

var sandbox = {
  window: {},
  document: mockDoc,
  Math: Math,
  parseFloat: parseFloat,
  console: console,
  Error: Error,
  JSON: JSON,
  Array: Array,
  String: String,
  Number: Number
};
vm.createContext(sandbox);
vm.runInContext(src, sandbox);

var G = sandbox.window.GnomonicSurface;

/* ─── Test 1: normalize3 preserves unit vector ─── */
(function() {
  var n = G.normalize3({ x: 1, y: 0, z: 0 });
  var len = Math.sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
  assert.ok(Math.abs(len - 1) < 1e-9);
  assert.ok(Math.abs(n.x - 1) < 1e-9);
  console.log('PASS 1: normalize3 preserves unit vector');
})();

/* ─── Test 2: normalize3 handles zero → north pole ─── */
(function() {
  var n = G.normalize3({ x: 0, y: 0, z: 0 });
  assert.ok(Math.abs(n.z - 1) < 1e-9);
  console.log('PASS 2: normalize3 zero → north pole');
})();

/* ─── Test 3: projectPoint north pole → (0,0) ─── */
(function() {
  var p = G.projectPoint({ x: 0, y: 0, z: 1 });
  assert.equal(p.visible, true);
  assert.ok(Math.abs(p.x) < 1e-9);
  assert.ok(Math.abs(p.y) < 1e-9);
  console.log('PASS 3: projectPoint north pole → origin');
})();

/* ─── Test 4: projectPoint invisible when z ≤ 0 ─── */
(function() {
  var p = G.projectPoint({ x: 1, y: 0, z: 0 });
  assert.equal(p.visible, false);
  console.log('PASS 4: projectPoint invisible at equator');
})();

/* ─── Test 5: projectPoint invisible below hemisphere ─── */
(function() {
  var p = G.projectPoint({ x: 0, y: 0, z: -0.5 });
  assert.equal(p.visible, false);
  console.log('PASS 5: projectPoint invisible below hemisphere');
})();

/* ─── Test 6: receiptToSpherePoint returns unit vector ─── */
(function() {
  var r = { angle: '0', centroid: { stop_metric: 0 } };
  var p = G.receiptToSpherePoint(r);
  var len = Math.sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
  assert.ok(Math.abs(len - 1) < 1e-6, 'len=' + len);
  console.log('PASS 6: receiptToSpherePoint returns unit vector');
})();

/* ─── Test 7: receiptToSpherePoint angle 0° → +x direction ─── */
(function() {
  var r = { angle: '0', centroid: { stop_metric: 10 } };
  var p = G.receiptToSpherePoint(r);
  assert.ok(p.x > 0.99, 'x=' + p.x);
  assert.ok(Math.abs(p.y) < 0.01);
  console.log('PASS 7: receiptToSpherePoint 0° → +x');
})();

/* ─── Test 8: receiptToSpherePoint angle 90° → +y direction ─── */
(function() {
  var r = { angle: '90', centroid: { stop_metric: 10 } };
  var p = G.receiptToSpherePoint(r);
  assert.ok(Math.abs(p.x) < 0.01);
  assert.ok(p.y > 0.99);
  console.log('PASS 8: receiptToSpherePoint 90° → +y');
})();

/* ─── Test 9: receiptPairToGnomonicLine visible ─── */
(function() {
  var a = { angle: '0', centroid: { stop_metric: 5 } };
  var b = { angle: '90', centroid: { stop_metric: 5 } };
  var line = G.receiptPairToGnomonicLine(a, b);
  assert.equal(line.visible, true);
  assert.ok(typeof line.ax === 'number');
  assert.ok(typeof line.bx === 'number');
  assert.ok(typeof line.len === 'number');
  assert.ok(line.len > 0);
  console.log('PASS 9: receiptPairToGnomonicLine visible');
})();

/* ─── Test 10: receiptPairToGnomonicLine invisible at equator ─── */
(function() {
  /* stop_metric=0 with angle 0 → r=0, z=1 → visible */
  var a = { angle: '0', centroid: { stop_metric: 10 } };
  /* stop_metric=10, angle 180 → r=1 at 180° → x=-1, y=0, z=0 → invisible */
  var b = { angle: '180', centroid: { stop_metric: 10 } };
  var line = G.receiptPairToGnomonicLine(a, b);
  assert.equal(line.visible, false);
  console.log('PASS 10: receiptPairToGnomonicLine invisible at horizon');
})();

/* ─── Test 11: projectPoint scaling ─── */
(function() {
  /* On sphere: x=0.5, y=0, z=sqrt(1-0.25)=0.866 */
  /* Projected: x = 120 * 0.5 / 0.866 = 69.28 */
  var p = G.projectPoint({ x: 0.5, y: 0, z: 0.8660254 });
  assert.equal(p.visible, true);
  assert.ok(Math.abs(p.x - 69.28) < 0.1);
  assert.ok(Math.abs(p.y) < 0.01);
  console.log('PASS 11: projectPoint scaling correct');
})();

console.log('\nAll tests passed.');
