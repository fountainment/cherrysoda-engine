# CherrySoda Engine

A C++ game engine based on bgfx and SDL2 (hobby project at early stage)

## Requirements

- CMake (>=3.8)
- Python (>=3.6)

### Linux
- GCC (>=5) or Clang (>=3.4)
- libSDL2-dev

### Windows
- Visual Studio (>=2017)

## How To Build

```sh
# Native on Linux or Window
git clone https://github.com/fountainment/cherrysoda-engine.git
cd cherrysoda-engine
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
```

```sh
# Wasm on Linux
git clone https://github.com/emscripten-core/emsdk.git
./emsdk/emsdk install latest
./emsdk/emsdk activate latest
source ./emsdk/emsdk_env.sh
git clone https://github.com/fountainment/cherrysoda-engine.git
cd cherrysoda-engine
mkdir build
cd build
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release
make -j8
```

## Current Status

- Finished the integration of bgfx and SDL2
- Replicated the simple ECS of Monocle engine
- Did some test on the bgfx shader, added a python script for shader compilation
- Added emscripten support
- Integrated tracy profiler
- Integrated rapidjson
- Replicated the sprite animation system of Monocle engine
- Integrated ImGui
- Added bgfx embedded shader support

## Demo

WebGL version (wasm) of Playground/VoxelExperiment:

https://shenyiwen.itch.io/demo

https://fountainment.gitee.io/VoxelExperiment
