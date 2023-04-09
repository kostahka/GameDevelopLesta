cmake_minimum_required(VERSION 3.5)

project(sdl3-lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

add_executable(sdl-bin-static src/main.cxx)
target_link_libraries(sdl-bin-static PUBLIC SDL3::SDL3-static)

add_executable(sdl-bin-shared src/main.cxx)
target_link_libraries(sdl-bin-shared PUBLIC SDL3::SDL3-shared)

include(CTest)

add_test(NAME sdl-static-code-zero COMMAND sdl-bin-static)
add_test(NAME sdl-shared-code-zero COMMAND sdl-bin-shared)

install(TARGETS sdl-bin-static sdl-bin-shared
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../bin/tests
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../bin/tests
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../bin/tests)
