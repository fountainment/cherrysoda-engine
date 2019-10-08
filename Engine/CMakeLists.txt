set(ENGINE_SRC
  CherrySoda/Engine.cpp
  CherrySoda/Engine.h
  CherrySoda/Entity.cpp
  CherrySoda/Entity.h
  CherrySoda/Graphics/Graphics.cpp
  CherrySoda/Graphics/Graphics.h
  CherrySoda/Interface/Window.cpp
  CherrySoda/Interface/Window.h
  CherrySoda/Utility/Color.cpp
  CherrySoda/Utility/Color.h
  CherrySoda/Utility/Log.cpp
  CherrySoda/Utility/Log.h
  CherrySoda/Utility/NumTypes.h
  CherrySoda/Utility/String.cpp
  CherrySoda/Utility/String.h
)

add_library(CherrySoda STATIC ${ENGINE_SRC})
target_include_directories(CherrySoda PUBLIC . ../External/glm-0.9.9.6)
target_link_libraries(CherrySoda PUBLIC glfw glad glm_static)
target_compile_features(CherrySoda PUBLIC cxx_std_14)
target_compile_definitions(CherrySoda PUBLIC GLFW_INCLUDE_NONE)