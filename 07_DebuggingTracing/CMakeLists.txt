cmake_minimum_required(VERSION 3.13)
project(wrapper C)

find_library(DL_LIBRARIES NAMES dl)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

add_library(wrapper_sign SHARED wrapper.c)
target_compile_definitions(wrapper_sign PRIVATE -DUSE_LOGO)
target_link_libraries(wrapper_sign ${DL_LIBRARIES})

add_library(wrapper_custom SHARED wrapper.c)
target_compile_definitions(wrapper_custom PRIVATE -DWRAPPER_PRINT="Custom text")
target_link_libraries(wrapper_custom ${DL_LIBRARIES})

add_library(wrapper_args SHARED wrapper.c)
target_link_libraries(wrapper_args ${DL_LIBRARIES})

add_executable(main main.c)

add_custom_target(run_with_sign ${CMAKE_COMMAND} -E env LD_PRELOAD=${CMAKE_CURRENT_BINARY_DIR}/libwrapper_sign.so ./main DEPENDS wrapper_sign main)
add_custom_target(run_with_args ${CMAKE_COMMAND} -E env LD_PRELOAD=${CMAKE_CURRENT_BINARY_DIR}/libwrapper_args.so ./main DEPENDS wrapper_args main)
add_custom_target(run_simple ./main DEPENDS wrapper_sign main)
add_custom_target(run_custom ${CMAKE_COMMAND} -E env LD_PRELOAD=${CMAKE_CURRENT_BINARY_DIR}/libwrapper_custom.so ./main DEPENDS wrapper_custom main)

add_custom_target(
	test
       	COMMAND cp ${CMAKE_SOURCE_DIR}/test.sh ${CMAKE_CURRENT_BINARY_DIR}	
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test.sh 
	DEPENDS main wrapper_sign wrapper_args wrapper_custom)

