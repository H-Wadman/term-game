set(utilsDir ${CMAKE_CURRENT_LIST_DIR}/utils PARENT_SCOPE)
set(utilsDir ${CMAKE_CURRENT_LIST_DIR}/utils)
set(applicationDir ${CMAKE_CURRENT_LIST_DIR}/application)
set(configDir ${CMAKE_CURRENT_LIST_DIR}/cmake-cfg)


# Ugly and bad but convenient
configure_file(${configDir}/build-path.h.in ${configDir}/build-path.h)

add_compile_definitions($<$<CONFIG:Debug>:DEBUG_FUNCTIONALITY>)

#Non local libraries
find_library(ncursesLib NAMES ncursesw ncurses)

function(add_subdirectory_targets_and_dependencies subdirList)
    foreach(subdir ${subdirList})
        add_subdirectory(${subdir})
    endforeach()
    foreach(subdir ${subdirList})
        include(${subdir}/dependencies.cmake)
    endforeach()
endfunction()

add_executable(run main.c)
list(APPEND subdirs "${utilsDir}" "${applicationDir}")
add_subdirectory_targets_and_dependencies("${subdirs}")

#run dependencies
target_link_libraries(run PRIVATE ${ncursesLib} start)
target_include_directories(run PRIVATE ${applicationDir})
