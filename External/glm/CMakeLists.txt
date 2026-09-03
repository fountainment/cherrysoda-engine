cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
cmake_policy(VERSION 3.5)

set(GLM_VERSION "1.0.3")
project(glm VERSION ${GLM_VERSION} LANGUAGES CXX)

add_subdirectory(glm)
add_library(glm::glm ALIAS glm)
