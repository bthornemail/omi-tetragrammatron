import type { ReceiptLike } from './receiptSurfaceCascade'

const SCALE = 120

export interface Vec3 {
  x: number
  y: number
  z: number
}

export interface ProjectedPoint {
  visible: boolean
  x: number
  y: number
}

export function normalize3(p: Vec3): Vec3 {
  const len = Math.sqrt(p.x * p.x + p.y * p.y + p.z * p.z)
  if (len < 1e-12) return { x: 0, y: 0, z: 1 }
  return { x: p.x / len, y: p.y / len, z: p.z / len }
}

export function projectPoint(p: Vec3 | null | undefined): ProjectedPoint {
  if (!p || p.z <= 1e-9) return { visible: false, x: 0, y: 0 }
  return { visible: true, x: (SCALE * p.x) / p.z, y: (SCALE * p.y) / p.z }
}

export function receiptToSpherePoint(receipt: ReceiptLike): Vec3 {
  const angleDeg = Number.parseFloat(String(receipt?.angle ?? 0)) || 0
  const sm = receipt?.centroid?.stop_metric ?? 0
  const theta = (angleDeg * Math.PI) / 180
  const r = Math.min(1, Math.abs(sm) / 10)
  const x = r * Math.cos(theta)
  const y = r * Math.sin(theta)
  const zsq = 1 - x * x - y * y
  return { x, y, z: zsq > 0 ? Math.sqrt(zsq) : 0 }
}

export function receiptPairToGnomonicLine(a: ReceiptLike, b: ReceiptLike) {
  const pa = projectPoint(receiptToSpherePoint(a))
  const pb = projectPoint(receiptToSpherePoint(b))
  return {
    visible: pa.visible && pb.visible,
    ax: pa.x,
    ay: pa.y,
    bx: pb.x,
    by: pb.y,
    len: Math.sqrt((pb.x - pa.x) * (pb.x - pa.x) + (pb.y - pa.y) * (pb.y - pa.y)),
  }
}

export function projectPointToSVG(p: Vec3): ProjectedPoint | null {
  const pj = projectPoint(p)
  if (!pj.visible) return null
  return pj
}
