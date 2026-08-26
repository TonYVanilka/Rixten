#pragma once

#include <stdint.h>
#include "core/resourceManager/Iloader.h"
#include "core/render/IRenderApi.h"

struct texture {
    int width, height, nrChannels;
    //must replace to global Rixten arena
    //size_t dataArenaOffset;
    unsigned char* data;
    uint32_t gpuIndex;
};

namespace textureLoader  {
    texture load(const char* path);
    void unload(texture& handle);
}
