cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(04-2-opengl)

set(ENGINE_DEV ON)
set(ENGINE_GAME_LIB_NAME game-opengl)

add_library(game-opengl SHARED src/game.cxx)

target_link_libraries(game-opengl PUBLIC game-engine)

add_subdirectory(engine)
