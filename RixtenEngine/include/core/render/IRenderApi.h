#pragma once
#include <cstdint>
#include "core/render/Mesh.h"

class IRenderApi {
public:
    virtual ~IRenderApi() = default;

    virtual bool Init() = 0;
    virtual void Destroy() = 0;

    virtual uint32_t createVertexBuffer(const void* data, size_t size) = 0;
    virtual uint32_t createIndexBuffer(const void* data, size_t size) = 0;
    virtual uint32_t createTexture(const void* data, size_t size, int width, int height, int nrChannels) = 0;

    virtual void frameBeing() = 0;
    virtual void Draw(Mesh& mesh) = 0;
    virtual void frameEnd() = 0;
    virtual void Clear() = 0;
    virtual void createWindow(int width_, int height_, const char* title_) = 0;

    virtual void createShader(const char* vertexShader, const char* fragmentShader) = 0;
    virtual void bindShader() = 0;
    virtual void setUniform(uint32_t shaderID, const char*) = 0; // need refactor
};