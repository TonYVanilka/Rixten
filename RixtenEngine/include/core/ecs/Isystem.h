#pragma once

class ecsManager;

struct ISystem {
    virtual ~ISystem() = default;
    virtual void Update(ecsManager& handle, float dt) = 0;
};
