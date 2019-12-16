cmake_minimum_required(VERSION 3.0)

SET(current_dir      ${CMAKE_CURRENT_SOURCE_DIR})   
SET(inc_dir          ${current_dir}/inc)
SET(src_dir          ${current_dir}/src)

INCLUDE_DIRECTORIES(
    "${inc_dir}"
    "${src_dir}/../"
)

FILE(GLOB source      "${src_dir}/*.cc")

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${current_dir}/../lib)

ADD_LIBRARY(gtest ${source})

SET_TARGET_PROPERTIES(gtest PROPERTIES  LINKER_LANGUAGE CXX)
