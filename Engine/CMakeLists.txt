set(ENGINE_SRC
  CherrySoda/Engine.cpp
  CherrySoda/Engine.h
  CherrySoda/Entity.cpp
  CherrySoda/Entity.h
  CherrySoda/Scene.cpp
  CherrySoda/Scene.h
  CherrySoda/Components/Component.cpp
  CherrySoda/Components/Component.h
  CherrySoda/Components/Graphics/GraphicsComponent.cpp
  CherrySoda/Components/Graphics/GraphicsComponent.h
  CherrySoda/Graphics/Graphics.cpp
  CherrySoda/Graphics/Graphics.h
  CherrySoda/Graphics/Mesh.h
  CherrySoda/Interface/Window.cpp
  CherrySoda/Interface/Window.h
  CherrySoda/InternalUtilities/ComponentList.cpp
  CherrySoda/InternalUtilities/ComponentList.h
  CherrySoda/InternalUtilities/EntityList.cpp
  CherrySoda/InternalUtilities/EntityList.h
  CherrySoda/InternalUtilities/RendererList.cpp
  CherrySoda/InternalUtilities/RendererList.h
  CherrySoda/Renderers/Renderer.h
  CherrySoda/Renderers/EverythingRenderer.cpp
  CherrySoda/Renderers/EverythingRenderer.h
  CherrySoda/Util/Camera.cpp
  CherrySoda/Util/Camera.h
  CherrySoda/Util/Color.cpp
  CherrySoda/Util/Color.h
  CherrySoda/Util/Log.cpp
  CherrySoda/Util/Log.h
  CherrySoda/Util/Math.h
  CherrySoda/Util/NumType.h
  CherrySoda/Util/STL.h
  CherrySoda/Util/String.cpp
  CherrySoda/Util/String.h
  CherrySoda/Util/Time.cpp
  CherrySoda/Util/Time.h
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