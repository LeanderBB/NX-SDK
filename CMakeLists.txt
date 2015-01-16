#------------------------------------------------------------------------------
# NX Project : Main CMake file
#------------------------------------------------------------------------------
project(NX)
cmake_minimum_required(VERSION 2.8.11)

set(NX_VERSION 0.1.0)

# Set flag to indicated this is a library build
set(NX_BUILD_SDK True)

# CMake costum module path
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Compiler Settings
include(cmake/nxcrosscompile.cmake)
include(cmake/nxcompiler.cmake)
include(cmake/nxapp.cmake)

# 3rdParty variables
add_subdirectory(3rdparty)
include_directories(${NX_3RDPARTY_DIR}/include)
link_directories(${NX_3RDPARTY_DIR}/lib)

#utilities
include(cmake/nxarchive.cmake)
include(cmake/nxresources.cmake)


# Components

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)


if(NOT DEFINED NX_CROSS_COMPILE)

#tools
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tools)

# tests
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/input)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/glinfo)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/io)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/args)

endif()

# install cmake files
install(DIRECTORY cmake/ DESTINATION cmake)
