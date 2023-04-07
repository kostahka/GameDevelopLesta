cmake_minimum_required(VERSION 3.5)

project(sdl-static)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

add_executable(sdl-bin-static src/main.cxx)
target_link_libraries(sdl-bin-static PUBLIC SDL3::SDL3-static)

include(CTest)

add_test(NAME sdl-static-code-zero COMMAND sdl-bin-static)

install(TARGETS sdl-bin-static
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../bin/tests
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../bin/tests
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../bin/tests)
