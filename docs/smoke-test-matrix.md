# Smoke-test matrix

This matrix lists quick checks for the current runtime modes. Run from the repository root after building `opencode.bin`.

## Build

| Check | Command | Expected result |
| --- | --- | --- |
| Compile runtime | `gcc -O2 -Wall -Wextra -Wno-misleading-indentation -o opencode.bin opencode.c -lm` | Produces `opencode.bin` |
| Print help | `./opencode.bin --help` | Lists runtime modes |

## Core evaluation

| Check | Command | Expected result |
| --- | --- | --- |
| Evaluate pair | `./opencode.bin --eval '(cons 1 2)'` | Emits accepted receipt |
| Start REPL | `./opencode.bin --repl` | Accepts expressions and `:quit` |
| Run file | `./opencode.bin --run /tmp/omi-inputs.txt` | Processes non-empty, non-comment lines |

## OMI and CPU

| Check | Command | Expected result |
| --- | --- | --- |
| Boot CPU | `./opencode.bin --boot` | Prints boot stages and final CPU state |
| Execute one frame | `./opencode.bin --cpu '<frame>'` | Prints CPU registers and halt state |
| Write seed | `./opencode.bin --seed /tmp/opencode.seed` | Writes packed boot ROM seed |

## Ring and autonomous behavior

| Check | Command | Expected result |
| --- | --- | --- |
| Dump ring | `./opencode.bin --ring` | Prints filled receipt slots |
| Watch ring | `./opencode.bin --watch` | Polls ring file and accepts watcher commands |
| Autonomous mode | `./opencode.bin --auto` | Generates cycles until closure |

## Geometry and rendering

| Check | Command | Expected result |
| --- | --- | --- |
| Geometry constants | `./opencode.bin --geom` | Prints Polybius/Fano/BQF constants |
| Twin summary | `./opencode.bin --twin` | Prints ring folds and frame counts |
| JSON frame | `./opencode.bin --render-frame > /tmp/frame.json` | Writes JSON frame |
| PPM frame | `./opencode.bin --render-ppm > /tmp/frame.ppm` | Writes binary PPM image |

## HTTP viewer

| Check | Command | Expected result |
| --- | --- | --- |
| Start server | `./opencode.bin --serve 8080` | Serves viewer and JSON endpoints |
| Fetch frame | `curl http://127.0.0.1:8080/frame` | Returns JSON frame |
| Fetch ring | `curl http://127.0.0.1:8080/ring` | Returns ring JSON |

## Minimal local validation sequence

```bash
gcc -O2 -Wall -Wextra -Wno-misleading-indentation -o opencode.bin opencode.c -lm
./opencode.bin --help
./opencode.bin --eval '(cons 1 2)'
./opencode.bin --geom
./opencode.bin --boot
./opencode.bin --twin
./opencode.bin --render-frame > /tmp/frame.json
./opencode.bin --render-ppm > /tmp/frame.ppm
```
