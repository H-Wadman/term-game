# base dependencies 
target_link_libraries(base PRIVATE logging menu_constants)

# utf8 dependencies
target_link_libraries(utf8 PRIVATE logging)


# Menu subdirectory
# menu_constants dependencies
target_include_directories(menu_constants PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(menu_constants PRIVATE start base)
# menu dependencies
target_include_directories(menu PRIVATE ${configDir})
target_link_libraries(menu PRIVATE utf8 logging ${ncursesLib})
# start dependencies
target_link_libraries(start PRIVATE menu state ${ncursesLib} menu_constants)

# Games subdirectory
# witness dependencies
target_link_libraries(witness PRIVATE utf8 vec logging base ${ncursesLib})
# sudoku dependencies
target_link_libraries(sudoku PRIVATE base ${ncursesLib})

