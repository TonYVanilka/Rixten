#pragma once
#include "core/ecs/Isystem.h"
#include "core/render/IRenderApi.h"
#include "core/platform/PlatformGLFW.h"
#include "core/resourceManager/ResourceManager.h"

struct RenderSystem : ISystem {

    IRenderApi* renderer;
    PlatformGLFW* platform;
    ResourceManager* resourceManager;

public:

    RenderSystem(IRenderApi* renderer_, PlatformGLFW* platform_, ResourceManager* resourceManager_);
    void Update(EcsManager& handle, float dt) override;
};
