#include "core/platform/PlatformGLFW.h"
#include "utils/logger.h"

void PlatformGLFW::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    auto* self = static_cast<PlatformGLFW*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        self->inputState->keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        self->inputState->keys[key] = false;
    }
}

void PlatformGLFW::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
}

void PlatformGLFW::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* self = static_cast<PlatformGLFW*>(glfwGetWindowUserPointer(window));

    self->inputState->mouseX = xpos;
    self->inputState->mouseY = ypos;
}

void PlatformGLFW::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
}

PlatformGLFW::PlatformGLFW() : windowHandle(nullptr), inputState(nullptr) {
}

PlatformGLFW::~PlatformGLFW() {
    glfwTerminate();
}

void PlatformGLFW::Init(int width_, int height_, const char* title_, InputState* inputState_) {
    glfwInit();

    // settings for platform
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // comment after first run

    windowHandle = glfwCreateWindow(width_, height_, title_, NULL, NULL);

    if (windowHandle == nullptr) {
        LOG_ERROR("Failed to Init GLFW window: ", title_);
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(windowHandle);
    
    glfwSetWindowUserPointer(windowHandle, this);
    glfwSetKeyCallback(windowHandle, KeyCallback);
    glfwSetCursorPosCallback(windowHandle, CursorPosCallback);

    glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // configure global opengl state
    // -----------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to Init Glad");
        glfwTerminate();
        return;
    }
    glEnable(GL_DEPTH_TEST);

    inputState = inputState_;
}

void PlatformGLFW::swapBuffer() {
    glfwSwapBuffers(windowHandle);
    glfwPollEvents();
}

void PlatformGLFW::poolEvent() {
    glfwPollEvents();
}

bool PlatformGLFW::IsWindowOpen() const {
    return glfwWindowShouldClose(windowHandle);
}

GLFWwindow* PlatformGLFW::GetWindowHandle() const {
    return windowHandle;
}
