cmake_minimum_required(VERSION 3.10)

project(Chip8Emulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(include)

add_library(chip8core SHARED
    src/core/Memory.cpp
    src/core/CPU.cpp
    src/core/Decoder.cpp
    src/core/Display.cpp
    src/core/Keypad.cpp
    src/core/Emulator.cpp
)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(chip8 src/gui/main.cpp src/gui/App.cpp)
target_link_libraries(chip8 PRIVATE 
    chip8core
    SFML::Graphics 
    SFML::Window 
    SFML::System
)
