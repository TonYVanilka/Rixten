#pragma once
#include <glm/glm.hpp>
#include "core/ecs/entity.h"

struct ActiveCamera {
    Entity entityId;
};

struct Camera {
    glm::vec3 cameraPos; // in future delegete it to another component
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float yaw;
    float pitch;
    bool firstMouse;
};