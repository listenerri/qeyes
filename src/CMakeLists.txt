
file(GLOB_RECURSE SRCS "*.cpp" "*.h")

add_executable(${ProjectName} ${SRCS})

target_include_directories(${ProjectName} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${ProjectName} PUBLIC
    Qt5::Widgets
)
