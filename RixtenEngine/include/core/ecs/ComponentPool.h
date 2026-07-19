#pragma once
#include "core/ecs/entity.h"
#include "core/memory/DArr.h"

constexpr uint32_t MinComponentsCount = MAXentities / 4;

template<typename T>
struct ComponentPool {
    
private:
    
    DArr<T> dense_components;
    DArr<uint32_t> sparse;
    DArr<uint32_t> entitiesIndices;

    uint32_t componentCount;
    uint32_t lastComponentIndex;

public:

    ComponentPool();
    ~ComponentPool();

    void addComponent(const Entity& handle, const T& component);
    void removeComponent(const Entity& handle);

    bool hasComponent(const Entity& handle) const;

    T* getComponent(const Entity& handle);
    DArr<T>& getComponents();

};

template <typename T>
inline ComponentPool<T>::ComponentPool() : dense_components(MinComponentsCount), sparse(MAXentities), 
    entitiesIndices(MAXentities) {

}

template <typename T>
inline ComponentPool<T>::~ComponentPool() {
    dense_components.free();
    sparse.free();
    entitiesIndices.free();
}

template <typename T>
inline void ComponentPool<T>::addComponent(const Entity& handle, const T& component) {
    
    dense_components.push_back(component);
    sparse.set(handle.index, dense_components.size() - 1);
    entitiesIndices.set(dense_components.size() - 1, handle.index);
    componentCount++;
}

template <typename T>
inline void ComponentPool<T>::removeComponent(const Entity& handle) {
    
    uint32_t removedComponent = sparse[handle.index];
    dense_components[removedComponent] = dense_components.back();

    sparse[entitiesIndices[dense_components.size() - 1]] = removedComponent;

    dense_components.delete_back();
    componentCount--;
}

template <typename T>
inline bool ComponentPool<T>::hasComponent(const Entity& handle) const {
    return sparse[handle.index] < dense_components.size();
}

template <typename T>
inline T* ComponentPool<T>::getComponent(const Entity& handle) {
     // need refactor in the future
    return &dense_components[sparse[handle.index]];
}

template <typename T>
inline DArr<T>& ComponentPool<T>::getComponents() {
    return dense_components;
}
