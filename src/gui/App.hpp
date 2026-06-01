#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "../../include/core/Emulator.hpp"


class App{
    public:
        App();
        void Run();
    private:
        sf::RenderWindow window;
        sf::Texture texture;
        sf::Sprite sprite;
        Emulator emulator;
        std::array<uint8_t, 64*32*4> sfmlBuffer;
};
