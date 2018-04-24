# Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
project(kvv)

# Add macos homebrew paths
if(APPLE)
	include_directories(/usr/local/include)
	link_directories(/usr/local/lib)
endif()
include_directories("${PROJECT_SOURCE_DIR}")
link_libraries(curl ncurses jsoncpp)
add_executable(kvv kvv.cpp)