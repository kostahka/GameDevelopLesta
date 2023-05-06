cmake_minimum_required(VERSION 3.5)

project(lesta CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(01-1-hello-world)
add_subdirectory(01-2-hellolib)
add_subdirectory(02-1-sdl3-version)
add_subdirectory(02-2-sdl3-window)
add_subdirectory(03-1-sdl-loop)
add_subdirectory(03-2-sdl-loop-engine-lib)
add_subdirectory(03-3-sdl-engine-hot-reload)
add_subdirectory(04-1-render-basics)
add_subdirectory(04-2-opengl)
