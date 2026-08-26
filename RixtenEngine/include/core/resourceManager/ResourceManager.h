#pragma once
#include <stdint.h>

#include "core/memory/DArr.h"
#include "core/memory/MemoryArena.h"
#include "core/render/IRenderApi.h"
#include "utils/typeIDgenerator.h"
#include "core/resourceManager/resourceTypes.h"

constexpr uint16_t MINassets = 3;

class ResourceManager {

    DArr<size_t> resourcesPools;
    MemoryArena& arena;

    struct ResourceDomine;

public:
    
    ResourceManager();
    ~ResourceManager();

    template <typename T>
    size_t registerResource(uint32_t resourcesCount = 1);
    template <typename T>
    DArr<T>* getPool();

    template <typename T, typename... Args>
    ResourceIndex createResource(Args&&... args);
    template <typename T>
    void removeResource(ResourceIndex index);

    template <typename T>
    T& getResource(ResourceIndex index);
};

// for delete components need other array
template <typename T>
inline size_t ResourceManager::registerResource(uint32_t resourcesCount) {
    size_t index = typeIDgenerator::id<ResourceDomine, T>();

    size_t arenaOffsetResource = arena.allocate(sizeof(DArr<T>), alignof(DArr<T>));
    new(arena.getPtr(arenaOffsetResource)) DArr<T>(resourcesCount);
    resourcesPools.set(index, arenaOffsetResource);

    return index;
}

template <typename T>
inline DArr<T>* ResourceManager::getPool() {
    size_t index = typeIDgenerator::id<ResourceDomine, T>();
    return reinterpret_cast<DArr<T>*>(arena.getPtr(resourcesPools[index]));
}

template <typename T, typename... Args>
inline ResourceIndex ResourceManager::createResource(Args&&... args) {
    DArr<T>* pool = getPool<T>();
    pool->emplace_back(std::forward<Args>(args)...);
    return pool->size() - 1;
}

// not realized
template <typename T>
inline void ResourceManager::removeResource(ResourceIndex index) {
}

template <typename T>
inline T& ResourceManager::getResource(ResourceIndex index) {
    DArr<T>* pool = getPool<T>();
    return (*pool)[index];
}
