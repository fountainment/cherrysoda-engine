set(ENGINE_SRC
  CherrySoda/Engine.cpp
  CherrySoda/Engine.h
  CherrySoda/Entity.cpp
  CherrySoda/Entity.h
  CherrySoda/Scene.cpp
  CherrySoda/Scene.h
  CherrySoda/Profile.h
  CherrySoda/Components/Component.cpp
  CherrySoda/Components/Component.h
  CherrySoda/Components/Graphics/GraphicsComponent.cpp
  CherrySoda/Components/Graphics/GraphicsComponent.h
  CherrySoda/Components/Graphics/Image.h
  CherrySoda/Components/Graphics/MeshGraphicsComponent.h
  CherrySoda/Graphics/Effect.h
  CherrySoda/Graphics/Graphics.cpp
  CherrySoda/Graphics/Graphics.h
  CherrySoda/Graphics/Mesh.h
  CherrySoda/Graphics/MTexture.cpp
  CherrySoda/Graphics/MTexture.h
  CherrySoda/Graphics/SpriteBatch.h
  CherrySoda/Graphics/Texture.cpp
  CherrySoda/Graphics/Texture.h
  CherrySoda/Input/MInput.cpp
  CherrySoda/Input/MInput.h
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
  CherrySoda/Renderers/SingleTagRenderer.cpp
  CherrySoda/Renderers/SingleTagRenderer.h
  CherrySoda/Util/BitTag.cpp
  CherrySoda/Util/BitTag.h
  CherrySoda/Util/Camera.cpp
  CherrySoda/Util/Camera.h
  CherrySoda/Util/Color.cpp
  CherrySoda/Util/Color.h
  CherrySoda/Util/Draw.cpp
  CherrySoda/Util/Draw.h
  CherrySoda/Util/Literals.h
  CherrySoda/Util/Log.cpp
  CherrySoda/Util/Log.h
  CherrySoda/Util/Math.cpp
  CherrySoda/Util/Math.h
  CherrySoda/Util/NumType.h
  CherrySoda/Util/STL.h
  CherrySoda/Util/String.cpp
  CherrySoda/Util/String.h
  CherrySoda/Util/Time.cpp
  CherrySoda/Util/Time.h
)

# Tracy Profiler
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tracy)
  set(ENGINE_SRC ${ENGINE_SRC} tracy/TracyClient.cpp tracy/Tracy.hpp)
  add_definitions(-DCHERRYSODA_USE_TRACY_PROFILER)
endif()

if(WINDOWS)
  include_directories(../External/bgfx/bx/include/compat/msvc)
endif()

add_library(CherrySoda STATIC ${ENGINE_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SRC})

if(EMSCRIPTEN)
  set(SDL2_INCLUDE_DIRS)
  set(SDL2_LIBRARIES)
else()
  if(NOT DEFINED ENV{SDL2_DIR})
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/FindSDL2")
  endif()
  find_package(SDL2 REQUIRED)
endif()

target_include_directories(CherrySoda PUBLIC . ../External/glm-0.9.9.7 ${SDL2_INCLUDE_DIRS})
set(CHERRYSODA_COMMON_LIBS bgfx bimg bimg_decode bx glm_static ${SDL2_LIBRARIES})
if(WINDOWS)
  target_link_libraries(CherrySoda PUBLIC ${CHERRYSODA_COMMON_LIBS} gdi32 kernel32 psapi)
else()
  target_link_libraries(CherrySoda PUBLIC ${CHERRYSODA_COMMON_LIBS} dl GL pthread X11)
endif()
target_compile_features(CherrySoda PUBLIC cxx_std_14)
target_compile_definitions(CherrySoda PUBLIC _CRT_SECURE_NO_WARNINGS)
set_target_properties(CherrySoda PROPERTIES FOLDER Engine)
