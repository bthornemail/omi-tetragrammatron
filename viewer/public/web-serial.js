/* Web Serial — IMO Carrier Adapter
 *
 * Carries bytes between browser and ESP32 over USB serial.
 * Supports two modes:
 *   text (default) — newline-delimited OMI text frames
 *   binary          — raw bytes for ESP32 ROM bootloader protocol
 * Mode is switchable at runtime via setBinaryMode().
 *
 * Belongs to IMO (Carrier Authority), not Metatron.
 */

class SerialCarrier {
  constructor(opts) {
    this.port = null;
    this._reader = null;
    this._writer = null;
    this.connected = false;

    this._textBuf = '';
    this._decoder = new TextDecoder();
    this._binaryMode = false;
    this._onBinary = null;

    this.onreceipt = opts?.onreceipt || (() => {});
    this.oncell = opts?.oncell || (() => {});
    this.onring = opts?.onring || (() => {});
    this.ongpio = opts?.ongpio || (() => {});
    this.onframe = opts?.onframe || (() => {});
    this.onerror = opts?.onerror || (() => {});
    this.ondisconnect = opts?.ondisconnect || (() => {});
    this.onstatus = opts?.onstatus || (() => {});
  }

  async connect(baudRate) {
    if (this.connected) throw new Error('Already connected');
    try {
      this.port = await navigator.serial.requestPort();
      await this.port.open({ baudRate: baudRate || 115200 });

      this.portInfo = this.port.getInfo ? this.port.getInfo() : {};

      this._reader = this.port.readable.getReader();
      this._writer = this.port.writable.getWriter();

      this.connected = true;
      this._binaryMode = false;
      this._textBuf = '';

      /* Expose a writer object with a write(Uint8Array) method,
       * used by DeviceInspector and Flasher for binary ROM commands. */
      var self = this;
      this.writer = {
        write: function(data) { return self._writer.write(data); }
      };

      this.onstatus('connected', this.portInfo);
      this._readLoop();
    } catch (e) {
      this.onerror('connect failed: ' + e.message);
      throw e;
    }
  }

  async disconnect() {
    if (!this.connected) return;
    this._binaryMode = false;
    this._onBinary = null;
    try {
      if (this._reader) { await this._reader.cancel(); this._reader = null; }
      if (this._writer) { this._writer = null; }
      if (this.port) {
        await this.port.close();
        this.port = null;
      }
    } catch (e) {
      this.onerror('disconnect: ' + e.message);
    }
    this.connected = false;
    this._textBuf = '';
    this.onstatus('disconnected');
    this.ondisconnect();
  }

  /* ─── Mode switching ─── */

  /* Switch to binary mode. onBinary(chunk) receives each raw Uint8Array.
   * Pass null for onBinary to switch back to text mode. */
  setBinaryMode(enabled, onBinary) {
    this._binaryMode = enabled;
    this._onBinary = onBinary || null;
    this._textBuf = '';
  }

  /* ─── Send (text) ─── */
  async send(line) {
    if (!this._writer) throw new Error('Serial not connected');
    var data = new TextEncoder().encode(line + '\n');
    await this._writer.write(data);
  }

  /* ─── Send (binary) ─── */
  async sendBinary(data) {
    if (!this._writer) throw new Error('Serial not connected');
    await this._writer.write(data);
  }

  /* ─── Read loop ─── */
  async _readLoop() {
    try {
      while (true) {
        var result = await this._reader.read();
        if (result.done) break;
        if (!result.value) continue;

        if (this._binaryMode) {
          if (this._onBinary) this._onBinary(result.value);
        } else {
          this._textBuf += this._decoder.decode(result.value, { stream: true });
          var lines = this._textBuf.split('\n');
          this._textBuf = lines.pop() || '';
          for (var i = 0; i < lines.length; i++) {
            var trimmed = lines[i].trim();
            if (trimmed) this._handleLine(trimmed);
          }
        }
      }
    } catch (e) {
      if (e.name !== 'CancelError') {
        this.onerror('read error: ' + e.message);
      }
    }
    this.connected = false;
    this.onstatus('disconnected');
    this.ondisconnect();
  }

  /* ─── Text line handling ─── */
  _handleLine(line) {
    this.onframe(line);

    if (line.startsWith('{') || line.startsWith('[')) {
      try {
        var obj = JSON.parse(line);
        if (obj.hash && obj.cy !== undefined) this.onreceipt(obj);
        else if (obj.ring || obj.filled !== undefined) this.onring(obj);
        else this.onframe(obj);
      } catch (_) {}
    }

    var cellMatch = line.match(
      /^0x[0-9a-fA-F]{4}(?:-0x[0-9a-fA-F]{4}){2}(?:\/0x[0-9a-fA-F]{4}){4}\?/
    );
    if (cellMatch) {
      this.oncell(line);
      return;
    }

    var gpioMatch = line.match(/^GPIO:\s*(\d+)\s*=\s*(\d+)/i);
    if (gpioMatch) {
      this.ongpio({ pin: parseInt(gpioMatch[1]), value: parseInt(gpioMatch[2]) });
      return;
    }

    var infoMatch = line.match(/^(\w+):\s*(.+)/);
    if (infoMatch) {
      this.onstatus(infoMatch[1] + ': ' + infoMatch[2]);
    }
  }

