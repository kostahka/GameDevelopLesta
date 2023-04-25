cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(04-1-render-basics)

add_executable(
    00-canvas-basic include/00-canvas-basic.hxx src/00-canvas-basic-main.cxx
                    src/00-canvas-basic.cxx)

target_include_directories(00-canvas-basic
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(
    01-line-render
    include/00-canvas-basic.hxx
    include/01-render-object.hxx
    src/00-canvas-basic.cxx
    src/01-line-render-main.cxx
    src/01-render-object.cxx)

target_include_directories(01-line-render
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(
    02-triangle-render
    include/00-canvas-basic.hxx
    include/01-render-object.hxx
    src/00-canvas-basic.cxx
    src/01-render-object.cxx
    src/02-triangle-render-main.cxx)

target_include_directories(02-triangle-render
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(
    03-triangle-indexed-render
    include/00-canvas-basic.hxx
    include/01-render-object.hxx
    src/00-canvas-basic.cxx
    src/01-render-object.cxx
    src/03-triangle-indexed-render-main.cxx)

target_include_directories(03-triangle-indexed-render
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(
    04-interpolated-triangle
    include/00-canvas-basic.hxx
    include/04-render-conveyor.hxx
    src/00-canvas-basic.cxx
    src/04-interpolated-triangle-main.cxx
    src/04-render-conveyor.cxx)

target_include_directories(04-interpolated-triangle
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
