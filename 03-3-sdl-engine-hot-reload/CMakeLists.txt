cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(game-engine-loop-lib)

find_package(SDL3 REQUIRED)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_library(game-hot-load-shared SHARED src/game.cxx)

add_executable(engine include/engine.hxx src/engine.cxx)
#target_include_directories(engine PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(game-hot-load-shared
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(engine PRIVATE SDL3::SDL3-static)

target_link_libraries(engine PRIVATE game-hot-load-shared)

#set_target_properties(engine PROPERTIES ENABLE_EXPORTS ON)

#target_link_libraries(game-hot-load-shared PRIVATE engine)
