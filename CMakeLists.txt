cmake_minimum_required(VERSION 3.28)
project(nCursesGame VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_OSX_SYSROOT "macosx" CACHE STRING "Sysroot to use on MacOS in order to satisfy clang-tidy")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
else ()
  add_compile_options(-Wall -Wextra -Werror -Werror=vla -Wno-type-limits -Wno-initializer-overrides -g -fno-omit-frame-pointer)
endif ()

if ("${CMAKE_C_COMPILER}" MATCHES "clang")
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif ()


set(asset_dir ${CMAKE_CURRENT_LIST_DIR}/assets)

add_subdirectory(source)
add_subdirectory(tests)
