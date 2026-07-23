#pragma once
#include "core/render/openGL/RixtenGlfwConfig.h"

struct window {
    
private:

    int height;
    int width;
    const char* title;

    GLFWwindow* handle;

public:

    window();
    ~window();

    void Init(int width_, int height_, const char* title_);
    void Destroy();
    void Update();

    bool IsShouldClose();

    void setSize(int width_, int height_);
    void setTitle(const char* title_);
};
