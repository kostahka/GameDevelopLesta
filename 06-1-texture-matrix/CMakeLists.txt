cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(06-1-texture-matrix)

set(ENGINE_DEV_TEX ON CACHE BOOL "engine load game dll with hot reload")
set(ENGINE_GAME_LIB_NAME_TEX "game-texture" CACHE STRING
                                                  "game lib name to load")

add_library(game-texture SHARED src/game.cxx)

add_subdirectory(engine)

include_directories(${CMAKE_CURRENT_LIST_DIR}/engine/include)

add_custom_target(
    copy_texture_shaders ALL
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/shaders/
        ${CMAKE_CURRENT_BINARY_DIR}/
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets/
        ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMENT "Copying shaders and assets into binary directory")

add_dependencies(game-texture copy_texture_shaders)

#target_link_libraries(game-opengl PRIVATE game-engine)
