cmake_minimum_required(VERSION 3.16)

project(FlexNode LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(flexnode_lib
    FlexNode.cpp
)

target_include_directories(flexnode_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(flexnode
    tui.cpp
)

target_link_libraries(flexnode
    PRIVATE
        flexnode_lib
)

add_executable(flexnode_tests
    Tests.cpp
)

target_link_libraries(flexnode_tests
    PRIVATE
        flexnode_lib
)
