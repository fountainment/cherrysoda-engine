# shaderc deps
set(BGFX_3RD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bgfx/3rdparty)
set(FCPP_DIR ${BGFX_3RD_DIR}/fcpp)
set(GLSLANG ${BGFX_3RD_DIR}/glslang)
set(GLSL_OPTIMIZER ${BGFX_3RD_DIR}/glsl-optimizer)
set(SPIRV_CROSS ${BGFX_3RD_DIR}/spirv-cross)
set(SPIRV_TOOLS ${BGFX_3RD_DIR}/spirv-tools)
set(SPIRV_HEADERS ${BGFX_3RD_DIR}/spirv-headers)
# fcpp
file(GLOB FCPP_SRC ${FCPP_DIR}/*.h
                   ${FCPP_DIR}/cpp1.c
                   ${FCPP_DIR}/cpp2.c
                   ${FCPP_DIR}/cpp3.c
                   ${FCPP_DIR}/cpp4.c
                   ${FCPP_DIR}/cpp5.c
                   ${FCPP_DIR}/cpp6.c)
add_library(fcpp STATIC ${FCPP_SRC})
target_compile_definitions(fcpp PRIVATE NINCLUDE=1 NWORK=65535 NBUFF=65535 OLD_PROCESSOR=0)
if(MSVC)
  target_compile_options(fcpp PRIVATE /wd4055 /wd4244 /wd4701 /wd4706)
else()
  target_compile_options(fcpp PRIVATE -Wno-implicit-fallthrough -Wno-incompatible-pointer-types -Wno-parentheses-equality)
endif()
# glslang
file(GLOB GLSLANG_SRC ${GLSLANG}/glslang/*/*.cpp
                      ${GLSLANG}/glslang/*/*.h
                      ${GLSLANG}/glslang/*/*/*.cpp
                      ${GLSLANG}/glslang/*/*/*.h
                      ${GLSLANG}/hlsl/*.cpp
                      ${GLSLANG}/hlsl/*.h
                      ${GLSLANG}/SPIRV/*.cpp
                      ${GLSLANG}/SPIRV/*.h
                      ${GLSLANG}/SPIRV//*/*.cpp
                      ${GLSLANG}/SPIRV/*/*.h
                      ${GLSLANG}/OGLCompilersDLL/*.cpp
                      ${GLSLANG}/OGLCompilersDLL/*.h)
list(REMOVE_ITEM GLSLANG_SRC ${GLSLANG}/glslang/OSDependent/Unix/main.cpp)
list(REMOVE_ITEM GLSLANG_SRC ${GLSLANG}/glslang/OSDependent/Windows/main.cpp)
if(WINDOWS)
  list(REMOVE_ITEM GLSLANG_SRC ${GLSLANG}/glslang/OSDependent/Unix/ossource.cpp)
else()
  list(REMOVE_ITEM GLSLANG_SRC ${GLSLANG}/glslang/OSDependent/Windows/ossource.cpp)
endif()
add_library(glslang STATIC ${GLSLANG_SRC})
target_compile_features(glslang PRIVATE cxx_std_14)
target_include_directories(glslang PRIVATE bgfx/3rdparty/glslang bgfx/3rdparty)
target_include_directories(glslang PRIVATE bgfx/3rdparty/spirv-tools/include)
target_include_directories(glslang PRIVATE bgfx/3rdparty/spirv-tools/source)
target_compile_definitions(glslang PRIVATE ENABLE_OPT=1 ENABLE_HLSL=1)
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
# glsl-optimizer
set(GLSL_OPTIMIZER_SRC ${GLSL_OPTIMIZER}/src/glsl/glcpp/glcpp.h
                       ${GLSL_OPTIMIZER}/src/glsl/glcpp/glcpp-lex.c
                       ${GLSL_OPTIMIZER}/src/glsl/glcpp/glcpp-parse.c
                       ${GLSL_OPTIMIZER}/src/glsl/glcpp/glcpp-parse.h
                       ${GLSL_OPTIMIZER}/src/glsl/glcpp/pp.c

                       ${GLSL_OPTIMIZER}/src/glsl/ast.h
                       ${GLSL_OPTIMIZER}/src/glsl/ast_array_index.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ast_expr.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ast_function.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ast_to_hir.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ast_type.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/builtin_functions.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/builtin_type_macros.h
                       ${GLSL_OPTIMIZER}/src/glsl/builtin_types.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/builtin_variables.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/glsl_lexer.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/glsl_optimizer.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/glsl_optimizer.h
                       ${GLSL_OPTIMIZER}/src/glsl/glsl_parser.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/glsl_parser.h
                       ${GLSL_OPTIMIZER}/src/glsl/glsl_parser_extras.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/glsl_parser_extras.h
                       ${GLSL_OPTIMIZER}/src/glsl/glsl_symbol_table.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/glsl_symbol_table.h
                       ${GLSL_OPTIMIZER}/src/glsl/glsl_types.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/glsl_types.h
                       ${GLSL_OPTIMIZER}/src/glsl/hir_field_selection.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_basic_block.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_basic_block.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_builder.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_builder.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_clone.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_constant_expression.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_equals.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_expression_flattening.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_expression_flattening.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_function.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_function_can_inline.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_function_detect_recursion.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_function_inlining.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_hierarchical_visitor.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_hierarchical_visitor.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_hv_accept.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_import_prototypes.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_optimization.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_print_glsl_visitor.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_print_glsl_visitor.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_print_metal_visitor.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_print_metal_visitor.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_print_visitor.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_print_visitor.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_rvalue_visitor.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_rvalue_visitor.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_stats.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_stats.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_uniform.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_unused_structs.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_unused_structs.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_validate.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_variable_refcount.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/ir_variable_refcount.h
                       ${GLSL_OPTIMIZER}/src/glsl/ir_visitor.h
                       ${GLSL_OPTIMIZER}/src/glsl/link_atomics.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/link_functions.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/link_interface_blocks.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/link_uniform_block_active_visitor.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/link_uniform_block_active_visitor.h
                       ${GLSL_OPTIMIZER}/src/glsl/link_uniform_blocks.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/link_uniform_initializers.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/link_uniforms.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/link_varyings.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/link_varyings.h
                       ${GLSL_OPTIMIZER}/src/glsl/linker.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/linker.h
                       ${GLSL_OPTIMIZER}/src/glsl/list.h
                       ${GLSL_OPTIMIZER}/src/glsl/loop_analysis.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/loop_analysis.h
                       ${GLSL_OPTIMIZER}/src/glsl/loop_controls.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/loop_unroll.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_clip_distance.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_discard.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_discard_flow.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_if_to_cond_assign.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_instructions.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_jumps.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_mat_op_to_vec.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_named_interface_blocks.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_noise.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_offset_array.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_output_reads.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_packed_varyings.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_packing_builtins.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_ubo_reference.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_variable_index_to_cond_assign.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_vec_index_to_cond_assign.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_vec_index_to_swizzle.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_vector.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_vector_insert.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/lower_vertex_id.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_algebraic.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_array_splitting.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_constant_folding.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_constant_propagation.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_constant_variable.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_copy_propagation.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_copy_propagation_elements.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_cse.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_dead_builtin_variables.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_dead_builtin_varyings.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_dead_code.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_dead_code_local.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_dead_functions.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_flatten_nested_if_blocks.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_flip_matrices.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_function_inlining.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_if_simplification.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_minmax.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_noop_swizzle.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_rebalance_tree.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_redundant_jumps.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_structure_splitting.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_swizzle_swizzle.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_tree_grafting.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/opt_vectorize.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/program.h
                       ${GLSL_OPTIMIZER}/src/glsl/s_expression.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/s_expression.h
                       ${GLSL_OPTIMIZER}/src/glsl/standalone_scaffolding.cpp
                       ${GLSL_OPTIMIZER}/src/glsl/standalone_scaffolding.h
                       ${GLSL_OPTIMIZER}/src/glsl/strtod.c
                       ${GLSL_OPTIMIZER}/src/glsl/strtod.h

                       ${GLSL_OPTIMIZER}/src/mesa/main/compiler.h
                       ${GLSL_OPTIMIZER}/src/mesa/main/config.h
                       ${GLSL_OPTIMIZER}/src/mesa/main/context.h
                       ${GLSL_OPTIMIZER}/src/mesa/main/core.h
                       ${GLSL_OPTIMIZER}/src/mesa/main/dd.h
                       ${GLSL_OPTIMIZER}/src/mesa/main/errors.h
                       ${GLSL_OPTIMIZER}/src/mesa/main/glheader.h
                       ${GLSL_OPTIMIZER}/src/mesa/main/glminimal.h
                       ${GLSL_OPTIMIZER}/src/mesa/main/imports.c
                       ${GLSL_OPTIMIZER}/src/mesa/main/imports.h
                       ${GLSL_OPTIMIZER}/src/mesa/main/macros.h
                       ${GLSL_OPTIMIZER}/src/mesa/main/mtypes.h
                       ${GLSL_OPTIMIZER}/src/mesa/main/simple_list.h

                       ${GLSL_OPTIMIZER}/src/mesa/program/hash_table.h
                       ${GLSL_OPTIMIZER}/src/mesa/program/prog_hash_table.c
                       ${GLSL_OPTIMIZER}/src/mesa/program/prog_instruction.h
                       ${GLSL_OPTIMIZER}/src/mesa/program/prog_parameter.h
                       ${GLSL_OPTIMIZER}/src/mesa/program/prog_statevars.h
                       ${GLSL_OPTIMIZER}/src/mesa/program/symbol_table.c
                       ${GLSL_OPTIMIZER}/src/mesa/program/symbol_table.h

                       ${GLSL_OPTIMIZER}/src/util/hash_table.c
                       ${GLSL_OPTIMIZER}/src/util/hash_table.h
                       ${GLSL_OPTIMIZER}/src/util/macros.h
                       ${GLSL_OPTIMIZER}/src/util/ralloc.c
                       ${GLSL_OPTIMIZER}/src/util/ralloc.h)
file(GLOB GLSL_OPTIMIZER_REMOVE_SRC ${GLSL_OPTIMIZER}/src/glsl/glcpp.c
                                    ${GLSL_OPTIMIZER}/src/glsl/glcpp/tests/*
                                    ${GLSL_OPTIMIZER}/src/glsl/glcpp/*.l
                                    ${GLSL_OPTIMIZER}/src/glsl/glcpp/*.y
                                    ${GLSL_OPTIMIZER}/src/glsl/ir_set_program_inouts.cpp
                                    ${GLSL_OPTIMIZER}/src/glsl/main.cpp
                                    ${GLSL_OPTIMIZER}/src/glsl/builtin_stubs.cpp)
list(REMOVE_ITEM GLSL_OPTIMIZER_SRC ${GLSL_OPTIMIZER_REMOVE_SRC})
add_library(glsl-optimizer STATIC ${GLSL_OPTIMIZER_SRC})
target_compile_features(glsl-optimizer PRIVATE cxx_std_14)
target_include_directories(glsl-optimizer PRIVATE ${GLSL_OPTIMIZER}/src)
target_include_directories(glsl-optimizer PRIVATE ${GLSL_OPTIMIZER}/include)
target_include_directories(glsl-optimizer PRIVATE ${GLSL_OPTIMIZER}/src/mesa)
target_include_directories(glsl-optimizer PRIVATE ${GLSL_OPTIMIZER}/src/mapi)
target_include_directories(glsl-optimizer PRIVATE ${GLSL_OPTIMIZER}/src/glsl)
if(MSVC)
  target_include_directories(glsl-optimizer PRIVATE ${GLSL_OPTIMIZER}/src/glsl/msvc)
  target_compile_definitions(glsl-optimizer PRIVATE __STDC__ __STDC_VERSION__=199901L strdup=_strdup alloca=_alloca isascii=__isascii)
  target_compile_options(glsl-optimizer PRIVATE /O1)
  target_compile_options(glsl-optimizer PRIVATE /wd4100 /wd4127 /wd4132 /wd4189 /wd4204 /wd4244 /wd4389 /wd4245 /wd4701 /wd4702 /wd4706 /wd4996)
else()
  target_compile_options(glsl-optimizer PRIVATE -O1)
endif()
if(MINGW OR LINUX OR MACOSX)
  target_compile_options(glsl-optimizer PRIVATE -fno-strict-aliasing
                                                -Wno-implicit-fallthrough
                                                -Wno-parentheses
                                                -Wno-sign-compare
                                                -Wno-unused-function
                                                -Wno-unused-parameter)
endif()
if(MACOSX)
  target_compile_options(glsl-optimizer PRIVATE -Wno-deprecated-register)
endif()
if(MINGW OR LINUX_GCC)
  target_compile_options(glsl-optimizer PRIVATE -Wno-misleading-indentation)
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
target_compile_features(spirv-cross PRIVATE cxx_std_14)
target_include_directories(spirv-cross PRIVATE ${SPIRV_CROSS}/include)
target_compile_definitions(spirv-cross PRIVATE SPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS)
if(MSVC)
  target_compile_options(spirv-cross PRIVATE /wd4018 /wd4245 /wd4706 /wd4715)
elseif(MINGW OR LINUX OR MACOSX)
  target_compile_options(spirv-cross PRIVATE -Wno-type-limits)
endif()
# spirv-tools
file(GLOB SPIRV_TOOLS_SRC ${SPIRV_TOOLS}/source/opt/*.cpp
                          ${SPIRV_TOOLS}/source/opt/*.h
                          ${SPIRV_TOOLS}/source/reduce/*.cpp
                          ${SPIRV_TOOLS}/source/reduce/*.h)
set(SPIRV_TOOLS_SRC ${SPIRV_TOOLS_SRC} ${SPIRV_TOOLS}/source/assembly_grammar.cpp
                                       ${SPIRV_TOOLS}/source/assembly_grammar.h
                                       ${SPIRV_TOOLS}/source/binary.cpp
                                       ${SPIRV_TOOLS}/source/binary.h
                                       ${SPIRV_TOOLS}/source/cfa.h
                                       ${SPIRV_TOOLS}/source/diagnostic.cpp
                                       ${SPIRV_TOOLS}/source/diagnostic.h
                                       ${SPIRV_TOOLS}/source/disassemble.cpp
                                       ${SPIRV_TOOLS}/source/disassemble.h
                                       ${SPIRV_TOOLS}/source/enum_set.h
                                       ${SPIRV_TOOLS}/source/enum_string_mapping.cpp
                                       ${SPIRV_TOOLS}/source/enum_string_mapping.h
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
                                       ${SPIRV_TOOLS}/source/text.cpp
                                       ${SPIRV_TOOLS}/source/text.h
                                       ${SPIRV_TOOLS}/source/text_handler.cpp
                                       ${SPIRV_TOOLS}/source/text_handler.h
                                       ${SPIRV_TOOLS}/source/util/bit_vector.cpp
                                       ${SPIRV_TOOLS}/source/util/bit_vector.h
                                       ${SPIRV_TOOLS}/source/util/bitutils.h
                                       ${SPIRV_TOOLS}/source/util/hex_float.h
                                       ${SPIRV_TOOLS}/source/util/parse_number.cpp
                                       ${SPIRV_TOOLS}/source/util/parse_number.h
                                       ${SPIRV_TOOLS}/source/util/string_utils.cpp
                                       ${SPIRV_TOOLS}/source/util/string_utils.h
                                       ${SPIRV_TOOLS}/source/util/timer.h
                                       ${SPIRV_TOOLS}/source/val/basic_block.cpp
                                       ${SPIRV_TOOLS}/source/val/construct.cpp
                                       ${SPIRV_TOOLS}/source/val/decoration.h
                                       ${SPIRV_TOOLS}/source/val/function.cpp
                                       ${SPIRV_TOOLS}/source/val/instruction.cpp
                                       ${SPIRV_TOOLS}/source/val/validate.cpp
                                       ${SPIRV_TOOLS}/source/val/validate.h
                                       ${SPIRV_TOOLS}/source/val/validate_adjacency.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_annotation.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_arithmetics.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_atomics.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_barriers.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_bitwise.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_builtins.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_capability.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_cfg.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_composites.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_constants.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_conversion.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_debug.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_decorations.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_derivatives.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_execution_limitations.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_extensions.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_function.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_id.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_image.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_instruction.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_interfaces.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_layout.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_literals.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_logicals.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_memory.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_memory_semantics.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_misc.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_mode_setting.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_non_uniform.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_primitives.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_scopes.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_small_type_uses.cpp
                                       ${SPIRV_TOOLS}/source/val/validate_type.cpp
                                       ${SPIRV_TOOLS}/source/val/validation_state.cpp)
add_library(spirv-opt STATIC ${SPIRV_TOOLS_SRC})
target_compile_features(spirv-opt PRIVATE cxx_std_14)
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

# shaderc
file(GLOB SHADERC_SRC bgfx/tools/shaderc/*.cpp bgfx/tools/shaderc/*.h bgfx/src/vertexlayout.* bgfx/src/shader_spirv.*)
add_executable(shaderc ${SHADERC_SRC})
target_compile_features(shaderc PRIVATE cxx_std_14)
target_include_directories(shaderc PRIVATE bx/include bimg/include bgfx/include)
target_include_directories(shaderc PRIVATE bgfx/3rdparty/webgpu/include)
target_include_directories(shaderc PRIVATE bgfx/3rdparty/dxsdk/include)
target_include_directories(shaderc PRIVATE ${FCPP_DIR})
target_include_directories(shaderc PRIVATE ${GLSLANG}/glslang/Public)
target_include_directories(shaderc PRIVATE ${GLSLANG}/glslang/Include)
target_include_directories(shaderc PRIVATE ${GLSLANG})
target_include_directories(shaderc PRIVATE ${GLSL_OPTIMIZER}/include)
target_include_directories(shaderc PRIVATE ${GLSL_OPTIMIZER}/src/glsl)
target_include_directories(shaderc PRIVATE ${SPIRV_CROSS})
target_include_directories(shaderc PRIVATE ${SPIRV_TOOLS}/include)
target_link_libraries(shaderc PRIVATE bx fcpp glslang glsl-optimizer spirv-opt spirv-cross)
if(MSVC)
  target_include_directories(shaderc PRIVATE ${GLSL_OPTIMIZER}/include/c99)
endif()
if(MACOS)
  target_link_libraries(shaderc PRIVATE Cocoa.framework)
endif()
if(WINDOWS)
  target_link_libraries(shaderc PRIVATE psapi)
elseif(LINUX OR MACOSX)
  target_link_libraries(shaderc PRIVATE dl pthread)
endif()
