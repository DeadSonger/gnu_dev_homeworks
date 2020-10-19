cmake_minimum_required(VERSION 3.13)

project(pcredemo C)

option(without-UTF "disable PCRE2_UCP definition" FALSE)

set(SOURCES window.c own_regexp.c)

set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)

find_library(PCRE2_LIBRARIES NAMES pcre2-8)
find_path(PCRE2_INCLUDE_DIRS pcre2.h)
if (NOT PCRE2_LIBRARIES OR NOT PCRE2_INCLUDE_DIRS)
	message(FATAL_ERROR "Can't find pcre2 library")
endif()

include_directories(${CURSES_INCLUDE_DIRS} ${PCRE2_INCLUDE_DIRS}) 

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

if (without-UTF)
	message(STATUS "Compile project without PCRE2_UCP")
	add_definitions("-DWITHOUT_UTF")
endif()	

add_executable(pcredemo ${SOURCES})

target_link_libraries(pcredemo ${CURSES_LIBRARIES} ${PCRE2_LIBRARIES})


add_custom_target(allclean)
set(allclean_files CMakeCache.txt *.cmake Makefile CMakeFiles ~* pcredemo)
add_custom_command(DEPENDS clean COMMAND rm ARGS -rf ${allclean_files} TARGET allclean)

