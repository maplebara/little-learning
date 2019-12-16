cmake_minimum_required(VERSION 3.0)

SET(current_dir      ${CMAKE_CURRENT_SOURCE_DIR})
SET(code_dir         ${current_dir}/code)
SET(ft_dir           ${current_dir}/ftest)
SET(3rd_dir          ${current_dir}/3rd)
SET(lib_dir          ${current_dir}/3rd/lib)

project(littleLearning)

add_compile_options(-std=c++11)

include_directories(
${code_dir}/inc
${3rd_dir}/gtest/inc/
)

FILE(GLOB SRC_LIST
    "${ft_dir}/src/*.cpp"
    "${code_dir}/src"
)

add_subdirectory(${3rd_dir}/gtest)

LINK_DIRECTORIES(${lib_dir})

ADD_EXECUTABLE(littleLearning ${SRC_LIST})

TARGET_LINK_LIBRARIES(littleLearning gtest pthread)
