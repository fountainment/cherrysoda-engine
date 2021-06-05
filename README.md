# CherrySoda Engine

[![Windows](https://github.com/fountainment/cherrysoda-engine/actions/workflows/windows.yml/badge.svg)](https://github.com/fountainment/cherrysoda-engine/actions/workflows/windows.yml)
[![Linux](https://github.com/fountainment/cherrysoda-engine/actions/workflows/linux.yml/badge.svg)](https://github.com/fountainment/cherrysoda-engine/actions/workflows/linux.yml)
[![MacOS](https://github.com/fountainment/cherrysoda-engine/actions/workflows/macos.yml/badge.svg)](https://github.com/fountainment/cherrysoda-engine/actions/workflows/macos.yml)
[![Emscripten](https://github.com/fountainment/cherrysoda-engine/actions/workflows/emscripten.yml/badge.svg)](https://github.com/fountainment/cherrysoda-engine/actions/workflows/emscripten.yml)
[![Licence](https://img.shields.io/github/license/fountainment/cherrysoda-engine)](https://github.com/fountainment/cherrysoda-engine/blob/master/LICENSE)

A tiny C++ game engine based on bgfx and SDL2 (inspired by Monocle engine)

## HTML5 Demos

**Web Assembly of Playground/VoxelExperiment:**

[VoxelExperiment](https://fountainment.github.io/demo/VoxelExperiment/) (github.io)

[VoxelExperiment](https://fountainment.gitee.io/VoxelExperiment/) (gitee.io)

## Requirements

- CMake >=3.12
- Python >=3.6

### Linux
- GCC >=5.2 or Clang >=3.4
- libSDL2-dev

### Windows
- Visual Studio >=2017

## How to Build

```sh
# Native on Linux, Windows or MacOS
git clone https://github.com/fountainment/cherrysoda-engine.git
cd cherrysoda-engine
cmake -E make_directory build
cd build
cmake ..
cmake --build . --config Release
```

```sh
# Web Assembly on Linux or MacOS
git clone https://github.com/emscripten-core/emsdk.git
./emsdk/emsdk install latest
./emsdk/emsdk activate latest
source ./emsdk/emsdk_env.sh
git clone https://github.com/fountainment/cherrysoda-engine.git
cd cherrysoda-engine
mkdir build
cd build
emcmake cmake ..
make -j
```

### How to Do Performance Profiling with Tracy Profiler

First, clone the tracy profiler and put the tracy folder into "Engine" directory:

```sh
cd Engine
git clone https://github.com/wolfpld/tracy.git
```

In this engine, profiling that use tracy profiler is only enabled in "Profile" build:

```sh
# Profile build on Linux
mkdir build_profile
cd build_profile
cmake .. -DCMAKE_BUILD_TYPE=Profile
make -j
```

And on Visual Studio, it's the "RelWithDebInfo" build option.

Then you can use tracy profiler to monitor performance.

```sh
# How to build tracy profiler on Linux
git clone https://github.com/wolfpld/tracy.git
cd tracy/profiler/build/unix/
make -j
```

## Tips

### To Use The Sublime Project

You need to install the CMakeBuilder plugin first

### To Pack Texture Atlas

You need to use Tools/crunch (build it youself)

The recommended parameters for crunch is: ```-j -p -u -t -s2048 -p8```

For more information about crunch you can look into https://github.com/fountainment/crunch

### To Enable SIMD

You need to add somthing like ```-DCHERRYSODA_USE_XXXX=1``` with cmake.

Currently, available options are "SSE2" "SSE42" "AVX2" and "NEON".

For example, ```cmake .. -DCHERRYSODA_USE_SSE2=1``` will enable SSE2.

You can use Bench/MathBench to see how glm's performance changes.

### To Build on Specific Device

Curently, there are cmake options provided for these devices:

Lemote Fuloong 2.0: ```cmake .. -DLOONGSON=1``` (use GALLIUM_DRIVER=softpipe)

PocketCHIP: ```cmake .. -DCHIP=1```

GameShell: ```cmake .. -DCPI=1```

### To Use Shader Compiler on Non-x86_64 Platform

You need to add ```-DCHERRYSODA_BUILD_SHADERC=1``` with cmake.

## Current Features 

- BGFX Integrated for Cross-Platform Graphics Rendering
- SDL2 Integrated for Cross-Platform Window and Audio
- GLM Intergrated for Math
- STB Intergrated (for now only) for OGG Audio File Loading
- ImGui Integrated for Engine GUI
- TracyProfiler Integrated for Profiling
- RapidJSON Integrated for Json Parsing
- CMixer Integrated for Audio Mixing
- Simple Entity-Component System
- Simple Sprite System (quite handy imo)
- Minimalism 2D Collision Detection (too simple but useful in common situations)
- BMFont File Loading and Pixel Font Rendering
- Simple 2D Particle System
- Audio Play (support wav and ogg loading)
- SIMD Compile Option
- HTML5 Support

## Licence

```
MIT License

Copyright (c) 2019-2021 C.Even

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## Licence of Monocle Engine

```

  m o n O c l e

   e n g i n e


Copyright (c) 2012-2014 Matt Thorson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```
