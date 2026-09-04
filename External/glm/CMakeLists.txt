add_library(glm INTERFACE)
target_include_directories(glm INTERFACE .)
target_compile_definitions(glm INTERFACE GLM_ENABLE_EXPERIMENTAL)
