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

if (MSVC)
    add_compile_options(/W4)
    #add_compile_options(/Wall)
else()
    add_compile_options(-fprofile-arcs -ftest-coverage)
    add_link_options(-fprofile-arcs -ftest-coverage)
endif()

 

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

