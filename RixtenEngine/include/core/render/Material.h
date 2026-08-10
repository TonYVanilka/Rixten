#pragma once
#include <stdint.h>
#include "core/render/ShaderBindings.h"

struct MaterialData {
    float color[3];
    int useTexture;
};

struct Material {
    uint32_t shaderProgram;
    uint32_t texture;
    uint32_t ubo;
};