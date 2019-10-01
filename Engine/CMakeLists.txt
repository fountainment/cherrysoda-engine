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
find_package(OpenGL REQUIRED)
set(CHERRYSODA_INCLUDE_DIRS .)
target_include_directories(CherrySoda PUBLIC ${CHERRYSODA_INCLUDE_DIRS})
target_link_libraries(CherrySoda PUBLIC glfw glad ${OPENGL_LIBRARIES})
target_compile_features(CherrySoda PUBLIC cxx_std_14)
