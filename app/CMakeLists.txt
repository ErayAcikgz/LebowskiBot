cmake_minimum_required(VERSION 3.21)

project(Lebowski VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(Lebowski
    src/main.cpp
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    src/pages/OverviewPage.cpp
    src/pages/OverviewPage.h
)

target_link_libraries(Lebowski PRIVATE Qt6::Widgets)

target_include_directories(Lebowski PRIVATE src)
