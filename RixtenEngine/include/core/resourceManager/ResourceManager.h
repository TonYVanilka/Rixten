#pragma once
#include "core/memory/DArr.h"

#include "core/render/Mesh.h"
#include "core/render/IRenderApi.h"

typedef uint32_t index;
constexpr uint16_t MINassets = 2;

class ResourceManager {

    DArr<Mesh> meshes;

public:
    
    ResourceManager();
    ~ResourceManager();

    index loadMesh(const char* path);

};