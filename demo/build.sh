#!/bin/bash
set -e

FFMPEG_DIR="$(cd "$(dirname "$0")/../ffmpeg" && pwd)"

emcc demo.c \
  -I"$FFMPEG_DIR" \
  -L"$FFMPEG_DIR/libavcodec" \
  -L"$FFMPEG_DIR/libavformat" \
  -L"$FFMPEG_DIR/libavutil" \
  -L"$FFMPEG_DIR/libswresample" \
  -L"$FFMPEG_DIR/libswscale" \
  -lavformat -lavcodec -lswresample -lswscale -lavutil \
  -s EXPORTED_FUNCTIONS='["_main","_get_version_info","_test_h264_decoder","_test_aac_decoder","_test_packet_alloc"]' \
  -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s MODULARIZE=1 \
  -s EXPORT_NAME=FFmpegDemo \
  -o demo.js

echo "Build complete: demo.js + demo.wasm"

