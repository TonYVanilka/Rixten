cmake_minimum_required(VERSION 4.3)

project(RixtenLauncher)

add_executable(RixtenLauncher
    ./rixten_launcher.cpp
)

target_link_libraries(RixtenLauncher
    RixtenEngine
)