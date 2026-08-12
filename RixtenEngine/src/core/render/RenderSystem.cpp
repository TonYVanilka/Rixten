#include "core/render/RenderSystem.h"
#include "core/ecs/EcsManager.h"
#include "core/render/Camera.h"

RenderSystem::RenderSystem(IRenderApi* renderer_, PlatformGLFW* platform_) : 
    renderer(renderer_), platform(platform_) {
}

void RenderSystem::Update(EcsManager& handle, float dt) {
    auto& meshes = handle.getPool<Mesh>()->getComponents();
    auto& mats = handle.getPool<Material>()->getComponents();
    auto& transforms = handle.getPool<glm::mat4>()->getComponents();
    auto& activeCamera = handle.getPool<ActiveCamera>()->getComponents();

    renderer->frameBeing();
    // need sort to match the material to the mesh
    for(int i = 0; i < meshes.size(); i++) {
        renderer->Draw(meshes[i], mats[i], transforms[i], 
            *handle.getComponent<Camera>(activeCamera[0].entityId));
    }
    platform->swapBuffer();
    //renderer->frameEnd();
}