#include "core/render/RenderSystem.h"
#include "core/ecs/EcsManager.h"

RenderSystem::RenderSystem(IRenderApi* renderer_) : renderer(renderer_) {
}

void RenderSystem::Update(EcsManager& handle, float dt) {
    auto& meshes = handle.getPool<Mesh>()->getComponents();
    auto& mats = handle.getPool<Material>()->getComponents();
    auto& tranforms = handle.getPool<glm::mat4>()->getComponents();

    renderer->frameBeing();
    // need sort to match the material to the mesh
    for(int i = 0; i < meshes.size(); i++) {
        renderer->Draw(meshes[i], mats[i], tranforms[i]);
    }
    renderer->frameEnd();
}