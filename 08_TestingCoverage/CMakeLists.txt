cmake_minimum_required(VERSION 3.13)
project(growBuffer C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O0  -fprofile-arcs -ftest-coverage")

add_library(grow_buffer STATIC buf.c)

add_executable(tests tests.c)
target_link_libraries(tests grow_buffer)

include(CTest)

add_test(test_buffer_init tests 0)
add_test(test_buffer_push_and_free tests 1)
add_test(test_buffer_push tests 2)
add_test(test_buffer_clear_empty tests 3)
add_test(test_buffer_clear_not_empty tests 4)
add_test(test_buffer_pop tests 5)
add_test(test_buffer_grow_and_trunc tests 6)
add_test(test_buffer_grow_mem_failure tests 7)
add_test(test_buffer_trunc_mem_failure tests 8)
add_test(test_buffer_two_trunc_mem_failure tests 9)
add_test(buffer_benchmark tests 10)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS tests)
add_custom_target(check_coverage COMMAND make ExperimentalCoverage DEPENDS check)
