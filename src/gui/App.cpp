#include "App.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

App::App()
    : window(sf::VideoMode({640, 320}), "chip8"), sprite(texture), emulator(),
      texture() {
          
  if (!texture.create(64, 32)) {
    std::runtime_error("Could not create screen texture");
  };
  sprite.setTexture(texture, true);
  sprite.setScale({10.f, 10.f});
  auto buff = emulator.GetDisplay().GetBuffer();
  for (int i = 0; i < 2048; i++) {
    if (buff[i]) {
      for (int j = 0; j < 4; j++) {
        sfmlBuffer[i * 4 + j] = 255;
      }
    } else {
      for (int j = 0; j < 4; j++) {
        sfmlBuffer[i * 4 + j] = j==3?255:0;
      }
    }
  }
}

void App::Run() {
  std::unordered_map<sf::Keyboard::Scancode, uint8_t> keyMap={
      {sf::Keyboard::Scan::Num1,0x1},{sf::Keyboard::Scan::Num2,0x2},{sf::Keyboard::Scan::Num3,0x3},{sf::Keyboard::Scan::Num4,0xc},
      {sf::Keyboard::Scan::Q,0x4},{sf::Keyboard::Scan::W,0x5},{sf::Keyboard::Scan::E,0x6},{sf::Keyboard::Scan::R,0xd},
      {sf::Keyboard::Scan::A,0x7},{sf::Keyboard::Scan::S,0x8},{sf::Keyboard::Scan::D,0x9},{sf::Keyboard::Scan::F,0xe},
      {sf::Keyboard::Scan::Z,0xa},{sf::Keyboard::Scan::X,0x0},{sf::Keyboard::Scan::C,0xb},{sf::Keyboard::Scan::V,0xf},
  };
  emulator.LoadROM("../roms/3-corax+.ch8");
  Keypad keypad=emulator.GetKeypad();
  window.setFramerateLimit(60);
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      if(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased){
          if(keyMap.count(event.key.scancode)){
              uint8_t key=keyMap[event.key.scancode];
              uint8_t state = event.type == sf::Event::KeyPressed;

              if(state != keypad.IsKeyPressed(key)){
                  keypad.SetKey(key,state);
              }
          }
      }
    }
    emulator.Tick();
    window.clear();
    auto buff = emulator.GetDisplay().GetBuffer();
    for (int i = 0; i < 2048; i++) {
      if (buff[i]) {
        for (int j = 0; j < 4; j++) {
          sfmlBuffer[i * 4 + j] = 255;
        }
      } else {
        sfmlBuffer[i * 4 + 3] = 255;
      }
    }

    texture.update(sfmlBuffer.data());
    window.draw(sprite);
    window.display();
  }
};
