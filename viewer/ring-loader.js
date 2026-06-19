/* Ring loader — fetches deterministic state from opencode.bin --serve */

class RingLoader {
  constructor(baseUrl) {
    this.baseUrl = baseUrl || '';
    this.lastFrame = null;
    this.listeners = [];
  }

  onUpdate(fn) { this.listeners.push(fn); }

  async fetchFrame() {
    const res = await fetch(this.baseUrl + '/frame');
    if (!res.ok) throw new Error('HTTP ' + res.status);
    return await res.json();
  }

  async fetchRing() {
    const res = await fetch(this.baseUrl + '/ring');
    if (!res.ok) throw new Error('HTTP ' + res.status);
    return await res.json();
  }

  async pollFrame(intervalMs) {
    while (true) {
      try {
        const frame = await this.fetchFrame();
        if (JSON.stringify(frame) !== JSON.stringify(this.lastFrame)) {
          this.lastFrame = frame;
          for (const fn of this.listeners) fn(frame);
        }
      } catch (e) {
        console.error('Poll error:', e);
      }
      await new Promise(r => setTimeout(r, intervalMs || 1000));
    }
  }

  async pollRing(intervalMs) {
    while (true) {
      try {
        const data = await this.fetchRing();
        for (const fn of this.listeners) fn(data);
      } catch (e) {
        console.error('Poll error:', e);
      }
      await new Promise(r => setTimeout(r, intervalMs || 1000));
    }
  }
}

/* Total time: 0.037s */

