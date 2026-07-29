#include "core/render/RenderSystem.h"
#include "core/ecs/EcsManager.h"

RenderSystem::RenderSystem(IRenderApi* renderer_) : renderer(renderer_) {
}

void RenderSystem::Update(EcsManager& handle, float dt) {
    auto& meshes = handle.getPool<Mesh>()->getComponents();

    renderer->frameBeing();
    for(int i = 0; i < meshes.size(); i++) {
        renderer->Draw(meshes[i]);
    }
    renderer->frameEnd();
}