cmake_minimum_required(VERSION 3.28)
project(nCursesGame VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
else ()
  add_compile_options(-Wall -Wextra -Werror -Werror=vla -Wno-type-limits -Wno-initializer-overrides -g -fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif ()


set(asset_dir ${CMAKE_CURRENT_LIST_DIR}/assets)

add_subdirectory(source)
add_subdirectory(tests)
