#pragma once

struct ISystem {
    virtual ~ISystem() = default;
    virtual void Update() = 0;
};
