cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(04-2-opengl)

set(ENGINE_DEV ON CACHE BOOL "engine load game dll with hot reload")
set(ENGINE_GAME_LIB_NAME "game-opengl" CACHE STRING "game lib name to load")

add_library(game-opengl SHARED src/game.cxx)

add_subdirectory(engine)

add_custom_target(
    copy_shaders ALL
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/shaders/
        ${CMAKE_CURRENT_BINARY_DIR}/
    COMMENT "Copying shaders into binary directory")

add_dependencies(game-opengl copy_shaders)

#target_link_libraries(game-opengl PRIVATE game-engine)
