cmake_minimum_required(VERSION 3.15)

project(RixtenEngine)

add_library(RixtenEngine SHARED
    ./core/rixten.cpp
)

target_include_directories(RixtenEngine PRIVATE
    ./include
)

# Rixten Settings
target_compile_definitions(RixtenEngine PRIVATE RIXTEN_EXPORTS)

# Windows
if (WIN32) 

endif() # Windows 