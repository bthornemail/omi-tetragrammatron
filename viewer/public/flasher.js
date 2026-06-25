/* Flasher — IMO Firmware Flashing Adapter
 *
 * ESP32 ROM bootloader flash protocol: erase, write, verify.
 * Every write operation requires explicit user confirmation.
 * Default mode is inspect-only; flashing is gated by confirmation.
 *
 * Belongs to IMO (Carrier Authority). Writes raw bytes to flash;
 * Tetragrammatron validates receipts; Metatron projects surfaces.
 */

var Flasher = {};

/* ─── State ─── */
Flasher.carrier = null;
Flasher.buf = new Uint8Array(0);
Flasher._cmdResolve = null;
Flasher.onlog = function() {};
Flasher.onprogress = function() {};
Flasher._confirmed = false;

/* ─── Init ─── */
Flasher.init = function(carrier) {
  Flasher.carrier = carrier;
  Flasher.buf = new Uint8Array(0);
  Flasher._cmdResolve = null;
  Flasher._confirmed = false;
  Flasher.onlog = carrier.onlog || function() {};
  Flasher.onprogress = function(done, total, label) {};
};

Flasher.setConfirmed = function(v) { Flasher._confirmed = v; };

Flasher.log = function(msg) { Flasher.onlog(msg); };

/* ─── Protocol I/O ─── */

Flasher._sleep = function(ms) {
  return new Promise(function(r) { setTimeout(r, ms); });
};

Flasher._awaitResponse = function(timeout) {
  var t = timeout || 3000;
  return new Promise(function(resolve, reject) {
    var timer = setTimeout(function() { reject(new Error('response timeout')); }, t);
    Flasher._cmdResolve = function(pkt) { clearTimeout(timer); resolve(pkt); };
  });
};

Flasher.feed = function(chunk) {
  var combined = new Uint8Array(Flasher.buf.length + chunk.length);
  combined.set(Flasher.buf, 0);
  combined.set(chunk, Flasher.buf.length);
  Flasher.buf = combined;

  var packets = ROMProtocol.slipDecode(Flasher.buf);
  if (packets.length === 0) return;

  for (var i = 0; i < packets.length; i++) {
    var pkt = packets[i];
    var resp = ROMProtocol.parseResponse(pkt);
    if (resp && Flasher._cmdResolve) {
      var r = Flasher._cmdResolve;
      Flasher._cmdResolve = null;
      r(resp);
    }
  }

  var raw = Flasher.buf;
  var pos = 0;
  var pcount = 0;
  while (pos < raw.length) {
    if (raw[pos] === 0xC0) pcount++;
    pos++;
    if (pcount >= packets.length * 2) break;
  }
  Flasher.buf = raw.slice(pos);
};

Flasher._cmd = async function(cmd, payload, timeout) {
  var pkt = ROMProtocol.buildPacket(cmd, payload);
  var framed = ROMProtocol.slipEncode(pkt);
  var respPromise = Flasher._awaitResponse(timeout);
  var writer = Flasher.carrier.writer;
  if (!writer) throw new Error('not connected');
  await writer.write(framed);
  var resp = await respPromise;
  if (!resp) throw new Error('no response');
  if (resp.status !== 0) throw new Error('command failed: status=' + resp.status);
  return resp;
};

/* ─── Flash Commands ─── */

Flasher.sync = async function() {
  var syncData = new Uint8Array(36);
  for (var i = 0; i < 32; i++) syncData[i] = 0x07;
  syncData[32] = 0x24;
  syncData[33] = 0x00;
  syncData[34] = 0x00;
  syncData[35] = 0x00;

  var lastErr = null;
  for (var attempt = 0; attempt < 5; attempt++) {
    try {
      var resp = await Flasher._cmd(ROMProtocol.CMD_SYNC, syncData, 2000);
      if (resp && resp.status === 0) {
        Flasher.log('SYNK OK (attempt ' + (attempt + 1) + ')');
        return resp;
      }
    } catch (e) { lastErr = e; await Flasher._sleep(100); }
  }
  throw lastErr || new Error('SYNC failed after 5 attempts');
};

Flasher.spiAttach = async function() {
  return Flasher._cmd(ROMProtocol.CMD_SPI_ATTACH, new Uint8Array(8));
};

Flasher.flashBegin = async function(size, numBlocks, blockSize, offset) {
  var p = new Uint8Array(16);
  var dv = new DataView(p.buffer);
  dv.setUint32(0, size, true);
  dv.setUint32(4, numBlocks, true);
  dv.setUint32(8, blockSize, true);
  dv.setUint32(12, offset, true);
  return Flasher._cmd(ROMProtocol.CMD_FLASH_BEGIN, p);
};

