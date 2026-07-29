#pragma once
#include "core/memory/MemoryArena.h"
#include "core/render/IRenderApi.h"
#include "core/render/openGL/WindowOpenGL.h"
#include "core/render/openGL/ShaderProgramOpenGL.h"
#include "core/render/Mesh.h"

class RendererOpenGL : public IRenderApi {

    size_t windowHandle;
    size_t shaderProgHandle;
    MemoryArena& arena;

    uint32_t vao;
    uint32_t vboHandle;

    GLenum chanelToFormar(int nrChannels);

public:

    RendererOpenGL();
    ~RendererOpenGL();

    bool Init() override;
    void Destroy() override;

    uint32_t createVertexBuffer(const void* data, size_t size) override;
    uint32_t createIndexBuffer(const void* data, size_t size) override;
    uint32_t createTexture(const void* data, size_t size, int width, int height, int nrChannels) override;

    void frameBeing() override;
    void Draw(Mesh& mesh) override;
    void frameEnd() override;
    void Clear() override;
    void createWindow(int width_, int height_, const char* title_) override;

    void createShader(const char* vertexShader, const char* fragmentShader) override;
    void bindShader() override;
    void setUniform(uint32_t shaderID, const char*) override;  // need refactor
};
