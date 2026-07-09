cmake_minimum_required(VERSION 3.15)

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
)

add_executable(RixtenTests 
    ./main.cpp
    ${Utils}
)

target_include_directories(RixtenTests PRIVATE
    ${ABOVE_DIR}/include
)
