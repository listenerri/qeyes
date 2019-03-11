cmake_minimum_required(VERSION 3.7)

set(VERSION 1.0)
set(BIN_NAME qeyes)

project(${BIN_NAME})

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_INSTALL_PREFIX /usr)

#add_subdirectory("plugins")

# Sources files
file(GLOB SRCS "*.h" "*.cpp")

# Find the library
find_package(PkgConfig REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(DtkWidget REQUIRED)

pkg_check_modules(X11 REQUIRED x11)
pkg_check_modules(XI REQUIRED xi)

add_executable(${BIN_NAME} ${SRCS})

target_include_directories(${BIN_NAME} PUBLIC
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5X11Extras_INCLUDE_DIRS}
    ${DtkWidget_INCLUDE_DIRS}
    ${X11_INCLUDE_DIRS}
    ${XI_INCLUDE_DIRS}
)

target_link_libraries(${BIN_NAME} PUBLIC
    ${Qt5Widgets_LIBRARIES}
    ${Qt5X11Extras_LIBRARIES}
    ${DtkWidget_LIBRARIES}
    ${X11_LIBRARIES}
    ${XI_LIBRARIES}
)

# bin
install(TARGETS ${BIN_NAME} DESTINATION bin)
