cmake_minimum_required(VERSION 3.10)

project(VisualizadorDeGrafos VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

add_executable(VisualizadorDeGrafos ${SOURCES} ${HEADERS})

target_link_libraries(VisualizadorDeGrafos PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui)

# Testing
add_executable(VerifyLogic 
    tests/VerifyLogic.cpp 
    src/Graph.cpp 
    src/GraphAlgorithms.cpp 
    src/FileController.cpp
)
target_include_directories(VerifyLogic PRIVATE include)
target_link_libraries(VerifyLogic PRIVATE Qt5::Core)
