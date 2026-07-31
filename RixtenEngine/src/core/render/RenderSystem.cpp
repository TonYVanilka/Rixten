#include "core/render/RenderSystem.h"
#include "core/ecs/EcsManager.h"

RenderSystem::RenderSystem(IRenderApi* renderer_) : renderer(renderer_) {
}

void RenderSystem::Update(EcsManager& handle, float dt) {
    auto& meshes = handle.getPool<Mesh>()->getComponents();
    auto& mats = handle.getPool<MaterialTexture>()->getComponents();

    renderer->frameBeing();
    // need sort to match the material to the mesh
    for(int i = 0; i < meshes.size(); i++) {
        renderer->Draw(meshes[i], mats[i]);
    }
    renderer->frameEnd();
}