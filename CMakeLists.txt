cmake_minimum_required(VERSION 3.16)

project(portal_chess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/external/imgui)

enable_testing()

add_subdirectory(src)
add_subdirectory(external/glad)
add_subdirectory(test)
