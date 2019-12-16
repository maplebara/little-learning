cmake_minimum_required(VERSION 3.0)

SET(current_dir     ${CMAKE_CURRENT_SOURCE_DIR})
SET(ft_dir          ${current_dir}/ftest)
SET(3rd_dir         ${current_dir}/3rd)
SET(lib_dir         ${current_dir}/3rd/lib)

project(littleLearning)

add_compile_options(-std=c++11)

include_directories(
basic/inc
algorithm/inc
concurrent/inc
3rd/gtest/inc/
)

FILE(GLOB SRC_LIST
    "${ft_dir}/src/*.cpp"
)

add_subdirectory(${3rd_dir}/gtest)

LINK_DIRECTORIES(${lib_dir})

ADD_EXECUTABLE(littleLearning ${SRC_LIST})

TARGET_LINK_LIBRARIES(littleLearning gtest pthread)
