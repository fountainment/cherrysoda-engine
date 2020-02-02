# CherrySoda Engine

A game engine based on bgfx and SDL2 (at a very early stage)

## Requirements

- Visual Studio (>=2017) **--On Windows**
- CMake (>=3.8)
- Python (>=3.6)
- libsdl2-dev **--On Linux**

## How To Build

1. git clone https://github.com/fountainment/cherrysoda-engine.git
2. cd cherrysoda-engine
3. git submodule update --init
4. python tool/download\_and\_setup\_sdl2\_for\_windows.py **--On Windows**
5. mkdir build & cd build & cmake ..
6. make **--On Linux**

## Current Status

- Finished the integration of bgfx and SDL2
- Replicated the the simple ECS of Monocle engine
- Did some test on the bgfx shader, added a python script for shader compilation