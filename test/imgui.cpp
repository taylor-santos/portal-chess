//
// Created by taylor-santos on 4/18/2021 at 00:46.
//

#include "gtest/gtest.h"

#include "glad/glad.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

static std::string error_desc;

static void
glfw_error_callback(int, const char *description) {
    error_desc = description;
}

TEST(ImGui, SuccessfullyInitializeGLFW) {
    glfwSetErrorCallback(glfw_error_callback);
    ASSERT_TRUE(glfwInit()) << error_desc;
}
