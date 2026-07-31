#include <cstdio>
#include "core/ecs/EcsManager.h"
#include "utils/logger.h"
#include "core/render/openGL/RendererOpenGL.h"
#include "core/render/RenderSystem.h"
#include "core/memory/MemoryArena.h"
#include "core/render/openGL/RendererOpenGL.h"
#include "core/render/openGL/Vertex.h"

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
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource1 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec2 iResolution;\n"
    "uniform float iTime;\n"
    "#define AA 2\n"
    "float calc( vec2 p, float time )\n"
    "{\n"
    "   float ltime = 0.5-0.5*cos(time*0.06);\n"
    "   float zoom = pow( 0.9, 50.0*ltime );\n"
    "   vec2  cen = vec2( 0.2655,0.301 ) + zoom*0.8*cos(4.0+2.0*ltime);\n"
    "   vec2 c = vec2( -0.745, 0.186 ) - 0.045*zoom*(1.0-ltime*0.5);\n"
    "   p = (2.0*p-iResolution.xy)/iResolution.y;\n"
    "   vec2 z = cen + (p-cen)*zoom;\n"
    "   float ld2 = 1.0;\n"
    "   float lz2 = dot(z,z);\n"
    "   for( int i=0; i<256; i++ )\n"
    "   {\n"
    "       ld2 *= 4.0*lz2;\n"
    "       z = vec2( z.x*z.x - z.y*z.y, 2.0*z.x*z.y ) + c;\n"
    "       lz2 = dot(z,z);\n"
    "       if( lz2>200.0 ) break;\n"
    "   }\n"
    "   float d = sqrt(lz2/ld2)*log(lz2);\n"
    "   return sqrt( clamp( (150.0/zoom)*d, 0.0, 1.0 ) );\n"
    "}\n"
    "void main()\n"
    "{\n"
    "   vec2 fragCoord = gl_FragCoord.xy;\n"
    "   float scol = 0.0;\n"
    "   for( int j=0; j<AA; j++ )\n"
    "   for( int i=0; i<AA; i++ )\n"
    "   {\n"
    "       vec2 of = -0.5 + vec2( float(i), float(j) )/float(AA);\n"
    "       scol += calc( fragCoord+of, iTime );\n"
    "   }\n"
    "   scol /= float(AA*AA);\n"
    "   vec3 vcol = pow( vec3(scol), vec3(0.9,1.1,1.4) );\n"
    "   vec2 uv = fragCoord/iResolution.xy;\n"
    "   vcol *= 0.7 + 0.3*pow(16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y),0.25);\n"
    "   FragColor = vec4( vcol, 1.0 );\n"
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

float vertices2[] = {
    // позиции           // цвета (R, G, B)
    -0.3f, 0.9f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right    (красный)
    -0.3f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right (зеленый)
    -0.9f, 0.1f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom left  (синий)
    -0.9f, 0.9f, 0.0f, 1.0f, 1.0f, 0.0f   // top left     (желтый)
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
    MaterialTexture simpleMat {
        renderer->createShader(vertexShaderSource, fragmentShaderSource),
        createTexture("image.png") 
    };

    MaterialTexture simpleMat1 {
        renderer->createShader(vertexShaderSource1, fragmentShaderSource1),
        0
    };

    RenderSystem renderSys(renderer);

    ecs.createPool<Mesh>();
    ecs.createPool<MaterialTexture>();
    ecs.RegisterSystem(renderSys);

    Entity simple = ecs.createEntity();
    Entity simple1 = ecs.createEntity();

    VertexLayout vl;
    vl.attributeCount = 2;
    vl.attributes[0] = {0, static_cast<uint32_t>(GL_FLOAT), 3, false};
    vl.attributes[1] = {0, static_cast<uint32_t>(GL_FLOAT), 2, false};
    vl.stride = sizeof(Vertex);

    VertexLayout vl1;
    vl1.attributeCount = 2;
    vl1.attributes[0] = {0, static_cast<uint32_t>(GL_FLOAT), 3, false};
    vl1.attributes[1] = {0, static_cast<uint32_t>(GL_FLOAT), 3, false};
    vl1.stride = sizeof(VertexColor);

    Mesh square = renderer->createMesh(vertices, sizeof(vertices), indices, sizeof(indices), vl);
    ecs.createComponent<Mesh>(simple, square);
    ecs.createComponent<MaterialTexture>(simple, simpleMat);
    Mesh square1 = renderer->createMesh(vertices2, sizeof(vertices2), indices2, sizeof(indices2), vl1);
    ecs.createComponent<Mesh>(simple1, square1);
    ecs.createComponent<MaterialTexture>(simple1, simpleMat1);

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
