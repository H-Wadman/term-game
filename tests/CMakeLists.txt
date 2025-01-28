include(CTest)

add_executable(vec_test vec_test.c)
target_include_directories(vec_test PRIVATE ${utilsDir})
add_test(NAME Vector COMMAND vec_test)
