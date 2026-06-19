/* Twin Viewer — WebGL 3D visualization of the digital twin universe */

/* Polybius grid: 5×5, cell centers at (cx, cy) in XY plane */
const POLYBIUS_CELLS = [];
for (let r = 1; r <= 5; r++) {
  for (let c = 1; c <= 5; c++) {
    POLYBIUS_CELLS.push({ row: r, col: c, x: (c - 3) * 1.2, z: (r - 3) * 1.2 });
  }
}

/* Fano plane: 7 points in 3D (projective plane embedded in R³) */
const FANO_POINTS_3D = [
  {x: 0, y: 0, z: 2.5},    /* 0 */
  {x: 2.2, y: 0, z: 1.2},  /* 1 */
  {x: -2.2, y: 0, z: 1.2}, /* 2 */
  {x: 1.4, y: 1.8, z: -0.8}, /* 3 */
  {x: -1.4, y: 1.8, z: -0.8},/* 4 */
  {x: 1.4, y: -1.8, z: -0.8},/* 5 */
  {x: -1.4, y: -1.8, z: -0.8},/* 6 */
];

class TwinViewer {
  constructor(container) {
    this.container = container;
    this.scene = new THREE.Scene();
    this.scene.background = new THREE.Color(0x0a0a14);

    this.camera = new THREE.PerspectiveCamera(45, container.clientWidth / container.clientHeight, 0.1, 100);
    this.camera.position.set(6, 5, 8);
    this.camera.lookAt(0, 0, 0);

    this.renderer = new THREE.WebGLRenderer({ antialias: true });
    this.renderer.setSize(container.clientWidth, container.clientHeight);
    this.renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
    container.appendChild(this.renderer.domElement);

    this.controls = new THREE.OrbitControls(this.camera, this.renderer.domElement);
    this.controls.enableDamping = true;
    this.controls.dampingFactor = 0.05;

    /* Lights */
    const ambient = new THREE.AmbientLight(0x404060);
    this.scene.add(ambient);
    const dir = new THREE.DirectionalLight(0xffffff, 1);
    dir.position.set(5, 10, 7);
    this.scene.add(dir);
    const dir2 = new THREE.DirectionalLight(0x4488ff, 0.5);
    dir2.position.set(-5, -3, -7);
    this.scene.add(dir2);

    /* Objects */
    this.polybiusGroup = new THREE.Group();
    this.fanoGroup = new THREE.Group();
    this.hopfGroup = new THREE.Group();
    this.spiralGroup = new THREE.Group();
    this.infoSprites = [];
    this.scene.add(this.polybiusGroup);
    this.scene.add(this.fanoGroup);
    this.scene.add(this.hopfGroup);
    this.scene.add(this.spiralGroup);

    this.animTime = 0;
    this.frameData = null;

    this.buildPolybiusGrid();
    this.buildFanoPlane();
    this.buildSpiralPath();
    this.buildAxes();
    this.buildSmithChart();

    window.addEventListener('resize', () => this.onResize());
  }

  buildAxes() {
    const axes = new THREE.AxesHelper(4);
    this.scene.add(axes);
  }

  buildPolybiusGrid() {
    const colors = {
      'US': 0xc83c3c, 'GS': 0x3cc83c, 'RS': 0x3c3cc8, 'FS': 0x3cc8c8
    };
    for (const cell of POLYBIUS_CELLS) {
      const geo = new THREE.PlaneGeometry(1.0, 1.0);
      const mat = new THREE.MeshStandardMaterial({
        color: 0x1a1a2e, transparent: true, opacity: 0.8,
        side: THREE.DoubleSide
      });
      const mesh = new THREE.Mesh(geo, mat);
      mesh.position.set(cell.x, 0, cell.z);
      mesh.rotation.x = -Math.PI / 2;
      mesh.userData = { row: cell.row, col: cell.col, baseMat: mat };
      this.polybiusGroup.add(mesh);

      /* Label */
      const canvas = document.createElement('canvas');
      canvas.width = 64; canvas.height = 64;
      const ctx = canvas.getContext('2d');
      ctx.fillStyle = '#ffffff';
      ctx.font = '24px monospace';
      ctx.textAlign = 'center';
      ctx.textBaseline = 'middle';
      ctx.fillText(`(${cell.row},${cell.col})`, 32, 32);
      const tex = new THREE.CanvasTexture(canvas);
      const spriteMat = new THREE.SpriteMaterial({ map: tex, depthTest: false });
      const sprite = new THREE.Sprite(spriteMat);
      sprite.position.set(cell.x, 0.05, cell.z);
      sprite.scale.set(0.8, 0.8, 1);
      this.polybiusGroup.add(sprite);

      /* Edge highlight */
      const edgeGeo = new THREE.EdgesGeometry(geo);
      const edgeMat = new THREE.LineBasicMaterial({ color: 0x444466 });
      const edgeLine = new THREE.LineSegments(edgeGeo, edgeMat);
      edgeLine.position.copy(mesh.position);
      edgeLine.rotation.copy(mesh.rotation);
      this.polybiusGroup.add(edgeLine);
    }
  }

