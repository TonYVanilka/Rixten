#include "core/render/openGL/RendererOpenGL.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include "core/render/openGL/Vertex.h"
#include "utils/logger.h"

RendererOpenGL::RendererOpenGL() : VAOs(1) /*refactor in future*/ {
}

RendererOpenGL::~RendererOpenGL() {
    Destroy();
}

bool RendererOpenGL::Init() {
    return true;
}

void RendererOpenGL::Destroy() {
}

void RendererOpenGL::setCamera(Camera& camera) {
    //currentCamera = camera;
}

Mesh RendererOpenGL::createMesh(
    const void* vertices, size_t vertSize, 
    const void* indices, size_t idxSize,
    VertexLayout& vertLayout ) {

    uint32_t vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxSize, indices, GL_STATIC_DRAW);

    uint8_t location = 0;
    uint32_t offset = 0;

    for(int i = 0; i < vertLayout.attributeCount; i++) {
        vertLayout.attributes[i].offset = offset;
        glVertexAttribPointer(
            location,
            vertLayout.attributes[i].count,
            static_cast<GLenum>(vertLayout.attributes[i].type),
            vertLayout.attributes[i].normalize,
            vertLayout.stride,
            (void*)(offset));
        LOG_ERROR(glGetError());
        offset += vertLayout.attributes[i].count * getTypeSize(vertLayout.attributes[i].type);
        glEnableVertexAttribArray(location);
        location++; 
    }

    // debug
    for (int i = 0; i < vertLayout.attributeCount; i++) {
        GLint enabled, size, stride;
        void* ptr;
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
        glGetVertexAttribPointerv(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, &ptr);
        LOG_DEBUG("attrib ", i, " enabled=", enabled, " size=", size, " stride=", stride, " offset=", ptr);
    }

    // // 0: position (vec3)
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // glEnableVertexAttribArray(0);

    // // 1: texCoords (vec2)
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    // unbind vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // not allowed unbind ebo while a vao is active!
    
    // unbind vao 
    glBindVertexArray(0);

    VAOs.push_back(vao);

    return Mesh{uint8_t(VAOs.size() - 1), vbo, ebo, uint32_t(idxSize / sizeof(unsigned int))};
}

uint32_t RendererOpenGL::createMaterialUBO(MaterialData& data) {
    if (sizeof(MaterialData) % 16 != 0) LOG_FATAL("UBO struct must be 16-byte aligned for std140");

    uint32_t ubo;

    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialData), &data, GL_STATIC_DRAW);

    glBindBufferBase(GL_UNIFORM_BUFFER, GlobalShaderBindings::BINDING_MATERIAL, ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return ubo;
}

uint32_t RendererOpenGL::createTexture(const void* data, int width, int height, int nrChannels) {
    unsigned int texture;
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, chanelToFormar(nrChannels), width, height, 0, chanelToFormar(nrChannels), GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

uint32_t RendererOpenGL::createShader(const char* vertexSS_, const char* fragmentSS_) {
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
    GLuint shaderProgramID = glCreateProgram();

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

    return shaderProgramID;
}

void RendererOpenGL::frameBeing() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RendererOpenGL::Draw(
    const Mesh& mesh,
    const Material& mat,
    const glm::mat4 transform,
    const Camera& camera 
) {

    if (mat.texture != 0) glBindTexture(GL_TEXTURE_2D, mat.texture);
    glBindBuffer(GL_UNIFORM_BUFFER, mat.ubo);

    glUseProgram(mat.shaderProgram);

    // REFACTOR IN FUTURE
    unsigned int model = glGetUniformLocation(mat.shaderProgram, "model");
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(transform));
    
    //debug 
    glm::mat4 projectionM = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    unsigned int projection = glGetUniformLocation(mat.shaderProgram, "projection");
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(projectionM));

    glm::mat4 viewM = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
    unsigned int view = glGetUniformLocation(mat.shaderProgram, "view");
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(viewM));
    // REFACTOR IN FUTURE

    glBindVertexArray(VAOs[mesh.layout]);
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
}

GLenum RendererOpenGL::chanelToFormar(int nrChannels) {
    switch (nrChannels) {
        case 1: return GL_RED;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: return GL_RGB;
    }
}

size_t RendererOpenGL::getTypeSize(uint32_t type) {
    switch(type) {
        case GL_FLOAT: return sizeof(GLfloat);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        case GL_BYTE: return sizeof(GLbyte);
        case GL_UNSIGNED_SHORT: return sizeof(GLushort);
        case GL_SHORT: return sizeof(GLshort);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_INT: return sizeof(GLint);
        case GL_HALF_FLOAT: return sizeof(GLhalf);
        case GL_DOUBLE: return sizeof(GLdouble);
        default: return 0;
    }
}