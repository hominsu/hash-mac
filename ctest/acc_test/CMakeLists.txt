CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

project(acc_test)

file(GLOB ProjectSRC
        "../../x_thread_pool/*.cc"
        "../../crypt/des/*.cc"
        "../../crypt/*.cc"
        "../../data/*.cc"
        "../../task/*.cc"
        "*.cc")

add_executable(${PROJECT_NAME} ${ProjectSRC})

macro(do_test ARG1)
    ADD_TEST(NAME ${PROJECT_NAME}_${ARG1}
            COMMAND ${PROJECT_NAME} ${ARG1})
    SET_TESTS_PROPERTIES(${PROJECT_NAME}_${ARG1}
            PROPERTIES PASS_REGULAR_EXPRESSION "pass")
endmacro(do_test)

do_test("111")