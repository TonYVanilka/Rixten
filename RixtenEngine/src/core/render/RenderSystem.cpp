#include "core/render/RenderSystem.h"
#include "core/ecs/EcsManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

RenderSystem::RenderSystem(IRenderApi* renderer_) : renderer(renderer_) {
}

void RenderSystem::Update(EcsManager& handle, float dt) {
    auto& meshes = handle.getPool<Mesh>()->getComponents();
    auto& mats = handle.getPool<Material>()->getComponents();
    auto& transforms = handle.getPool<glm::mat4>()->getComponents();

    renderer->frameBeing();
    // need sort to match the material to the mesh
    for(int i = 0; i < meshes.size(); i++) {
        renderer->Draw(meshes[i], mats[i], transforms[i]);
    }
    renderer->frameEnd();
}