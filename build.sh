#!/bin/bash
set -e

# download emscripten if not exist
if [ ! -d "emsdk" ]; then
  echo "Cloning emsdk..."
  git clone https://github.com/emscripten-core/emsdk.git
else
  echo "emsdk already exists, skipping download."
fi


cd emsdk
echo "Installing latest emsdk..."
./emsdk install latest
echo "Activating latest emsdk..."
./emsdk activate latest
echo "Sourcing emsdk environment..."
source ./emsdk_env.sh
echo "emsdk setup complete."
cd ..

# download ffmpeg souce code if not exist
if [ ! -d "ffmpeg" ]; then
  echo "Cloning ffmpeg..."
  git clone https://git.ffmpeg.org/ffmpeg.git
else
  echo "ffmpeg already exists, skipping download."
fi

cp ./config.sh ./ffmpeg/
cd ffmpeg
echo "Configuring ffmpeg for emscripten..."
chmod +x config.sh
./config.sh
echo "Building ffmpeg with emscripten..."
if ! emmake make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu); then
  echo "ERROR: ffmpeg compilation failed!"
  echo "Last 50 lines of config.log:"
  tail -50 ffbuild/config.log 2>/dev/null || echo "config.log not found."
  exit 1
fi
echo "ffmpeg build complete."
cd ..

# Build and run the demo
cd demo
chmod +x ./build.sh
./build.sh

# Run the demo using a simple HTTP server
echo "Starting HTTP server to serve demo files..."
echo "Demo available at: http://localhost:8080"
if command -v python3 &>/dev/null; then
  python3 -m http.server 8080
elif command -v python &>/dev/null; then
  python -m SimpleHTTPServer 8080
else
  echo "ERROR: Python is not installed. Please install Python to run the demo."
  exit 1
fi