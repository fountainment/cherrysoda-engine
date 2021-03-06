# CherrySoda Engine

[![TravisCI](https://img.shields.io/travis/com/fountainment/cherrysoda-engine?branch=master)](https://travis-ci.com/github/fountainment/cherrysoda-engine)
[![Licence](https://img.shields.io/github/license/fountainment/cherrysoda-engine)](https://github.com/fountainment/cherrysoda-engine/blob/master/LICENSE)

A C++ game engine based on bgfx and SDL2 (hobby project at early stage)

## Requirements

- CMake >=3.8 (emscripten build require 3.12 or above)
- Python >=2.7 (emcmake require 3.6 or above)

### Linux
- GCC >=5.2 or Clang >=3.4
- libSDL2-dev

### Windows
- Visual Studio >=2017

## How to Build

```sh
# Native on Linux or Windows
git clone https://github.com/fountainment/cherrysoda-engine.git
cd cherrysoda-engine
cmake -E make_directory build
cd build
cmake ..
cmake --build . --config Release
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
- Added SIMD compile option
- Ported sfxr (a simple synthersizer sound effect generator)
- Added travisCI config
- Added doxygen config
- Added CMake config of shaderc


## Demo

WebGL version (wasm) of Playground/VoxelExperiment:

https://fountainment.github.io/demo/VoxelExperiment/
