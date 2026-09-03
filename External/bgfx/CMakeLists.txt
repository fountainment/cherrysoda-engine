# bx
file(GLOB BX_SRC bx/include/bx/*.h bx/include/bx/inline/*.inl bx/src/*.cpp bx/scripts/*.natvis)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/bx/src/amalgamated.cpp PROPERTIES HEADER_FILE_ONLY ON)

add_library(bx STATIC ${BX_SRC})
# newer bx headers require C++20 (concepts/requires) - propagates to all consumers
target_compile_features(bx PUBLIC cxx_std_20)
target_include_directories(bx PRIVATE bx/include bx/3rdparty)
if(WINDOWS)
  if(MSVC)
    target_include_directories(bx PUBLIC bx/include/compat/msvc)
  elseif(MINGW)
    target_include_directories(bx PUBLIC bx/include/compat/mingw)
  endif()
endif()
if(MACOSX)
  target_include_directories(bx PUBLIC bx/include/compat/osx)
endif()
if(LINUX)
  target_include_directories(bx PUBLIC bx/include/compat/linux)
endif()

if(BGFX_CONFIG_DEBUG)
  target_compile_definitions(bx PUBLIC BX_CONFIG_DEBUG=1)
else()
  target_compile_definitions(bx PUBLIC BX_CONFIG_DEBUG=$<CONFIG:Debug>)
endif()
target_compile_options(bx PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus /Zc:preprocessor>)

# bx/include/bx/simd_t.h includes smmintrin.h unconditionally, so x86/x64 builds need SSE4.2.
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|amd64|AMD64|i[3-6]86|x86)$" OR
   CMAKE_CXX_COMPILER_ARCHITECTURE_ID MATCHES "^(x86_64|amd64|AMD64|i[3-6]86|x86)$")
  target_compile_options(bx PUBLIC $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-msse4.2>)
endif()

if(WIN32)
  target_link_libraries(bx PUBLIC psapi)
elseif(APPLE)
  find_library(FOUNDATION_LIBRARY Foundation)
  mark_as_advanced(FOUNDATION_LIBRARY)
  target_link_libraries(bx PUBLIC ${FOUNDATION_LIBRARY})
elseif(LINUX)
  find_package(Threads)
  target_link_libraries(bx PUBLIC ${CMAKE_THREAD_LIBS_INIT} dl rt)
endif()

# bimg
file(GLOB BIMG_SRC bimg/include/bimg/*.h bimg/src/image.cpp bimg/src/*.h
                   bimg/3rdparty/astc-encoder/source/*.cpp bimg/3rdparty/astc-encoder/source/*.h)

add_library(bimg STATIC ${BIMG_SRC})
target_compile_definitions(bimg PRIVATE ASTCENC_F16C=0 ASTCENC_NEON=0)
target_include_directories(bimg PRIVATE bx/include bimg/include bimg/3rdparty/astc-encoder/include)
target_link_libraries(bimg PUBLIC bx)
target_compile_features(bimg PRIVATE cxx_std_20)


# bimg_decode
file(GLOB BIMG_DECODE_SRC bimg/include/bimg/*.h bimg/src/image_decode*.cpp bimg/src/*.h)
# AVIF decoding (libavif + dav1d), enabled by default in bimg
set(BIMG_DECODE_AVIF_SRC bimg/3rdparty/dav1d/dav1d-amalgamated.c
                         bimg/3rdparty/dav1d/dav1d-bitdepth-8.c
                         bimg/3rdparty/dav1d/dav1d-bitdepth-16.c
                         bimg/3rdparty/libavif/libavif-amalgamated.c)
set_source_files_properties(${BIMG_DECODE_AVIF_SRC} PROPERTIES C_STANDARD 11)

add_library(bimg_decode STATIC ${BIMG_DECODE_SRC} ${BIMG_DECODE_AVIF_SRC})
target_compile_definitions(bimg_decode PRIVATE AVIF_CODEC_DAV1D)
target_link_libraries(bimg_decode PUBLIC bx)
target_compile_features(bimg_decode PRIVATE cxx_std_20)
target_include_directories(bimg_decode PRIVATE bx/include bimg/include bimg/3rdparty bimg/3rdparty/tinyexr/deps
                                               bimg/3rdparty/libavif bimg/3rdparty/libavif/include bimg/3rdparty/libavif/third_party/libyuv/include
                                               bimg/3rdparty/dav1d bimg/3rdparty/dav1d/include)
if(MSVC)
  target_include_directories(bimg_decode PRIVATE bimg/3rdparty/dav1d/include/compat/msvc)
endif()


# bimg_encode
# file(GLOB BIMG_ENCODE_SRC bimg/include/bimg/*.h bimg/src/image_cubemap_filter.cpp bimg/src/image_encode.cpp bimg/src/*.h)

# add_library(bimg_encode STATIC ${BIMG_ENCODE_SRC})
# target_include_directories(bimg_encode PRIVATE bx/include bimg/include bimg/3rdparty bimg/3rdparty/iqa/include)
# target_compile_features(bimg_encode PRIVATE cxx_std_14)


# bgfx
file(GLOB BGFX_SRC include/bgfx/*.h bgfx/src/*.cpp bgfx/src/*.h)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/bgfx/src/amalgamated.cpp PROPERTIES HEADER_FILE_ONLY ON)
if(WINDOWS)
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/bgfx/src/glcontext_glx.cpp
                              ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/src/glcontext_egl.cpp
                              PROPERTIES HEADER_FILE_ONLY ON)
endif()
add_library(bgfx STATIC ${BGFX_SRC})
target_link_libraries(bgfx PUBLIC bx)
target_compile_features(bgfx PRIVATE cxx_std_20)
target_include_directories(bgfx PUBLIC bx/include bimg/include bgfx/include)
target_include_directories(bgfx PRIVATE bgfx/3rdparty bgfx/3rdparty/khronos)
if(LINUX)
  target_include_directories(bgfx PRIVATE bgfx/3rdparty/directx-headers/include/directx
                                           bgfx/3rdparty/directx-headers/include
                                           bgfx/3rdparty/directx-headers/include/wsl/stubs)
endif()
if(MSVC OR MINGW)
  target_include_directories(bgfx PRIVATE bgfx/3rdparty/directx-headers/include/directx)
endif()
if(MACOSX)
  target_compile_definitions(bgfx PUBLIC BGFX_CONFIG_RENDERER_METAL=1)
elseif(ANDROID OR NOT CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)|(mips64)|(MIPS64)")
  target_link_libraries(bgfx PUBLIC GLESv2 EGL)
  target_compile_definitions(bgfx PUBLIC BGFX_CONFIG_RENDERER_OPENGLES=1)
endif()
# Defining any BGFX_CONFIG_RENDERER_* switches config.h to the "explicit" preset where
# every other renderer defaults to 0, so WebGPU must be requested explicitly.
# The renderer dlopens libwebgpu_dawn.{so,dylib} / webgpu_dawn.dll at runtime.
if(CHERRYSODA_USE_WEBGPU AND NOT EMSCRIPTEN AND NOT ANDROID)
  target_compile_definitions(bgfx PUBLIC BGFX_CONFIG_RENDERER_WEBGPU=1)
endif()
target_compile_definitions(bgfx PRIVATE BGFX_CONFIG_MULTITHREADED=0)
target_compile_definitions(bgfx PRIVATE BGFX_CONFIG_VIDEO=0)
target_compile_definitions(bgfx PUBLIC BGFX_CONFIG_MAX_VIEWS=8)
# in Xcode these files must be marked as objective-c++ (instead of renaming to .mm)
if(XCODE)
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/bgfx/src/renderer_vk.cpp
                              ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/src/renderer_webgpu.cpp
                              PROPERTIES LANGUAGE OBJCXX XCODE_EXPLICIT_FILE_TYPE sourcecode.cpp.objcpp)
endif()

if(CHERRYSODA_BUILD_SHADERC)
  include(${CMAKE_CURRENT_SOURCE_DIR}/shaderc.cmake)
endif()
