set(ENGINE_SRC
  CherrySoda/Engine.cpp
  CherrySoda/Engine.h
  CherrySoda/Entity.cpp
  CherrySoda/Entity.h
  CherrySoda/Components/Component.cpp
  CherrySoda/Components/Component.h
  CherrySoda/Graphics/Graphics.cpp
  CherrySoda/Graphics/Graphics.h
  CherrySoda/Interface/Window.cpp
  CherrySoda/Interface/Window.h
  CherrySoda/Utility/Color.cpp
  CherrySoda/Utility/Color.h
  CherrySoda/Utility/Log.cpp
  CherrySoda/Utility/Log.h
  CherrySoda/Utility/NumType.h
  CherrySoda/Utility/String.cpp
  CherrySoda/Utility/String.h
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  include_directories(../External/bgfx/bx/include/compat/msvc)
endif()

add_library(CherrySoda STATIC ${ENGINE_SRC})
if(NOT DEFINED ENV{SDL2_DIR})
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")
endif()
find_package(SDL2 REQUIRED)
target_include_directories(CherrySoda PUBLIC . ../External/glm-0.9.9.6 ${SDL2_INCLUDE_DIRS})
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(CherrySoda PUBLIC bgfx bimg bx gdi32 kernel32 psapi glm_static ${SDL2_LIBRARIES})
else()
  target_link_libraries(CherrySoda PUBLIC bgfx bimg bx dl GL pthread X11 glm_static ${SDL2_LIBRARIES})
endif()
target_compile_features(CherrySoda PUBLIC cxx_std_14)
target_compile_definitions(CherrySoda PUBLIC _CRT_SECURE_NO_WARNINGS)