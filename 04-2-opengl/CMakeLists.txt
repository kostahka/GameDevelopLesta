cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(04-2-opengl)

set(ENGINE_DEV ON CACHE BOOL "engine load game dll with hot reload")
set(ENGINE_GAME_LIB_NAME "game-opengl" CACHE STRING "game lib name to load")

add_library(game-opengl SHARED src/game.cxx)

add_subdirectory(engine)
