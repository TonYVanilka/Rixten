#include "RendererOpenGL.h"

#include <glad.h>

#include "utils/logger.h"

RendererOpenGL::RendererOpenGL() {
}

RendererOpenGL::~RendererOpenGL() {
}

uint32_t RendererOpenGL::createVertexBuffer(const void* data, size_t size) {
    return 0;
}

uint32_t RendererOpenGL::createIndexBuffer(const void* data, size_t size) {
    return 0;
}

uint32_t RendererOpenGL::createTexture(const void* data, size_t size) {
    return 0;
}

void RendererOpenGL::Draw() {
}

void RendererOpenGL::Clear() {
}

void RendererOpenGL::createWindow(int width_, int height_, const char* title_) {
}

uint32_t RendererOpenGL::createShader(const char* vertexShader, const char* fragmentShader) {
    
}

void RendererOpenGL::bindShader(uint32_t shaderID) {
}

void RendererOpenGL::setUniform(uint32_t shaderID, const char*) {
}
