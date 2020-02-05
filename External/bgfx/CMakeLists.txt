if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  include_directories(bx/include/compat/msvc)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#bx
file(GLOB BX_SRC bx/include/bx/*.h bx/include/bx/inline/*.inl bx/src/*.cpp )
list(REMOVE_ITEM BX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/bx/src/amalgamated.cpp)
list(REMOVE_ITEM BX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/bx/src/crtnone.cpp)

add_library(bx STATIC ${BX_SRC})
target_compile_definitions(bx PRIVATE __STDC_FORMAT_MACROS)
target_include_directories(bx PRIVATE bx/include bx/3rdparty)
target_compile_features(bx PRIVATE cxx_std_14)


# bimg
file(GLOB BIMG_SRC bimg/include/bimg/*. bimg/include/bimg/*.h bimg/src/image.cpp bimg/src/image_gnf.cpp bimg/src/*.h bimg/3rdparty/astc-codec/src/decoder/*.cc)

add_library(bimg STATIC ${BIMG_SRC})
target_include_directories(bimg PRIVATE bx/include bimg/include bimg/3rdparty/astc-codec bimg/3rdparty/astc-codec/include)
target_compile_features(bimg PRIVATE cxx_std_14)


# bimg_decode
file(GLOB BIMG_DECODE_SRC bimg/include/bimg/*.h bimg/src/image_decode.cpp bimg/src/*.h)

add_library(bimg_decode STATIC ${BIMG_DECODE_SRC})
target_include_directories(bimg_decode PRIVATE bx/include bimg/include bimg/3rdparty)
target_compile_features(bimg_decode PRIVATE cxx_std_14)


# bgfx
file(GLOB BGFX_SRC include/bgfx/**.h bgfx/src/*.cpp bgfx/src/*.h)
list(REMOVE_ITEM BGFX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/src/amalgamated.cpp)
list(REMOVE_ITEM BGFX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/src/ctrone.cpp)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  list(REMOVE_ITEM BGFX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/src/glcontext_glx.cpp)
  list(REMOVE_ITEM BGFX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/src/glcontext_egl.cpp)
endif()
add_library(bgfx STATIC ${BGFX_SRC})
target_compile_definitions(bgfx PRIVATE __STDC_FORMAT_MACROS)
target_include_directories(bgfx PUBLIC bx/include bimg/include bgfx/include)
target_include_directories(bgfx PRIVATE bgfx/3rdparty bgfx/3rdparty/dxsdk/include bgfx/3rdparty/khronos)
if(NOT CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
  target_link_libraries(bgfx PUBLIC GLESv2 EGL)
  target_compile_definitions(bgfx PUBLIC BGFX_CONFIG_RENDERER_OPENGLES=1)
endif()
include(ProcessorCount)
ProcessorCount(N)
if(N EQUAL 1)
  target_compile_definitions(bgfx PUBLIC BGFX_CONFIG_MULTITHREADED=0)
endif()
target_compile_definitions(bgfx PUBLIC BGFX_CONFIG_RENDERER_OPENGL_MIN_VERSION=32)
target_compile_definitions(bgfx PUBLIC BGFX_CONFIG_MAX_VIEWS=8)
target_compile_features(bgfx PRIVATE cxx_std_14)