#include "core/render/openGL/WindowOpenGL.h"
#include "utils/logger.h"

WindowOpenGL::WindowOpenGL(int width_, int height_, const char* title_) {
    handle = glfwCreateWindow(width_, height_, title_, NULL, NULL);

    if (handle == nullptr) {
        LOG_ERROR("Failed to Init OpenGL window: ", title_);
        RixtenGlfwConfig::terminate();
        return;
    }

    glfwMakeContextCurrent(handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to Init Glad");
        RixtenGlfwConfig::terminate();
        return;
    }
}

WindowOpenGL::~WindowOpenGL() {
    RixtenGlfwConfig::terminate();
}

void WindowOpenGL::Update() {
    glfwSwapBuffers(handle);
    glfwPollEvents();
    // a temporary solution
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
}

bool WindowOpenGL::IsShouldClose() {
    return glfwWindowShouldClose(handle);
}

void WindowOpenGL::setSize(int width, int height) {
    glfwSetWindowSize(handle, width, height);
}

void WindowOpenGL::setTitle(const char* title_) {
    glfwSetWindowTitle(handle, title_);
}