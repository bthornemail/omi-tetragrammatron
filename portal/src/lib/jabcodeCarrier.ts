export const CARRIER = 'omi-barcode'
export const PROFILE = 'omi-jabcode'
export const COMPLIANCE = 'debug-only'

export const BSI_8_COLOR_PALETTE = [
  '#000000',
  '#ff00ff',
  '#ffff00',
  '#00ffff',
  '#ff0000',
  '#00ff00',
  '#0000ff',
  '#ffffff',
] as const

export const PALETTE = BSI_8_COLOR_PALETTE

export const OMI_GAUGE_PREHEADER = new Uint8Array([0xff, 0x00, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0xff])

export interface ParsedPointer {
  S: Uint16Array
  prefix: number | null
}

export interface FrameShape {
  ok: boolean
  errors: string[]
  warnings: string[]
  LL: number | null
  NN: number | null
  MM: number | null
}

export interface DebugMatrix {
  width: number
  height: number
  cells: Uint8Array
  palette: readonly string[]
}

export interface DebugCarrier {
  carrier: string
  profile: string
  compliance: typeof COMPLIANCE
  kind: 'debug-matrix'
  S: Uint16Array
  bytes: Uint8Array
  payloadBytes: Uint8Array
  matrix: DebugMatrix
  svg: string
}

export interface DecodedCarrier {
  carrier: string
  profile: string
  compliance: typeof COMPLIANCE
  S: Uint16Array
  bytes: Uint8Array
  pointer: string
  shape: FrameShape
  fields: ReturnType<typeof extractFrameFields> | null
  requiresAlgebraicValidation: true
}

export function parsePointer(text: string): ParsedPointer {
  let s = text.trim()
  let prefix: number | null = null
  if (s.startsWith('omi-')) s = s.slice(4)
  const slashIdx = s.indexOf('/')
  if (slashIdx >= 0) {
    const prefixStr = s.slice(slashIdx + 1)
    prefix = Number.parseInt(prefixStr, 10)
    if (Number.isNaN(prefix)) throw new Error(`invalid prefix: ${prefixStr}`)
    s = s.slice(0, slashIdx)
  }
  const parts = s.split('-')
  if (parts.length !== 8) throw new Error('expected 8 hex segments')
  const S = new Uint16Array(8)
  for (let i = 0; i < 8; i++) {
    const v = Number.parseInt(parts[i], 16)
    if (Number.isNaN(v) || v < 0 || v > 0xffff) throw new Error(`invalid segment at ${i}`)
    S[i] = v
  }
  return { S, prefix }
}

export function formatPointer(S: ArrayLike<number>, prefix: number | null = 48): string {
  if (!S || S.length !== 8) throw new Error('expected uint16[8]')
  const parts: string[] = []
  for (let i = 0; i < 8; i++) parts.push(Number(S[i]).toString(16).padStart(4, '0'))
  return `omi-${parts.join('-')}/${prefix ?? 48}`
}

export function packFrame(S: ArrayLike<number>): Uint8Array {
  if (!S || S.length !== 8) throw new Error('expected uint16[8]')
  const out = new Uint8Array(16)
  for (let i = 0; i < 8; i++) {
    const v = Number(S[i])
    if (!Number.isInteger(v) || v < 0 || v > 0xffff) {
      throw new Error(`segment out of uint16 range at ${i}`)
    }
    out[i * 2] = (v >>> 8) & 0xff
    out[i * 2 + 1] = v & 0xff
  }
  return out
}

export function unpackFrame(bytes: ArrayLike<number>): Uint16Array {
  if (!bytes || bytes.length !== 16) throw new Error('expected 16 bytes')
  const S = new Uint16Array(8)
  for (let i = 0; i < 8; i++) S[i] = ((bytes[i * 2] << 8) | bytes[i * 2 + 1]) & 0xffff
  return S
}

