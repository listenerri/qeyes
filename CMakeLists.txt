cmake_minimum_required(VERSION 3.7)

set(VERSION 1.0)

set(ProjectName qeyes)

project(${ProjectName})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(src)
