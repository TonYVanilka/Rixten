#pragma once
#include <cstdint>

struct Mesh {
    uint32_t index;

    uint32_t vbo;
    uint32_t ibo;
    uint32_t vertexCount;
    uint32_t indexCount;
};