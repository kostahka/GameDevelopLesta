cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(hello_sdl)

find_package(SDL3 REQUIRED)

add_executable(bin-hello-sdl src/main.cxx)

target_link_libraries(bin-hello-sdl PUBLIC SDL3::SDL3-static)

install(TARGETS bin-hello-sdl
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../bin/tests
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../bin/tests
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../bin/tests)
