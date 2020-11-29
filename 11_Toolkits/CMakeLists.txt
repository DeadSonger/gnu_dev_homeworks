cmake_minimum_required(VERSION 3.13)
project(toolkit C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
find_package (PkgConfig REQUIRED)
pkg_check_modules (GLIB2   glib-2.0)

add_executable(main counter.c)

target_include_directories(main PRIVATE ${GLIB2_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(main ${GLIB2_LIBRARIES})
