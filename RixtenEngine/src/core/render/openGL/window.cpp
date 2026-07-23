#include "core/render/openGL/window.h"
#include "utils/logger.h"

window::window() {
}

window::~window() {
    Destroy();
}

void window::Init(int width_, int height_, const char* title_) {
    RixtenGlfwConfig::init();

    handle = glfwCreateWindow(width_, height_, title_, NULL, NULL);
    
    if(handle == nullptr) {
        LOG_ERROR("Failed to Init OpenGL window: ", title_);  
        Destroy();
        return;
    }

    glfwMakeContextCurrent(handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to Init Glad");
        Destroy();
        return;
    }
}

void window::Destroy() {
    RixtenGlfwConfig::terminate();
}

void window::Update() {
    glfwSwapBuffers(handle);
    glfwPollEvents();
    // a temporary solution
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
}    

bool window::IsShouldClose() {
    return glfwWindowShouldClose(handle);
}

void window::setSize(int width, int height) {
    glfwSetWindowSize(handle, width, height);
}

void window::setTitle(const char* title_) {
    glfwSetWindowTitle(handle, title_);
}