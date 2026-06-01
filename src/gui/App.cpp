#include "App.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <stdexcept>

App::App()
    : window(sf::VideoMode({640, 320}), "chip8"), sprite(texture), emulator(),
      texture() {
          
    emulator.LoadROM("/home/gieniu/Uni/jipp/chip-8emu/roms/IBM Logo.ch8");
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
  window.setFramerateLimit(60);
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    emulator.Tick();
    window.clear();
    auto buff = emulator.GetDisplay().GetBuffer();
            for(int i=0;i<64*32;i++){
            if(i%64==0){
                std::cout<<std::endl;
            }
            if(buff[i]){
                std::cout<<"█";
            }else{
                std::cout<<" ";
            }
        }
        std::cout<<std::endl;

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
