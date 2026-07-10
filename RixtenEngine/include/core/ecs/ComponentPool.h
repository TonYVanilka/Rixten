#pragma once
#include "core/ecs/ecs.h"
#include "core/memory/DArr.h"

constexpr uint32_t MinComponentsCount = MAXentities / 4;

template<typename T>
struct ComponentPool {
    
private:

    DArr<T> components;
    DArr<uint32_t> sparse;

    uint32_t componentIndices;
    uint32_t componentCount;

public:

    ComponentPool();
    ~ComponentPool();

    void addComponent(const Entity& handle, const T& component);
    void dellComponent(const Entity& handle);

    T& getComponent(const Entity& handle);

};

template <typename T>
inline ComponentPool<T>::ComponentPool() : components(MinComponentsCount), sparse(MAXentities) {

}

template <typename T>
inline ComponentPool<T>::~ComponentPool() {
    components.free();
    sparse.free();
}

template <typename T>
inline void ComponentPool<T>::addComponent(const Entity& handle, const T& component) {
    components.push_back(component);
    componentIndices++;
    sparse.set(handle.index, componentIndices);
    componentCount++;
}

// not complete 
template <typename T>
inline void ComponentPool<T>::dellComponent(const Entity& handle) {
    T& MoveComponent = components.back();

    components.set(sparse[handle.index], MoveComponent);
    sparse[handle.index] = MoveComponent;
    
}

template <typename T>
inline T& ComponentPool<T>::getComponent(const Entity& handle) {
    if (handle.index > componentCount) return; // need refactor in the future 
    return components[sparse[handle.index]];
}