  updatePolybiusGrid(frameCounts) {
    const colors = {
      'US': new THREE.Color(0xc83c3c),
      'GS': new THREE.Color(0x3cc83c),
      'RS': new THREE.Color(0x3c3cc8),
      'FS': new THREE.Color(0x3cc8c8)
    };
    const total = (frameCounts.US || 0) + (frameCounts.GS || 0) + (frameCounts.RS || 0) + (frameCounts.FS || 0) || 1;
    const counts = [frameCounts.US || 0, frameCounts.GS || 0, frameCounts.RS || 0, frameCounts.FS || 0];
    const names = ['US','GS','RS','FS'];

    this.polybiusGroup.children.forEach(child => {
      if (child.type === 'Mesh') {
        const r = child.userData.row;
        const c = child.userData.col;
        const idx = (r - 2) * 4 + (c - 2);
        if (idx >= 0 && idx < 16) {
          /* Distribute counts across cells */
          const fi = idx % 4;
          const frac = counts[fi] / total;
          const col = colors[names[fi]].clone().multiplyScalar(0.3 + 0.7 * frac);
          child.material.color.copy(col);
          child.material.opacity = 0.5 + 0.5 * frac;
        }
      }
    });
  }

  buildFanoPlane() {
    /* Points */
    const pointColors = [0xff4444, 0x44ff44, 0x4444ff, 0xffff44, 0xff44ff, 0x44ffff, 0xffffff];
    for (let i = 0; i < 7; i++) {
      const p = FANO_POINTS_3D[i];
      const geo = new THREE.SphereGeometry(0.15, 12, 12);
      const mat = new THREE.MeshStandardMaterial({
        color: pointColors[i], emissive: pointColors[i], emissiveIntensity: 0.3
      });
      const mesh = new THREE.Mesh(geo, mat);
      mesh.position.set(p.x, p.y, p.z);
      mesh.userData = { fanoIdx: i };
      this.fanoGroup.add(mesh);

      /* Label */
      const canvas = document.createElement('canvas');
      canvas.width = 32; canvas.height = 32;
      const ctx = canvas.getContext('2d');
      ctx.fillStyle = '#ffffff';
      ctx.font = '16px monospace';
      ctx.textAlign = 'center';
      ctx.textBaseline = 'middle';
      ctx.fillText(`${i}`, 16, 16);
      const tex = new THREE.CanvasTexture(canvas);
      const spriteMat = new THREE.SpriteMaterial({ map: tex, depthTest: false });
      const sprite = new THREE.Sprite(spriteMat);
      sprite.position.set(p.x, p.y + 0.3, p.z);
      sprite.scale.set(0.5, 0.5, 1);
      this.fanoGroup.add(sprite);
    }

    /* Lines (edges) */
    for (let li = 0; li < 7; li++) {
      const line = FANO_LINES[li];
      for (let j = 0; j < 3; j++) {
        const p1 = FANO_POINTS_3D[line[j]];
        const p2 = FANO_POINTS_3D[line[(j + 1) % 3]];
        const points = [
          new THREE.Vector3(p1.x, p1.y, p1.z),
          new THREE.Vector3(p2.x, p2.y, p2.z)
        ];
        const geo = new THREE.BufferGeometry().setFromPoints(points);
        const mat = new THREE.LineBasicMaterial({
          color: 0x666688, transparent: true, opacity: 0.4
        });
        const lineObj = new THREE.Line(geo, mat);
        this.fanoGroup.add(lineObj);
      }
    }
  }

