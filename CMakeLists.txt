cmake_minimum_required(VERSION 3.10)
project(cs2_weapon_control VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_GENERATOR_PLATFORM x64)
endif()

add_executable(cs2_weapon_control main.cpp mouse_control.cpp keyboard_input.cpp weapon_recoil.cpp counterstrafe.cpp)

target_link_libraries(cs2_weapon_control PRIVATE user32 gdi32 kernel32)
