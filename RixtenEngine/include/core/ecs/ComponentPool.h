#pragma once
#include "core/ecs/IcomponentPool.h"
#include "core/memory/DArr.h"

// refactor if need optimization
constexpr uint16_t MinComponentsCount = MAXentities;

template<typename T>
struct ComponentPool : IcomponentPool {
    
private:
    
    DArr<T> dense_components;
    DArr<uint32_t> sparse;
    DArr<uint32_t> entitiesIndices;

    uint32_t componentCount;

public:

    ComponentPool(uint16_t componentCount = MinComponentsCount, 
        uint16_t spraceMinComponentCount = MAXentities);
    ~ComponentPool();

    void addComponent(const Entity& handle, const T& component);
    void removeComponent(const Entity& handle) override;

    bool hasComponent(const Entity& handle) const override;

    T* getComponent(const Entity& handle);
    DArr<T>& getComponents();

};

template <typename T>
inline ComponentPool<T>::ComponentPool(uint16_t componentCount, uint16_t spraceMinComponentCount) : 
    dense_components(componentCount), sparse(MAXentities), entitiesIndices(componentCount), componentCount(0) {
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
    if (!hasComponent(handle)) {LOG_WARN("Component pool hasn't Entity ", handle.index) return;}
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