  buildSpiralPath() {
    /* 5040-slot spiral helix */
    const total = 5040;
    const points = [];
    for (let i = 0; i < total; i += 20) {
      const t = (i / total) * Math.PI * 8;
      const radius = 2.0 + (i / total) * 1.5;
      const x = Math.cos(t) * radius;
      const z = Math.sin(t) * radius;
      const y = (i / total) * 3.0 - 1.5;
      points.push(new THREE.Vector3(x, y, z));
    }
    const geo = new THREE.BufferGeometry().setFromPoints(points);
    const mat = new THREE.LineBasicMaterial({
      color: 0x334466, transparent: true, opacity: 0.3
    });
    const spiral = new THREE.Line(geo, mat);
    this.spiralGroup.add(spiral);
  }

  updateSpiral(receipts) {
    /* Remove old dots */
    const toRemove = [];
    this.spiralGroup.children.forEach(child => {
      if (child.userData && child.userData.isDot) toRemove.push(child);
    });
    for (const c of toRemove) {
      this.spiralGroup.remove(c);
      c.geometry.dispose();
      c.material.dispose();
    }

    const total = 5040;
    const frameColors = [0xc83c3c, 0x3cc83c, 0x3c3cc8, 0x3cc8c8];

    for (const r of (receipts || [])) {
      const slot = r.twin ? r.twin.slot : 0;
      if (slot >= total) continue;
      const t = (slot / total) * Math.PI * 8;
      const radius = 2.0 + (slot / total) * 1.5;
      const x = Math.cos(t) * radius;
      const z = Math.sin(t) * radius;
      const y = (slot / total) * 3.0 - 1.5;

      const fi = r.twin ? (r.twin.frame === 'US' ? 0 : r.twin.frame === 'GS' ? 1 : r.twin.frame === 'RS' ? 2 : 3) : 0;
      const color = frameColors[fi];

      const geo = new THREE.SphereGeometry(0.04, 6, 6);
      const mat = new THREE.MeshStandardMaterial({
        color: color, emissive: color, emissiveIntensity: 0.2
      });
      const mesh = new THREE.Mesh(geo, mat);
      mesh.position.set(x, y, z);
      mesh.userData = { isDot: true };
      this.spiralGroup.add(mesh);
    }
  }

  updateHopfArrows(receipts) {
    /* Remove old arrows */
    const toRemove = [];
    this.hopfGroup.children.forEach(child => {
      if (child.userData && child.userData.isArrow) toRemove.push(child);
    });
    for (const c of toRemove) {
      this.hopfGroup.remove(c);
      if (c.geometry) c.geometry.dispose();
      if (c.material) c.material.dispose();
    }

    const maxArrows = 100;
    const stride = Math.max(1, Math.floor((receipts || []).length / maxArrows));
    for (let i = 0; i < (receipts || []).length; i += stride) {
      const r = receipts[i];
      if (!r.twin) continue;
      const hopf = r.twin.hopf || [0, 0, 1];
      const dir = new THREE.Vector3(hopf[0], hopf[1], hopf[2]);
      if (dir.length() < 0.001) continue;
      dir.normalize().multiplyScalar(0.5);

      const origin = new THREE.Vector3(
        (r.twin.cell ? r.twin.cell[1] - 3 : 0) * 0.5,
        0.5,
        (r.twin.cell ? r.twin.cell[0] - 3 : 0) * 0.5
      );

      const arrow = new THREE.ArrowHelper(dir, origin, 0.5, 0x88ccff, 0.15, 0.1);
      arrow.userData = { isArrow: true };
      this.hopfGroup.add(arrow);
    }
  }

  updateInfo(frameData) {
    const info = document.getElementById('twin-info');
    if (!info) return;
    if (!frameData || !frameData.twin) {
      info.innerHTML = '<div class="info-loading">Waiting for ring data...</div>';
      return;
    }
    const t = frameData.twin;
    const ring = t.ring || {};
    const summary = t.summary || {};
    const fc = summary.frame_counts || [];
    const hf = summary.hopf_flux || [0, 0, 0];

    let fcHtml = '';
    for (const f of fc) {
      fcHtml += `<span class="frame-${f.name.toLowerCase()}">${f.name}: ${f.count}</span> `;
    }

    info.innerHTML = `
      <div class="info-grid">
        <div><b>Cycle</b> ${t.cycle || '?'}</div>
        <div><b>Filled</b> ${summary.filled || 0} / ${ring.size || 5040}</div>
        <div><b>XOR</b> 0x${ring.xor || '0000'}</div>
        <div><b>SUM</b> 0x${ring.sum || '0000'}</div>
        <div><b>ROT</b> 0x${ring.rot || '0000'}</div>
        <div><b>Hopf flux</b> (${hf[0]}, ${hf[1]}, ${hf[2]})</div>
        <div><b>Frames</b> ${fcHtml}</div>
        <div><b>Receipts</b> ${(t.receipts || []).length}</div>
      </div>
    `;
  }

