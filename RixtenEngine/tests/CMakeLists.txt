cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

project(RixtenTests)

if (WIN32)
    set(ABOVE_DIR 
        d:/cppprojects/Rixten/RixtenEngine/
    )        
else ()
    set(ABOVE_DIR
        /home/MoonLight/repos/Rixten/RixtenEngine/
    )
endif ()

set(Utils 
    ${ABOVE_DIR}/src/core/memory/MemoryArena.cpp
    ${ABOVE_DIR}/include/core/memory/DArr.h
    ${ABOVE_DIR}/include/core/ecs/componentPool.h
    ${ABOVE_DIR}/src/core/ecs/entity.cpp
    ${ABOVE_DIR}/src/core/ecs/EcsManager.cpp
    ${ABOVE_DIR}/include/utils/typeIDgenerator.h
    ${ABOVE_DIR}/include/core/ecs/Isystem.h
    ${ABOVE_DIR}/include/core/ecs/IcomponentPool.h
    ${ABOVE_DIR}/include/utils/logger.h

    ${ABOVE_DIR}src/core/platform/PLatformGLFW.cpp
    ${ABOVE_DIR}src/core/render/openGL/RendererOpenGL.cpp
    ${ABOVE_DIR}src/core/render/RenderSystem.cpp

    ${ABOVE_DIR}include/core/resourceManager/stb_image.h
    ${ABOVE_DIR}src/core/MovementSystem.cpp
)

add_executable(RixtenTests 
    ./main.cpp
    ${Utils}
)

target_include_directories(RixtenTests PRIVATE
    ${ABOVE_DIR}/include
)

# Rixten TEST Settings
#target_compile_definitions(RixtenTests PRIVATE DISABLE_LOGGING)
#target_compile_definitions(RixtenTests PRIVATE VULCAN_API)

# Library links

# GLAD
add_library(glad STATIC ${ABOVE_DIR}/external/glad/src/glad.c)
target_include_directories(glad PUBLIC ${ABOVE_DIR}/external/glad/include)

target_link_libraries(${CMAKE_PROJECT_NAME}  PRIVATE glad)
target_include_directories(${CMAKE_PROJECT_NAME}  PRIVATE ${ABOVE_DIR}/external/glad/include)

target_link_libraries(${CMAKE_PROJECT_NAME}  PRIVATE glad)
target_include_directories(${CMAKE_PROJECT_NAME}  PRIVATE ${ABOVE_DIR}/external/glad/include)

# Glm
target_include_directories(RixtenTests PRIVATE ${ABOVE_DIR}/external/glm)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(../external/glfw ../build/glfw)	
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME}  PRIVATE OpenGL::GL)

# compiler flags 
if(MSVC)
    target_compile_options(RixtenTests PRIVATE
        /fsanitize=address
        /Od
        /RTC-
    )
endif()