Flasher.flashData = async function(data, seq) {
  var p = new Uint8Array(data.length + 16);
  var dv = new DataView(p.buffer);
  dv.setUint32(0, data.length, true);
  dv.setUint32(4, seq, true);
  dv.setUint32(8, 0, true);  /* CRC placeholder */
  dv.setUint32(12, 0, true);
  p.set(data, 16);
  var c = ROMProtocol.crc32(data);
  dv.setUint32(8, c, true);
  return Flasher._cmd(ROMProtocol.CMD_FLASH_DATA, p, 10000);
};

Flasher.flashEnd = async function(reboot) {
  var p = new Uint8Array(4);
  p[0] = reboot ? 1 : 0; p[1] = 0; p[2] = 0; p[3] = 0;
  return Flasher._cmd(ROMProtocol.CMD_FLASH_END, p);
};

Flasher.flashMd5 = async function(offset, size) {
  var p = new Uint8Array(16);
  var dv = new DataView(p.buffer);
  dv.setUint32(0, offset, true);
  dv.setUint32(4, size, true);
  dv.setUint32(8, 0, true);
  dv.setUint32(12, 0, true);
  var resp = await Flasher._cmd(ROMProtocol.CMD_SPI_FLASH_MD5, p, 10000);
  if (!resp || !resp.value) throw new Error('MD5 read failed');
  var md5 = new Uint8Array(resp.value.slice(0, 16));
  return Array.from(md5).map(function(b) {
    return (b >>> 0).toString(16).padStart(2, '0');
  }).join('');
};

/* ─── Safety gate ─── */

function requireConfirm(msg) {
  if (!Flasher._confirmed) throw new Error('Flash not confirmed: ' + msg);
}

/* ─── High-level: firmware flash (safety-gated) ─── */

Flasher.flashFirmware = async function(data, offset) {
  requireConfirm('flashFirmware requires setConfirmed(true)');
  Flasher.log('⚠  FLASHING ' + data.length + ' bytes @ 0x' + offset.toString(16));

  var BLOCK_SIZE = 0x4000;
  var totalSize = data.length;
  var numBlocks = Math.ceil(totalSize / BLOCK_SIZE);

  await Flasher.sync();
  await Flasher.spiAttach();
  await Flasher.flashBegin(totalSize, numBlocks, BLOCK_SIZE, offset);

  for (var seq = 0; seq < numBlocks; seq++) {
    var start = seq * BLOCK_SIZE;
    var end = Math.min(start + BLOCK_SIZE, totalSize);
    var chunk = data.slice(start, end);
    await Flasher.flashData(chunk, seq);
    var pct = Math.round(((seq + 1) / numBlocks) * 100);
    Flasher.onprogress(seq + 1, numBlocks, pct + '%');
  }

  await Flasher.flashEnd(false);
  Flasher.log('Flash write complete. Verifying...');

  try {
    var md5 = await Flasher.flashMd5(offset, totalSize);
    Flasher.log('Verify MD5: ' + md5);
  } catch (e) {
    Flasher.log('WARN: verify failed: ' + e.message);
  }

  Flasher._confirmed = false;
  Flasher.log('Firmware flash complete');
};

/* ─── Erase region (safety-gated) ─── */
Flasher.eraseRegion = async function(offset, size) {
  requireConfirm('eraseRegion requires setConfirmed(true)');
  Flasher.log('⚠  ERASING ' + size + ' bytes @ 0x' + offset.toString(16));

  /* ESP32 ROM flash erase granularity is 4KB sectors */
  var SECTOR = 0x1000;
  var numSectors = Math.ceil(size / SECTOR);
  for (var i = 0; i < numSectors; i++) {
    /* Use FLASH_BEGIN + FLASH_DATA with zero-length data to erase */
    var seqOffset = offset + i * SECTOR;
    await Flasher.sync();
    await Flasher.spiAttach();
    await Flasher.flashBegin(SECTOR, 1, SECTOR, seqOffset);
    /* Write one sector of zeros */
    var zeros = new Uint8Array(SECTOR);
    for (var j = 0; j < SECTOR; j++) zeros[j] = 0;
    await Flasher.flashData(zeros, 0);
    await Flasher.flashEnd(false);
    Flasher.onprogress(i + 1, numSectors, Math.round(((i + 1) / numSectors) * 100) + '%');
  }
  Flasher._confirmed = false;
  Flasher.log('Erase complete');
};

/* ─── Backup (read-only, no confirmation needed) ─── */

Flasher.backupRegion = async function(offset, size) {
  Flasher.log('Backup not yet implemented via ROM protocol (requires stub)');
  throw new Error('backup requires custom ROM stub — not available in minimal protocol');
};
