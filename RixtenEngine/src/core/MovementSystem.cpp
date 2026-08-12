#include "core/MovementSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "core/platform/PlatformGLFW.h"
#include "core/render/Camera.h"

const float SPEED = 2.5f;

void MovementSystem::Update(EcsManager& handle, float dt) {

    auto& transforms = handle.getPool<glm::mat4>()->getComponents();
    auto& input = handle.getPool<InputState>()->getComponents();
    auto& activeCamera = handle.getPool<ActiveCamera>()->getComponents();

    for(int i = 0; i < transforms.size(); i++) {
        float MovementSpeed = dt * SPEED;
        Camera* camera = handle.getComponent<Camera>(activeCamera[0].entityId);

        if(camera->firstMouse == true) {
            input[0].mouseDeltaX = input[0].mouseX;
            input[0].mouseDeltaY = input[0].mouseY;
            camera->firstMouse = false;
        }

        float xoffset = input[0].mouseX - input[0].mouseDeltaX;
        float yoffset = input[0].mouseDeltaY - input[0].mouseY;  // reversed since y-coordinates range from bottom to top
        input[0].mouseDeltaX = input[0].mouseX;
        input[0].mouseDeltaY = input[0].mouseY;

        const float sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera->yaw += xoffset;
        camera->pitch += yoffset;

        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        direction.y = sin(glm::radians(camera->pitch));
        direction.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
        camera->cameraFront = glm::normalize(direction);

        if(input[0].keys[GLFW_KEY_W] == true)
            camera->cameraPos += MovementSpeed * camera->cameraFront;
        if(input[0].keys[GLFW_KEY_S] == true)
            camera->cameraPos -= MovementSpeed * camera->cameraFront;
        if(input[0].keys[GLFW_KEY_D] == true)
            camera->cameraPos += glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * MovementSpeed;
        if(input[0].keys[GLFW_KEY_A] == true)
            camera->cameraPos -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * MovementSpeed;
    }
}