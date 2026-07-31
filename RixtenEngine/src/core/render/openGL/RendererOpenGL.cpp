#include "core/render/openGL/RendererOpenGL.h"

#include <glad/glad.h>

#include "core/render/openGL/RixtenGlfwConfig.h"
#include "core/render/openGL/Vertex.h"
#include "utils/logger.h"

RendererOpenGL::RendererOpenGL() : arena(MemoryArena::GetInstance()), 
    windowHandle(0), VAOs(2) /*refactor in future*/, shaderProg(0) {
}

RendererOpenGL::~RendererOpenGL() {
    Destroy();
    RixtenGlfwConfig::terminate();
}

bool RendererOpenGL::Init() {
    RixtenGlfwConfig::init();
    return true;
}

void RendererOpenGL::Destroy() {

    reinterpret_cast<WindowOpenGL*>(arena.getPtr(windowHandle))->~WindowOpenGL();
    arena.deallocateByOffset(windowHandle);
}

Mesh RendererOpenGL::createMesh(
    const void* vertices, size_t vertSize, 
    const void* indices, size_t idxSize,
    VertexLayout& vertLayout ) {

    uint32_t vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxSize, indices, GL_STATIC_DRAW);

    uint8_t location = 0;
    uint32_t offset = 0;

    for(int i = 0; i < vertLayout.attributeCount; i++) {
        vertLayout.attributes[i].offset = offset;
        glVertexAttribPointer(
            location,
            vertLayout.attributes[i].count,
            static_cast<GLenum>(vertLayout.attributes[i].type),
            vertLayout.attributes[i].normalize,
            vertLayout.stride,
            (void*)(offset));
            LOG_ERROR(glGetError());
        offset += vertLayout.attributes[i].count * getTypeSize(vertLayout.attributes[i].type);
        glEnableVertexAttribArray(location);
        location++; 
    }

    // // 0: position (vec3)
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // glEnableVertexAttribArray(0);

    // // 1: texCoords (vec2)
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    // unbind vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // not allowed unbind ebo while a vao is active!
    
    // unbind vao 
    glBindVertexArray(0);

    VAOs.push_back(vao);

    return Mesh{uint8_t(VAOs.size() - 1), vbo, ebo, uint32_t(idxSize / sizeof(unsigned int))};
}

uint32_t RendererOpenGL::createVertexBuffer(const void* data, size_t size) {
    uint32_t vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return vbo;
}

uint32_t RendererOpenGL::createIndexBuffer(const void* data, size_t size) {
    uint32_t ebo;

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return ebo;
}

uint32_t RendererOpenGL::createTexture(const void* data, size_t size, int width, int height, int nrChannels) {
    unsigned int texture;
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, chanelToFormar(nrChannels), width, height, 0, chanelToFormar(nrChannels), GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

void RendererOpenGL::frameBeing() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RendererOpenGL::Draw(const Mesh& mesh, const MaterialTexture& mat) {
    glBindTexture(GL_TEXTURE_2D, mat.texture);

    ShaderProgramOpenGL::Use(mat.shaderProgram);

    // Получение и установка uniform-переменных
    GLint iResolutionLoc = glGetUniformLocation(mat.shaderProgram, "iResolution");
    GLint iTimeLoc = glGetUniformLocation(mat.shaderProgram, "iTime");

    auto win = reinterpret_cast<WindowOpenGL*>(arena.getPtr(windowHandle))->GetWindowHandle();

    if (iResolutionLoc != -1) {
        // Получение размеров окна/буфера
        int width, height;
        // Если у вас есть доступ к окну
        glfwGetFramebufferSize(win, &width, &height);
        // Или используйте сохранённые значения
        // width = screenWidth; height = screenHeight;
        glUniform2f(iResolutionLoc, (GLfloat)width, (GLfloat)height);
        //glUniform2f(iResolutionLoc, 400, 400);
    }

    if (iTimeLoc != -1) {
        // Время в секундах
        glUniform1f(iTimeLoc, (GLfloat)glfwGetTime());
    }

    glBindVertexArray(VAOs[mesh.layout]);
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
}

void RendererOpenGL::frameEnd() {
    reinterpret_cast<WindowOpenGL*>(arena.getPtr(windowHandle))->Update();
}

void RendererOpenGL::Clear() {
}

void RendererOpenGL::createWindow(int width_, int height_, const char* title_) {
    windowHandle = arena.allocate(sizeof(WindowOpenGL), alignof(WindowOpenGL));
    new(arena.getPtr(windowHandle)) WindowOpenGL(width_, height_, title_);
}

uint32_t RendererOpenGL::createLayout(uint32_t count) {
    return 0;
}

uint32_t RendererOpenGL::createShader(const char* vertexShader, const char* fragmentShader) {
    shaderProg = ShaderProgramOpenGL::createShaderProg(vertexShader, fragmentShader);
    return shaderProg;
}

uint32_t RendererOpenGL::createVAO() {
    uint32_t vao;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    

    return 0;
}

void RendererOpenGL::bindShader() {
    ShaderProgramOpenGL::Use(shaderProg);
}

void RendererOpenGL::setUniform(uint32_t shaderID, const char*) {
}

GLenum RendererOpenGL::chanelToFormar(int nrChannels) {
    switch (nrChannels) {
    case 1: return GL_RED;
    case 3: return GL_RGB;
    case 4: return GL_RGBA;
    default: GL_RGB;
    }
}

size_t RendererOpenGL::getTypeSize(uint32_t type) {
    switch(type) {
        case GL_FLOAT: return sizeof(GLfloat);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        case GL_BYTE: return sizeof(GLbyte);
        case GL_UNSIGNED_SHORT: return sizeof(GLushort);
        case GL_SHORT: return sizeof(GLshort);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_INT: return sizeof(GLint);
        case GL_HALF_FLOAT: return sizeof(GLhalf);
        case GL_DOUBLE: return sizeof(GLdouble);
        default: return 0;
    }
}