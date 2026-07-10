#pragma once

#include <cstdint>

#include "core/memory/DArr.h"

constexpr uint16_t MAXentities = 500;

struct Entity {
    uint32_t index;
    uint16_t generation;
};

struct ECS {

private:

    DArr<Entity> entities;
    DArr<uint32_t> sparse;

    uint32_t entityCount;
    uint32_t entityIndices;
    uint32_t freeSlotsHeader;
    
public:

    ECS();
    ~ECS();

    Entity& createEntity();
    void deleteEntity(const Entity& handle);

    Entity& getEntity(const Entity& handle);
};