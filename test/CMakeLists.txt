cmake_minimum_required(VERSION 3.20)

# A helper function that wraps add_test with the pattern we are using for testing
# e.g. we have 1 source file that allways links camarg 
macro(camarg_test)
    cmake_parse_arguments(TEST "" "NAME" "" ${ARGN} )
    message("test: ${TEST_NAME}")
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME} camarg)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} )
endmacro()

camarg_test(NAME single_flag)
camarg_test(NAME single_arg)
camarg_test(NAME single_cmd)
camarg_test(NAME invalid_flag)
camarg_test(NAME invalid_arg)
camarg_test(NAME invalid_cmd)