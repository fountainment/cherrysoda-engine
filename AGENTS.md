# AGENTS.md

CherrySoda Engine — a lightweight cross-platform C++20 game engine (a port of Monocle engine) built on bgfx for rendering and SDL2 for platform/windowing. Targets Windows, Linux, macOS, HTML5 (Emscripten), Android, plus some hobby boards. The engine C++ level is driven by vendored bx headers (currently requires C++20). Hobby project, no release process — keep changes simple and aligned with existing style.

## Layout

- `Engine/CherrySoda/` — engine sources, built as static lib `CherrySoda`. Subfolders: core (`Engine`, `Entity`, `Scene`), `Colliders/`, `Components/{Graphics,Logic}/`, `Graphics/` (+ `shaders/`), `Input/`, `Interface/`, `InternalUtilities/`, `Particles/`, `Renderers/`, `Util/`.
- `External/` — vendored deps: bgfx, glm, imgui, rapidjson, stb, tinyxml2, cmixer, cgltf, gtest, tracy. Don't hand-edit these; bgfx is updated via `Tools/update_bgfx.py`.
- `Tools/` — Python build tooling (`compile_shader.py`, `create_project.py`, `setup_sdl2_for_windows.py`, …), prebuilt `bin/shaderc` (Windows/Linux x64 only), crunch, sfxr port, `ParticleEditor/`.
- `Examples/`, `Playground/`, `Bench/`, `Tests/` — sample apps; each registers itself with the `add_cherrysoda_executable()` function defined at the top of the root `CMakeLists.txt`.
- `cmake/` — helper modules (`FindSDL2`, `LTO.cmake`, `SysDetect.cmake`).

## Build

Requires CMake >= 3.13 and Python >= 3 (configure fails hard without Python). SDL2 must be installed via system package (`brew install sdl2` on macOS, `libSDL2-dev` on Linux); on Windows SDL2 is auto-downloaded into `External/SDL2-2.0.20` if `SDL2_DIR` is unset.

Preferred (CMake presets, Ninja, clang on Unix / MSVC on Windows, output in `out/build/<preset>/`):

```sh
cmake --preset unix-debug          # or unix-release / unix-profile; windows-* on Windows
cmake --build out/build/unix-debug
```

Or plain CMake: `cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build`.

Build types: `Debug`, `Release` (default), `Profile` (= RelWithDebInfo + Tracy enabled). Useful options: `-DCHERRYSODA_BUILD_EXAMPLES/PLAYGROUND/BENCH/TESTS=OFF`, `-DCHERRYSODA_LIGHT_BUILD=ON` (engine only), `-DCHERRYSODA_BUILD_SHADERC=ON` (presets `unix-shaderc`/`windows-shaderc`).

## Test

GoogleTest executables built under the build dir's `Tests/` folder (`AllTests`, `CalcTest`, `CollideTest`, `UtilTest`). Run from that folder:

```sh
TRACY_NO_INVARIANT_CHECK=1 ./AllTests
```

The env var is required (this is exactly what CI does in `.github/workflows/`). No ctest integration — run the binaries directly. Adding a test: create `Tests/<Name>Test.cpp`, then append a `set(CHERRYSODA_EXECUTABLE_SRC …) / add_cherrysoda_executable(<Name>Test)` block in `Tests/CMakeLists.txt`.

## Conventions and hard constraints

- **C++20, exceptions and RTTI disabled** (`-fno-exceptions -fno-rtti`, also on MSVC). No `throw`/`try`/`catch`/`dynamic_cast` and no removed-in-C++20 APIs (`std::random_shuffle`, `std::result_of`, `u8"..."` string literals as `char*`); use `CHERRYSODA_ASSERT` and error codes/null returns instead.
- Everything lives in namespace `cherrysoda`. C-style `typedef` aliases (`typedef std::string String;`), member prefix `m_`, tab indentation, include guards named `_CHERRYSODA_…_H_`.
- Includes use the engine-root path: `#include <CherrySoda/Util/Log.h>`.
- Getter/setter pairs are one overloaded name via macros in `Util/Math.h` (`CHERRYSODA_GETTER_SETTER_OF_TYPE/VEC2/VEC3/BOOL`, e.g. `Position()` / `Position(v)`) — follow that pattern for new members.
- Logging/debug output only through `Util/Log.h` macros: `CHERRYSODA_LOG`, `CHERRYSODA_DEBUG`, `CHERRYSODA_ASSERT`. `CHERRYSODA_DEBUG`/`ASSERT` compile away unless built as `Debug`.
- **New engine files must be added manually to `ENGINE_SRC` in `Engine/CMakeLists.txt`** (no globbing). Embedded shader files go in `EMBEDDED_SHADER_SRC` there.
- Executables are games: subclass `cherrysoda::Engine`, use the `CHERRYSODA_DEFAULT_MAIN` macro from `Main.h` (see `Examples/Minimal`). Set `CHERRYSODA_EXECUTABLE_SRC` then call `add_cherrysoda_executable(Name)`; an `assets/` folder next to the app is copied into its binary dir automatically.

## Shaders

bgfx-style `.sc` shaders (`vs_*.sc`/`fs_*.sc`) are compiled at build time by `Tools/compile_shader.py` into per-platform `.bin` files under `assets/shaders/`. Profiles: GLSL `430`, ESSL `300_es`, Metal, SPIR-V, and WGSL (`wgsl/` folder, for the WebGPU renderer); DX9/DX11 only compile on Windows hosts. Old profiles (GLSL `120`, ESSL default, DX9 renderer) were removed with the bgfx rev 9149 update. shaderc comes either as a prebuilt binary (`Tools/bin/shaderc` for Windows/Linux x64) or `Tools/bin/shaderc.local` (built automatically when configured with `CHERRYSODA_BUILD_SHADERC=ON`, e.g. the `unix-shaderc` preset — this is how you get a working shaderc on macOS). Engine's own shaders live in `Engine/CherrySoda/Graphics/shaders/` and are embedded into the lib as `embedded_shaders/*.bin.h` (regenerated whenever shaderc is available).

## bgfx / WebGPU

Vendored bgfx/bimg/bx live in `External/bgfx/{bgfx,bimg,bx}` and are updated by cloning the three repos as siblings of this repo, then running `Tools/update_bgfx.py` (then expect to fix build breaks and regenerate embedded shaders). `External/bgfx/CMakeLists.txt` + `shaderc.cmake` are engine-authored build scripts modeled after https://github.com/bkaradzic/bgfx.cmake. The WebGPU renderer is enabled by default (`CHERRYSODA_USE_WEBGPU`, desktop only); it dlopens `libwebgpu_dawn.dylib/.so` / `webgpu_dawn.dll` at runtime, so ship that library next to the app to actually use it. shaderc links Dawn's Tint for WGSL compilation — don't strip the `tint` target. Note: defining any `BGFX_CONFIG_RENDERER_*` makes all other renderers default to off in bgfx's config.h.

## Gotchas

- `Profile` builds link Tracy; running test/profile binaries without `TRACY_NO_INVARIANT_CHECK=1` can trip invariant asserts in CI environments.
- If a sibling `../cherrysoda-projects` repo with a `CMakeLists.txt` exists it is auto-added and forces `CHERRYSODA_LIGHT_BUILD=ON` — engine-target changes can silently skip Examples/Tests in that setup.
- Emscripten builds disable tools/tests and use `-s USE_SDL=2`; per-app `index.html` is generated by `Tools/generate_index_html.py` unless the app provides one.
- `out/` is the preset build output directory — don't commit or hand-edit anything under it.
