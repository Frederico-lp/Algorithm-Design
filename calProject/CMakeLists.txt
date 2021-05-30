cmake_minimum_required(VERSION 3.17)
project(calProject)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 14)

# Build GraphViewerCpp
set(GRAPHVIEWERCPP_PATH GraphViewerCpp)
add_subdirectory(${GRAPHVIEWERCPP_PATH})
include_directories(${GRAPHVIEWERCPP_PATH}/include)
include_directories(include)
add_executable(calProject src/main.cpp src/Node.cpp src/LoadFiles.cpp include/MapViewer.h)

target_link_libraries(calProject graphviewer)