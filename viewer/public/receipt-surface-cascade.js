(function() {
/* receipt-surface-cascade.js — Receipt → Cascade Slot → CSS Surface */
/* Pure functions only. No Math.random, no Date, no IO.              */

/* ═══════════════════════════════════════════════════════════════════
 * Existing surface functions
 * ═══════════════════════════════════════════════════════════════════ */

function receiptToCascadeSlot(receipt) {
  var angleDeg = parseFloat(receipt && receipt.angle) || 0;
  var sm = (receipt && receipt.centroid && receipt.centroid.stop_metric) || 0;
  var lane = Math.floor(((angleDeg + 180) % 360) / 36);
  var band = Math.min(Math.floor(Math.abs(sm)), 9);
  if (lane < 0) lane = 0;
  if (lane > 9) lane = 9;
  var radius = Math.min(Math.abs(sm) / 10, 1);
  return { lane: lane, band: band, angleDeg: angleDeg, stopMetric: sm, radius: radius };
}

function cascadeSlotToCSSVars(slot) {
  var hue = slot.lane * 36;
  var sat = 60 + slot.band * 4;
  var lit = 50 - slot.band * 3;
  return {
    '--omi-lane':         String(slot.lane),
    '--omi-band':         String(slot.band),
    '--omi-hue':          String(hue),
    '--omi-saturation':   sat + '%',
    '--omi-lightness':    lit + '%',
    '--omi-angle':        slot.angleDeg.toFixed(2) + 'deg',
    '--omi-stop-metric':  slot.stopMetric.toFixed(3),
    '--omi-color':        'hsl(' + hue + ',' + sat + '%,' + lit + '%)'
  };
}

function applyCSSVars(el, vars) {
  for (var k in vars)
    if (vars.hasOwnProperty(k))
      el.style.setProperty(k, vars[k]);
}

function applyCascadeSlotToRoot(slot) {
  var root = document.documentElement;
  root.style.setProperty('--omi-lane', String(slot.lane));
  root.style.setProperty('--omi-band', String(slot.band));
  root.style.setProperty('--omi-smith-angle', slot.angleDeg.toFixed(2) + 'deg');
  root.style.setProperty('--omi-smith-radius', slot.radius.toFixed(3));
  root.dataset.omiLane = String(slot.lane);
  root.dataset.omiBand = String(slot.band);
  root.dataset.smith = slot.lane + ':' + slot.band;
}

/* ═══════════════════════════════════════════════════════════════════
 * Mediant / rational slope functions
 * ═══════════════════════════════════════════════════════════════════ */

function receiptToRational(receipt) {
  var angleDeg = parseFloat(receipt && receipt.angle) || 0;
  angleDeg = ((angleDeg % 360) + 360) % 360;
  var sm = receipt && receipt.centroid && typeof receipt.centroid.stop_metric === 'number'
    ? receipt.centroid.stop_metric
    : 0;
  var b = Math.max(1, Math.round(angleDeg) || 1);
  var a = Math.max(1, Math.round(Math.abs(sm) * 1000) || 1);
  return { a: a, b: b, value: a / b, angleDeg: angleDeg, stopMetric: sm };
}

function mediant(left, right) {
  var a = left.a + right.a;
  var b = left.b + right.b;
  return { a: a, b: b, value: a / b };
}

function determinant(left, right) {
  return left.b * right.a - left.a * right.b;
}

function determinantClass(det) {
  var n = Math.abs(det);
  if (n === 0) return 'equivalent';
  if (n === 1) return 'adjacent';
  return 'wide';
}

function meanLadder(x, y) {
  var eps = 1e-9;
  x = Math.max(eps, Math.abs(x));
  y = Math.max(eps, Math.abs(y));
  var hm = (2 * x * y) / (x + y);
  var gm = Math.sqrt(x * y);
  var am = (x + y) / 2;
  var qm = Math.sqrt((x * x + y * y) / 2);
  return { hm: hm, gm: gm, am: am, qm: qm };
}

/* ═══════════════════════════════════════════════════════════════════
 * Receipt pair → surface projection
 * ═══════════════════════════════════════════════════════════════════ */

function receiptPairToSurface(leftReceipt, rightReceipt) {
  var left = receiptToRational(leftReceipt);
  var right = receiptToRational(rightReceipt);
  var m = mediant(left, right);
  var det = determinant(left, right);
  var cls = determinantClass(det);
  var means = meanLadder(left.value, right.value);
  var angleDeg = Math.atan(m.value) * 180 / Math.PI;
  var lane = Math.floor((((angleDeg + 180) % 360) / 36)) % 10;
  var pressure = means.qm;
  var band = Math.min(9, Math.max(0, Math.floor(Math.log1p(pressure + Math.abs(det)) % 10)));
  var radius = Math.min(1, Math.log1p(pressure) / 10);
  return {
    left: left,
    right: right,
    mediant: m,
    det: det,
    detClass: cls,
    means: means,
    lane: lane,
    band: band,
    angleDeg: angleDeg,
    radius: radius
  };
}

function applySurfaceToRoot(surface) {
  var root = document.documentElement;
  root.style.setProperty('--omi-mediant-a', String(surface.mediant.a));
  root.style.setProperty('--omi-mediant-b', String(surface.mediant.b));
  root.style.setProperty('--omi-mediant-value', surface.mediant.value.toFixed(6));
  root.style.setProperty('--omi-det', String(surface.det));
  root.style.setProperty('--omi-hm', surface.means.hm.toFixed(6));
  root.style.setProperty('--omi-gm', surface.means.gm.toFixed(6));
  root.style.setProperty('--omi-am', surface.means.am.toFixed(6));
  root.style.setProperty('--omi-qm', surface.means.qm.toFixed(6));
  root.dataset.mediant = surface.mediant.a + ':' + surface.mediant.b;
  root.dataset.mediantDet = String(surface.det);
  root.dataset.mediantGap = surface.detClass;
  root.dataset.meanBand =
    surface.detClass === 'equivalent' ? 'hm' :
    surface.detClass === 'adjacent' ? 'gm' :
    'qm';
}

/* ═══════════════════════════════════════════════════════════════════
 * Genaille SVG
 * ═══════════════════════════════════════════════════════════════════ */

function renderGenailleSVG(lane, band, angleDeg, surface) {
  var w = 220, h = 220, cols = 10, rows = 10;
  var cw = w / cols, rh = h / rows;
  var cells = [], labels = [], paths = [];
  angleDeg = angleDeg || 0;
  surface = surface || null;

  for (var c = 0; c < cols; c++) {
    var x = c * cw;
    labels.push(
      '<text x="' + (x + cw / 2) + '" y="10" text-anchor="middle" fill="' +
      (c === lane ? '#c9b99a' : '#3a3530') + '" font-size="8" font-family="monospace">' +
      c + '</text>'
    );
    for (var r = 0; r < rows; r++) {
      var y = 20 + r * rh;
      var active = (c === lane && r === band);
      cells.push(
        '<rect x="' + (x + 1) + '" y="' + y + '" width="' + (cw - 2) +
        '" height="' + (rh - 2) + '" fill="' +
        (active ? '#c9b99a' : (c % 2 === r % 2 ? '#141210' : '#0e0c0a')) +
        '" stroke="' + (active ? '#c9b99a' : '#1e1c18') + '" stroke-width="' +
        (active ? 1.5 : 0.5) + '" rx="1"' +
        (active ? ' filter="url(#gl-glow)"' : '') + '/>'
      );
      if (active) {
        var cy = y + rh / 2;
        paths.push('<circle cx="' + (x + cw / 2) + '" cy="' + cy +
          '" r="3" fill="#00ff44"/>');
        paths.push('<line x1="' + (x + cw / 2) + '" y1="20" x2="' +
          (x + cw / 2) + '" y2="' + cy +
          '" stroke="#00ff44" stroke-width="1" opacity="0.5"/>');
        var aRad = angleDeg * Math.PI / 180;
        paths.push('<line x1="' + (x + cw / 2) + '" y1="' + cy +
          '" x2="' + (x + cw / 2 + Math.cos(aRad) * 40) +
          '" y2="' + (cy - Math.sin(aRad) * 40) +
          '" stroke="#c9b99a" stroke-width="1" stroke-dasharray="2 2"/>');
      }
    }
  }

  var footer = '';
  if (surface) {
    footer = '<text x="4" y="226" fill="#3a3530" font-size="5" font-family="monospace">' +
      'm=' + surface.mediant.a + ':' + surface.mediant.b +
      ' det=' + surface.det +
      ' ' + surface.detClass +
      '</text>';
  }

  return '<svg viewBox="0 0 220 230" xmlns="http://www.w3.org/2000/svg">' +
    '<defs><filter id="gl-glow"><feGaussianBlur stdDeviation="1" result="b"/>' +
    '<feMerge><feMergeNode in="b"/><feMergeNode in="SourceGraphic"/></feMerge></filter></defs>' +
    '<rect width="220" height="230" fill="#080706" rx="3"/>' +
    labels.join('') + cells.join('') + paths.join('') + footer + '</svg>';
}

/* ═══════════════════════════════════════════════════════════════════
 * Orchestration
 * ═══════════════════════════════════════════════════════════════════ */

var previousReceipt = null;

function ingestFromReceipt(receipt) {
  var s = receiptToCascadeSlot(receipt);
  var vars = cascadeSlotToCSSVars(s);
  applyCSSVars(document.documentElement, vars);
  applyCascadeSlotToRoot(s);

  if (previousReceipt) {
    var surface = receiptPairToSurface(previousReceipt, receipt);
    applyCascadeSlotToRoot(surface);
    applySurfaceToRoot(surface);
    var genEl = document.getElementById('genaille-svg');
    if (genEl) genEl.innerHTML = renderGenailleSVG(surface.lane, surface.band, surface.angleDeg, surface);
  } else {
    var genEl0 = document.getElementById('genaille-svg');
    if (genEl0) genEl0.innerHTML = renderGenailleSVG(s.lane, s.band, s.angleDeg);
  }

  previousReceipt = receipt;
}

window.ReceiptSurfaceCascade = {
  /* existing */
  receiptToCascadeSlot: receiptToCascadeSlot,
  cascadeSlotToCSSVars: cascadeSlotToCSSVars,
  applyCSSVars:         applyCSSVars,
  applyCascadeSlotToRoot: applyCascadeSlotToRoot,
  renderGenailleSVG:    renderGenailleSVG,
  ingestFromReceipt:    ingestFromReceipt,
  /* new */
  receiptToRational:    receiptToRational,
  mediant:              mediant,
  determinant:          determinant,
  determinantClass:     determinantClass,
  meanLadder:           meanLadder,
  receiptPairToSurface: receiptPairToSurface,
  applySurfaceToRoot:   applySurfaceToRoot
};
})();
