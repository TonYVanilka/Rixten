#pragma once
#include <glad/glad.h>

struct ShaderProgramOpenGL {

    GLuint shaderProgramID;
    
public:

    ShaderProgramOpenGL(const char* vertexSS_, const char* fragmentSS_);
    ~ShaderProgramOpenGL();

    void Use();

};
