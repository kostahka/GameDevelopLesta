cmake_minimum_required(VERSION 3.5)

project(hello-lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(hello-lib-static STATIC include/hello-lib.hxx src/hello-lib.cxx)
target_include_directories(hello-lib-static
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(hello-lib-shared SHARED include/hello-lib.hxx src/hello-lib.cxx)
target_include_directories(hello-lib-shared
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS hello-lib-static hello-lib-shared
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../bin/tests
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../bin/tests
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../bin/tests)
