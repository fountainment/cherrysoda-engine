# CherrySoda Engine

[![Windows](https://github.com/fountainment/cherrysoda-engine/actions/workflows/windows.yml/badge.svg)](https://github.com/fountainment/cherrysoda-engine/actions/workflows/windows.yml)
[![Linux](https://github.com/fountainment/cherrysoda-engine/actions/workflows/linux.yml/badge.svg)](https://github.com/fountainment/cherrysoda-engine/actions/workflows/linux.yml)
[![MacOS](https://github.com/fountainment/cherrysoda-engine/actions/workflows/macos.yml/badge.svg)](https://github.com/fountainment/cherrysoda-engine/actions/workflows/macos.yml)
[![Emscripten](https://github.com/fountainment/cherrysoda-engine/actions/workflows/emscripten.yml/badge.svg)](https://github.com/fountainment/cherrysoda-engine/actions/workflows/emscripten.yml)
[![Licence](https://img.shields.io/github/license/fountainment/cherrysoda-engine)](https://github.com/fountainment/cherrysoda-engine/blob/master/LICENSE)

A lightweight cross-platform C++ game engine based on bgfx and SDL2 (inspired by Monocle engine)

## Introduction

This is an engine I made for anticipating gamejam.

In 2018, I tried using Monocle engine (the engine of the game Celeste) which is written in C#.
Its design is quite simple and handy, then I wonder if I can use it to make game for different platforms.

The result is this engine, it ported most parts of Monocle engine, and added some useful features.
I have to admit that the port quality is not very good, anyway, after a few gamejams, it's basically usable now.

## Demos

[https://fountainment.github.io/demo/](https://fountainment.github.io/demo/)

Demo source:
- [BulletHellJam2021_BulletHellTrial](https://github.com/fountainment/bullethelljam2021-bullethelltrial)

## API Document

[https://fountainment.github.io/doc/](https://fountainment.github.io/doc/)

## Pros and Cons

### Pros

- The engine code is less than 20k lines, which means it only take a very limitted time to read through
- And it's not too time consuming to build this engine on low-end devices like PocketCHIP and GameShell (sometimes the cross compiling environment is hard to establish)
- The webassembly build size can be very small (mininal around 2 MB), so it will load fast on web
- Some tedious workflow processes are handled by python script

### Cons

- This engine only support a very limitted amount of features (see Current Features)
- There is no scripting support, all game coding needs to be done in C++
- The memory management is not done well at the moment
- This engine requires the user to be comfortable with command-line workflow

## Current Features

- Features Ported From Monocle Engine:
  - Entity-Component System
  - BitTag and Tag-Based Renderer
  - 2D Sprite Animation System
  - 2D Collision Detection
  - 2D Camera
  - Tile-Based Game Utilities (TileSet, TileGrid, Grid)
  - BMFont File Loading and Pixel Font Rendering
  - 2D Particle System
  - Ease Functions
  - Some Useful Components (Alarm, StateMachine, Tween, ...)
- Tools:
  - Crunch (a texture packer) (modified) (under MIT Licence)
  - Sfxr (an 8-bit sound effect generator) (ported) (under MIT Licence)
  - A Particle Effect Editor
- BGFX Integrated for Cross-Platform Graphics Rendering
- SDL2 Integrated for Cross-Platform Window and Audio
- GLM Intergrated for Math
- STB Intergrated (for now only) for OGG Audio File Loading
- ImGui Integrated for Engine GUI
- TracyProfiler Integrated for Profiling
- RapidJSON Integrated for JSON Parsing
- TinyXML2 Integrated for XML Parsing
- GoogleTest Integrated for Testing
- CMixer Integrated for Audio Mixing
- Cgltf Integrated for GLTF Model File Loading
- Audio Play (support wav and ogg loading)
- MeshGraphicsComponent
- SIMD Compile Option
- HTML5 Support


## Contribution

At current stage, the top priority is to make this engine more robust and more handy.

So, please don't hesitate to create an issue or pull request if you saw a silly bug.

And if you encounter any problem or inconvenience when using this engine,

you can create an issue, then I might be able to help you.

## Requirements

- CMake >=3.12
- Python >=3.6

### Linux

- GCC >=5.2 or Clang >=3.4
- libSDL2-dev

### Windows

- Visual Studio >=2017 or MinGW(GCC >= 5.2)

### MacOS

- brew install sdl2

## How to Build

```sh
# How to build executable
git clone https://github.com/fountainment/cherrysoda-engine.git
cd cherrysoda-engine
cmake -E make_directory build
cd build
cmake ..
cmake --build . --config Release
```

```sh
# How to build webassembly
git clone https://github.com/emscripten-core/emsdk.git
./emsdk/emsdk install latest
./emsdk/emsdk activate latest
source ./emsdk/emsdk_env.sh
git clone https://github.com/fountainment/cherrysoda-engine.git
cd cherrysoda-engine
cmake -E make_directory build
cd build
emcmake cmake ..
make -j8
```

## How to Create Project

Use Tools/create_project.py, it will copy the project template and do some string replacement.

For example:

```
cd Playground
python3 ../Tools/create_projects.py HelloWorld
```

## How to Do Performance Profiling with Tracy Profiler

First, clone the tracy profiler and put the tracy folder into "Engine" directory:

```sh
cd Engine
git clone https://github.com/wolfpld/tracy.git
```

In this engine, profiling that use tracy profiler is only enabled in "Profile" build:

(for Visual Studio and Xcode, it's the "RelWithDebInfo" build option)

```sh
# Profile build on Linux
mkdir build_profile
cd build_profile
cmake .. -DCMAKE_BUILD_TYPE=Profile
make -j8
```

Then you can use tracy profiler to monitor performance.

```sh
# How to build tracy profiler on Linux
git clone https://github.com/wolfpld/tracy.git
cd tracy/profiler/build/unix/
make -j8
```

### How to Create Project

Use Tools/create_project.py, it will copy the project template and do some string replacement.

For example:

```
cd Playground
python3 ../Tools/create_projects.py HelloWorld
```

## Tips

### To Use The Sublime Project

You need to install the CMakeBuilder plugin first

It's recommended to use ```git update-index --assume-unchanged cherrysoda.sublime-project``` to ignore project file change locally.

### To Pack Texture Atlas

First, you need to build "Tools/crunch".

And if you did use "create_project.py" to create a project, you will see a "pack_atlas.py" in project folder.

Create a folder called "textures" in your project folder,

put your textures into it, and then ```python3 pack_atlas.py```.

### To Enable SIMD

You need to add somthing like ```-DCHERRYSODA_USE_XXXX=1``` with cmake command.

Currently, available options are "SSE2" "SSE42" "AVX2" and "NEON".

For example, ```cmake .. -DCHERRYSODA_USE_SSE2=1``` will enable SSE2.

You can use Bench/MathBench to see how glm's performance changes.

### To Build on Specific Device

Curently, there are cmake options provided for these devices:

PocketCHIP: ```cmake .. -DCHIP=1```

GameShell: ```cmake .. -DCPI=1```

### To Use Shader Compiler

Prebuilt shaderc binary are available on x86_64-Windows and x86_64-Linux.

If you want or have to build shader compiler on your own, you can add ```-DCHERRYSODA_BUILD_SHADERC=1``` with cmake command.

You can also copy the shaderc binary from bgfx build, copy to "Tools/bin/shaderc.local" (add .exe suffix on Windows)


## Licence

```
MIT License

Copyright (c) 2019-2022 C.Even

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
