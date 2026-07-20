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

    DArr<uint16_t> generation;
    DArr<uint32_t> freeList;
    uint32_t entitiesCount;
    
public:

    ECS();
    ~ECS();

    Entity createEntity();
    void deleteEntity(const Entity& handle);

    bool isValid(const Entity& handle);

};