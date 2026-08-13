#pragma once
#include "core/memory/DArr.h"
#include "core/render/IRenderApi.h"
#include "core/render/Mesh.h"
#include <glad/glad.h>

class RendererOpenGL : public IRenderApi {

    // vertex layout
    // in future has sence emplace it at resource system
    DArr<uint32_t> VAOs;

    GLenum chanelToFormar(int nrChannels);
    size_t getTypeSize(uint32_t type);

public:

    RendererOpenGL();
    ~RendererOpenGL();

    bool Init() override;
    void Destroy() override;

    void setViewportSize(int width, int height) override;

    Mesh createMesh(
        const void* vertices, size_t vertSize, 
        const void* indices, size_t idxSize,
        VertexLayout& vertLayout
    ) override;

    uint32_t createMaterialUBO(MaterialData& data) override;
    uint32_t createTexture(const void* data, int width, int height, int nrChannels) override;
    uint32_t createShader(const char* vertexSS_, const char* fragmentSS_);

    void frameBeing() override;
    void Draw(
        const Mesh& mesh,
        const Material& mat,
        const glm::mat4 transform,
        const Camera& camera 
    ) override;
};
