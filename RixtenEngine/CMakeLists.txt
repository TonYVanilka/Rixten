cmake_minimum_required(VERSION 4.3)

project(RixtenEngine)

add_library(RixtenEngine SHARED
    ./core/rixten.cpp
)

target_include_directories(RixtenEngine PUBLIC
    ./include
)