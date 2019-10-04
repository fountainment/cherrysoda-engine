set(ENGINE_SRC
  CherrySoda/Engine.cpp
  CherrySoda/Engine.h
  CherrySoda/Graphic/Graphic.cpp
  CherrySoda/Graphic/Graphic.h
  CherrySoda/Interface/Window.cpp
  CherrySoda/Interface/Window.h
  CherrySoda/Utility/Color.cpp
  CherrySoda/Utility/Color.h
  CherrySoda/Utility/NumTypes.h
  CherrySoda/Utility/String.cpp
  CherrySoda/Utility/String.h
)

add_library(CherrySoda STATIC ${ENGINE_SRC})
target_include_directories(CherrySoda PUBLIC .)
target_link_libraries(CherrySoda PUBLIC glfw glad glm_static)
target_compile_features(CherrySoda PUBLIC cxx_std_14)