#pragma once
#include "core/render/openGL/RixtenGlfwConfig.h"

struct WindowOpenGL {
    
private:

    int height;
    int width;
    const char* title;

    GLFWwindow* handle;

public:

    WindowOpenGL(int width_, int height_, const char* title_);
    ~WindowOpenGL();

    void Update();

    bool IsShouldClose();

    void setSize(int width_, int height_);
    void setTitle(const char* title_);
};
