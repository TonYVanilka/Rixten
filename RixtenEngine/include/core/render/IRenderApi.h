#pragma once
#include <cstdint>
#include <glm/glm.hpp>

#include "core/render/Camera.h"
#include "core/render/Material.h"
#include "core/render/Mesh.h"
#include "core/render/VertexLayout.h"

class IRenderApi {
public:
    virtual ~IRenderApi() = default;

    virtual bool Init() = 0;
    virtual void Destroy() = 0;

    virtual Mesh createMesh(
        const void* vertices, size_t vertSize, 
        const void* indices, size_t idxSize, 
        VertexLayout& vertLayout
    ) = 0;

    virtual void setCamera(Camera* camera) = 0;

    virtual uint32_t createMaterialUBO(MaterialData& data) = 0;
    virtual uint32_t createTexture(const void* data, int width, int height, int nrChannels) = 0;

    virtual void frameBeing() = 0;
    virtual void Draw(const Mesh& mesh, const Material& mat, const glm::mat4 transform) = 0;
    virtual void frameEnd() = 0;
    virtual void createWindow(int width_, int height_, const char* title_) = 0;

    virtual uint32_t createShader(const char* vertexShader, const char* fragmentShader) = 0;
    virtual void bindShader() = 0;
};