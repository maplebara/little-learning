cmake_minimum_required(VERSION 3.0)

SET(current_dir      ${CMAKE_CURRENT_SOURCE_DIR})
SET(code_dir         ${current_dir}/code)
SET(ft_dir           ${current_dir}/ftest)
SET(3rd_dir          ${current_dir}/3rd)
SET(lib_dir          ${current_dir}/3rd/lib)

project(littleLearning)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ggdb -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb -std=c11")

include_directories( 
${code_dir}/inc
${3rd_dir}/gtest/inc/
${ft_dir}/inc
${current_dir}/levelDb/util/inc
${current_dir}/levelDb/repo/inc
${current_dir}/levelDb/net
${current_dir}/levelDb/db/inc
)

FILE(GLOB_RECURSE  SRC_LIST
    "${ft_dir}/src/*.cpp"
    "${code_dir}/src/*"
    "${current_dir}/levelDb/db/*"
    "${current_dir}/levelDb/net/*"
    "${current_dir}/levelDb/repo/*"
    "${current_dir}/levelDb/util/*"
)

add_subdirectory(${3rd_dir}/gtest)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(leveldb REQUIRED)
if(leveldb_FOUND)
    include_directories(${leveldb_INCLUDE_DIRS})
endif()

LINK_DIRECTORIES(${lib_dir})

ADD_EXECUTABLE(littleLearning ${SRC_LIST})

TARGET_LINK_LIBRARIES(littleLearning hiredis leveldb gtest event event_pthreads pthread)
