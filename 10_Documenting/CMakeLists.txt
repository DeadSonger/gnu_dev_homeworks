cmake_minimum_required(VERSION 3.13)
project(localization C)

option(BUILD_DOC "Build documentation" ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

add_executable(game main.c)
target_compile_definitions(game PRIVATE LOCALE_PATH="user/share/locale")

add_executable(game_ru main.c)
add_dependencies(game_ru locale_gen)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/title.dox
        DEPENDS game
        COMMAND ./game --help-dx > ${CMAKE_CURRENT_SOURCE_DIR}/title.dox
    )
    
    add_custom_command(
        OUTPUT html
        DEPENDS title.dox
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/html ${CMAKE_CURRENT_BINARY_DIR}/latex
        COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/html ${CMAKE_CURRENT_BINARY_DIR}/html
        COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/latex ${CMAKE_CURRENT_BINARY_DIR}/latex
        COMMAND rm ${CMAKE_CURRENT_SOURCE_DIR}/title.dox
    )
    
    add_custom_target(docs DEPENDS html)

endif (DOXYGEN_FOUND)

find_program(HELP2MAN_FOUND help2man)
if (NOT HELP2MAN_FOUND)
    message(FATAL_ERROR "Can't find help2man tool")
endif()

add_custom_command(
    OUTPUT game.1
    COMMAND help2man -o game.1 --name="game" ./game
    DEPENDS game
)

add_custom_command(
    OUTPUT game.1.gz
    DEPENDS game.1
    COMMAND gzip game.1
)
add_custom_target(man DEPENDS game.1.gz)

add_custom_command(
	OUTPUT game.po
	DEPENDS update_pot
	COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/game.po" game.po
	COMMAND msgmerge -U game.po new_game.pot)

add_custom_target(update_po DEPENDS game.po)

add_custom_command(
	OUTPUT new_game.pot
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/game.pot" "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
	COMMAND xgettext -k_ -c "${CMAKE_CURRENT_SOURCE_DIR}/main.c" -o update_game.pot
	COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/game.po" new_game.pot
	COMMAND msgmerge -U new_game.pot update_game.pot
)
add_custom_target(update_pot DEPENDS new_game.pot)

add_custom_command(
	OUTPUT "ru/LC_MESSAGES/game.mo"
	DEPENDS update_po
	COMMAND mkdir -p "ru/LC_MESSAGES/"
	COMMAND msgfmt new_game.pot -o "ru/LC_MESSAGES/game.mo"
)
add_custom_target(locale_gen DEPENDS "ru/LC_MESSAGES/game.mo")
