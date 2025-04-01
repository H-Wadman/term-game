include_directories(${utilsDir})

add_library(base base.c)
add_library(menu menu.c)
add_library(vec vec.c)
add_library(utf8 io/utf8.c)
add_library(logging io/logging.c)
add_library(witness games/witness.c)
add_library(sudoku games/sudoku.c)
