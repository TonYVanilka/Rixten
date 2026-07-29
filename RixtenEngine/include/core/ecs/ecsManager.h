#pragma once
#include "core/ecs/entity.h"
#include "core/ecs/componentPool.h"
#include "core/ecs/Isystem.h"
#include "utils/typeIDgenerator.h"

#include "core/memory/MemoryArena.h" // need refactor

class EcsManager {

private:

    DArr<size_t> pools;
    DArr<ISystem*> systems;
    ECS ecs;

    MemoryArena& arena;

public:

    EcsManager();
    ~EcsManager();

    void Update();

    Entity createEntity();
    void removeEntity(Entity handle);

    template <typename T>
    size_t createPool();
    template <typename T>
    ComponentPool<T>* getPool();

    template <typename T>
    T* createComponent(Entity handle, T component);
    template <typename T>
    T* getComponent(Entity handle);
    template <typename T>
    void removeComponent(Entity handle, T component);

    void RegisterSystem(ISystem& handle);
};

template <typename T>
inline size_t EcsManager::createPool() {
    size_t indx = typeIDgenerator::id<T>();

    size_t arenaOffset = arena.allocate(sizeof(ComponentPool<T>), alignof(ComponentPool<T>));
    new(arena.getPtr(arenaOffset)) ComponentPool<T>();
    pools.set(indx, arenaOffset);

    return indx;
}

template <typename T>
inline ComponentPool<T>* EcsManager::getPool() {
    size_t indx = typeIDgenerator::id<T>();
    return reinterpret_cast<ComponentPool<T>*>(arena.getPtr(pools[indx]));
}

template <typename T>
inline T* EcsManager::createComponent(Entity handle, T component) {
    ComponentPool<T>* pool = getPool<T>();
    pool->addComponent(handle, component);
    return pool->getComponent(handle);
}

template <typename T>
inline T* EcsManager::getComponent(Entity handle) {
    ComponentPool<T>* pool = getPool<T>();
    return pool->getComponent(handle);
}

template <typename T>
inline void EcsManager::removeComponent(Entity handle, T component) {
    ComponentPool<T>* pool = getPool<T>();
    pool->removeComponent(handle, component);
}
