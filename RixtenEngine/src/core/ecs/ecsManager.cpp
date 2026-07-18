#include "core/ecs/ecsManager.h"

ecsManager::ecsManager() : pools(8), systems(8), arena(MemoryArena::GetInstance()) {
}

ecsManager::~ecsManager() {
}

void ecsManager::Update() {
    for(int i = 0; i < systems.size(); i++) {
        systems[i]->Update(*this, 4.6); // need add dt
    }
}

Entity ecsManager::createEntity() {
    return ecs.createEntity();
}

// doesn't work
void ecsManager::removeEntity(Entity handle) {
    // ecs.deleteEntity(handle);
    // for(int i = 0; i < pools.size(); i++) {
    //     ComponentPool<R>
    //     pools[i].removeComponent();
    // }
}

void ecsManager::RegisterSystem(ISystem& handle) {
    systems.push_back(&handle);
}
