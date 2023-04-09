cmake_minimum_required(VERSION 3.5)

project(lesta CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

add_subdirectory(01-1-hello-world)
add_subdirectory(01-2-hellolib)
add_subdirectory(02-1-sdl3)
