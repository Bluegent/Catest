cmake_minimum_required(VERSION 3.2)
project(CatestProject)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/../")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Framework)
add_subdirectory(FrameworkTest)
add_subdirectory(Exercises)