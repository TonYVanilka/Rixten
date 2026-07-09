#pragma once

#include <cstdint>

#include "core/memory/DArr.h"

constexpr uint16_t MAXentities = 500;

struct Entity {
    uint32_t index;
    uint32_t generation;
};

struct ECS {

private:

    DArr<Entity> entities;
    
public:

    ECS();
    ~ECS();

    Entity& createEntity();
    void deleteEntity(uint32_t index);
};

