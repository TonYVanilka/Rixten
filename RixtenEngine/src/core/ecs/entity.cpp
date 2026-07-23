#include "core/ecs/entity.h"

ECS::ECS() : generation(MAXentities), 
    freeList(MAXentities / 2), // MAXentities / 2 for memory economy 
    entitiesCount(0) {
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
        if(generation.size() + 1 > MAXentities) {
            LOG_WARN("generation list of entities overfull, resize");
            generation.resize(MAXentities * 2);
        }
        generation.push_back(0);
        return Entity{uint16_t(generation.size() - 1), 0};
    }
}

void ECS::deleteEntity(const Entity& handle) {
    generation[handle.index]++;
    if (freeList.size() + 1 > freeList.maxSize()) {
        LOG_WARN("freeList list of entities overfull, resize");
        freeList.resize(freeList.maxSize() * 2);
    }
    freeList.push_back(handle.index);
}

// need call when you get component and same function
bool ECS::isValid(const Entity& handle) {
    // LOG_WARN("Entity ", handle.index, " no valide ", 
    //     handle.index < generation.size() && handle.generation == generation[handle.index]
    // );
    return handle.index < generation.size() && handle.generation == generation[handle.index];
}