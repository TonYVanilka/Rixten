#include "core/ecs/entity.h"

ECS::ECS() : generation(MAXentities), freeList(MAXentities / 2), entitiesCount(0) {
}

ECS::~ECS() {
    generation.free();
    freeList.free();
}

Entity ECS::createEntity() {
    if(!freeList.empty()) {
        uint32_t index = freeList.back();
        freeList.delete_back();
        return Entity{index, generation[index]};
    } else {
        generation.set(entitiesCount, 0);
        return Entity{entitiesCount++, 0};
    }
}

void ECS::deleteEntity(const Entity& handle) {
    generation[handle.index]++;
    freeList.push_back(handle.index);
}

// need call when you get component and same function
bool ECS::isValid(const Entity& handle) {
    return handle.index < entitiesCount && handle.generation == generation[handle.index];
}