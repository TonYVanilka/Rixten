#include "ecs.h"

ECS::ECS() : 
    entities(MAXentities), 
    sparse(MAXentities), /* devide by 2 for economy memory */
    countInSparse(0),
    entityCount(0) {
}

ECS::~ECS() {
    entities.free();
    sparse.free();
}

Entity& ECS::createEntity() {
    
    entities.push_back({entityCount, 0});
    entityCount++;
    
    sparse.set(entityCount, entities.size());
    countInSparse++;

    return entities.back();
}

void ECS::deleteEntity(const Entity& handle) {

    Entity movedEntity = entities.back();

    entities.set(sparse[handle.index], entities.back());
    sparse[handle.index];
    
    entities.delete_back();

}

Entity& ECS::getEntity(const Entity& handle) {
    if (handle.index > entityCount) return 0; // need refactor in the future 
    return entities[sparse[handle.index]];
}
