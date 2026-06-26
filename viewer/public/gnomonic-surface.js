(function() {
/* gnomonic-surface.js — Sphere → Plane flattening projection */
/* Pure functions only. No Math.random, no Date, no IO.        */

var SCALE = 120;

function normalize3(p) {
  var len = Math.sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
  if (len < 1e-12) return { x: 0, y: 0, z: 1 };
  return { x: p.x / len, y: p.y / len, z: p.z / len };
}

function projectPoint(p) {
  if (!p || p.z <= 1e-9) return { visible: false, x: 0, y: 0 };
  return {
    visible: true,
    x: SCALE * p.x / p.z,
    y: SCALE * p.y / p.z
  };
}

function receiptToSpherePoint(receipt) {
  var angleDeg = parseFloat(receipt && receipt.angle) || 0;
  var sm = (receipt && receipt.centroid && receipt.centroid.stop_metric) || 0;
  var theta = angleDeg * Math.PI / 180;
  var r = Math.min(1, Math.abs(sm) / 10);
  var x = r * Math.cos(theta);
  var y = r * Math.sin(theta);
  var zsq = 1 - x * x - y * y;
  var z = zsq > 0 ? Math.sqrt(zsq) : 0;
  return { x: x, y: y, z: z };
}

function receiptPairToGnomonicLine(a, b) {
  var pa = projectPoint(receiptToSpherePoint(a));
  var pb = projectPoint(receiptToSpherePoint(b));
  return {
    visible: pa.visible && pb.visible,
    ax: pa.x, ay: pa.y,
    bx: pb.x, by: pb.y,
    len: Math.sqrt((pb.x - pa.x) * (pb.x - pa.x) + (pb.y - pa.y) * (pb.y - pa.y))
  };
}

function projectPointToSVG(p, svgW, svgH) {
  var pj = projectPoint(p);
  if (!pj.visible) return null;
  return {
    visible: true,
    x: pj.x,
    y: pj.y
  };
}

/* ── DOM wiring ── */

var prevReceipt = null;

function ingestReceiptPair(receipt) {
  var gEl = document.getElementById('minimap-gnomonic');
  if (!gEl) { prevReceipt = receipt; return; }

  if (prevReceipt) {
    var line = receiptPairToGnomonicLine(prevReceipt, receipt);
    if (line.visible) {
      gEl.innerHTML =
        '<line x1="' + line.ax.toFixed(2) + '" y1="' + line.ay.toFixed(2) +
        '" x2="' + line.bx.toFixed(2) + '" y2="' + line.by.toFixed(2) +
        '" stroke="#c9b99a" stroke-width="1.5" opacity="0.6" stroke-linecap="round" filter="url(#mg-sm)"/>';
    }
  }

  prevReceipt = receipt;
}

window.GnomonicSurface = {
  normalize3: normalize3,
  projectPoint: projectPoint,
  receiptToSpherePoint: receiptToSpherePoint,
  receiptPairToGnomonicLine: receiptPairToGnomonicLine,
  projectPointToSVG: projectPointToSVG,
  ingestReceiptPair: ingestReceiptPair
};
})();
