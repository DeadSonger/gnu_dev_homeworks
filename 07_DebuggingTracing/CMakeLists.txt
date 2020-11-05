cmake_minimum_required(VERSION 3.13)
project(wrapper C)

find_library(DL_LIBRARIES NAMES dl)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

add_library(wrapper SHARED wrapper.c)
target_link_libraries(wrapper ${DL_LIBRARIES})

enable_testing()

add_test("RemoveNotFixFile" "${CMAKE_SOURCE_DIR}/tests/can_remove_test.sh")
add_test("RemoveFixFile" "${CMAKE_SOURCE_DIR}/tests/cant_remove_test.sh")

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS wrapper)

