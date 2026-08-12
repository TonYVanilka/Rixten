#pragma once
#include "core/ecs/EcsManager.h"
#include "core/ecs/Isystem.h"

class MovementSystem : public ISystem {

private:
    void Update(EcsManager& handle, float dt) override;
};