  /* ─── Convenience send helpers ─── */
  async sendDeclaration(sexpr) { await this.send(sexpr); }
  async sendSync() { await this.send(':sync'); }
  async sendSurface(surface) { await this.send(':surface ' + surface); }
  async sendGPIO(pin, value) { await this.send('GPIO:' + pin + '=' + value); }
}

/* ─── ROM Protocol — shared SLIP/CRC32/command layer ───
 *
 * Pure functions for the ESP32 ROM serial bootloader protocol.
 * Used by both DeviceInspector (read) and Flasher (write).
 * Belongs to IMO (Carrier Authority).
 */

var ROMProtocol = {};

(function() {
  var t = new Uint32Array(256);
  for (var i = 0; i < 256; i++) {
    var c = i;
    for (var j = 0; j < 8; j++) c = (c & 1) ? (0xEDB88320 ^ (c >>> 1)) : (c >>> 1);
    t[i] = c;
  }
  ROMProtocol._crcTable = t;
})();

ROMProtocol.crc32 = function(data) {
  var c = 0xFFFFFFFF;
  var t = ROMProtocol._crcTable;
  for (var i = 0; i < data.length; i++) c = t[(c ^ data[i]) & 0xFF] ^ (c >>> 8);
  return (c ^ 0xFFFFFFFF) >>> 0;
};

ROMProtocol.slipEncode = function(payload) {
  var out = [0xC0];
  for (var i = 0; i < payload.length; i++) {
    var b = payload[i];
    if (b === 0xC0) { out.push(0xDB, 0xDC); }
    else if (b === 0xDB) { out.push(0xDB, 0xDD); }
    else { out.push(b); }
  }
  out.push(0xC0);
  return new Uint8Array(out);
};

ROMProtocol.slipDecode = function(buf) {
  var packets = [];
  var start = -1;
  for (var i = 0; i < buf.length; i++) {
    if (buf[i] === 0xC0) {
      if (start >= 0 && i > start) {
        var raw = buf.slice(start, i);
        var decoded = [];
        var esc = false;
        for (var j = 0; j < raw.length; j++) {
          var b = raw[j];
          if (esc) {
            if (b === 0xDC) decoded.push(0xC0);
            else if (b === 0xDD) decoded.push(0xDB);
            else decoded.push(b);
            esc = false;
          } else if (b === 0xDB) { esc = true; }
          else { decoded.push(b); }
        }
        packets.push(new Uint8Array(decoded));
      }
      start = -1;
    } else if (start < 0) { start = i; }
  }
  return packets;
};

ROMProtocol.buildPacket = function(cmd, payload) {
  var p = payload || new Uint8Array(0);
  var len = p.length;
  var chk = 0xEF;
  if (cmd === 0x02 || cmd === 0x03 || cmd === 0x05 || cmd === 0x07) {
    chk = 0;
    for (var i = 0; i < p.length; i++) chk ^= p[i];
  }
  var hdr = new Uint8Array([0x00, cmd, len & 0xFF, (len >> 8) & 0xFF, chk]);
  var pkt = new Uint8Array(hdr.length + p.length);
  pkt.set(hdr, 0);
  pkt.set(p, hdr.length);
  return pkt;
};

ROMProtocol.parseResponse = function(raw) {
  if (raw.length < 4) return null;
  return {
    dir: raw[0],
    status: raw[1],
    len: raw[2] | (raw[3] << 8),
    value: raw.length > 5 ? raw.slice(5, 5 + (raw[2] | (raw[3] << 8))) : new Uint8Array(0)
  };
};

ROMProtocol.CMD_SYNC = 0x08;
ROMProtocol.CMD_READ_REG = 0x0A;
ROMProtocol.CMD_WRITE_REG = 0x09;
ROMProtocol.CMD_SPI_ATTACH = 0x0D;
ROMProtocol.CMD_FLASH_BEGIN = 0x02;
ROMProtocol.CMD_FLASH_DATA = 0x03;
ROMProtocol.CMD_FLASH_END = 0x04;
ROMProtocol.CMD_SPI_FLASH_MD5 = 0x13;

ROMProtocol.REG_CHIP_ID = 0x40001000;
ROMProtocol.REG_EFUSE_MAC = 0x3FF5E000;
ROMProtocol.REG_EFUSE_CHIP_PKG = 0x6001A004;
ROMProtocol.REG_EFUSE_CHIP_REV = 0x6001A008;