export function validateFrameShape(S: ArrayLike<number>): FrameShape {
  const result: FrameShape = { ok: true, errors: [], warnings: [], LL: null, NN: null, MM: null }
  if (!S || S.length !== 8) {
    result.ok = false
    result.errors.push(`expected uint16[8], got ${S ? S.length : 'null'}`)
    return result
  }
  if (S[1] !== 0x03bf) result.errors.push(`S1 expected 0x03bf, got 0x${S[1].toString(16)}`)
  if (S[6] !== 0x039f) result.errors.push(`S6 expected 0x039f, got 0x${S[6].toString(16)}`)
  if ((S[0] & 0xff) !== 0x00) result.warnings.push('S0 low byte expected 0x00')
  if ((S[3] >>> 8) !== 0x2b) result.warnings.push('S3 high byte expected 0x2b')
  if ((S[4] >>> 8) !== 0x2f) result.warnings.push('S4 high byte expected 0x2f')
  if ((S[7] & 0xff) !== 0xff) result.warnings.push('S7 low byte expected 0xff')
  const LL0 = S[0] >>> 8
  const LL3 = S[3] & 0xff
  const LL4 = S[4] & 0xff
  const LL7 = S[7] >>> 8
  if (!(LL0 === LL3 && LL3 === LL4 && LL4 === LL7)) {
    result.warnings.push(`LL inconsistency: ${LL0}, ${LL3}, ${LL4}, ${LL7}`)
  }
  result.ok = result.errors.length === 0
  if (result.ok) {
    result.LL = LL0
    result.NN = S[2]
    result.MM = S[5]
  }
  return result
}

export function extractFrameFields(S: ArrayLike<number>) {
  return {
    LL: (S[0] >>> 8) & 0xff,
    NN: S[2],
    MM: S[5],
    openOmicron: S[1],
    closeOmicron: S[6],
    prefix: null as number | null,
  }
}

export function encodePayload(S: ArrayLike<number>): Uint8Array {
  return packFrame(S)
}

export function encodeBarcodePayload(S: ArrayLike<number>): Uint8Array {
  const frameBytes = packFrame(S)
  const out = new Uint8Array(OMI_GAUGE_PREHEADER.length + frameBytes.length)
  out.set(OMI_GAUGE_PREHEADER)
  out.set(frameBytes, OMI_GAUGE_PREHEADER.length)
  return out
}

export function decodePayload(bytes: Uint8Array) {
  const S = unpackFrame(bytes)
  const shape = validateFrameShape(S)
  return { S, shape, bytes }
}

function bytesToBits(bytes: Uint8Array): number[] {
  const bits: number[] = []
  for (const byte of bytes) for (let b = 7; b >= 0; b--) bits.push((byte >>> b) & 1)
  return bits
}

function bitsToColorIndices(bits: number[]): number[] {
  const indices: number[] = []
  for (let i = 0; i + 2 < bits.length; i += 3) indices.push((bits[i] << 2) | (bits[i + 1] << 1) | bits[i + 2])
  return indices
}