  update(frameData) {
    this.frameData = frameData;
    if (!frameData || !frameData.twin) return;

    const t = frameData.twin;
    const summary = t.summary || {};
    const receipts = t.receipts || [];

    /* Convert frame_counts to dict */
    const fcounts = {};
    for (const f of (summary.frame_counts || [])) {
      fcounts[f.name] = f.count;
    }
    this.updatePolybiusGrid(fcounts);
    this.updateSpiral(receipts);
    this.updateHopfArrows(receipts);
    this.updateSmithChart(receipts);
    this.updateInfo(frameData);

    /* Ring occupancy bar */
    const bar = document.getElementById('ring-bar');
    if (bar) {
      const filled = summary.filled || 0;
      const pct = Math.min(100, (filled / 5040) * 100);
      bar.style.width = pct + '%';
      bar.textContent = filled + ' / 5040';
    }
  }

  buildSmithChart() {
    this.smithCanvas = document.getElementById('smith-canvas');
    this.smithTooltip = document.getElementById('smith-tooltip');
    if (!this.smithCanvas) return;
    this.smithCtx = this.smithCanvas.getContext('2d');
    const S = 240;
    this.smithScale = S;
    this.smithCx = this.smithCanvas.width / 2;
    this.smithCy = this.smithCanvas.height / 2;

    const ctx = this.smithCtx;
    ctx.clearRect(0, 0, this.smithCanvas.width, this.smithCanvas.height);

    /* Outer circle */
    ctx.strokeStyle = '#444466';
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.arc(this.smithCx, this.smithCy, S, 0, 2 * Math.PI);
    ctx.stroke();

    /* Grid clipped to outer circle */
    ctx.save();
    ctx.beginPath();
    ctx.arc(this.smithCx, this.smithCy, S, 0, 2 * Math.PI);
    ctx.clip();

    ctx.strokeStyle = '#333355';
    ctx.lineWidth = 0.5;

    /* Constant resistance circles: r = 0.5, 1, 2, 5 */
    const rs = [0.5, 1, 2, 5];
    for (const r of rs) {
      const cx = this.smithCx + (r / (1 + r)) * S;
      const rad = (1 / (1 + r)) * S;
      ctx.beginPath();
      ctx.arc(cx, this.smithCy, rad, 0, 2 * Math.PI);
      ctx.stroke();
    }

    /* Constant reactance arcs (full circles clipped): x = ±0.5, ±1, ±2, ±5 */
    const xs = [0.5, 1, 2, 5];
    for (const x of xs) {
      const cy = this.smithCy - (1 / x) * S;
      const rad = (1 / x) * S;
      ctx.beginPath();
      ctx.arc(this.smithCx + S, cy, rad, 0, 2 * Math.PI);
      ctx.stroke();
      const cy2 = this.smithCy + (1 / x) * S;
      ctx.beginPath();
      ctx.arc(this.smithCx + S, cy2, rad, 0, 2 * Math.PI);
      ctx.stroke();
    }
    ctx.restore();

    /* Center crosshair */
    ctx.strokeStyle = '#ffffff';
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(this.smithCx - 6, this.smithCy);
    ctx.lineTo(this.smithCx + 6, this.smithCy);
    ctx.moveTo(this.smithCx, this.smithCy - 6);
    ctx.lineTo(this.smithCx, this.smithCy + 6);
    ctx.stroke();
    ctx.fillStyle = '#ffffff';
    ctx.beginPath();
    ctx.arc(this.smithCx, this.smithCy, 3, 0, 2 * Math.PI);
    ctx.fill();

    this.smithPoints = [];
    this.smithHoverIdx = -1;

    this.smithCanvas.addEventListener('mousemove', (e) => this.onSmithHover(e));
    this.smithCanvas.addEventListener('mouseleave', () => {
      this.smithHoverIdx = -1;
      this.smithTooltip.style.display = 'none';
      this.drawSmithPoints();
    });
  }

