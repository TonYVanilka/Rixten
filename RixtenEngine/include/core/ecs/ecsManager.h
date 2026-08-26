#pragma once

#include "core/ecs/Isystem.h"
#include "core/ecs/componentPool.h"
#include "core/ecs/entity.h"
#include "core/memory/MemoryArena.h"  // need refactor
#include "utils/typeIDgenerator.h"

class EcsManager {

private:

    DArr<size_t> pools;
    DArr<ISystem*> systems;
    ECS ecs;

    MemoryArena& arena;

    struct EcsDomain;

public:

    EcsManager();
    ~EcsManager();

    void Update(float deltaTime);

    Entity createEntity();
    void removeEntity(const Entity& handle);

    template <typename T>
    size_t createPool(uint16_t componentCount = MAXentities, 
        uint16_t spraceMinComponentCount = MAXentities);
    template <typename T>
    ComponentPool<T>* getPool();

    template <typename T, typename... Args>
    T* createComponent(const Entity& handle, Args&&... args);
    template <typename T>
    T* getComponent(const Entity& handle);
    template <typename T>
    void removeComponent(const Entity& handle);

    void RegisterSystem(ISystem& handle);
};

template <typename T>
inline size_t EcsManager::createPool(uint16_t componentCount, uint16_t spraceMinComponentCount) {
    size_t indx = typeIDgenerator::id<EcsDomain, T>();

    size_t arenaOffset = arena.allocate(sizeof(ComponentPool<T>), alignof(ComponentPool<T>));
    new(arena.getPtr(arenaOffset)) ComponentPool<T>(componentCount);
    pools.set(indx, arenaOffset);

    return indx;
}

template <typename T>
inline ComponentPool<T>* EcsManager::getPool() {
    size_t indx = typeIDgenerator::id<EcsDomain,T>();
    return reinterpret_cast<ComponentPool<T>*>(arena.getPtr(pools[indx]));
}

template <typename T, typename... Args>
inline T* EcsManager::createComponent(const Entity& handle, Args&&... args) {
    ComponentPool<T>* pool = getPool<T>();
    pool->addComponent(handle, std::forward<Args>(args)...);
    return pool->getComponent(handle);
}

template <typename T>
inline T* EcsManager::getComponent(const Entity& handle) {
    ComponentPool<T>* pool = getPool<T>();
    return pool->getComponent(handle);
}

template <typename T>
inline void EcsManager::removeComponent(const Entity& handle) {
    if (!ecs.isValid(handle)) {LOG_WARN("ECS: can't remove entity, Entity nonvalid", handle.index) return;}
    ComponentPool<T>* pool = getPool<T>();
    pool->removeComponent(handle);
}
