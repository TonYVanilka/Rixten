cmake_minimum_required(VERSION 3.15)

project(RixtenEngine)

add_library(RixtenEngine SHARED
    ./rixten.cpp
)

target_include_directories(RixtenEngine PUBLIC
    ./exports
)

target_include_directories(RixtenEngine PRIVATE
    ./includes
)


# Rixten Settings
target_compile_definitions(RixtenEngine PRIVATE RIXTEN_EXPORTS)
#target_compile_definitions(RixtenTests PRIVATE DISABLE_LOGGING)

# Windows
if (WIN32) 

endif() # Windows 