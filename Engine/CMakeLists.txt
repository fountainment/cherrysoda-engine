set(ENGINE_SRC
  CherrySoda/Engine.cpp
  CherrySoda/Engine.h
  CherrySoda/Interface/Window.cpp
  CherrySoda/Interface/Window.h
  CherrySoda/Utility/NumTypes.h
  CherrySoda/Utility/String.cpp
  CherrySoda/Utility/String.h
)

add_library(CherrySoda STATIC ${ENGINE_SRC})
target_include_directories(CherrySoda PUBLIC .)
target_link_libraries(CherrySoda PUBLIC glfw glad opengl32)
target_compile_features(CherrySoda PUBLIC cxx_std_20)
