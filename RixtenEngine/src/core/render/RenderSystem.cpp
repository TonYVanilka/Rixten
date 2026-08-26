#include "core/render/RenderSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/ecs/EcsManager.h"
#include "core/platform/PlatformGLFW.h"
#include "core/render/Camera.h"
#include "core/resourceManager/resources/ModelResource.h"
#include "core/ecs/components/model.h"

RenderSystem::RenderSystem(IRenderApi* renderer_, PlatformGLFW* platform_, ResourceManager* resourceManager_) : 
    renderer(renderer_), platform(platform_), resourceManager(resourceManager_) {
}

void RenderSystem::Update(EcsManager& handle, float dt) {
    auto& meshes = handle.getPool<MeshGPU>()->getComponents();
    auto& mats = handle.getPool<Material>()->getComponents();

    //auto& model = handle.getPool<Model>()->getComponents();
    
    auto& transforms = handle.getPool<glm::mat4>()->getComponents();
    auto& activeCamera = handle.getPool<ActiveCamera>()->getComponents();
    auto& input = handle.getPool<InputState>()->getComponents();
    
    renderer->setViewportSize(input[0].width, input[0].height);
    renderer->clear();
    // need sort to match the material to the mesh
    for (int i = 0; i < meshes.size(); i++) {
        //auto& modelResource = resourceManager->getResource<ModelResource>(model[i].modelResource);

        renderer->Draw(meshes[i], mats[i], transforms[i],
                       *handle.getComponent<Camera>(activeCamera[0].entityId));

        // for (int j = 0; j < modelResource.subMeshes.size(); j++) {
        //     auto& mesh = resourceManager->getResource<MeshResource>(modelResource.subMeshes[j].mesh);
        //     auto& material = resourceManager->getResource<MeshResource>(modelResource.subMeshes[j].material);
        // }
        transforms[i] = glm::rotate(transforms[i], glm::radians(dt * 20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    }
    platform->swapBuffer();
}