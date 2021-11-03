CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

project(des_test)

file(GLOB ProjectSRC
        "../../crypt/des/*.cc"
        "*.cc")

add_executable(${PROJECT_NAME} ${ProjectSRC})

macro(do_test ARG1 ARG2)
    ADD_TEST(NAME ${PROJECT_NAME}_${ARG1}_${ARG2}
            COMMAND ${PROJECT_NAME} ${ARG1} ${ARG2})
    SET_TESTS_PROPERTIES(${PROJECT_NAME}_${ARG1}_${ARG2}
            PROPERTIES PASS_REGULAR_EXPRESSION ${ARG2})
endmacro(do_test)

do_test("12345678" "apple!!!")
do_test("87654321" "apple!!!")
do_test("apple!!!" "hello!!!")
do_test("hello!!!" "apple!!!")
