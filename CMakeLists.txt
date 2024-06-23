cmake_minimum_required(VERSION 3.8)

project(GameEngine)

set(CMAKE_CXX_STANDARD 20)
add_compile_definitions(GLEW_STATIC)

file(GLOB_RECURSE SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE INCLUDE "${CMAKE_SOURCE_DIR}/include/*.h" "${CMAKE_SOURCE_DIR}/src/*.tpp")
file(GLOB_RECURSE LIB "${CMAKE_SOURCE_DIR}/lib/*.lib")

add_subdirectory(demo)

add_library(Generic ${SRC} ${INCLUDE})

target_include_directories(Generic PRIVATE include)
set_target_properties(Generic PROPERTIES LINKER_LANGUAGE CXX)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SRC} ${INCLUDE})