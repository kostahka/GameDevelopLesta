cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(game-engine)

find_package(SDL3 REQUIRED)

add_subdirectory(efsw)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(ENGINE_DEV OFF CACHE BOOL "engine load game dll with hot reload")
set(ENGINE_GAME_LIB_NAME "" CACHE STRING "game lib name to load")

set(GLAD_SRC "glad/src/glad.c")

set(ENGINE_SRC "src/engine.cxx" "include/engine.hxx" ${GLAD_SRC})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/glad/include)

if(ENGINE_DEV)

    add_executable(game-engine ${ENGINE_SRC})
    target_link_libraries(game-engine PRIVATE SDL3::SDL3-static)
    target_compile_definitions(game-engine PUBLIC ENGINE_DEV)
    target_compile_definitions(
        game-engine PUBLIC ENGINE_GAME_LIB_NAME="${ENGINE_GAME_LIB_NAME}")

    target_link_libraries(game-engine PRIVATE efsw)

    target_include_directories(${ENGINE_GAME_LIB_NAME}
                               PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_compile_definitions(${ENGINE_GAME_LIB_NAME} PUBLIC ENGINE_DEV)

    target_link_libraries(game-engine INTERFACE ${ENGINE_GAME_LIB_NAME})
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
