#pragma once
#include <glad/glad.h>

namespace ShaderProgramOpenGL {

    GLuint createShaderProg(const char* vertexSS_, const char* fragmentSS_);
    void Use(GLuint shaderProgramID);
};
