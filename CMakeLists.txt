cmake_minimum_required(VERSION 3.16)

project(FlexNode VERSION 0.1.0 LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(FLEXNODE_BUILD_TUI "Build the interactive TUI executable" OFF)
option(FLEXNODE_BUILD_TESTS "Build the flexnode_tests executable" OFF)
option(FLEXNODE_BUILD_TUI_PLIST "Generate tui.plist in the build directory via clang static analysis" OFF)
option(FLEXNODE_ENABLE_INSTALL "Enable installation and CMake package exports" ON)

add_library(flexnode_lib
    src/FlexNode.cpp
)
add_library(FlexNode::flexnode ALIAS flexnode_lib)

set_target_properties(flexnode_lib PROPERTIES
    EXPORT_NAME flexnode
    OUTPUT_NAME flexnode
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
target_compile_features(flexnode_lib PUBLIC cxx_std_17)

target_include_directories(flexnode_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(FLEXNODE_ENABLE_INSTALL)
    install(TARGETS flexnode_lib
        EXPORT FlexNodeTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(EXPORT FlexNodeTargets
        FILE FlexNodeTargets.cmake
        NAMESPACE FlexNode::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FlexNode
    )

    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/FlexNodeConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/FlexNodeConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FlexNode
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/FlexNodeConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/FlexNodeConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/FlexNodeConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FlexNode
    )
endif()

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
                -I${CMAKE_CURRENT_SOURCE_DIR}/include
                -I${CMAKE_CURRENT_SOURCE_DIR}/testing
                --analyze
                -Xanalyzer -analyzer-output=plist
                -o ${FLEXNODE_TUI_PLIST}
                ${CMAKE_CURRENT_SOURCE_DIR}/tui/tui.cpp
        DEPENDS
                "${CMAKE_CURRENT_SOURCE_DIR}/tui/tui.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/include/flexnode/FlexNode.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/testing/FlexNodeTests.h"
        COMMENT "Generating ${FLEXNODE_TUI_PLIST}"
        VERBATIM
    )

    add_custom_target(flexnode_tui_plist ALL
        DEPENDS "${FLEXNODE_TUI_PLIST}"
    )
endif()
