import { describe, expect, it } from 'vitest'
import {
  BSI_8_COLOR_PALETTE,
  decodeFromDebugMatrix,
  decodeFromJABCodeImage,
  encodeBarcodePayload,
  encodeToDebugMatrix,
  encodeToJABCode,
  formatPointer,
  packFrame,
  parsePointer,
  validateFrameShape,
} from './jabcodeCarrier'

const GOLDEN_POINTER = 'omi-0100-03bf-7c00-2b01-2f01-1434-039f-01ff/48'
const GOLDEN_S = [0x0100, 0x03bf, 0x7c00, 0x2b01, 0x2f01, 0x1434, 0x039f, 0x01ff]
const GOLDEN_HEX = '010003bf7c002b012f011434039f01ff'

function hex(bytes: Uint8Array): string {
  return Array.from(bytes, (byte) => byte.toString(16).padStart(2, '0')).join('')
}

describe('jabcodeCarrier debug carrier', () => {
  it('parses and formats OMI pointers', () => {
    const r = parsePointer(GOLDEN_POINTER)
    expect(r.prefix).toBe(48)
    expect(Array.from(r.S)).toEqual(GOLDEN_S)
    expect(formatPointer(new Uint16Array(GOLDEN_S), 48)).toBe(GOLDEN_POINTER)
  })

  it('packs and validates frame shape', () => {
    expect(hex(packFrame(new Uint16Array(GOLDEN_S)))).toBe(GOLDEN_HEX)
    expect(validateFrameShape(new Uint16Array(GOLDEN_S)).ok).toBe(true)
    const bad = new Uint16Array(GOLDEN_S)
    bad[1] = 0xdead
    expect(validateFrameShape(bad).ok).toBe(false)
  })

  it('encodes and decodes deterministic debug matrices', () => {
    const encoded = encodeToDebugMatrix(new Uint16Array(GOLDEN_S))
    expect(encoded.kind).toBe('debug-matrix')
    expect(encoded.carrier).toBe('omi-barcode')
    expect(encoded.profile).toBe('omi-jabcode')
    expect(encoded.compliance).toBe('debug-only')
    expect(encoded.svg).toContain('data-omi-carrier="omi-barcode"')
    expect(encoded.svg).toContain('data-omi-profile="omi-jabcode"')
    expect(encoded.svg).toContain('data-omi-compliance="debug-only"')
    expect(decodeFromDebugMatrix(GOLDEN_HEX).requiresAlgebraicValidation).toBe(true)
    expect(decodeFromDebugMatrix(GOLDEN_POINTER).pointer).toBe(GOLDEN_POINTER)
    expect(() => parsePointer('bad')).toThrow()
  })

  it('uses barcode payload and reserves real JABCode APIs', () => {
    expect(Array.from(BSI_8_COLOR_PALETTE)).toEqual([
      '#000000',
      '#ff00ff',
      '#ffff00',
      '#00ffff',
      '#ff0000',
      '#00ff00',
      '#0000ff',
      '#ffffff',
    ])
    expect(hex(encodeBarcodePayload(new Uint16Array(GOLDEN_S)))).toBe(`ff001c1d1e1f20ff${GOLDEN_HEX}`)
    expect(() => encodeToJABCode()).toThrow(/BSI TR-03137/)
    expect(() => decodeFromJABCodeImage()).toThrow(/BSI TR-03137/)
  })
})
