#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/ecs/EcsManager.h"
#include "core/memory/MemoryArena.h"
#include "core/render/RenderSystem.h"
#include "core/render/openGL/RendererOpenGL.h"
#include "core/render/openGL/Vertex.h"
#include "utils/logger.h"
#include "core/platform/PlatformGLFW.h"
#include "core/render/Camera.h"

#include "core/MovementSystem.h"

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

const char* vertexShaderSource1 =
    "#version 420 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 model;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "   TexCoord = aTexCoord;\n"
    "}\0";

const char* fragmentShaderSource1 =
    "#version 420 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoord;\n"
    "\n"
    "uniform sampler2D ourTexture;\n"
    "uniform int useTexture;\n"
    "\n"
    "layout (std140, binding = 2) uniform MaterialBlock {\n"
    "    vec3 color;\n"
    "    int useTexture;\n"
    "} material;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    if (material.useTexture != 0) {\n"
    "        FragColor = texture(ourTexture, TexCoord);\n"
    "        //FragColor = vec4(TexCoord, 0.0, 1.0);\n"
    "    } else {\n"
    "        FragColor = vec4(material.color, 1.0);\n"
    "    }\n"
    "}\n\0";

float verticesC[] = {
    // positions          // texture coords
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    // Left face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
    // Right face
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f
};

unsigned int indicesC[] = {
    // Front face
    0, 1, 2,
    0, 2, 3,
    // Back face
    4, 6, 5,
    4, 7, 6,
    // Left face
    8, 9, 10,
    8, 10, 11,
    // Right face
    12, 13, 14,
    12, 14, 15,
    // Top face
    16, 17, 18,
    16, 18, 19,
    // Bottom face
    20, 22, 21,
    20, 23, 22
};

float vertices[] = {
    // positions          // texture coords
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,    // top right
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f    // top left
};
unsigned int indices[] = {
    0, 1, 3,  // first triangle
    1, 2, 3   // second triangle
};

float verticesCube[] = {
    // позиции          // текстурные координаты
    // Задняя грань (z = -0.5)
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 0
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,   // 1
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,    // 2
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // 3

    // Передняя грань (z = 0.5)
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // 4
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // 5
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    // 6
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // 7

    // Левая грань (x = -0.5)
    -0.5f, 0.5f, 0.9f, 1.0f, 0.0f,    // 8
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 9
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // 10
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // 11

    // Правая грань (x = 0.5)
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // 12
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 13
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // 14
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // 15

    // Нижняя грань (y = -0.5)
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // 16
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,   // 17
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,    // 18
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // 19

    // Верхняя грань (y = 0.5)
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // 20
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 21
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // 22
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f    // 23
};

unsigned int indicesCube[] = {
    // Задняя грань
    0, 1, 2, 2, 3, 0,
    // Передняя грань
    4, 5, 6, 6, 7, 4,
    // Левая грань
    8, 9, 10, 10, 11, 8,
    // Правая грань
    12, 13, 14, 14, 15, 12,
    // Нижняя грань
    16, 17, 18, 18, 19, 16,
    // Верхняя грань
    20, 21, 22, 22, 23, 20
};

IRenderApi* renderer;
Camera* camera;
uint32_t createTexture(const char* path);

int main() {

    // just like in future init in RixtenRoot
    MemoryArena& arena = MemoryArena::GetInstance();
    
    EcsManager ecs;
    
    InputState inputState = {};

    Entity singltone = ecs.createEntity();

    ecs.createPool<InputState>(1);
    ecs.createComponent<InputState>(singltone, inputState);
    ecs.createPool<ActiveCamera>(1);
    
    PlatformGLFW platform;
    platform.Init(800, 600, "Rixten GLFW window", ecs.getComponent<InputState>(singltone));

    Camera cameraComponent = {
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    Camera cameraComponent1 = {
        glm::vec3(0.0f, 0.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    ecs.createPool<Camera>();
    Entity camera = ecs.createEntity();
    ecs.createComponent<Camera>(camera, cameraComponent);

    Entity camera1 = ecs.createEntity();
    ecs.createComponent<Camera>(camera1, cameraComponent1);

    ActiveCamera aCamera = {camera};

    //Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
    ecs.createComponent<ActiveCamera>(singltone, aCamera);
    
    #ifdef VULCAN_API
    renderer = nullptr
    #error Doesn't support vulcan
    #else
    renderer = new(arena.getPtr(arena.allocate(sizeof(RendererOpenGL), alignof(RendererOpenGL))))
    RendererOpenGL();
    #endif
    
    // create transformations
    glm::mat4 transform = glm::mat4(1.0f);  // make sure to initialize matrix to identity matrix first
    transform = glm::rotate(transform, glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    
    
    renderer->Init();
    
    MaterialData matData{1.0f, 0.0, 1.0, true};
    Material simpleMat {
        renderer->createShader(vertexShaderSource1, fragmentShaderSource1),
        createTexture("bresenham4.png"),
        renderer->createMaterialUBO(matData)
    };
    
    
    RenderSystem renderSys(renderer, &platform);
    MovementSystem myvSys;
    
    ecs.createPool<Mesh>();
    ecs.createPool<Material>();
    ecs.createPool<glm::mat4>();

    ecs.RegisterSystem(renderSys);
    ecs.RegisterSystem(myvSys);

    Entity simple = ecs.createEntity();
    
    VertexLayout vl;
    vl.attributeCount = 2;
    vl.attributes[0] = {0, static_cast<uint32_t>(GL_FLOAT), 3, false};
    vl.attributes[1] = {0, static_cast<uint32_t>(GL_FLOAT), 2, false};
    vl.stride = sizeof(Vertex);
    
    Mesh square = renderer->createMesh(verticesC, sizeof(verticesC), indicesC, sizeof(indicesC), vl);
    ecs.createComponent<Mesh>(simple, square);
    ecs.createComponent<Material>(simple, simpleMat);
    ecs.createComponent<glm::mat4>(simple, transform);

    ecs.getComponent<ActiveCamera>(singltone)->entityId = camera1;

    while (true) {
        platform.poolEvent();
        ecs.Update();
    }
}

uint32_t createTexture(const char* path) {
    int width, height, nrChannels;
    uint32_t texture;
    // stbi call new out of global Rixten arena!
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        texture = renderer->createTexture(data, width, height, nrChannels);
    } else {
        LOG_ERROR("Failed to load texture");
    }
    stbi_image_free(data);
    return texture;
}