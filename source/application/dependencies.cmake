# menu_constants dependencies
target_include_directories(menu_constants PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${utilsDir})
target_link_libraries(menu_constants PRIVATE start base PUBLIC menu)
# start dependencies
target_include_directories(start PRIVATE ${CMAKE_CURRENT_LIST_DIR} PUBLIC ${utilsDir})
target_link_libraries(start PRIVATE menu state ${ncursesLib} menu_constants sudoku PUBLIC base)

