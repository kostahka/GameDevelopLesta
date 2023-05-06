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

set(ENGINE_SRC "src/engine.cxx" "include/engine.hxx")

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

if(ENGINE_DEV)

    add_executable(game_engine ${ENGINE_SRC})
    target_link_libraries(game_engine PRIVATE SDL3::SDL3-static)
    target_compile_definitions(game_engine PRIVATE ENGINE_DEV)
    target_compile_definitions(
        game_engine PRIVATE ENGINE_GAME_LIB_NAME="${ENGINE_GAME_LIB_NAME}")

    target_link_libraries(game_engine PRIVATE efsw)

else()

    #Shared engine lib
    add_library(game-engine-shared SHARED ${ENGINE_SRC})
    target_link_libraries(game-engine-shared PRIVATE SDL3::SDL3-static)

    #Static engine lib
    add_library(game-engine-static STATIC ${ENGINE_SRC})
    target_link_libraries(game-engine-static PRIVATE SDL3::SDL3-static)

endif()
