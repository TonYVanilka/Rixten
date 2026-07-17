#include "core/ecs/entity.h"

ECS::ECS() : generation(MAXentities), freeList(MAXentities / 2), entitesCount(0) {
}

ECS::~ECS() {
    generation.free();
    freeList.free();
}

Entity ECS::createEntity() {
    if(!freeList.empty()) {
        // generation.push_back(0);
        uint32_t index = freeList.back();
        freeList.delete_back();
        return Entity{index, generation[index]};
    } else {
        //generation.
        generation.set(entitesCount, 0);
        return Entity{entitesCount++, 0};
    }
}

void ECS::deleteEntity(const Entity& handle) {
    generation[handle.index]++;
    freeList.push_back(handle.index);
}

bool ECS::isValid(const Entity& handle) {
    return handle.generation == generation[handle.index];
}