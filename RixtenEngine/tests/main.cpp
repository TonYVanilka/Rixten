#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/MovementSystem.h"
#include "core/ecs/EcsManager.h"
#include "core/memory/MemoryArena.h"
#include "core/platform/PlatformGLFW.h"
#include "core/render/Camera.h"
#include "core/render/RenderSystem.h"
#include "core/render/Vertex.h"
#include "core/render/openGL/RendererOpenGL.h"
#include "core/resourceManager/ResourceManager.h"
#include "core/resourceManager/resources/TextureResource.h"
#include "core/resourceManager/resources/ModelResource.h"
#include "utils/logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <core/resourceManager/stb_image.h>

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormals;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
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
    "layout (location = 1) in vec3 aNormals;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
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
    "        vec4 texColor = texture(ourTexture, TexCoord);\n"
    "        // Отбрасываем пиксели с низким альфа-каналом (для спрайтов, деревьев и т.д.)\n"
    "        if (texColor.a < 0.1) discard;\n"
    "        FragColor = texColor;\n"
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
uint32_t createTexture(const char* path);

int main() {
    
    // just like in future init in RixtenRoot
    MemoryArena& arena = MemoryArena::GetInstance();
    ResourceManager resourceManager;
    EcsManager ecs;
    
    InputState inputState = {};
    inputState.width = 800;
    inputState.height = 600;
    
    Entity singltone = ecs.createEntity();
    
    ecs.createPool<InputState>(1, 1);
    ecs.createComponent<InputState>(singltone, inputState);
    ecs.createPool<ActiveCamera>(1, 1);
    
    PlatformGLFW platform;
    platform.Init(800, 600, "Rixten GLFW window", ecs.getComponent<InputState>(singltone));
    
    Camera cameraComponent = {
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f,
        0.0f,
        true
    };
    
    ecs.createPool<Camera>();
    Entity camera = ecs.createEntity();
    ecs.createComponent<Camera>(camera, cameraComponent);
    
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
    
    //resourceManager.registerResource<texture>(1);
    resourceManager.registerResource<VertexLayout>(1);
    resourceManager.registerResource<MeshResource>(12);
    resourceManager.registerResource<ModelResource>(1);

    // ResourceIndex bresenham4 = resourceManager.createResource<texture>(textureLoader::load("7c77fe21d1286a3f.png.png"));
    // texture bresenham4Resource = resourceManager.getResource<texture>(bresenham4);
    // renderer->createTexture(
    //     bresenham4Resource.data, 
    //     bresenham4Resource.width, 
    //     bresenham4Resource.height, 
    //     bresenham4Resource.nrChannels
    // );
    
    // textureLoader::unload(bresenham4Resource);

    // create transformations
    //glm::mat4 transform = glm::mat4(1.0f);  // make sure to initialize matrix to identity matrix first
    //transform = glm::rotate(transform, glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    
    renderer->Init();

    MaterialData matData{1.0f, 0.0, 1.0, true};

    RenderSystem renderSys(renderer, &platform, &resourceManager);
    MovementSystem myvSys;
    
    ecs.createPool<MeshGPU>(12);
    ecs.createPool<Material>(12);
    ecs.createPool<glm::mat4>(12);

    ecs.RegisterSystem(renderSys);
    ecs.RegisterSystem(myvSys);

    ModelLoaderGLTF modelLoader(&resourceManager);

    //ResourceIndex suzanne = resourceManager.createResource<Model>(modelLoader.load("justin.glb"));
    //Model& suz = resourceManager.getResource<Model>(suzanne);

    ModelResource suz = modelLoader.load("justin.glb");
    texture justinTexture = textureLoader::load("gggg.png");
    uint32_t textureJ = renderer->createTexture(
        justinTexture.data, justinTexture.width, justinTexture.height, justinTexture.nrChannels
    ); 

    for(int i = 0; i < suz.subMeshes.size(); i++) {
        Entity suzanne_glb = ecs.createEntity();

        MeshResource& subMesh = resourceManager.getResource<MeshResource>(suz.subMeshes[i].mesh);
        VertexLayout& subMeshVL = resourceManager.getResource<VertexLayout>(subMesh.vertexLayout);
        for (uint32_t i = 0; i < 5 && i; ++i) {
            float* v =
                reinterpret_cast<float*>(arena.getPtr(subMesh.verticesArenaOffset) + i * subMeshVL.stride);
        }
        MeshGPU subM = renderer->createMesh(
            subMeshVL,  
            arena.getPtr(subMesh.verticesArenaOffset),
            subMesh.verticesCount, 
            arena.getPtr(subMesh.indicesArenaOffset),
            subMesh.indicesCount
        );
        ecs.createComponent<MeshGPU>(suzanne_glb, subM);
        ecs.createComponent<Material>(suzanne_glb, 
            renderer->createShader(vertexShaderSource1, fragmentShaderSource1),
            textureJ, //bresenham4Resource.gpuIndex,
            renderer->createMaterialUBO(matData)
        );
        ecs.createComponent<glm::mat4>(suzanne_glb, glm::mat4(1.0f));
    } 

    // Entity simple = ecs.createEntity();
    
    // VertexLayout vl;
    // vl.attributeCount = 2;
    // vl.attributes[0] = {0, AttributeType::float_type, 3, false};
    // vl.attributes[1] = {0, AttributeType::float_type, 2, false};
    // //vl.attributes[2] = {0, static_cast<uint32_t>(GL_FLOAT), 3, false};
    // vl.stride = sizeof(Vertex);
    
    // Mesh square = renderer->createMesh(vl, verticesC, sizeof(verticesC), indicesC, sizeof(indicesC));
    // ecs.createComponent<Mesh>(simple, square);

    float deltaTime = 0.0f;  // Time between current frame and last frame
    float lastFrame = 0.0f;  // Time of last frame

    float fps = 0.0f;
    int frameCount = 0;
    float timeAccumulator = 0.0f;
    int i = 0;
    while (!platform.WindowShouldClose()) {

        if (ecs.getComponent<InputState>(singltone)->keys[GLFW_KEY_ESCAPE]) {
            platform.~PlatformGLFW();
            break;
        };

        // if (ecs.getComponent<InputState>(singltone)->keysPressed[GLFW_KEY_F] && i < suz.subMeshes.size()) {
        //     Entity suzanne_glb = ecs.createEntity();

        //     MeshResource& subMesh = resourceManager.getResource<MeshResource>(suz.subMeshes[i].mesh);
        //     VertexLayout& subMeshVL = resourceManager.getResource<VertexLayout>(subMesh.vertexLayout);
        //     for (uint32_t i = 0; i < 5 && i; ++i) {
        //         float* v =
        //             reinterpret_cast<float*>(arena.getPtr(subMesh.verticesArenaOffset) + i * subMeshVL.stride);
        //     }
        //     Mesh subM = renderer->createMesh(
        //         subMeshVL,  
        //         arena.getPtr(subMesh.verticesArenaOffset),
        //         subMesh.verticesCount, 
        //         arena.getPtr(subMesh.indicesArenaOffset),
        //         subMesh.indicesCount
        //     );
        //     ecs.createComponent<Mesh>(suzanne_glb, subM);
        //     ecs.createComponent<Material>(suzanne_glb, 
        //         renderer->createShader(vertexShaderSource1, fragmentShaderSource1),
        //         texture, //bresenham4Resource.gpuIndex,
        //         renderer->createMaterialUBO(matData)
        //     );
        //     ecs.createComponent<glm::mat4>(suzanne_glb, glm::mat4(1.0f));
        //     i++;
        // }

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        frameCount++;
        timeAccumulator += deltaTime;

        // // Если прошла 1 секунда (или чуть больше)
        // if (timeAccumulator >= 1.0f) {
        //     // Запоминаем FPS
        //     fps = (float)frameCount / timeAccumulator;

        //     // Сбрасываем счетчики для следующей секунды
        //     frameCount = 0;
        //     timeAccumulator = 0.0f;

        //     // Теперь переменная 'fps' стабильна и ее можно вывести
        //     LOG_DEBUG("FPS: ", fps);
        // }

        platform.poolEvent();
        ecs.Update(deltaTime);
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