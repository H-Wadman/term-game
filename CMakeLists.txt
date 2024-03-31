cmake_minimum_required(VERSION 3.28)
project(nCursesGame VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
else ()
    add_compile_options(-Wall -Wextra -Werror -g -fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif ()

set(utilsDir ${CMAKE_CURRENT_SOURCE_DIR}/utils)


add_executable(run main.c)
add_library(utils ${utilsDir}/utils.c)


target_include_directories(run PRIVATE ${utilsDir})
target_include_directories(utils PRIVATE ${utilsDir})

find_library(ncursesLib ncurses)
target_link_libraries(run PRIVATE ${ncursesLib} utils)
