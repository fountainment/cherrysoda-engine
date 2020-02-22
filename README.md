# CherrySoda Engine

A C++ game engine based on bgfx and SDL2 (currently at early stage)

## Requirements

- CMake (>=3.8)
- Python (>=3.6)

### Windows
- Visual Studio (>=2017)

### Linux
- gcc (>=5) or clang (>=3.4)
- libsdl2-dev

## How To Build

```sh
git clone https://github.com/fountainment/cherrysoda-engine.git
cd cherrysoda-engine
git submodule update --init
# For Windows
# python Tools/setup\_sdl2\_for\_windows.py
mkdir build & cd build & cmake ..
```

## Current Status

- Finished the integration of bgfx and SDL2
- Replicated the the simple ECS of Monocle engine
- Did some test on the bgfx shader, added a python script for shader compilation
- Added emscripten support
