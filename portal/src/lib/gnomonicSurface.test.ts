import { describe, expect, it } from 'vitest'
import {
  normalize3,
  projectPoint,
  receiptPairToGnomonicLine,
  receiptToSpherePoint,
} from './gnomonicSurface'

describe('gnomonicSurface', () => {
  it('normalizes vectors and hides horizon points', () => {
    const n = normalize3({ x: 1, y: 0, z: 0 })
    expect(Math.abs(Math.sqrt(n.x * n.x + n.y * n.y + n.z * n.z) - 1)).toBeLessThan(1e-9)
    expect(projectPoint({ x: 1, y: 0, z: 0 }).visible).toBe(false)
  })

  it('projects receipt points deterministically', () => {
    const p = receiptToSpherePoint({ angle: '0', centroid: { stop_metric: 10 } })
    expect(p.x).toBeGreaterThan(0.99)
    const line = receiptPairToGnomonicLine(
      { angle: '0', centroid: { stop_metric: 5 } },
      { angle: '90', centroid: { stop_metric: 5 } },
    )
    expect(line.visible).toBe(true)
    expect(line.len).toBeGreaterThan(0)
  })

  it('keeps projection scaling stable', () => {
    const scaled = projectPoint({ x: 0.5, y: 0, z: 0.8660254 })
    expect(Math.abs(scaled.x - 69.28)).toBeLessThan(0.1)
  })
})
