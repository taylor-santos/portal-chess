find_package(OpenGL REQUIRED)

set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/external/imgui)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends)

set(IMGUI_SRC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

set(BUILD_SRC
        sample.cpp
        board.cpp)

add_executable(${PROJECT_NAME} ${IMGUI_SRC} ${BUILD_SRC} main.cpp)

add_library(${PROJECT_NAME}_lib STATIC ${BUILD_SRC})

target_link_libraries(${PROJECT_NAME}
        glad
        glfw
        OpenGL::GL
        ${CMAKE_DL_LIBS})
