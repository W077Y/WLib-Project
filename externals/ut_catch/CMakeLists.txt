cmake_minimum_required (VERSION 3.19)

set(target_name "UT_CATCH")


message(STATUS "    -> ${target_name}")
add_library(${target_name} STATIC)

target_include_directories(${target_name}
 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc"
)

target_sources(${target_name} 
 PUBLIC "${CMAKE_CURRENT_LIST_DIR}/inc/catch.hpp"
)

target_sources(${target_name}
 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/inc/ut_catch.hpp"
 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/ut_catch.cpp"
)


