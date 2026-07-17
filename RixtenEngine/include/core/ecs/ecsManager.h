#pragma once
#include "core/ecs/entity.h"
#include "core/ecs/componentPool.h"
#include "core/ecs/system.h"

class ecsManager {

private:

   // DArr<ComponentPool<T>> pools;

public:

    ecsManager();
    ~ecsManager();

    Entity createEntity(Entity handle);
    void removeEntity(Entity handle);

    template <typename T>
    ComponentPool<T>& createComponent(ComponentPool<T> pool);
    template <typename T>
    T& AddComponent(Entity handle, ComponentPool<T>& pool, T component);
    template <typename T>
    void RemoveComponent(Entity handle, ComponentPool<T>& pool, T component);

    void RegisterSystem(ISystem handle);
};

template <typename T>
inline ComponentPool<T>& ecsManager::createComponent(ComponentPool<T> pool) {
    // TODO: insert return statement here
}

template <typename T>
inline T& ecsManager::AddComponent(Entity handle, ComponentPool<T>& pool, T component) {
    // TODO: insert return statement here
}

template <typename T>
inline void ecsManager::RemoveComponent(Entity handle, ComponentPool<T>& pool, T component) {
}
