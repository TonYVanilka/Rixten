#include "core/render/openGL/ShaderProgramOpenGL.h"
#include "utils/logger.h"

ShaderProgramOpenGL::ShaderProgramOpenGL(const char* vertexSS_, const char* fragmentSS_) {
    int success;
    char infoLog[512];

    // compile Shaders 
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexSS_, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        LOG_ERROR("Failed to compile vertex shader: ", infoLog);
    }

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentSS_, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        LOG_ERROR("Failed to compile fragment shader: ", infoLog);
    }

    // Link shaders with shader program
    shaderProgramID = glCreateProgram();

    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);

    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        LOG_ERROR("Failed to compile shader program: ", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ShaderProgramOpenGL::~ShaderProgramOpenGL() {
}

void ShaderProgramOpenGL::Use() {
    glUseProgram(shaderProgramID);
}
