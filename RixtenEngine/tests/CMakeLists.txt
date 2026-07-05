cmake_minimum_required(VERSION 3.15)

project(RixtenTests)

set(Utils 
    d:/cppprojects/Rixten/RixtenEngine/core/memory/MemoryArena.cpp
)

add_executable(RixtenTests 
    main.cpp
    ${Utils}
)

target_include_directories(RixtenTests PRIVATE
    d:/cppprojects/Rixten/RixtenEngine/include
)
