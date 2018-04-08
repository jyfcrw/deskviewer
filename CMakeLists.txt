project(DeskViewer CXX)
cmake_minimum_required(VERSION 3.0)

add_definitions(-D_UNICODE -DUNICODE)

find_package(zlib)
include_directories(${ZLIB_INCLUDE_DIRS})

file(GLOB SOURCES src/*.cpp src/*.hpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
