# shaderc deps (bgfx rev 9149: fcpp and glsl-optimizer were removed upstream;
# shaderc now has a built-in preprocessor and uses Tint for WGSL/WebGPU)
set(BGFX_3RD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/3rdparty)
set(GLSLANG ${BGFX_3RD_DIR}/glslang)
set(SPIRV_CROSS ${BGFX_3RD_DIR}/spirv-cross)
set(SPIRV_TOOLS ${BGFX_3RD_DIR}/spirv-tools)
set(SPIRV_HEADERS ${BGFX_3RD_DIR}/spirv-headers)
set(TINT ${BGFX_3RD_DIR}/dawn)

# glslang
file(GLOB_RECURSE GLSLANG_SRC ${GLSLANG}/glslang/*.cpp ${GLSLANG}/glslang/*.h
                              ${GLSLANG}/SPIRV/*.cpp ${GLSLANG}/SPIRV/*.h)
if(WINDOWS)
  list(FILTER GLSLANG_SRC EXCLUDE REGEX "glslang/OSDependent/Unix/.*")
else()
  list(FILTER GLSLANG_SRC EXCLUDE REGEX "glslang/OSDependent/Windows/.*")
endif()
add_library(glslang STATIC ${GLSLANG_SRC})
target_compile_features(glslang PRIVATE cxx_std_20)
target_compile_definitions(glslang PRIVATE ENABLE_OPT=1 ENABLE_HLSL=1)
target_include_directories(glslang PUBLIC ${GLSLANG} ${GLSLANG}/glslang/Public ${GLSLANG}/glslang/Include)
target_include_directories(glslang PRIVATE ${BGFX_3RD_DIR} ${SPIRV_TOOLS}/include ${SPIRV_TOOLS}/source)
if(MINGW OR LINUX)
  target_compile_options(glslang PRIVATE -Wno-logical-op -Wno-maybe-uninitialized)
endif()
if(MINGW OR LINUX OR MACOSX)
  target_compile_options(glslang PRIVATE -fno-strict-aliasing
                                         -Wno-ignored-qualifiers
                                         -Wno-implicit-fallthrough
                                         -Wno-missing-field-initializers
                                         -Wno-reorder
                                         -Wno-return-type
                                         -Wno-shadow
                                         -Wno-sign-compare
                                         -Wno-switch
                                         -Wno-undef
                                         -Wno-unknown-pragmas
                                         -Wno-unused-function
                                         -Wno-unused-parameter
                                         -Wno-unused-variable)
endif()
if(MACOSX)
  target_compile_options(glslang PRIVATE -Wno-c++11-extensions
                                         -Wno-unused-const-variable
                                         -Wno-deprecated-register)
endif()
if(LINUX_GCC)
  target_compile_options(glslang PRIVATE -Wno-unused-but-set-variable)
endif()

# spirv-tools (spirv-opt)
file(GLOB SPIRV_TOOLS_GLOB_SRC ${SPIRV_TOOLS}/source/opt/*.cpp ${SPIRV_TOOLS}/source/opt/*.h
                               ${SPIRV_TOOLS}/source/reduce/*.cpp ${SPIRV_TOOLS}/source/reduce/*.h
                               ${SPIRV_TOOLS}/source/val/*.cpp ${SPIRV_TOOLS}/source/val/*.h)
set(SPIRV_TOOLS_SRC ${SPIRV_TOOLS_GLOB_SRC}
                    ${SPIRV_TOOLS}/source/assembly_grammar.cpp
                    ${SPIRV_TOOLS}/source/assembly_grammar.h
                    ${SPIRV_TOOLS}/source/binary.cpp
                    ${SPIRV_TOOLS}/source/binary.h
                    ${SPIRV_TOOLS}/source/cfa.h
                    ${SPIRV_TOOLS}/source/diagnostic.cpp
                    ${SPIRV_TOOLS}/source/diagnostic.h
                    ${SPIRV_TOOLS}/source/disassemble.cpp
                    ${SPIRV_TOOLS}/source/disassemble.h
                    ${SPIRV_TOOLS}/source/enum_set.h
                    ${SPIRV_TOOLS}/source/ext_inst.cpp
                    ${SPIRV_TOOLS}/source/ext_inst.h
                    ${SPIRV_TOOLS}/source/extensions.cpp
                    ${SPIRV_TOOLS}/source/extensions.h
                    ${SPIRV_TOOLS}/source/instruction.h
                    ${SPIRV_TOOLS}/source/latest_version_glsl_std_450_header.h
                    ${SPIRV_TOOLS}/source/latest_version_opencl_std_header.h
                    ${SPIRV_TOOLS}/source/latest_version_spirv_header.h
                    ${SPIRV_TOOLS}/source/libspirv.cpp
                    ${SPIRV_TOOLS}/source/macro.h
                    ${SPIRV_TOOLS}/source/name_mapper.cpp
                    ${SPIRV_TOOLS}/source/name_mapper.h
                    ${SPIRV_TOOLS}/source/opcode.cpp
                    ${SPIRV_TOOLS}/source/opcode.h
                    ${SPIRV_TOOLS}/source/operand.cpp
                    ${SPIRV_TOOLS}/source/operand.h
                    ${SPIRV_TOOLS}/source/parsed_operand.cpp
                    ${SPIRV_TOOLS}/source/parsed_operand.h
                    ${SPIRV_TOOLS}/source/print.cpp
                    ${SPIRV_TOOLS}/source/print.h
                    ${SPIRV_TOOLS}/source/software_version.cpp
                    ${SPIRV_TOOLS}/source/spirv_constant.h
                    ${SPIRV_TOOLS}/source/spirv_definition.h
                    ${SPIRV_TOOLS}/source/spirv_endian.cpp
                    ${SPIRV_TOOLS}/source/spirv_endian.h
                    ${SPIRV_TOOLS}/source/spirv_optimizer_options.cpp
                    ${SPIRV_TOOLS}/source/spirv_reducer_options.cpp
                    ${SPIRV_TOOLS}/source/spirv_target_env.cpp
                    ${SPIRV_TOOLS}/source/spirv_target_env.h
                    ${SPIRV_TOOLS}/source/spirv_validator_options.cpp
                    ${SPIRV_TOOLS}/source/spirv_validator_options.h
                    ${SPIRV_TOOLS}/source/table.cpp
                    ${SPIRV_TOOLS}/source/table.h
                    ${SPIRV_TOOLS}/source/table2.cpp
                    ${SPIRV_TOOLS}/source/table2.h
                    ${SPIRV_TOOLS}/source/text.cpp
                    ${SPIRV_TOOLS}/source/text.h
                    ${SPIRV_TOOLS}/source/text_handler.cpp
                    ${SPIRV_TOOLS}/source/text_handler.h
                    ${SPIRV_TOOLS}/source/to_string.cpp
                    ${SPIRV_TOOLS}/source/to_string.h
                    ${SPIRV_TOOLS}/source/util/bit_vector.cpp
                    ${SPIRV_TOOLS}/source/util/bit_vector.h
                    ${SPIRV_TOOLS}/source/util/bitutils.h
                    ${SPIRV_TOOLS}/source/util/hex_float.h
                    ${SPIRV_TOOLS}/source/util/parse_number.cpp
                    ${SPIRV_TOOLS}/source/util/parse_number.h
                    ${SPIRV_TOOLS}/source/util/string_utils.cpp
                    ${SPIRV_TOOLS}/source/util/string_utils.h
                    ${SPIRV_TOOLS}/source/util/timer.h)
add_library(spirv-opt STATIC ${SPIRV_TOOLS_SRC})
target_compile_features(spirv-opt PRIVATE cxx_std_20)
target_include_directories(spirv-opt PRIVATE ${SPIRV_TOOLS})
target_include_directories(spirv-opt PRIVATE ${SPIRV_TOOLS}/include)
target_include_directories(spirv-opt PRIVATE ${SPIRV_TOOLS}/include/generated)
target_include_directories(spirv-opt PRIVATE ${SPIRV_TOOLS}/source)
target_include_directories(spirv-opt PRIVATE ${SPIRV_HEADERS}/include)
if(MSVC)
  target_compile_options(spirv-opt PRIVATE /wd4127 /wd4389 /wd4702 /wd4706)
elseif(MINGW OR LINUX OR MACOSX)
  target_compile_options(spirv-opt PRIVATE -Wno-switch)
endif()
if(MINGW OR LINUX_GCC)
  target_compile_options(spirv-opt PRIVATE -Wno-misleading-indentation)
endif()

# spirv-cross
set(SPIRV_CROSS_SRC ${SPIRV_CROSS}/spirv.hpp
                    ${SPIRV_CROSS}/spirv_cfg.cpp
                    ${SPIRV_CROSS}/spirv_cfg.hpp
                    ${SPIRV_CROSS}/spirv_common.hpp
                    ${SPIRV_CROSS}/spirv_cpp.cpp
                    ${SPIRV_CROSS}/spirv_cpp.hpp
                    ${SPIRV_CROSS}/spirv_cross.cpp
                    ${SPIRV_CROSS}/spirv_cross.hpp
                    ${SPIRV_CROSS}/spirv_cross_parsed_ir.cpp
                    ${SPIRV_CROSS}/spirv_cross_parsed_ir.hpp
                    ${SPIRV_CROSS}/spirv_cross_util.cpp
                    ${SPIRV_CROSS}/spirv_cross_util.hpp
                    ${SPIRV_CROSS}/spirv_glsl.cpp
                    ${SPIRV_CROSS}/spirv_glsl.hpp
                    ${SPIRV_CROSS}/spirv_hlsl.cpp
                    ${SPIRV_CROSS}/spirv_hlsl.hpp
                    ${SPIRV_CROSS}/spirv_msl.cpp
                    ${SPIRV_CROSS}/spirv_msl.hpp
                    ${SPIRV_CROSS}/spirv_parser.cpp
                    ${SPIRV_CROSS}/spirv_parser.hpp
                    ${SPIRV_CROSS}/spirv_reflect.cpp
                    ${SPIRV_CROSS}/spirv_reflect.hpp)
add_library(spirv-cross STATIC ${SPIRV_CROSS_SRC})
target_compile_features(spirv-cross PRIVATE cxx_std_20)
target_include_directories(spirv-cross PRIVATE ${SPIRV_CROSS}/include)
target_compile_definitions(spirv-cross PRIVATE SPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS)
if(MINGW OR LINUX)
  target_compile_options(spirv-cross PRIVATE -Wno-maybe-uninitialized)
endif()
if(MSVC)
  target_compile_options(spirv-cross PRIVATE /wd4018 /wd4245 /wd4706 /wd4715)
elseif(MINGW OR LINUX OR MACOSX)
  target_compile_options(spirv-cross PRIVATE -Wno-type-limits)
endif()
if(MACOSX)
  target_compile_options(spirv-cross PRIVATE -Wno-deprecated-this-capture -Wno-nan-infinity-disabled)
endif()

# tint (Dawn's WGSL compiler, used by shaderc for WebGPU shaders)
file(GLOB_RECURSE TINT_SRC ${TINT}/src/tint/utils/*.cc ${TINT}/src/tint/utils/*.h
                           ${TINT}/src/tint/lang/core/*.cc ${TINT}/src/tint/lang/core/*.h
                           ${TINT}/src/tint/lang/null/*.cc ${TINT}/src/tint/lang/null/*.h
                           ${TINT}/src/tint/lang/spirv/*.cc ${TINT}/src/tint/lang/spirv/*.h
                           ${TINT}/src/tint/lang/wgsl/*.cc ${TINT}/src/tint/lang/wgsl/*.h
                           ${TINT}/src/tint/api/*.cc ${TINT}/src/tint/api/*.h)
add_library(tint STATIC ${TINT_SRC})
target_compile_features(tint PRIVATE cxx_std_20)
target_include_directories(tint PUBLIC ${TINT} ${TINT}/src/tint)
target_include_directories(tint PRIVATE ${TINT}/third_party/protobuf/src
                                        ${TINT}/third_party/abseil-cpp
                                        ${SPIRV_TOOLS}
                                        ${SPIRV_TOOLS}/include
                                        ${SPIRV_TOOLS}/include/generated
                                        ${SPIRV_HEADERS}/include)
target_compile_definitions(tint PRIVATE TINT_BUILD_GLSL_WRITER=0
                                        TINT_BUILD_HLSL_WRITER=0
                                        TINT_BUILD_MSL_WRITER=0
                                        TINT_BUILD_NULL_WRITER=0
                                        TINT_BUILD_SPV_READER=1
                                        TINT_BUILD_SPV_WRITER=0
                                        TINT_BUILD_WGSL_READER=0
                                        TINT_BUILD_WGSL_WRITER=1
                                        TINT_ENABLE_IR_VALIDATION=0)
if(WINDOWS)
  target_compile_definitions(tint PRIVATE TINT_BUILD_IS_LINUX=0 TINT_BUILD_IS_MAC=0 TINT_BUILD_IS_WIN=1)
elseif(MACOSX)
  target_compile_definitions(tint PRIVATE TINT_BUILD_IS_LINUX=0 TINT_BUILD_IS_MAC=1 TINT_BUILD_IS_WIN=0)
else()
  target_compile_definitions(tint PRIVATE TINT_BUILD_IS_LINUX=1 TINT_BUILD_IS_MAC=0 TINT_BUILD_IS_WIN=0)
endif()
if(MSVC)
  target_compile_options(tint PRIVATE /Zc:preprocessor)
endif()
if(MACOSX)
  target_compile_options(tint PRIVATE -Wno-unknown-warning-option)
endif()

# shaderc
file(GLOB SHADERC_SRC bgfx/tools/shaderc/*.cpp bgfx/tools/shaderc/*.h bgfx/src/vertexlayout.* bgfx/src/shader*)
add_executable(shaderc ${SHADERC_SRC})
target_compile_features(shaderc PRIVATE cxx_std_20)
target_include_directories(shaderc PRIVATE bx/include bimg/include bgfx/include)
target_include_directories(shaderc PRIVATE ${GLSLANG})
target_include_directories(shaderc PRIVATE ${SPIRV_CROSS})
target_include_directories(shaderc PRIVATE ${SPIRV_TOOLS}/include)
target_include_directories(shaderc PRIVATE ${TINT} ${TINT}/src)
if(NOT WINDOWS)
  # d3d4linux + directx-headers for legacy HLSL (SM 5.0) on linux/macos.
  # include/directx itself is needed for <dxcapi.h>/<d3d12shader.h> pulled in by
  # the DXIL path (SHADERC_CONFIG_HAS_DXC is on for linux; windows gets dxcapi.h
  # from the Windows SDK instead).
  target_include_directories(shaderc PRIVATE ${BGFX_3RD_DIR}/d3d4linux/include
                                             ${BGFX_3RD_DIR}/directx-headers/include
                                             ${BGFX_3RD_DIR}/directx-headers/include/directx
                                             ${BGFX_3RD_DIR}/directx-headers/include/wsl/stubs)
endif()
target_link_libraries(shaderc PRIVATE bx glslang spirv-opt spirv-cross tint)
if(MSVC)
  target_link_libraries(shaderc PRIVATE psapi)
endif()
if(MACOSX)
  find_library(COCOA_LIBRARY Cocoa)
  target_link_libraries(shaderc PRIVATE ${COCOA_LIBRARY})
endif()
if(LINUX)
  target_link_libraries(shaderc PRIVATE dl pthread)
endif()

# DXIL compiler is dynamically loaded at runtime - copy the needed binaries
# alongside shaderc (linux only; no macOS DXC library available)
set(DXCOMPILER_RUNTIME)
if(LINUX)
  set(DXCOMPILER_RUNTIME ${BGFX_3RD_DIR}/../tools/bin/linux/libdxcompiler.so)
elseif(WINDOWS)
  set(DXCOMPILER_RUNTIME ${BGFX_3RD_DIR}/../tools/bin/windows/dxcompiler.dll)
endif()
if(DXCOMPILER_RUNTIME)
  add_custom_command(TARGET shaderc
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DXCOMPILER_RUNTIME} $<TARGET_FILE_DIR:shaderc>)
endif()

if(NOT WINDOWS)
  add_custom_command(TARGET shaderc
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:shaderc>/shaderc ${CHERRYSODA_TOOL_PATH}/bin/shaderc.local)
else()
  add_custom_command(TARGET shaderc
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:shaderc>/shaderc.exe ${CHERRYSODA_TOOL_PATH}/bin/shaderc.local.exe)
endif()

# shaderc dlopens the DXC runtime for s_6_* (DXIL) profiles, so it must sit next to
# shaderc.local as well, not only next to the build-tree binary
set(DXC_RUNTIME_FOR_TOOLS)
if(LINUX)
  set(DXC_RUNTIME_FOR_TOOLS
      ${BGFX_3RD_DIR}/../tools/bin/linux/libdxcompiler.so
      ${BGFX_3RD_DIR}/../tools/bin/linux/libdxil.so)
elseif(WINDOWS)
  set(DXC_RUNTIME_FOR_TOOLS
      ${BGFX_3RD_DIR}/../tools/bin/windows/dxcompiler.dll
      ${BGFX_3RD_DIR}/../tools/bin/windows/dxil.dll)
endif()
foreach(dxc_runtime IN LISTS DXC_RUNTIME_FOR_TOOLS)
  add_custom_command(TARGET shaderc
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different ${dxc_runtime} ${CHERRYSODA_TOOL_PATH}/bin)
endforeach()
