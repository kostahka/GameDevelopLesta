cmake_minimum_required(VERSION 3.5)

project(01-1-hello-world LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01-1-hello-world main.cpp)

install(TARGETS 01-1-hello-world
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin/tests
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin/tests
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin/tests)

include(CTest)

add_test(NAME check_result_code_zero COMMAND 01-1-hello-world)
