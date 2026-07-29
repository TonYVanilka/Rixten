#include "core/render/openGL/RendererOpenGL.h"

#include <glad/glad.h>

#include "core/render/openGL/RixtenGlfwConfig.h"
#include "core/render/openGL/Vertex.h"
#include "utils/logger.h"

RendererOpenGL::RendererOpenGL() : arena(MemoryArena::GetInstance()), 

 windowHandle(0), shaderProgHandle(0) {
}

RendererOpenGL::~RendererOpenGL() {
    Destroy();
    RixtenGlfwConfig::terminate();
}

bool RendererOpenGL::Init() {
    RixtenGlfwConfig::init();
    return true;
}

void RendererOpenGL::Destroy() {

    reinterpret_cast<ShaderProgramOpenGL*>(arena.getPtr(shaderProgHandle))->~ShaderProgramOpenGL();
    arena.deallocateByOffset(shaderProgHandle);

    reinterpret_cast<WindowOpenGL*>(arena.getPtr(windowHandle))->~WindowOpenGL();
    arena.deallocateByOffset(windowHandle);
}

uint32_t RendererOpenGL::createVertexBuffer(const void* data, size_t size) {
    uint32_t vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return vbo;
}

uint32_t RendererOpenGL::createIndexBuffer(const void* data, size_t size) {
    uint32_t ebo;

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return ebo;
}

// doesn't realize
uint32_t RendererOpenGL::createTexture(const void* data, size_t size, int width, int height, int nrChannels) {
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
void RendererOpenGL::frameBeing() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RendererOpenGL::Draw(Mesh& mesh) {

    glBindTexture(GL_TEXTURE_2D, mesh.texture);
    glBindVertexArray(vao);

    bindShader();
    //reinterpret_cast<VAO*>(arena.getPtr(vaoHandle))->bind();

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    
    // 0: position (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // 1: texCoords (vec2)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
}

void RendererOpenGL::frameEnd() {
    reinterpret_cast<WindowOpenGL*>(arena.getPtr(windowHandle))->Update();
}

void RendererOpenGL::Clear() {
}

void RendererOpenGL::createWindow(int width_, int height_, const char* title_) {
    windowHandle = arena.allocate(sizeof(WindowOpenGL), alignof(WindowOpenGL));
    new(arena.getPtr(windowHandle)) WindowOpenGL(width_, height_, title_);
}

void RendererOpenGL::createShader(const char* vertexShader, const char* fragmentShader) {
    shaderProgHandle = arena.allocate(sizeof(ShaderProgramOpenGL), alignof(ShaderProgramOpenGL));
    new(arena.getPtr(shaderProgHandle)) ShaderProgramOpenGL(vertexShader, fragmentShader);
    // create VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void RendererOpenGL::bindShader() {
    reinterpret_cast<ShaderProgramOpenGL*>(arena.getPtr(shaderProgHandle))->Use();
}

void RendererOpenGL::setUniform(uint32_t shaderID, const char*) {
}

GLenum RendererOpenGL::chanelToFormar(int nrChannels) {
    switch (nrChannels) {
    case 1: return GL_RED;
    case 3: return GL_RGB;
    case 4: return GL_RGBA;
    default:
        return GL_RGB;
    }
}