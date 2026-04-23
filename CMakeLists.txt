cmake_minimum_required(VERSION 3.16)

project(FlexNode LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(FLEXNODE_BUILD_TUI "Build the interactive TUI executable" OFF)
option(FLEXNODE_BUILD_TESTS "Build the flexnode_tests executable" OFF)
option(FLEXNODE_BUILD_TUI_PLIST "Generate tui.plist in the build directory via clang static analysis" OFF)

add_library(flexnode_lib
    src/FlexNode.cpp
)

target_include_directories(flexnode_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(FLEXNODE_BUILD_TUI OR FLEXNODE_BUILD_TESTS)
    add_library(flexnode_testing
        testing/FlexNodeTestRunner.cpp
        testing/FlexNodeUnitTests.cpp
        testing/FlexNodeWorkflowTests.cpp
    )

    target_include_directories(flexnode_testing
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/testing
    )

    target_link_libraries(flexnode_testing
        PUBLIC
            flexnode_lib
    )
endif()

if(FLEXNODE_BUILD_TUI)
    add_executable(flexnode
        tui/tui.cpp
    )

    target_link_libraries(flexnode
        PRIVATE
            flexnode_testing
    )
endif()

if(FLEXNODE_BUILD_TESTS)
    add_executable(flexnode_tests
        tui/Tests.cpp
    )

    target_link_libraries(flexnode_tests
        PRIVATE
            flexnode_testing
    )
endif()

if(FLEXNODE_BUILD_TUI_PLIST)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "FLEXNODE_BUILD_TUI_PLIST requires Clang or AppleClang.")
    endif()

    set(FLEXNODE_TUI_PLIST "${CMAKE_BINARY_DIR}/tui.plist")
    add_custom_command(
        OUTPUT "${FLEXNODE_TUI_PLIST}"
        COMMAND "${CMAKE_CXX_COMPILER}"
                -std=c++${CMAKE_CXX_STANDARD}
                -I${CMAKE_CURRENT_SOURCE_DIR}/src
                -I${CMAKE_CURRENT_SOURCE_DIR}/testing
                --analyze
                -Xanalyzer -analyzer-output=plist
                -o ${FLEXNODE_TUI_PLIST}
                ${CMAKE_CURRENT_SOURCE_DIR}/tui/tui.cpp
        DEPENDS
                "${CMAKE_CURRENT_SOURCE_DIR}/tui/tui.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/FlexNode.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/testing/FlexNodeTests.h"
        COMMENT "Generating ${FLEXNODE_TUI_PLIST}"
        VERBATIM
    )

    add_custom_target(flexnode_tui_plist ALL
        DEPENDS "${FLEXNODE_TUI_PLIST}"
    )
endif()
