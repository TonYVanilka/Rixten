#include "core/ecs/EcsManager.h"

EcsManager::EcsManager() : pools(8), systems(8), arena(MemoryArena::GetInstance()) {
}

EcsManager::~EcsManager() {
}

void EcsManager::Update() {
    for(int i = 0; i < systems.size(); i++) {
        systems[i]->Update(*this, 0.0f); // need add dt
    }
}

Entity EcsManager::createEntity() {
    return ecs.createEntity();
}

// can has problems, i don't test generation in entity
void EcsManager::removeEntity(Entity handle) {
    if (!ecs.isValid(handle)) {LOG_WARN("can't remove entity, Entity nonvalid", handle.index) return;}
    for(int i = 0; i < pools.size(); i++) {
        IcomponentPool* pool = reinterpret_cast<IcomponentPool*>(arena.getPtr(pools[i]));
        if (pool->hasComponent(handle)) 
            pool->removeComponent(handle);
    }
    ecs.deleteEntity(handle);
}

void EcsManager::RegisterSystem(ISystem& handle) {
    systems.push_back(&handle);
}
