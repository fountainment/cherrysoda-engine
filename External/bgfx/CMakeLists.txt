if(WINDOWS)
  if(MSVC)
    include_directories(bx/include/compat/msvc)
  else()
    include_directories(bx/include/compat/mingw)
  endif()
endif()

# bx
file(GLOB BX_SRC bx/include/bx/*.h bx/include/bx/inline/*.inl bx/src/*.cpp bx/scriptes/*.natvis)
list(REMOVE_ITEM BX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/bx/src/amalgamated.cpp)
list(REMOVE_ITEM BX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/bx/src/crtnone.cpp)

add_library(bx STATIC ${BX_SRC})
target_compile_features(bx PRIVATE cxx_std_14)
target_include_directories(bx PRIVATE bx/include bx/3rdparty)
if(WINDOWS)
  if(MSVC)
    target_include_directories(bx PUBLIC bx/include/compat/msvc)
  else()
    target_include_directories(bx PUBLIC bx/include/compat/mingw)
  endif()
endif()
if(MACOSX)
  target_include_directories(bx PUBLIC bx/include/compat/osx)
endif()

if(BGFX_CONFIG_DEBUG)
  target_compile_definitions(bx PUBLIC BX_CONFIG_DEBUG=1)
else()
  target_compile_definitions(bx PUBLIC BX_CONFIG_DEBUG=$<CONFIG:Debug>)
endif()

# bimg
file(GLOB BIMG_SRC bimg/include/bimg/*.h bimg/src/image.cpp bimg/src/image_gnf.cpp bimg/src/*.h
                   bimg/3rdparty/astc-codec/src/decoder/*.h bimg/3rdparty/astc-codec/src/decoder/*.cc
                   bimg/3rdparty/tinyexr/deps/miniz/miniz.*)

add_library(bimg STATIC ${BIMG_SRC})
target_include_directories(bimg PRIVATE bx/include bimg/include bimg/3rdparty/astc-codec bimg/3rdparty/astc-codec/include bimg/3rdparty/tinyexr/deps/miniz)
target_link_libraries(bimg PUBLIC bx)
target_compile_features(bimg PRIVATE cxx_std_14)


# bimg_decode
file(GLOB BIMG_DECODE_SRC bimg/include/bimg/*.h bimg/src/image_decode.cpp bimg/src/*.h)

add_library(bimg_decode STATIC ${BIMG_DECODE_SRC})
target_link_libraries(bimg_decode PUBLIC bx)
target_compile_features(bimg_decode PRIVATE cxx_std_14)
target_include_directories(bimg_decode PRIVATE bx/include bimg/include bimg/3rdparty bimg/3rdparty/tinyexr/deps/miniz)


# bimg_encode
# file(GLOB BIMG_ENCODE_SRC bimg/include/bimg/*.h bimg/src/image_cubemap_filter.cpp bimg/src/image_encode.cpp bimg/src/*.h)

# add_library(bimg_encode STATIC ${BIMG_ENCODE_SRC})
# target_include_directories(bimg_encode PRIVATE bx/include bimg/include bimg/3rdparty bimg/3rdparty/iqa/include)
# target_compile_features(bimg_encode PRIVATE cxx_std_14)


# bgfx
file(GLOB BGFX_SRC include/bgfx/*.h bgfx/src/*.cpp bgfx/src/*.h)
list(REMOVE_ITEM BGFX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/src/amalgamated.cpp)
list(REMOVE_ITEM BGFX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/src/ctrone.cpp)
if(WINDOWS)
  list(REMOVE_ITEM BGFX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/src/glcontext_glx.cpp)
  list(REMOVE_ITEM BGFX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/src/glcontext_egl.cpp)
endif()
if(MACOSX)
  file(GLOB MACOSX_BGFX_SRC bgfx/src/glcontext_*.mm bgfx/src/renderer_*.mm)
  set(BGFX_SRC ${BGFX_SRC} ${MACOSX_BGFX_SRC})
endif()
add_library(bgfx STATIC ${BGFX_SRC})
target_link_libraries(bgfx PUBLIC bx)
target_compile_features(bgfx PRIVATE cxx_std_14)
target_include_directories(bgfx PUBLIC bx/include bimg/include bgfx/include)
target_include_directories(bgfx PRIVATE bgfx/3rdparty bgfx/3rdparty/dxsdk/include bgfx/3rdparty/khronos)
if(MACOSX)
  target_compile_definitions(bgfx PUBLIC BGFX_CONFIG_RENDERER_METAL=1)
elseif(ANDROID OR NOT CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)|(mips64)|(MIPS64)")
  target_link_libraries(bgfx PUBLIC GLESv2 EGL)
  target_compile_definitions(bgfx PUBLIC BGFX_CONFIG_RENDERER_OPENGLES=1)
endif()
target_compile_definitions(bgfx PRIVATE BGFX_CONFIG_MULTITHREADED=0)
# target_compile_definitions(bgfx PRIVATE BGFX_CONFIG_RENDERER_OPENGL=1)
# target_compile_definitions(bgfx PRIVATE BGFX_CONFIG_RENDERER_VULKAN=1)
if(WINDOWS)
  # DX9 still perform lower latency than DX11 on some devices
  target_compile_definitions(bgfx PRIVATE BGFX_CONFIG_RENDERER_DIRECT3D9=1)
endif()
target_compile_definitions(bgfx PUBLIC BGFX_CONFIG_MAX_VIEWS=8)

if(CHERRYSODA_BUILD_SHADERC)
  include(${CMAKE_CURRENT_SOURCE_DIR}/shaderc.cmake)
endif()
