#pragma once
#include "core/ecs/Isystem.h"
#include "core/render/IRenderApi.h"

struct RenderSystem : ISystem {

    IRenderApi* renderer;

public:

    RenderSystem(IRenderApi* renderer_);
    void Update(EcsManager& handle, float dt) override;
};
