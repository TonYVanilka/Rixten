#pragma once
#include "core/resourceManager/Iloader.h"
#include "core/resourceManager/ResourceManager.h"

template<typename T>
struct Resource {

    ResourceManager& manager;
    Iloader<T>& loader;
    
public:

    Resource(ResourceManager& manager_, Iloader<T>& loader_);
    ResourceIndex load(const char* path);
};

template <typename T>
inline Resource<T>::Resource(ResourceManager& manager_, Iloader<T>& loader_) : 
    manager(manager_), loader(loader_) {
}

template <typename T>
inline ResourceIndex Resource<T>::load(const char* path) {
    T resource = loader.load(path);    
    return manager.createResource(resource);
}
