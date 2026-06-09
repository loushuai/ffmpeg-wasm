# FFmpeg WASM

Compile [FFmpeg](https://ffmpeg.org/) to WebAssembly using [Emscripten](https://emscripten.org/), with a browser-based demo.

## Overview

This project cross-compiles FFmpeg's **H.264** and **AAC** decoders to WebAssembly, enabling client-side audio/video decoding directly in the browser — no server or native plugin required.

The demo app verifies that the compiled WASM module works correctly by testing decoder instantiation, codec context allocation, and packet/frame management against the FFmpeg C API.

## Project Structure

```
.
├── build.sh          # One-shot build: SDK → FFmpeg → demo → serve
├── config.sh         # FFmpeg configure flags (wasm32 cross-compile)
├── demo/
│   ├── demo.c        # C test harness: version info, H.264/AAC decoder tests
│   ├── build.sh      # Emscripten link step for the demo
│   └── index.html    # Browser UI with per-test buttons
└── README.md
```

## Prerequisites

- **Python 3** (for the HTTP server that hosts the demo)
- **Git** (to clone emsdk and FFmpeg sources)
- A Unix-like shell (Linux, macOS, or WSL on Windows)

Everything else (Emscripten SDK, FFmpeg source) is fetched automatically by the build script.

## Quick Start

```bash
./build.sh
```

This single command:

1. Clones and installs the **Emscripten SDK** (if not already present)
2. Clones the **FFmpeg** source (if not already present)
3. Configures FFmpeg for `wasm32` with H.264 and AAC decoder support
4. Compiles FFmpeg to WebAssembly via `emmake make`
5. Builds the demo C program into `demo/demo.js` + `demo/demo.wasm`
6. Starts a local HTTP server on **http://localhost:8080**

Open the URL in your browser, click a test button, and you should see green `PASS` results.

## Build Configuration

The FFmpeg build is stripped down to the essentials (see [`config.sh`](config.sh)):

| Enabled | Disabled |
|---|---|
| H.264 decoder + parser | All encoders, muxers, demuxers |
| AAC decoder + parser | All filters, protocols, devices |
| `avformat` library | Hardware acceleration, x86 asm |

This keeps the resulting `.wasm` binary as small as possible. To add more decoders or features, edit `config.sh` and rebuild.

## Demo Functions

| Function | What it tests |
|---|---|
| `get_version_info` | Prints libavcodec/libavformat version strings |
| `test_h264_decoder` | Finds the H.264 decoder, opens a codec context |
| `test_aac_decoder` | Finds the AAC decoder, opens a codec context |
| `test_packet_alloc` | Allocates/frees `AVPacket` and `AVFrame` |

All functions are exported and callable from JavaScript via `Module.ccall()`.

## Using in Your Own Project

After building, copy `demo/demo.js` and `demo/demo.wasm` into your web project, then initialize the module:

```js
FFmpegDemo({
  print: console.log,
  printErr: console.error
}).then(module => {
  // Call exported FFmpeg functions
  module.ccall('get_version_info', 'number', [], []);
});
```

## License

FFmpeg is licensed under LGPL/GPL. See [ffmpeg.org](https://ffmpeg.org/legal.html) for details.
