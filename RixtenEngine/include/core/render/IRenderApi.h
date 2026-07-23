#pragma once 
#include <cstdint>

class IRenderApi {
public:
    virtual ~IRenderApi() = default;

    virtual uint32_t createVertexBuffer(const void* data, size_t size) = 0;
    virtual uint32_t createIndexBuffer(const void* data, size_t size) = 0;
    virtual uint32_t createTexture(const void* data, size_t size) = 0;

    virtual void Draw() = 0;
    virtual void Clear() = 0;
    virtual void createWindow(int width_, int height_, const char* title_) = 0;

    virtual uint32_t createShader(const char* vertexShader, const char* fragmentShader) = 0;
    virtual void bindShader(uint32_t shaderID) = 0;
    virtual void setUniform(uint32_t shaderID, const char*) = 0; // need refactor
};