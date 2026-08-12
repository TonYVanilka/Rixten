#include "core/MovementSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "core/platform/PlatformGLFW.h"
#include "core/render/Camera.h"

const float SPEED = 0.2f;

void MovementSystem::Update(EcsManager& handle, float dt) {

    auto& transforms = handle.getPool<glm::mat4>()->getComponents();
    auto& input = handle.getPool<InputState>()->getComponents();
    auto& activeCamera = handle.getPool<ActiveCamera>()->getComponents();

    for(int i = 0; i < transforms.size(); i++) {
        Camera* camera = handle.getComponent<Camera>(activeCamera[0].entityId);
        if(input[0].keys[GLFW_KEY_W] == true) 
            camera->cameraPos += SPEED * camera->cameraFront;
        if(input[0].keys[GLFW_KEY_S] == true) 
            camera->cameraPos -= SPEED * camera->cameraFront;
        if(input[0].keys[GLFW_KEY_D] == true)
            camera->cameraPos += glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * SPEED;
        if(input[0].keys[GLFW_KEY_A] == true)
            camera->cameraPos -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * SPEED;
    }

}