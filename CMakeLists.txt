cmake_minimum_required(VERSION 3.10)
project(iLab-ParaCL)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

flex_target(
  scaner
  ${CMAKE_CURRENT_SOURCE_DIR}/Parser/Scaner.l
  ${CMAKE_CURRENT_BINARY_DIR}/src/Parser/lex.cpp
)
bison_target(
  parser
  ${CMAKE_CURRENT_SOURCE_DIR}/Parser/Parser.y
  ${CMAKE_CURRENT_BINARY_DIR}/src/Parser/Parser.cpp
  COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/src/Parser/Parser.h"
)
add_flex_bison_dependency(scaner parser)

if(NOT DEFINED LLVM_PATH)
    message(
        FATAL_ERROR
        "Can`t find llvm library, use -DLLVM_PATH=<path_to_llvm_dir>"
    )
endif()
set(LLVM_CONFIG_PATH "${LLVM_PATH}/bin/llvm-config")
execute_process(COMMAND ${LLVM_CONFIG_PATH} --ldflags --system-libs --libs all OUTPUT_VARIABLE LLVM_FLAGS)
string(REGEX REPLACE "\n" " " LLVM_FLAGS "${LLVM_FLAGS}")
string(REGEX REPLACE " $" "" LLVM_FLAGS "${LLVM_FLAGS}")

link_directories(${LLVM_PATH}/lib)
include_directories(${LLVM_PATH}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src/Parser)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTAnalys.cpp PROPERTIES COMPILE_FLAGS "-frtti")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNode.cpp PROPERTIES COMPILE_FLAGS "-frtti")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/Codegen/Codegen.cpp PROPERTIES COMPILE_FLAGS "-frtti")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/Codegen/CGVisitor.cpp PROPERTIES COMPILE_FLAGS "-frtti")
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/src/Parser/lex.cpp PROPERTIES COMPILE_FLAGS "-frtti")
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/src/Parser/Parser.cpp PROPERTIES COMPILE_FLAGS "-frtti")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/Parser/Driver.cpp PROPERTIES COMPILE_FLAGS "-frtti")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/Runtime/Runtime.cpp PROPERTIES COMPILE_FLAGS "")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/Interpret/Interpret.cpp PROPERTIES COMPILE_FLAGS "-frtti")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/main.cpp PROPERTIES COMPILE_FLAGS "-fno-rtti")

set(
  src
  ${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTAnalys.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNode.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Codegen/Codegen.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Codegen/CGVisitor.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/src/Parser/lex.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/src/Parser/Parser.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Parser/Driver.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Runtime/Runtime.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Interpret/Interpret.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
add_executable(ParaCL ${src})
target_link_libraries(ParaCL ${LLVM_FLAGS})