cmake_minimum_required(VERSION 3.28)
project(nCursesGame VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_OSX_SYSROOT "macosx" CACHE STRING "Sysroot to use on MacOS in order to satisfy clang-tidy")

set(warning_flags -Wall -Wextra -Werror -Werror=vla -Wno-type-limits -Wno-initializer-overrides)
set(debug_flags ${warning_flags};-g;-fno-omit-frame-pointer)

add_compile_options(
    "$<$<CONFIG:Debug>:${debug_flags}>"
    "$<$<CONFIG:Release>:-O3>"
)
if ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    add_compile_options("$<$<CONFIG:Debug>:-fsanitize=address,undefined>")
    add_link_options("$<$<CONFIG:Debug>:-fsanitize=address,undefined>")
endif ()


set(asset_dir ${CMAKE_CURRENT_LIST_DIR}/assets)

add_subdirectory(source)
add_subdirectory(tests)
