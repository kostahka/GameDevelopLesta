cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(sdl-loop)

find_package(SDL3 REQUIRED)

add_executable(bin-sdl-loop src/main.cxx)

target_link_libraries(bin-sdl-loop PUBLIC SDL3::SDL3-static)

install(TARGETS bin-sdl-loop
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../bin/tests
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../bin/tests
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../bin/tests)
