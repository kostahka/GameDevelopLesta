cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(game-engine-loop-lib)

find_package(SDL3 REQUIRED)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

#STATIC
add_library(engine-static STATIC include/engine.hxx src/engine.cxx)
target_include_directories(engine-static
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(engine-static PRIVATE SDL3::SDL3-static)

add_executable(game-static src/game.cxx)

target_link_libraries(game-static PUBLIC engine-static)

#SHARED
add_library(engine-shared SHARED include/engine.hxx src/engine.cxx)
target_include_directories(engine-shared
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(engine-shared PRIVATE SDL3::SDL3-static)

add_executable(game-shared src/game.cxx)

target_link_libraries(game-shared PUBLIC engine-shared)
