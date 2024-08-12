cmake_minimum_required(VERSION 3.19)



project(PROJECT_WLib
 VERSION 0.0.0.0
 DESCRIPTION "A dummy Project"
 HOMEPAGE_URL ""
 LANGUAGES C CXX ASM
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

message(STATUS "########################")
message(STATUS "##  ${CMAKE_PROJECT_NAME}")
message(STATUS "########################")

message(STATUS "##  EXTERNALS")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/externals")
message(STATUS "########################")

message(STATUS "##  LIBS")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/wlib")
message(STATUS "########################")

message(STATUS "##  TESTS")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tst")
message(STATUS "########################")


include(CTest)

