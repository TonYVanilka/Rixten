#include "core/ecs/ecs.h"

ECS::ECS() : 
    entities(MAXentities), 
    sparse(MAXentities),
    entityCount(0),
    entityIndices(0) {
}

ECS::~ECS() {
    entities.free();
    sparse.free();
}

Entity& ECS::createEntity() {
    
    entities.push_back({entityIndices, 0});
    
    sparse.set(entityIndices, entities.size() - 1);
    
    entityIndices++;
    entityCount++;
    return entities.back();
}

void ECS::deleteEntity(const Entity& handle) {

    Entity movedEntity = entities.back();

    entities.set(sparse[handle.index], movedEntity);
    sparse[handle.index] = movedEntity.index;
    
    entities.delete_back();
    entityCount--;
}

Entity& ECS::getEntity(const Entity& handle) {
    if (handle.index > entityCount); // need refactor in the future 
    return entities[sparse[handle.index]];
}