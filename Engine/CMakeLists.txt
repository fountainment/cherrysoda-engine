set(ENGINE_SRC
  CherrySoda/CherrySoda.h
  CherrySoda/Engine.cpp
  CherrySoda/Engine.h
  CherrySoda/Entity.cpp
  CherrySoda/Entity.h
  CherrySoda/Main.h
  CherrySoda/Scene.cpp
  CherrySoda/Scene.h
  CherrySoda/Colliders/Collide.cpp
  CherrySoda/Colliders/Collide.h
  CherrySoda/Colliders/Collider.cpp
  CherrySoda/Colliders/Collider.h
  CherrySoda/Colliders/Circle.cpp
  CherrySoda/Colliders/Circle.h
  CherrySoda/Components/Component.cpp
  CherrySoda/Components/Component.h
  CherrySoda/Components/Graphics/GraphicsComponent.cpp
  CherrySoda/Components/Graphics/GraphicsComponent.h
  CherrySoda/Components/Graphics/Image.h
  CherrySoda/Components/Graphics/MeshGraphicsComponent.h
  CherrySoda/Components/Graphics/Sprite.cpp
  CherrySoda/Components/Graphics/Sprite.h
  CherrySoda/Components/Logic/Alarm.cpp
  CherrySoda/Components/Logic/Alarm.h
  CherrySoda/Components/Logic/Shaker.cpp
  CherrySoda/Components/Logic/Shaker.h
  CherrySoda/Components/Logic/StateMachine.cpp
  CherrySoda/Components/Logic/StateMachine.h
  CherrySoda/Components/Logic/Tween.cpp
  CherrySoda/Components/Logic/Tween.h
  CherrySoda/Components/Logic/Wiggler.cpp
  CherrySoda/Components/Logic/Wiggler.h
  CherrySoda/Graphics/Atlas.cpp
  CherrySoda/Graphics/Atlas.h
  CherrySoda/Graphics/Effect.h
  CherrySoda/Graphics/Graphics.cpp
  CherrySoda/Graphics/Graphics.h
  CherrySoda/Graphics/Mesh.h
  CherrySoda/Graphics/MTexture.cpp
  CherrySoda/Graphics/MTexture.h
  CherrySoda/Graphics/SpriteBatch.cpp
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
  CherrySoda/InternalUtilities/TagLists.cpp
  CherrySoda/InternalUtilities/TagLists.h
  CherrySoda/Renderers/Renderer.h
  CherrySoda/Renderers/EverythingRenderer.cpp
  CherrySoda/Renderers/EverythingRenderer.h
  CherrySoda/Renderers/SingleTagRenderer.cpp
  CherrySoda/Renderers/SingleTagRenderer.h
  CherrySoda/Util/BitTag.cpp
  CherrySoda/Util/BitTag.h
  CherrySoda/Util/Calc.cpp
  CherrySoda/Util/Calc.h
  CherrySoda/Util/Camera.cpp
  CherrySoda/Util/Camera.h
  CherrySoda/Util/ChoiceSet.h
  CherrySoda/Util/Chooser.h
  CherrySoda/Util/Color.cpp
  CherrySoda/Util/Color.h
  CherrySoda/Util/Draw.cpp
  CherrySoda/Util/Draw.h
  CherrySoda/Util/Ease.cpp
  CherrySoda/Util/Ease.h
  CherrySoda/Util/GUI.cpp
  CherrySoda/Util/GUI.h
  CherrySoda/Util/Literals.h
  CherrySoda/Util/Log.cpp
  CherrySoda/Util/Log.h
  CherrySoda/Util/Math.cpp
  CherrySoda/Util/Math.h
  CherrySoda/Util/NumType.h
  CherrySoda/Util/Profile.cpp
  CherrySoda/Util/Profile.h
  CherrySoda/Util/Json.h
  CherrySoda/Util/STL.h
  CherrySoda/Util/String.cpp
  CherrySoda/Util/String.h
  CherrySoda/Util/Time.cpp
  CherrySoda/Util/Time.h
)

set(EMBEDDED_SHADER_SRC
  CherrySoda/Graphics/shaders/varying.def.sc
  CherrySoda/Graphics/shaders/vs_basic.sc
  CherrySoda/Graphics/shaders/fs_basic.sc
  CherrySoda/Graphics/shaders/vs_sprite.sc
  CherrySoda/Graphics/shaders/fs_sprite.sc
)

# Embedded shader compilation
set(SHADER_COMPILE_STAMP "")
if(NOT "${EMBEDDED_SHADER_SRC}" STREQUAL "")
  if(WINDOWS OR LINUX OR EMSCRIPTEN)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
      set(SHADER_COMPILE_STAMP ${CMAKE_CURRENT_BINARY_DIR}/shader_compile.stamp)
      add_custom_command(OUTPUT ${SHADER_COMPILE_STAMP}
                         COMMAND python ${CHERRYSODA_TOOL_PATH}/compile_shader.py "${CMAKE_CURRENT_SOURCE_DIR}/CherrySoda/Graphics/" --file-list \"${EMBEDDED_SHADER_SRC}\" -E
                         COMMAND ${CMAKE_COMMAND} -E touch ${SHADER_COMPILE_STAMP}
                         DEPENDS ${EMBEDDED_SHADER_SRC}
                         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
  endif()
endif()


if(WINDOWS)
  include_directories(../External/bgfx/bx/include/compat/msvc)
endif()

add_library(CherrySoda STATIC ${ENGINE_SRC} ${EMBEDDED_SHADER_SRC} ${SHADER_COMPILE_STAMP})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SRC} ${EMBEDDED_SHADER_SRC})

if(EMSCRIPTEN)
  set(SDL2_INCLUDE_DIRS)
  set(SDL2_LIBRARIES)
else()
  if(NOT DEFINED ENV{SDL2_DIR})
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/FindSDL2")
  endif()
  find_package(SDL2 REQUIRED)
endif()

target_include_directories(CherrySoda PUBLIC . ${SDL2_INCLUDE_DIRS})
set(CHERRYSODA_COMMON_LIBS imgui bgfx bimg bimg_decode bx glm rapidjson ${SDL2_LIBRARIES})
if(WINDOWS)
  target_link_libraries(CherrySoda PUBLIC ${CHERRYSODA_COMMON_LIBS})
else()
  target_link_libraries(CherrySoda PUBLIC ${CHERRYSODA_COMMON_LIBS} dl GL pthread X11)
endif()
target_compile_features(CherrySoda PUBLIC cxx_std_14)
if(MSVC)
  target_compile_definitions(CherrySoda PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()


# Tracy Profiler
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tracy)
  target_compile_definitions(CherrySoda PUBLIC CHERRYSODA_USE_TRACY_PROFILER)
endif()

set_target_properties(CherrySoda PROPERTIES FOLDER Engine)
