#pragma once
#include "core/memory/MemoryArena.h"
#include "core/memory/DArr.h"
#include "core/render/IRenderApi.h"
#include "core/render/openGL/WindowOpenGL.h"
#include "core/render/openGL/ShaderProgramOpenGL.h"
#include "core/render/Mesh.h"

class RendererOpenGL : public IRenderApi {

    size_t windowHandle;
    MemoryArena& arena;
    uint32_t shaderProg;
    
    // vertex layout
    DArr<uint32_t> VAOs;

    GLenum chanelToFormar(int nrChannels);
    size_t getTypeSize(uint32_t type);

public:

    RendererOpenGL();
    ~RendererOpenGL();

    bool Init() override;
    void Destroy() override;

    Mesh createMesh(
        const void* vertices, size_t vertSize, 
        const void* indices, size_t idxSize,
        VertexLayout& vertLayout
    ) override;

    uint32_t createVertexBuffer(const void* data, size_t size) override;
    uint32_t createIndexBuffer(const void* data, size_t size) override;
    uint32_t createTexture(const void* data, size_t size, int width, int height, int nrChannels) override;

    void frameBeing() override;
    void Draw(const Mesh& mesh, const MaterialTexture& mat) override;
    void frameEnd() override;
    void Clear() override;
    void createWindow(int width_, int height_, const char* title_) override;

    uint32_t createLayout(uint32_t count);
    uint32_t createShader(const char* vertexShader, const char* fragmentShader) override;
    uint32_t createVAO();
    void bindShader() override;
    void setUniform(uint32_t shaderID, const char*) override;  // need refactor
};
