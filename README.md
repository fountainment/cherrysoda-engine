# CherrySoda Engine

[![Windows](https://github.com/fountainment/cherrysoda-engine/actions/workflows/windows.yml/badge.svg)](https://github.com/fountainment/cherrysoda-engine/actions/workflows/windows.yml)
[![Linux](https://github.com/fountainment/cherrysoda-engine/actions/workflows/linux.yml/badge.svg)](https://github.com/fountainment/cherrysoda-engine/actions/workflows/linux.yml)
[![MacOS](https://github.com/fountainment/cherrysoda-engine/actions/workflows/macos.yml/badge.svg)](https://github.com/fountainment/cherrysoda-engine/actions/workflows/macos.yml)
[![Emscripten](https://github.com/fountainment/cherrysoda-engine/actions/workflows/emscripten.yml/badge.svg)](https://github.com/fountainment/cherrysoda-engine/actions/workflows/emscripten.yml)
[![Licence](https://img.shields.io/github/license/fountainment/cherrysoda-engine)](https://github.com/fountainment/cherrysoda-engine/blob/master/LICENSE)

A lightweight cross-platform C++ game engine based on bgfx and SDL2

## Introduction

CherrySoda engine is a C++ port of Monocle engine, with some changes and additional functionalities.

It's a hobby project, I used it to participate in several gamejams.

## Demos

[https://fountainment.github.io/demo/](https://fountainment.github.io/demo/)

## API List

[https://fountainment.github.io/doc/](https://fountainment.github.io/doc/)

Monocle engine UML:
[monocle-engine.png](https://fountainment.github.io/assets/monocle-engine.png)

## Requirements

- CMake >=3.12
- Python >=3.6

### Windows

- Visual Studio >=2017 or MinGW(GCC >= 5.2)

### Linux

- GCC >=5.2 or Clang >=3.4
- libSDL2-dev

### MacOS

- brew install sdl2

## Current Features

- Features Ported from Monocle Engine:
  - simple entity-component-system
  - simple tag-based renderer
  - simple tag-based 2d collision detection
  - 2d camera
  - 2d sprite animation system
  - 2d cpu particle system
  - bmfont file loading and pixel font rendering
  - ease functions
  - some useful components (Alarm, StateMachine, Tween, ...)
  - tile-based game utilities (TileSet, TileGrid, Grid)
- Additional Features
  - command console powered by ImGui
  - simple audio playback (support wav and ogg loading)
  - gltf file loading (only mesh data)
  - mesh rendering
  - html5 support
- Tools:
  - crunch (a texture packer) (modified) (MIT Licence)
  - sfxr (an 8-bit sound effect generator) (ported) (MIT Licence)
  - particle effect editor
