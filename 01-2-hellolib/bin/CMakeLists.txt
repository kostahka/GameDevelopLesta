cmake_minimum_required(VERSION 3.5)

project(hello-bin LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hello-bin-static src/hello-bin.cxx)
target_link_libraries(hello-bin-static LINK_PUBLIC hello-lib-static)

add_executable(hello-bin-shared src/hello-bin.cxx)
target_link_libraries(hello-bin-shared LINK_PUBLIC hello-lib-shared)

install(TARGETS hello-bin-shared hello-bin-static
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../bin/tests
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../bin/tests
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../bin/tests)
