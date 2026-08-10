#include <cstdio>
#include <glm/gtc/matrix_transform.hpp>

#include "core/ecs/EcsManager.h"
#include "core/memory/MemoryArena.h"
#include "core/render/RenderSystem.h"
#include "core/render/openGL/RendererOpenGL.h"
#include "core/render/openGL/Vertex.h"
#include "utils/logger.h"

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
    "uniform mat4 transform;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transform * vec4(aPos, 1.0f);\n"
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
    "    } else {\n"
    "        FragColor = vec4(material.color, 1.0);\n"
    "    }\n"
    "}\n\0";

float vertices[] = {
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

    // create transformations
    glm::mat4 transform = glm::mat4(1.0f);  // make sure to initialize matrix to identity matrix first
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    renderer->Init();

    MaterialData matData{1.0f, 0.0, 1.0, false};
    
    renderer->createWindow(800, 600, "Test Rixten Window");

    uint32_t txt = createTexture("image.png");

    LOG_DEBUG(txt);

    Material simpleMat {
        renderer->createShader(vertexShaderSource1, fragmentShaderSource1),
        txt,
        renderer->createMaterialUBO(matData)
    };

    RenderSystem renderSys(renderer);

    ecs.createPool<Mesh>();
    ecs.createPool<Material>();
    ecs.createPool<glm::mat4>();
    ecs.RegisterSystem(renderSys);

    Entity simple = ecs.createEntity();

    VertexLayout vl;
    vl.attributeCount = 1;
    vl.attributes[0] = {0, static_cast<uint32_t>(GL_FLOAT), 3, false};
    vl.attributes[0] = {0, static_cast<uint32_t>(GL_FLOAT), 2, false};
    vl.stride = sizeof(Vertex);

    Mesh square = renderer->createMesh(vertices, sizeof(vertices), indices, sizeof(indices), vl);
    ecs.createComponent<Mesh>(simple, square);
    ecs.createComponent<Material>(simple, simpleMat);
    ecs.createComponent<glm::mat4>(simple, transform);

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