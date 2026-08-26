#pragma once
#include <cstdint>
#include <glm/glm.hpp>

#include "core/render/Camera.h"
#include "core/render/VertexLayout.h"
#include "core/render/Mesh.h"
#include "core/render/Material.h"

class IRenderApi {
public:
    virtual ~IRenderApi() = default;

    virtual bool Init() = 0;
    virtual void Destroy() = 0;

    virtual void setViewportSize(int width, int height) = 0;

    virtual MeshGPU createMesh(
        VertexLayout& vertLayout, 
        const void* vertices, size_t vertSize, 
        const void* indices, size_t idxSize
    ) = 0;

    virtual uint32_t createMaterialUBO(MaterialData& data) = 0;
    virtual uint32_t createTexture(const void* data, int width, int height, int nrChannels) = 0;
    virtual uint32_t createShader(const char* vertexSS_, const char* fragmentSS_) = 0;

    virtual void clear() = 0;
    virtual void Draw(
        const MeshGPU& mesh,
        const Material& mat,
        const glm::mat4 transform,
        const Camera& camera
    ) = 0;
    //virtual void frameEnd() = 0;
};