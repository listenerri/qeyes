cmake_minimum_required(VERSION 3.7)

set(VERSION 1.0)

set(ProjectName qeyes)

project(${ProjectName})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# win32 下使用 UTF-8 编码
if (MSVC)
    add_compile_options("/source-charset:utf-8" "/execution-charset:utf-8")
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(src)
