#pragma once
#include <cstdint>

struct Attribute {
    uint32_t offset;
    uint32_t type;
    uint8_t count;
    bool normalize;
};

struct VertexLayout {
    Attribute attributes[8];
    uint32_t stride;
    uint8_t attributeCount;
};