  onSmithHover(e) {
    if (!this.smithPoints || !this.smithPoints.length) return;
    const rect = this.smithCanvas.getBoundingClientRect();
    const mx = e.clientX - rect.left;
    const my = e.clientY - rect.top;
    const scaleX = this.smithCanvas.width / rect.width;
    const scaleY = this.smithCanvas.height / rect.height;
    const cx = mx * scaleX;
    const cy = my * scaleY;

    let closest = -1;
    let minDist = 20; /* hover radius in canvas pixels */
    for (let i = 0; i < this.smithPoints.length; i++) {
      const p = this.smithPoints[i];
      const d = Math.hypot(cx - p.x, cy - p.y);
      if (d < minDist) { minDist = d; closest = i; }
    }
    if (closest !== this.smithHoverIdx) {
      this.smithHoverIdx = closest;
      this.drawSmithPoints();
      if (closest >= 0) {
        const p = this.smithPoints[closest];
        this.smithTooltip.style.display = 'block';
        this.smithTooltip.style.left = (e.clientX + 12) + 'px';
        this.smithTooltip.style.top = (e.clientY - 10) + 'px';
        const fi = ['US','GS','RS','FS'][p.fi] || '?';
        this.smithTooltip.innerHTML =
          `<b>${fi}</b> cy=${p.cy} slot=${p.slot} ` +
          `Γ=(${p.gr.toFixed(3)},${p.gi.toFixed(3)}) ` +
          `z=(${p.zr.toFixed(2)},${p.zi.toFixed(2)}) ` +
          `ρ=${p.rho.toFixed(3)} θ=${(p.theta * 180 / Math.PI).toFixed(1)}°`;
      } else {
        this.smithTooltip.style.display = 'none';
      }
    }
  }

  drawSmithPoints() {
    const ctx = this.smithCtx;
    const scale = this.smithScale;
    const cx = this.smithCx;
    const cy = this.smithCy;
    const colors = ['#ff4444','#44ff44','#4444ff','#44ffff'];
    if (!ctx) return;

    /* Redraw grid background (simple clear + static re-draw would be expensive;
       just clear and redraw the whole thing. For efficiency, draw on overlay canvas. */
    /* Clear bottom layer — but we don't want to erase the grid! Use a second canvas for points. */
    /* Instead, draw points directly over the grid and use copy for hover later. */
    ctx.save();
    ctx.beginPath();
    ctx.arc(cx, cy, scale, 0, 2 * Math.PI);
    ctx.clip();

    /* Draw all points */
    for (let i = 0; i < this.smithPoints.length; i++) {
      const p = this.smithPoints[i];
      const px = cx + p.gr * scale;
      const py = cy - p.gi * scale; /* SVG y-flip */
      const isHover = (i === this.smithHoverIdx);
      const radius = isHover ? 5 : 2.5;
      ctx.beginPath();
      ctx.arc(px, py, radius, 0, 2 * Math.PI);
      ctx.fillStyle = colors[p.fi] || '#888';
      ctx.globalAlpha = isHover ? 1.0 : 0.7;
      ctx.fill();
      if (isHover) {
        ctx.strokeStyle = '#ffffff';
        ctx.lineWidth = 1.5;
        ctx.stroke();
      }
    }
    ctx.restore();
    ctx.globalAlpha = 1.0;
  }

  updateSmithChart(receipts) {
    if (!this.smithCtx) return;
    this.smithPoints = [];
    for (const r of (receipts || [])) {
      const sm = r.twin && r.twin.smith;
      if (!sm) continue;
      this.smithPoints.push({
        gr: sm.gamma[0], gi: sm.gamma[1],
        zr: sm.z[0], zi: sm.z[1],
        rho: sm.rho, theta: sm.theta,
        fi: r.twin.frame === 'US' ? 0 : r.twin.frame === 'GS' ? 1 : r.twin.frame === 'RS' ? 2 : 3,
        cy: r.cy, slot: r.twin.slot
      });
    }
    this.drawSmithPoints();
  }

  animate() {
    this.animTime += 0.01;
    this.controls.update();
    this.renderer.render(this.scene, this.camera);
    requestAnimationFrame(() => this.animate());
    this.renderer.render(this.scene, this.camera);
  }

  onResize() {
    const w = this.container.clientWidth;
    const h = this.container.clientHeight;
    this.camera.aspect = w / h;
    this.camera.updateProjectionMatrix();
    this.renderer.setSize(w, h);
  }
}

/* Total time: 0.037s */
