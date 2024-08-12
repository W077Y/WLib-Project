cmake_minimum_required (VERSION 3.19)


set(target_name "TEST_WLIB_CRC")
#message(STATUS "########################|")
 message(STATUS "    -> ${target_name}")

add_executable(${target_name})

target_include_directories(${target_name}
 PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
)

target_sources(${target_name}
 PRIVATE "${TEST_MAIN_CPP}"
 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_wlib-CRC_8.cpp"
 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_wlib-CRC_16_ccitt_false.cpp"
 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_wlib-CRC_16_ccitt_zero.cpp"
 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_wlib-CRC_32.cpp"
 PRIVATE "${CMAKE_CURRENT_LIST_DIR}/tst_wlib-CRC_64_go_iso.cpp"
)

target_link_libraries(${target_name} PRIVATE UT_CATCH)
target_link_libraries(${target_name} PUBLIC WLIB)

add_test(${target_name} ${target_name})



