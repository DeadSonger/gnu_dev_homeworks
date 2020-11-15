cmake_minimum_required(VERSION 3.13)
project(localization C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

add_executable(game main.c)
target_compile_definitions(game PRIVATE LOCALE_PATH="user/share/locale")

add_executable(game_ru main.c)
add_dependencies(game_ru locale_gen)


add_custom_command(
	OUTPUT game.po
	DEPENDS update_pot
	COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/game.po" game.po
	COMMAND msgmerge -U game.po new_game.pot
)
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
