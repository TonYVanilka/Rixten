#pragma once
#include "core/render/IRenderApi.h"

class RendererOpenGL : IRenderApi {

public:

    RendererOpenGL();
    ~RendererOpenGL();

    uint32_t createVertexBuffer(const void* data, size_t size);
    uint32_t createIndexBuffer(const void* data, size_t size);
    uint32_t createTexture(const void* data, size_t size);

    void Draw();
    void Clear();
    void createWindow(int width_, int height_, const char* title_);

    uint32_t createShader(const char* vertexShader, const char* fragmentShader);
    void bindShader(uint32_t shaderID);
    void setUniform(uint32_t shaderID, const char*);  // need refactor

};
