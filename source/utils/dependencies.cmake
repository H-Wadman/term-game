# base dependencies 
target_link_libraries(base PRIVATE logging)

# utf8 dependencies
target_link_libraries(utf8 PRIVATE logging)


# menu dependencies
target_include_directories(menu PRIVATE ${configDir})
target_link_libraries(menu PRIVATE utf8 logging ${ncursesLib})

# Games subdirectory
# witness dependencies
target_link_libraries(witness PRIVATE utf8 vec logging base ${ncursesLib})
# sudoku dependencies
target_link_libraries(sudoku PRIVATE base ${ncursesLib})

