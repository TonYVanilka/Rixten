#include "core/ecs/EcsManager.h"

EcsManager::EcsManager() : pools(8), systems(8), arena(MemoryArena::GetInstance()) {
}

EcsManager::~EcsManager() {
}

void EcsManager::Update(float deltaTime) {
    for(int i = 0; i < systems.size(); i++) {
        systems[i]->Update(*this, deltaTime);  // need add dt
    }
}

Entity EcsManager::createEntity() {
    return ecs.createEntity();
}

// can has problems, has optimization with bit masks
// can has problem with generation
void EcsManager::removeEntity(const Entity& handle) {
    if (!ecs.isValid(handle)) {LOG_WARN("ECS: can't remove entity, Entity nonvalid", handle.index) return;}
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
