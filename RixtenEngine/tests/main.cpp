#include <cstdio>
#include "core/ecs/EcsManager.h"
#include "utils/logger.h"
#include "core/render/openGL/RendererOpenGL.h"
#include "core/render/RenderSystem.h"
#include "core/memory/MemoryArena.h"
#include "core/render/openGL/RendererOpenGL.h"

#define STB_IMAGE_IMPLEMENTATION
#include <core/resourceManager/stb_image.h>

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   TexCoord = aTexCoord;\n"
    "}\0";
const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(ourTexture, TexCoord);\n"
    "}\n\0";

// Первый квадрат (центр)
float vertices[] = {
    // позиции          // текстурные координаты
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,    // top right
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f    // top left
};
unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};

// Второй квадрат (слева сверху)
float vertices2[] = {
    // позиции           // текстурные координаты
    -0.3f, 0.9f, 0.0f, 1.0f, 1.0f,  // top right
    -0.3f, 0.1f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.9f, 0.1f, 0.0f, 0.0f, 0.0f,  // bottom left
    -0.9f, 0.9f, 0.0f, 0.0f, 1.0f   // top left
};
unsigned int indices2[] = {
    0, 1, 3,
    1, 2, 3};

// Третий квадрат (справа снизу)
float vertices3[] = {
    // позиции           // текстурные координаты
    0.9f, -0.1f, 0.0f, 1.0f, 1.0f,  // top right
    0.9f, -0.9f, 0.0f, 1.0f, 0.0f,  // bottom right
    0.1f, -0.9f, 0.0f, 0.0f, 0.0f,  // bottom left
    0.1f, -0.1f, 0.0f, 0.0f, 1.0f   // top left
};
unsigned int indices3[] = {
    0, 1, 3,
    1, 2, 3};

IRenderApi* renderer;
uint32_t createTexture(const char* path);

int main() {
    MemoryArena& arena = MemoryArena::GetInstance();

    EcsManager ecs;


    #ifdef VULCAN_API
        renderer = nullptr
        #error Doesn't support vulcan
    #else
        renderer = new(arena.getPtr(arena.allocate(sizeof(RendererOpenGL), alignof(RendererOpenGL)))) 
        RendererOpenGL();
    #endif

    renderer->Init();
    renderer->createWindow(800, 600, "Test Rixten Window");
    renderer->createShader(vertexShaderSource, fragmentShaderSource);

    RenderSystem renderSys(renderer);

    ecs.createPool<Mesh>();
    ecs.RegisterSystem(renderSys);

    Entity notTriangle = ecs.createEntity();
    Entity notTriangle1 = ecs.createEntity();
    Entity notTriangle2 = ecs.createEntity();

    uint32_t firstTexture = createTexture("bresenham4.png");
    uint32_t secondTexture = createTexture("image.png");
    uint32_t thirdTexture = createTexture("{47B42AFB-E60D-48DB-A8FA-DD38B4C06C89}.png");

    Mesh triangle = {
        0,
        renderer->createVertexBuffer(vertices, sizeof(vertices)),
        renderer->createIndexBuffer(indices, sizeof(indices)),
        sizeof(vertices),
        sizeof(indices),
        firstTexture};

    Mesh triangle2 = {
        0,
        renderer->createVertexBuffer(vertices2, sizeof(vertices2)),
        renderer->createIndexBuffer(indices2, sizeof(indices2)),
        sizeof(vertices2),
        sizeof(indices2),
        secondTexture};

    Mesh triangle3 = {
        0,
        renderer->createVertexBuffer(vertices3, sizeof(vertices3)),
        renderer->createIndexBuffer(indices3, sizeof(indices3)),
        sizeof(vertices3),
        sizeof(indices3),
        thirdTexture};

    ecs.createComponent<Mesh>(notTriangle2, triangle3);
    ecs.createComponent<Mesh>(notTriangle1, triangle2);
    ecs.createComponent<Mesh>(notTriangle, triangle);

    while (true) {
        ecs.Update();
    }
}

uint32_t createTexture(const char* path) {
    int width, height, nrChannels;
    uint32_t texture;
    // stbi call new out of global Rixten arena!
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        texture = renderer->createTexture(data, sizeof(data), width, height, nrChannels);
    } else {
        LOG_ERROR("Failed to load texture");
    }
    stbi_image_free(data);
    return texture;
}
