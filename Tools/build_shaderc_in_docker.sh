#!/bin/bash

# Rebuild the prebuilt Tools/bin/shaderc for Linux x64 inside Docker.
# Runs from any host with Docker (macOS/Windows/Linux). Ships three files:
# Tools/bin/shaderc plus the DXC runtime pair libdxcompiler.so/libdxil.so that
# shaderc dlopens for s_6_0 (DXIL) compilation -- keep them side by side.
#
# - Stage 1 builds on ubuntu:22.04 (glibc 2.35) so the binary itself runs on
#   any distro from 22.04 up, including GitHub's ubuntu-latest. DXIL support is
#   compiled in; it activates wherever libdxcompiler.so loads (needs glibc
#   >= 2.38, i.e. ubuntu 24.04+).
# - Stage 2 verifies the installed artifacts on ubuntu:24.04, running every
#   profile compile_shader.py can invoke, s_6_0 included (fatal).
# - Exports the current working tree (minus .git/out/Tmp) so uncommitted fixes
#   are picked up.
# - Pinned to linux/amd64 so the prebuilt stays x86_64 even on Apple Silicon
#   (Docker Desktop runs the container through Rosetta/QEMU).
# - The build image can be overridden via $1 (keep it old; the verification
#   image via $2 must stay >= 24.04 for the DXC runtime).

set -euo pipefail

REPO_ROOT=$(cd "$(dirname "$0")/.." && pwd)
BUILD_IMAGE=${1:-ubuntu:22.04}
VERIFY_IMAGE=${2:-ubuntu:24.04}
WORK=$(mktemp -d "${TMPDIR:-/tmp}/cherrysoda-shaderc.XXXXXX")

# COPYFILE_DISABLE: bsdtar on macOS synthesizes AppleDouble `._*` entries into
# the tar, which CMake's source globs would then pick up as real sources.
COPYFILE_DISABLE=1 tar -C "$REPO_ROOT" \
    --exclude ./.git --exclude ./out --exclude ./Tmp --exclude ./.zcode --exclude ./Projects \
    --exclude './._*' --exclude '*/._*' \
    -cf "$WORK/src.tar" .

docker run --rm --platform linux/amd64 -v "$WORK":/work "$BUILD_IMAGE" bash -exc '
  apt-get update
  apt-get install -y --no-install-recommends \
    build-essential cmake python3 ca-certificates \
    libgl1-mesa-dev x11proto-core-dev libx11-dev libxext-dev libxcursor-dev \
    libxrandr-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev

  mkdir /src
  tar -xf /work/src.tar -C /src
  cmake -S /src -B /src/build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCHERRYSODA_BUILD_SHADERC=ON \
    -DCHERRYSODA_BUILD_TOOLS=OFF \
    -DCHERRYSODA_BUILD_EXAMPLES=OFF \
    -DCHERRYSODA_BUILD_PLAYGROUND=OFF \
    -DCHERRYSODA_BUILD_BENCH=OFF \
    -DCHERRYSODA_BUILD_TESTS=OFF
  cmake --build /src/build --target shaderc --parallel "$(nproc)"

  BIN=$(find /src/build -type f -name shaderc)
  strip --strip-all "$BIN"

  # Quick gate: the non-DX profiles compile_shader.py invokes on a Linux host.
  # (The stale prebuilt failed wgsl/spirv here.) s_6_0 is verified later, on
  # the verification image whose glibc can load libdxcompiler.so.
  S=/src/Engine/CherrySoda/Graphics/shaders
  for prof in "linux 430" "linux spirv" "linux wgsl" "osx metal" "android 300_es"; do
    set -- $prof
    "$BIN" -f "$S/vs_basic.sc" -o /tmp/o.bin --platform "$1" --type vertex \
           -i /src/External/bgfx/bgfx/src -i "$S" --profile "$2"
    test "$(stat -c%s /tmp/o.bin)" -gt 32
  done

  cp "$BIN" /work/shaderc
  cp /src/Tools/bin/libdxcompiler.so /src/Tools/bin/libdxil.so /work/
'

install -m 755 "$WORK/shaderc"        "$REPO_ROOT/Tools/bin/shaderc"
install -m 755 "$WORK/libdxcompiler.so" "$REPO_ROOT/Tools/bin/libdxcompiler.so"
install -m 755 "$WORK/libdxil.so"       "$REPO_ROOT/Tools/bin/libdxil.so"

# Final acceptance on a modern distro: run the exact shipped pair (shaderc +
# dlopened DXC runtime side by side) through every profile, s_6_0 included.
docker run --rm --platform linux/amd64 \
    -v "$REPO_ROOT":/repo:ro "$VERIFY_IMAGE" bash -exc '
  BIN=/repo/Tools/bin/shaderc
  S=/repo/Engine/CherrySoda/Graphics/shaders
  # (windows s_5_0/DXBC is deliberately absent: it routes through d3d4linux ->
  # Wine + d3dcompiler_47.dll, an environment feature a stock container never
  # has; it stays a Windows-prebuilt/Wine-user capability.)
  for prof in "linux 430" "linux spirv" "linux wgsl" "osx metal" "android 300_es" "windows s_6_0"; do
    set -- $prof
    echo "== $1 / $2"
    "$BIN" -f "$S/vs_basic.sc" -o /tmp/o.bin --platform "$1" --type vertex \
           -i /repo/External/bgfx/bgfx/src -i "$S" --profile "$2" -O 3
    test "$(stat -c%s /tmp/o.bin)" -gt 32
  done
'

rm -rf "$WORK"
echo "Tools/bin/shaderc (+ libdxcompiler.so, libdxil.so) rebuilt from $BUILD_IMAGE, verified on $VERIFY_IMAGE (linux/amd64)."
