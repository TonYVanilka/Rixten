#include "core/ecs/ecsManager.h"

ecsManager::ecsManager() : pools(8), systems(8), arena(MemoryArena::GetInstance()) {
}

ecsManager::~ecsManager() {
}

void ecsManager::Update() {
    for(int i = 0; i < systems.size(); i++) {
        systems[i]->Update(*this, 0.0f); // need add dt
    }
}

Entity ecsManager::createEntity() {
    return ecs.createEntity();
}

// can has problems, i don't test generation in entity
void ecsManager::removeEntity(Entity handle) {
    if (!ecs.isValid(handle)) {LOG_WARN("can't remove entity, Entity nonvalid", handle) return;}
    for(int i = 0; i < pools.size(); i++) {
        IcomponentPool* pool = reinterpret_cast<IcomponentPool*>(arena.getPtr() + pools[i]);
        if (pool->hasComponent(handle)) 
            pool->removeComponent(handle);
    }
    ecs.deleteEntity(handle);
}

void ecsManager::RegisterSystem(ISystem& handle) {
    systems.push_back(&handle);
}