export function renderDebugMatrix(S: ArrayLike<number>, options: { moduleSize?: number; padding?: number } = {}): DebugCarrier {
  const matrixSize = 21
  const finderSize = 5
  const frameBytes = packFrame(S)
  const payloadBytes = encodeBarcodePayload(S)
  const colorIndices = bitsToColorIndices(bytesToBits(payloadBytes))
  const cells = new Uint8Array(matrixSize * matrixSize)
  cells.fill(255)

  const setCell = (x: number, y: number, val: number) => {
    cells[y * matrixSize + x] = val
  }
  const getCell = (x: number, y: number) => cells[y * matrixSize + x]
  const isFinder = (x: number, y: number) =>
    (x < finderSize && y < finderSize) ||
    (x >= matrixSize - finderSize && y < finderSize) ||
    (x < finderSize && y >= matrixSize - finderSize) ||
    (x >= matrixSize - finderSize && y >= matrixSize - finderSize)
  const drawFinder = (ox: number, oy: number) => {
    for (let fy = 0; fy < finderSize; fy++) {
      for (let fx = 0; fx < finderSize; fx++) {
        const isEdge = fy === 0 || fy === finderSize - 1 || fx === 0 || fx === finderSize - 1
        setCell(ox + fx, oy + fy, isEdge ? 0 : 7)
      }
    }
  }

  drawFinder(0, 0)
  drawFinder(matrixSize - finderSize, 0)
  drawFinder(0, matrixSize - finderSize)
  drawFinder(matrixSize - finderSize, matrixSize - finderSize)

  let ci = 0
  for (let y = 0; y < matrixSize && ci < colorIndices.length; y++) {
    for (let x = 0; x < matrixSize && ci < colorIndices.length; x++) {
      if (!isFinder(x, y) && getCell(x, y) === 255) setCell(x, y, colorIndices[ci++])
    }
  }

  const matrix = { width: matrixSize, height: matrixSize, cells, palette: PALETTE }
  return {
    carrier: CARRIER,
    profile: PROFILE,
    compliance: COMPLIANCE,
    kind: 'debug-matrix',
    S: new Uint16Array(S as ArrayLike<number>),
    bytes: frameBytes,
    payloadBytes,
    matrix,
    svg: matrixToSVG(matrix, options),
  }
}

export function matrixToSVG(matrix: DebugMatrix, options: { moduleSize?: number; padding?: number } = {}): string {
  const ms = options.moduleSize ?? 10
  const pad = options.padding ?? 2
  const tw = matrix.width * ms + pad * 2
  const th = matrix.height * ms + pad * 2
  const rects: string[] = []
  for (let y = 0; y < matrix.height; y++) {
    for (let x = 0; x < matrix.width; x++) {
      const idx = matrix.cells[y * matrix.width + x]
      if (idx === 255) continue
      const color = matrix.palette[idx % matrix.palette.length]
      rects.push(`<rect x="${pad + x * ms}" y="${pad + y * ms}" width="${ms}" height="${ms}" fill="${color}"/>`)
    }
  }
  return `<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 ${tw} ${th}" data-omi-carrier="${CARRIER}" data-omi-profile="${PROFILE}" data-omi-compliance="${COMPLIANCE}">${rects.join('')}</svg>`
}

export function encodeToDebugMatrix(S: ArrayLike<number>, options?: { moduleSize?: number; padding?: number }): DebugCarrier {
  return renderDebugMatrix(S, options)
}

export function decodeFromDebugMatrix(input: Uint8Array | string): DecodedCarrier {
  let bytes: Uint8Array
  let pointer: string | null = null
  if (input instanceof Uint8Array) {
    bytes = input
  } else {
    const s = input.trim()
    if (s.startsWith('omi-')) {
      const parsed = parsePointer(s)
      bytes = packFrame(parsed.S)
      pointer = s
    } else if (/^[0-9a-fA-F]{32}$/.test(s)) {
      bytes = new Uint8Array(16)
      for (let i = 0; i < 16; i++) bytes[i] = Number.parseInt(s.slice(i * 2, i * 2 + 2), 16)
    } else {
      throw new Error('unknown input format: expected pointer or 32-char hex')
    }
  }
  const decoded = decodePayload(bytes)
  return {
    carrier: CARRIER,
    profile: PROFILE,
    compliance: COMPLIANCE,
    S: decoded.S,
    bytes,
    pointer: pointer ?? formatPointer(decoded.S),
    shape: decoded.shape,
    fields: decoded.shape.ok ? extractFrameFields(decoded.S) : null,
    requiresAlgebraicValidation: true,
  }
}

export function encodeToJABCode(): never {
  throw new Error('omi-jabcode requires a standards-compliant BSI TR-03137 encoder before this API can return a symbol')
}

export function decodeFromJABCodeImage(): never {
  throw new Error('omi-jabcode image decoding requires a standards-compliant BSI TR-03137 decoder')
}

export function decodeFromJABCodeMatrix(): never {
  throw new Error('omi-jabcode matrix decoding requires a standards-compliant BSI TR-03137 decoder')
}
