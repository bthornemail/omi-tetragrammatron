import { describe, expect, it } from 'vitest'
import {
  decodeFromJABCode,
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
    const encoded = encodeToJABCode(new Uint16Array(GOLDEN_S))
    expect(encoded.kind).toBe('debug-matrix')
    expect(encoded.profile).toBe('omi.jabcode.rs.v0')
    expect(encoded.svg).toContain('data-omi-profile="omi.jabcode.rs.v0"')
    expect(decodeFromJABCode(GOLDEN_HEX).requiresAlgebraicValidation).toBe(true)
    expect(decodeFromJABCode(GOLDEN_POINTER).pointer).toBe(GOLDEN_POINTER)
    expect(() => parsePointer('bad')).toThrow()
  })
})
