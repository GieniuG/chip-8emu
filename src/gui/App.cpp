#include "App.hpp"
#include "../../include/core/RomLoader.hpp"
#include "../../include/core/AudioBeeper.hpp"
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <unordered_map>

App::App()
    : window(sf::VideoMode({640, 320}), "chip8"), sprite(texture), emulator(),
      texture() {

  if (!texture.create(64, 32)) {
    throw std::runtime_error("Could not create screen texture");
  };
  sprite.setTexture(texture, true);
  sprite.setScale({10.f, 10.f});
}

bool App::LoadRom(const std::string& path){
  try{
      RomLoader loader;
      auto romData = loader.Load(path);
      emulator.Reset();
      emulator.LoadROM(romData);
  }catch(const std::exception& e){
      std::cerr << "Błąd krytyczny: " << e.what() << std::endl;
      return false;
  }
  return true;
}

void App::Run() {
    bool pause=false;
  std::unordered_map<sf::Keyboard::Scancode, uint8_t> keyMap = {
      {sf::Keyboard::Scan::Num1, 0x1}, {sf::Keyboard::Scan::Num2, 0x2},
      {sf::Keyboard::Scan::Num3, 0x3}, {sf::Keyboard::Scan::Num4, 0xc},
      {sf::Keyboard::Scan::Q, 0x4},    {sf::Keyboard::Scan::W, 0x5},
      {sf::Keyboard::Scan::E, 0x6},    {sf::Keyboard::Scan::R, 0xd},
      {sf::Keyboard::Scan::A, 0x7},    {sf::Keyboard::Scan::S, 0x8},
      {sf::Keyboard::Scan::D, 0x9},    {sf::Keyboard::Scan::F, 0xe},
      {sf::Keyboard::Scan::Z, 0xa},    {sf::Keyboard::Scan::X, 0x0},
      {sf::Keyboard::Scan::C, 0xb},    {sf::Keyboard::Scan::V, 0xf},
  };


  Keypad *keypad = emulator.GetKeypad();
  window.setFramerateLimit(60);

  sf::Image screenImage;
  screenImage.create(64, 32, sf::Color::Black);
    
  AudioBeeper beeper;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      if (event.type == sf::Event::KeyPressed ||
          event.type == sf::Event::KeyReleased) {
        if (keyMap.count(event.key.scancode)) {
          uint8_t key = keyMap[event.key.scancode];
          uint8_t state = event.type == sf::Event::KeyPressed;

          if (state != keypad->IsKeyPressed(key)) {
            keypad->SetKey(key, state);
          }
        }else{
            if(event.type == sf::Event::KeyPressed){
                switch(event.key.scancode){
                    case sf::Keyboard::Scan::P:
                        pause=pause^1;
                    case sf::Keyboard::Scan::O:
                        LoadRom("../roms/IBM Logo.ch8");
                }
                
            }
        }
      }
    }
    if(!pause){
        for (int i = 0; i < 10; i++)
          emulator.Tick();
        emulator.UpdateTimers();
    }
    window.clear();

    if (emulator.isBeeping()) {
      if(!beeper.isPlaying()) {
          beeper.play();
      }
    } else {
      if (beeper.isPlaying()) {
        beeper.stop();
      }
    }
    auto buff = emulator.GetDisplay().GetBuffer();
    for (int y = 0; y < 32; ++y) {
      for (int x = 0; x < 64; ++x) {
        if (buff[y * 64 + x]) {
          screenImage.setPixel(x, y, sf::Color::White);
        } else {
          screenImage.setPixel(x, y, sf::Color::Black);
        }
      }
    }

    texture.update(screenImage);
    window.draw(sprite);
    window.display();
  }
};
