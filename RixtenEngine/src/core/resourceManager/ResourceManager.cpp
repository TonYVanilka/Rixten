#include "core/resourceManager/ResourceManager.h"
#include "core/memory/MemoryArena.h"

ResourceManager::ResourceManager() : resourcesPools(MINassets), arena(MemoryArena::GetInstance()) {
}

ResourceManager::~ResourceManager() {
}
