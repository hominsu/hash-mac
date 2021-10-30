cmake_minimum_required(VERSION 3.20)
project(Des)

set(CMAKE_CXX_STANDARD 17)

# ============================== select operating system  ==============================
message("")
message("Operation system is ${CMAKE_SYSTEM}")
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    message("Current platform: Linux")
    link_directories(/usr/local/lib)
    include_directories(/usr/local/include)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    message("Current platform: Darwin")
    link_directories(/opt/homebrew/lib)
    include_directories(/opt/homebrew/include)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    message("Current platform: Windows")
else ()
    message(FATAL_ERROR "Platform ${CMAKE_SYSTEM_NAME} is not support for this project")
endif ()


# ============================== find_package ==============================
# Threads
find_package(Threads REQUIRED)
if (Threads_FOUND)
    message("")
endif ()


# ============================== add executable and link libraries ==============================
file(GLOB ProjectSRC
        "crypt/des/*cc"
        "crypt/*cc"
        "task/*.cc"
        "*.cc")

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DDebug)
    message(STATUS "CMake Build Type: Debug")
    message("")
elseif (CMAKE_BUILD_TYPE STREQUAL Release)
    ADD_DEFINITIONS(-DRelease)
    message(STATUS "CMake Build Type: Release")
    message("")
endif ()

option(BUILD_TEST "Whether or not to build the test" ON)
if (${BUILD_TEST})
    message(STATUS "Building tests")
    enable_testing()
    add_subdirectory(test/des_test)
    add_subdirectory(test/crypt_test)
endif ()

add_executable(${PROJECT_NAME} ${ProjectSRC})
target_link_libraries(${PROJECT_NAME})
