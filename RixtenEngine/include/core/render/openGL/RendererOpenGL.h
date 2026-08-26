#pragma once
#include "core/memory/DArr.h"
#include "core/render/IRenderApi.h"
#include <glad/glad.h>

class RendererOpenGL : public IRenderApi {

    // vertex layout
    // in future has sence emplace it at resource system
    DArr<uint32_t> VAOs;

    GLenum getVertexLayoutType(AttributeType type);
    GLenum chanelToFormar(int nrChannels);
    size_t getTypeSize(uint32_t type);

public:

    RendererOpenGL();
    ~RendererOpenGL();

    bool Init() override;
    void Destroy() override;

    void setViewportSize(int width, int height) override;

    MeshGPU createMesh(
        VertexLayout& vertLayout, 
        const void* vertices, size_t vertSize, 
        const void* indices, size_t idxSize
    ) override;

    uint32_t createMaterialUBO(MaterialData& data) override;
    uint32_t createTexture(const void* data, int width, int height, int nrChannels) override;
    uint32_t createShader(const char* vertexSS_, const char* fragmentSS_);

    void clear() override;
    void Draw(
        const MeshGPU& mesh,
        const Material& mat,
        const glm::mat4 transform,
        const Camera& camera 
    ) override;
};
