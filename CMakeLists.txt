cmake_minimum_required(VERSION 3.28)
project(nCursesGame VERSION 0.1.0 LANGUAGES C)
include(CTest)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
else ()
  add_compile_options(-Wall -Wextra -Werror -Werror=vla -Wno-type-limits -g -fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif ()

set(utilsDir ${CMAKE_CURRENT_SOURCE_DIR}/utils)
set(configDir ${CMAKE_CURRENT_SOURCE_DIR}/cmake-cfg)

configure_file(${configDir}/build-path.h.in ${configDir}/build-path.h)

add_executable(run main.c)
add_library(utf8 ${utilsDir}/utf8.c)
add_library(menu ${utilsDir}/menu.c)
add_library(menu_constants ${utilsDir}/menu_constants.c)
add_library(start start.c)
add_library(state ${utilsDir}/state.c)

#start dependencies
target_include_directories(start PRIVATE ${utilsDir})
target_link_libraries(start PRIVATE menu state)

#menu_constants dependencies
target_include_directories(menu_constants PRIVATE ${CMAKE_SOURCE_DIR})

#menu dependencies
target_include_directories(menu PRIVATE ${configDir})
target_link_libraries(menu PRIVATE utf8)

#state dependencies
target_include_directories(start PRIVATE ${utilsDir})

#run dependencies
target_include_directories(run PRIVATE ${utilsDir})
find_library(ncursesLib NAMES ncursesw ncurses)
target_link_libraries(run PRIVATE ${ncursesLib} utf8 menu menu_constants start)

add_subdirectory(tests)
