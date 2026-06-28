export interface ReceiptLike {
  angle?: string | number
  centroid?: { stop_metric?: number }
}

export interface CascadeSlot {
  lane: number
  band: number
  angleDeg: number
  stopMetric: number
  radius: number
}

export interface RationalReceipt {
  a: number
  b: number
  value: number
  angleDeg?: number
  stopMetric?: number
}

export interface ReceiptSurface {
  left: RationalReceipt
  right: RationalReceipt
  mediant: RationalReceipt
  det: number
  detClass: 'equivalent' | 'adjacent' | 'wide'
  means: { hm: number; gm: number; am: number; qm: number }
  lane: number
  band: number
  angleDeg: number
  radius: number
}

export function receiptToCascadeSlot(receipt: ReceiptLike): CascadeSlot {
  const angleDeg = Number.parseFloat(String(receipt?.angle ?? 0)) || 0
  const sm = receipt?.centroid?.stop_metric ?? 0
  let lane = Math.floor(((angleDeg + 180) % 360) / 36)
  const band = Math.min(Math.floor(Math.abs(sm)), 9)
  if (lane < 0) lane = 0
  if (lane > 9) lane = 9
  return { lane, band, angleDeg, stopMetric: sm, radius: Math.min(Math.abs(sm) / 10, 1) }
}

export function cascadeSlotToCSSVars(slot: CascadeSlot): Record<string, string> {
  const hue = slot.lane * 36
  const sat = 60 + slot.band * 4
  const lit = 50 - slot.band * 3
  return {
    '--omi-lane': String(slot.lane),
    '--omi-band': String(slot.band),
    '--omi-hue': String(hue),
    '--omi-saturation': `${sat}%`,
    '--omi-lightness': `${lit}%`,
    '--omi-angle': `${slot.angleDeg.toFixed(2)}deg`,
    '--omi-stop-metric': slot.stopMetric.toFixed(3),
    '--omi-color': `hsl(${hue},${sat}%,${lit}%)`,
  }
}

export function receiptToRational(receipt: ReceiptLike): RationalReceipt {
  let angleDeg = Number.parseFloat(String(receipt?.angle ?? 0)) || 0
  angleDeg = ((angleDeg % 360) + 360) % 360
  const sm = typeof receipt?.centroid?.stop_metric === 'number' ? receipt.centroid.stop_metric : 0
  const b = Math.max(1, Math.round(angleDeg) || 1)
  const a = Math.max(1, Math.round(Math.abs(sm) * 1000) || 1)
  return { a, b, value: a / b, angleDeg, stopMetric: sm }
}

export function mediant(left: RationalReceipt, right: RationalReceipt): RationalReceipt {
  const a = left.a + right.a
  const b = left.b + right.b
  return { a, b, value: a / b }
}

export function determinant(left: Pick<RationalReceipt, 'a' | 'b'>, right: Pick<RationalReceipt, 'a' | 'b'>): number {
  return left.b * right.a - left.a * right.b
}

export function determinantClass(det: number): ReceiptSurface['detClass'] {
  const n = Math.abs(det)
  if (n === 0) return 'equivalent'
  if (n === 1) return 'adjacent'
  return 'wide'
}

export function meanLadder(x: number, y: number) {
  const eps = 1e-9
  x = Math.max(eps, Math.abs(x))
  y = Math.max(eps, Math.abs(y))
  const hm = (2 * x * y) / (x + y)
  const gm = Math.sqrt(x * y)
  const am = (x + y) / 2
  const qm = Math.sqrt((x * x + y * y) / 2)
  return { hm, gm, am, qm }
}

export function receiptPairToSurface(leftReceipt: ReceiptLike, rightReceipt: ReceiptLike): ReceiptSurface {
  const left = receiptToRational(leftReceipt)
  const right = receiptToRational(rightReceipt)
  const m = mediant(left, right)
  const det = determinant(left, right)
  const means = meanLadder(left.value, right.value)
  const angleDeg = (Math.atan(m.value) * 180) / Math.PI
  const lane = Math.floor(((angleDeg + 180) % 360) / 36) % 10
  const pressure = means.qm
  const band = Math.min(9, Math.max(0, Math.floor(Math.log1p(pressure + Math.abs(det)) % 10)))
  return {
    left,
    right,
    mediant: m,
    det,
    detClass: determinantClass(det),
    means,
    lane,
    band,
    angleDeg,
    radius: Math.min(1, Math.log1p(pressure) / 10),
  }
}

export function renderGenailleSVG(lane: number, band: number, angleDeg = 0, surface?: ReceiptSurface): string {
  const w = 220
  const h = 220
  const cols = 10
  const rows = 10
  const cw = w / cols
  const rh = h / rows
  const cells: string[] = []
  const labels: string[] = []
  const paths: string[] = []

  for (let c = 0; c < cols; c++) {
    const x = c * cw
    labels.push(`<text x="${x + cw / 2}" y="10" text-anchor="middle" fill="${c === lane ? '#c9b99a' : '#3a3530'}" font-size="8" font-family="monospace">${c}</text>`)
    for (let r = 0; r < rows; r++) {
      const y = 20 + r * rh
      const active = c === lane && r === band
      cells.push(`<rect x="${x + 1}" y="${y}" width="${cw - 2}" height="${rh - 2}" fill="${active ? '#c9b99a' : c % 2 === r % 2 ? '#141210' : '#0e0c0a'}" stroke="${active ? '#c9b99a' : '#1e1c18'}" stroke-width="${active ? 1.5 : 0.5}" rx="1"${active ? ' filter="url(#gl-glow)"' : ''}/>`)
      if (active) {
        const cy = y + rh / 2
        const aRad = (angleDeg * Math.PI) / 180
        paths.push(`<circle cx="${x + cw / 2}" cy="${cy}" r="3" fill="#00ff44"/>`)
        paths.push(`<line x1="${x + cw / 2}" y1="20" x2="${x + cw / 2}" y2="${cy}" stroke="#00ff44" stroke-width="1" opacity="0.5"/>`)
        paths.push(`<line x1="${x + cw / 2}" y1="${cy}" x2="${x + cw / 2 + Math.cos(aRad) * 40}" y2="${cy - Math.sin(aRad) * 40}" stroke="#c9b99a" stroke-width="1" stroke-dasharray="2 2"/>`)
      }
    }
  }

  const footer = surface
    ? `<text x="4" y="226" fill="#3a3530" font-size="5" font-family="monospace">m=${surface.mediant.a}:${surface.mediant.b} det=${surface.det} ${surface.detClass}</text>`
    : ''
  return `<svg viewBox="0 0 220 230" xmlns="http://www.w3.org/2000/svg"><defs><filter id="gl-glow"><feGaussianBlur stdDeviation="1" result="b"/><feMerge><feMergeNode in="b"/><feMergeNode in="SourceGraphic"/></feMerge></filter></defs><rect width="220" height="230" fill="#080706" rx="3"/>${labels.join('')}${cells.join('')}${paths.join('')}${footer}</svg>`
}
