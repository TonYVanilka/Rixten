#pragma once

class EcsManager;

struct ISystem {
    virtual ~ISystem() = default;
    virtual void Update(EcsManager& handle, float dt) = 0;
};
