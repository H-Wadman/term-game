set(utilsDir ${CMAKE_CURRENT_LIST_DIR}/utils PARENT_SCOPE)
set(utilsDir ${CMAKE_CURRENT_LIST_DIR}/utils)
set(configDir ${CMAKE_CURRENT_LIST_DIR}/cmake-cfg)


# Ugly and bad but convenient
configure_file(${configDir}/build-path.h.in ${configDir}/build-path.h)

#Non local libraries
find_library(ncursesLib NAMES ncursesw ncurses)


add_executable(run main.c)
add_subdirectory(${utilsDir})
include(${utilsDir}/dependencies.cmake)


#run dependencies
target_link_libraries(run PRIVATE ${ncursesLib} start base)
target_include_directories(run PRIVATE ${utilsDir})
