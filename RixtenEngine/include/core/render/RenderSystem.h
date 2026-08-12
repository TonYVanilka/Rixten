#pragma once
#include "core/ecs/Isystem.h"
#include "core/render/IRenderApi.h"
#include "core/platform/PlatformGLFW.h"

struct RenderSystem : ISystem {

    IRenderApi* renderer;
    PlatformGLFW* platform;

public:

    RenderSystem(IRenderApi* renderer_, PlatformGLFW* platform_);
    void Update(EcsManager& handle, float dt) override;
};
