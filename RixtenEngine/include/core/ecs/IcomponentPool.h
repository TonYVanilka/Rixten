#pragma once
#include "core/ecs/entity.h"

struct IcomponentPool {
    virtual ~IcomponentPool() = default;
    virtual void removeComponent(const Entity& handle) = 0;
    virtual bool hasComponent(const Entity& handle) const = 0;
};
