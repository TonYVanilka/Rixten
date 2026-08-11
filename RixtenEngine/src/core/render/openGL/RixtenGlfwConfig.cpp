#include "core/render/openGL/RixtenGlfwConfig.h"

void RixtenGlfwConfig::init() {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // comment after first run

}

void RixtenGlfwConfig::terminate() {
    glfwTerminate();
}