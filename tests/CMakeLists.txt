include(CTest)

add_executable(vec_test vec_test.c)
target_include_directories(vec_test PRIVATE ${utilsDir})
add_test(NAME Vector COMMAND vec_test)

add_executable(sudoku_test sudoku_test.c)
target_include_directories(sudoku_test PRIVATE ${utilsDir})
target_link_libraries(sudoku_test PRIVATE start)
add_test(NAME Sudoku COMMAND sudoku_test)
