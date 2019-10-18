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
target_compile_features(bgfx PRIVATE cxx_std_14)