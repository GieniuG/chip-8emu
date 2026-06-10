cmake_minimum_required(VERSION 3.10)

include(FetchContent)

project(Chip8Emulator VERSION 1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS ON)
set(SFML_STATIC_LIBRARIES FALSE CACHE BOOL "" FORCE)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.4
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
    ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG v2.6.1 
)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
FetchContent_MakeAvailable(ImGui-SFML)

include_directories(include)

add_library(chip8core SHARED
    src/core/Memory.cpp
    src/core/CPU.cpp
    src/core/Decoder.cpp
    src/core/Display.cpp
    src/core/Keypad.cpp
    src/core/Emulator.cpp
    src/core/RomLoader.cpp
    src/core/AudioBeeper.cpp
)

find_package(SFML 2 COMPONENTS audio graphics window system REQUIRED)



add_executable(chip8 src/gui/main.cpp src/gui/App.cpp)
target_link_libraries(chip8 PRIVATE 
    chip8core
    sfml-graphics 
    sfml-window 
    sfml-system
    sfml-audio
    ImGui-SFML::ImGui-SFML
)

target_link_libraries(chip8core PUBLIC sfml-audio sfml-system)
