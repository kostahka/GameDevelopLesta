cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(engine-texture)

find_package(SDL3 REQUIRED)

#add_subdirectory(efsw)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(ENGINE_DEV_TEX OFF CACHE BOOL "engine load game dll with hot reload")
set(ENGINE_GAME_LIB_NAME_TEX "" CACHE STRING "game lib name to load")

set(GLAD_SRC "glad/src/glad.c")

set(ENGINE_SRC
    "src/engine.cxx"
    "src/shader-program.cxx"
    "src/vertex-array-object.cxx"
    "src/file-last-modify-listener.cxx"
    "src/user-events.cxx"
    "src/handle-user-event.hxx"
    "src/handle-file-modify.hxx"
    "src/picopng.hxx"
    "src/texture.cxx"
    "include/texture.hxx"
    "include/user-events.hxx"
    "include/file-last-modify-listener.hxx"
    "include/engine.hxx"
    "include/shader-program.hxx"
    "include/vertex-array-object.hxx"
    "include/color.hxx"
    "include/transform3d.hxx"
    "include/vertex.hxx"
    ${GLAD_SRC})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/glad/include)

if(ENGINE_DEV_TEX)

    add_executable(engine-texture ${ENGINE_SRC})
    target_link_libraries(engine-texture PRIVATE SDL3::SDL3-static)
    target_compile_definitions(engine-texture PUBLIC ENGINE_DEV)
    target_compile_definitions(
        engine-texture
        PUBLIC ENGINE_GAME_LIB_NAME="${ENGINE_GAME_LIB_NAME_TEX}")

    target_link_libraries(engine-texture PRIVATE efsw)

    set_target_properties(engine-texture PROPERTIES ENABLE_EXPORTS ON)

    target_include_directories(${ENGINE_GAME_LIB_NAME_TEX}
                               PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_compile_definitions(${ENGINE_GAME_LIB_NAME_TEX} PUBLIC ENGINE_DEV)
    target_compile_definitions(
        ${ENGINE_GAME_LIB_NAME_TEX}
        PUBLIC ENGINE_GAME_LIB_NAME="${ENGINE_GAME_LIB_NAME_TEX}")

    #target_link_libraries(engine-texture INTERFACE ${ENGINE_GAME_LIB_NAME_TEX})
else()

    #Shared engine lib
    add_library(game-engine-shared SHARED ${ENGINE_SRC})
    target_link_libraries(game-engine-shared PRIVATE SDL3::SDL3-static)

    target_link_libraries(game-engine-shared PRIVATE efsw)

    #Static engine lib
    add_library(game-engine-static STATIC ${ENGINE_SRC})
    target_link_libraries(game-engine-static PRIVATE SDL3::SDL3-static)

    target_link_libraries(game-engine-static PRIVATE efsw)
endif()
