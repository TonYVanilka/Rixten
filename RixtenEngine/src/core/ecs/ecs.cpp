#include "ecs.h"

ECS::ECS() : entities(MAXentities) {
}

ECS::~ECS() {
    entities.free();
}

Entity& ECS::createEntity() {
    entities.push_back({uint32_t(entities.size()), 0});
}

void ECS::deleteEntity(uint32_t index) {
    entities[index];
}
