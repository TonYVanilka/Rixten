cmake_minimum_required(VERSION 3.15)

project(RixtenLauncher)

add_executable(RixtenLauncher
    ./rixten_launcher.cpp
)

target_link_libraries(RixtenLauncher
    RixtenEngine
)

# Windows
if (WIN32) 

# add .dll to dir with launcher
add_custom_command(TARGET RixtenLauncher POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:RixtenEngine>
        $<TARGET_FILE_DIR:RixtenLauncher>
    COMMENT "Copying RixtenEngine.dll to output directory"
)

endif() # Windows 