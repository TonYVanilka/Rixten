#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

struct InputState {
    bool keys[GLFW_KEY_LAST];
    bool keyIsPressed[GLFW_KEY_LAST];
    bool keyIsReleased[GLFW_KEY_LAST];
    double mouseX, mouseY;
    double mouseDeltaX, mouseDeltaY;
};

class PlatformGLFW {

    GLFWwindow* windowHandle;
    InputState* inputState;

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:

    PlatformGLFW();
    ~PlatformGLFW();

    void Init(int width_, int height_, const char* title_, InputState* inputState_);
    void swapBuffer();
    void poolEvent();
    bool IsWindowOpen() const;

    GLFWwindow* GetWindowHandle() const;

};