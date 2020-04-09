cmake_minimum_required(VERSION 2.8)

project(pbtolua)

set(pbtolua_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(pbtolua_SOURCE_DIR ${pbtolua_ROOT_DIR}/src)
set(pbtolua_THIRD_PARTY_DIR ${pbtolua_ROOT_DIR}/third_party)
set(pbtolua_BIN_DIR ${pbtolua_ROOT_DIR}/bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${pbtolua_ROOT_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${pbtolua_ROOT_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${pbtolua_ROOT_DIR}/bin)

include_directories(${pbtolua_SOURCE_DIR} ${pbtolua_THIRD_PARTY_DIR}/protobuf/include)
link_directories(${pbtolua_THIRD_PARTY_DIR}/protobuf/lib)

add_library(pbtolua
    ${pbtolua_SOURCE_DIR}/utils.h
    ${pbtolua_SOURCE_DIR}/tolua_generator.h
    ${pbtolua_SOURCE_DIR}/utils.cc
    ${pbtolua_SOURCE_DIR}/tolua_generator.cc)

target_link_libraries(pbtolua PUBLIC protoc protobuf)

add_executable(protoc_tolua ${pbtolua_SOURCE_DIR}/protoc_tolua.cc)
target_link_libraries(protoc_tolua pbtolua)

add_executable(protoc_gen_tolua ${pbtolua_SOURCE_DIR}/protoc_gen_tolua.cc)
target_link_libraries(protoc_gen_tolua pbtolua)
