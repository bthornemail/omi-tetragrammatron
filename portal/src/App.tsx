import { useEffect, useMemo, useState } from 'react'
import {
  decodeFromDebugMatrix,
  encodeToDebugMatrix,
  formatPointer,
  parsePointer,
} from './lib/jabcodeCarrier'
import {
  cascadeSlotToCSSVars,
  receiptPairToSurface,
  receiptToCascadeSlot,
  renderGenailleSVG,
  type ReceiptLike,
} from './lib/receiptSurfaceCascade'
import { receiptPairToGnomonicLine, receiptToSpherePoint } from './lib/gnomonicSurface'
import './App.css'

const GOLDEN_POINTER = 'omi-0100-03bf-7c00-2b01-2f01-1434-039f-01ff/48'
const API_BASE = import.meta.env.VITE_API_URL ?? ''

const DEMO_RECEIPTS: ReceiptLike[] = [
  { angle: '45', centroid: { stop_metric: 2.4 } },
  { angle: '135', centroid: { stop_metric: 5.1 } },
  { angle: '270', centroid: { stop_metric: 7.2 } },
]

interface RuntimeFrame {
  ring?: { filled?: number; xor?: string; sum?: string; rot?: string }
  receipts?: unknown[]
  [key: string]: unknown
}

async function fetchJson<T>(path: string): Promise<T | null> {
  try {
    const res = await fetch(`${API_BASE}${path}`)
    if (!res.ok) return null
    return (await res.json()) as T
  } catch {
    return null
  }
}

function App() {
  const [frame, setFrame] = useState<RuntimeFrame | null>(null)
  const [ring, setRing] = useState<unknown | null>(null)
  const [status, setStatus] = useState('static demo')
  const [pointer, setPointer] = useState(GOLDEN_POINTER)

  useEffect(() => {
    let alive = true
    async function loadRuntime() {
      const [nextFrame, nextRing] = await Promise.all([
        fetchJson<RuntimeFrame>('/frame'),
        fetchJson<unknown>('/ring'),
      ])
      if (!alive) return
      setFrame(nextFrame)
      setRing(nextRing)
      setStatus(nextFrame || nextRing ? 'runtime connected' : 'static demo')
    }
    void loadRuntime()
    return () => {
      alive = false
    }
  }, [])

  const carrier = useMemo(() => {
    try {
      const parsed = parsePointer(pointer)
      return { ok: true as const, parsed, encoded: encodeToDebugMatrix(parsed.S), decoded: decodeFromDebugMatrix(pointer) }
    } catch (error) {
      return { ok: false as const, error: error instanceof Error ? error.message : String(error) }
    }
  }, [pointer])

  const receipts = DEMO_RECEIPTS
  const currentSlot = receiptToCascadeSlot(receipts[1])
  const surface = receiptPairToSurface(receipts[0], receipts[1])
  const gnomonic = receiptPairToGnomonicLine(receipts[0], receipts[1])
  const spherePoint = receiptToSpherePoint(receipts[1])
  const cssVars = cascadeSlotToCSSVars(currentSlot)
  const ringSummary = frame?.ring

  return (
    <main className="portal-shell">
      <header className="topbar">
        <div>
          <p className="eyebrow">OMI portal</p>
          <h1>Omi-Ring Accepted-State Surface</h1>
        </div>
        <span className={status === 'runtime connected' ? 'status live' : 'status'}>{status}</span>
      </header>

      <section className="runtime-grid">
        <article className="panel primary">
          <div className="panel-head">
            <h2>Runtime Frame</h2>
            <code>core/omicron.bin --serve</code>
          </div>
          <div className="metrics">
            <div>
              <span>filled</span>
              <strong>{ringSummary?.filled ?? receipts.length}</strong>
            </div>
            <div>
              <span>xor</span>
              <strong>{ringSummary?.xor ?? 'demo'}</strong>
            </div>
            <div>
              <span>ring</span>
              <strong>{ring ? 'loaded' : 'demo'}</strong>
            </div>
          </div>
          <pre>{JSON.stringify(frame ?? { mode: 'static demo', receipts }, null, 2)}</pre>
        </article>

        <article className="panel">
          <div className="panel-head">
            <h2>Barcode Carrier</h2>
            <code>debug-only</code>
          </div>
          <label className="field">
            <span>OMI pointer</span>
            <input value={pointer} onChange={(event) => setPointer(event.target.value)} />
          </label>
          {carrier.ok ? (
            <>
              <div className="carrier-preview" dangerouslySetInnerHTML={{ __html: carrier.encoded.svg }} />
              <dl>
                <dt>formatted</dt>
                <dd>{formatPointer(carrier.parsed.S, carrier.parsed.prefix)}</dd>
                <dt>shape</dt>
                <dd>{carrier.decoded.shape.ok ? 'recognized, validation required' : 'rejected shape'}</dd>
                <dt>carrier</dt>
                <dd>
                  {carrier.encoded.carrier} / {carrier.encoded.profile}
                </dd>
              </dl>
            </>
          ) : (
            <p className="error">{carrier.error}</p>
          )}
        </article>

        <article className="panel">
          <div className="panel-head">
            <h2>Accepted-State Surface</h2>
            <code>{surface.detClass}</code>
          </div>
          <div className="surface-vars">
            {Object.entries(cssVars).map(([key, value]) => (
              <span key={key}>
                {key}: {value}
              </span>
            ))}
          </div>
          <div className="svg-frame" dangerouslySetInnerHTML={{ __html: renderGenailleSVG(surface.lane, surface.band, surface.angleDeg, surface) }} />
        </article>

        <article className="panel">
          <div className="panel-head">
            <h2>Gnomonic Projection</h2>
            <code>{gnomonic.visible ? 'visible' : 'horizon'}</code>
          </div>
          <svg className="gnomonic" viewBox="-140 -140 280 280" role="img" aria-label="Gnomonic projection">
            <circle cx="0" cy="0" r="120" />
            <line x1="-130" y1="0" x2="130" y2="0" />
            <line x1="0" y1="-130" x2="0" y2="130" />
            {gnomonic.visible && (
              <line x1={gnomonic.ax} y1={gnomonic.ay} x2={gnomonic.bx} y2={gnomonic.by} className="active-line" />
            )}
          </svg>
          <dl>
            <dt>sphere</dt>
            <dd>
              {spherePoint.x.toFixed(3)}, {spherePoint.y.toFixed(3)}, {spherePoint.z.toFixed(3)}
            </dd>
            <dt>length</dt>
            <dd>{gnomonic.len.toFixed(3)}</dd>
          </dl>
        </article>
      </section>
    </main>
  )
}

export default App
