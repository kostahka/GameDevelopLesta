cmake_minimum_required(VERSION 3.5)

project(01-2-hellolib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(bin)
add_subdirectory(lib)

include(CTest)

add_test(NAME static-hello-result-zero COMMAND hello-bin-static)
add_test(NAME shared-hello-result-zero COMMAND hello-bin-shared)

if(WIN32)
    set_target_properties(hello-bin-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                      ${CMAKE_BINARY_DIR})

    set_target_properties(
        hello-lib-shared
        PROPERTIRES
        RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR})
endif()
