﻿cmake_minimum_required (VERSION 3.19)

set(TEST_MAIN_CPP "${CMAKE_CURRENT_LIST_DIR}/tst_main/tst_main.cpp" CACHE INTERNAL "path to tst_main.cpp")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/CRC")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/HASH")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/BLOB")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Callback")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Container")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Publisher")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/IO")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Memory_Storage")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Provider")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/String-Builder")

