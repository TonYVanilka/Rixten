#pragma once

#include "core/resourceManager/cgltf.h"

#include "core/memory/DArr.h"

#include "core/resourceManager/ResourceManager.h"
#include "core/render/VertexLayout.h"
#include "core/render/IRenderApi.h"

// struct MeshGPU {
//     uint8_t layout;
//     uint32_t vbo;
//     uint32_t ibo;
//     uint32_t indexCount;
// };

struct MeshResource {
    ResourceIndex vertexLayout;
    size_t verticesArenaOffset;
    size_t indicesArenaOffset;
    uint32_t verticesCount;
    uint32_t indicesCount;
};

struct SubModelResource {
    ResourceIndex mesh;
    ResourceIndex meshGPU;
    ResourceIndex material;
};

struct ModelResource {
    DArr<SubModelResource> subMeshes;
    // must replace to global Rixten arena
    //unsigned char* data;
};

struct ModelLoaderGLTF {
    ResourceManager* mngr;
    MemoryArena& arena;

    AttributeType getAttributeType(cgltf_component_type type);    
    uint32_t componentSize(cgltf_component_type type);
public:

    ModelLoaderGLTF(ResourceManager* mngr_);

    ModelResource load(const char* path);
    SubModelResource loadSubMesh(cgltf_primitive& primitive);
    void unload();

};