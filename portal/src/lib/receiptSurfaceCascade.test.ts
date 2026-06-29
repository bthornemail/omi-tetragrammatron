import { describe, expect, it } from 'vitest'
import {
  determinant,
  determinantClass,
  meanLadder,
  mediant,
  receiptPairToSurface,
  receiptToRational,
  renderGenailleSVG,
} from './receiptSurfaceCascade'

describe('receiptSurfaceCascade', () => {
  it('converts accepted-state records to rational positive forms', () => {
    const rational = receiptToRational({ angle: '90', centroid: { stop_metric: 5 } })
    expect(rational.a).toBeGreaterThan(0)
    expect(rational.b).toBeGreaterThan(0)
    expect(rational.value).toBeGreaterThan(0)
  })

  it('keeps mediant, determinant, and class behavior stable', () => {
    expect(mediant({ a: 1, b: 2, value: 0.5 }, { a: 1, b: 3, value: 1 / 3 })).toMatchObject({ a: 2, b: 5 })
    expect(determinant({ a: 1, b: 2 }, { a: 2, b: 3 })).toBe(1)
    expect(determinantClass(0)).toBe('equivalent')
    expect(determinantClass(-1)).toBe('adjacent')
    expect(determinantClass(5)).toBe('wide')
  })

  it('orders the mean ladder and renders Genaille SVG', () => {
    const means = meanLadder(1, 2)
    expect(means.hm).toBeLessThanOrEqual(means.gm + 1e-12)
    expect(means.gm).toBeLessThanOrEqual(means.am + 1e-12)
    expect(means.am).toBeLessThanOrEqual(means.qm + 1e-12)
    const surface = receiptPairToSurface(
      { angle: '0', centroid: { stop_metric: 1 } },
      { angle: '180', centroid: { stop_metric: 5 } },
    )
    expect(surface.lane).toBeGreaterThanOrEqual(0)
    expect(surface.lane).toBeLessThanOrEqual(9)
    expect(surface.band).toBeGreaterThanOrEqual(0)
    expect(surface.band).toBeLessThanOrEqual(9)
    expect(renderGenailleSVG(surface.lane, surface.band, surface.angleDeg, surface)).toContain('<svg')
  })
})
