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
    ${ABOVE_DIR}/src/core/ecs/ecsManager.cpp
    ${ABOVE_DIR}/include/utils/typeIDgenerator.h
    ${ABOVE_DIR}/include/core/ecs/Isystem.h
    ${ABOVE_DIR}/include/core/ecs/IcomponentPool.h
)

add_executable(RixtenTests 
    ./main.cpp
    ${Utils}
)

target_include_directories(RixtenTests PRIVATE
    ${ABOVE_DIR}/include
)

# compiler flags 
if (MSVC)
    target_compile_options(RixtenTests PRIVATE -fsanitize=address -g -O1 -fno-omit-frame-pointer)
endif()