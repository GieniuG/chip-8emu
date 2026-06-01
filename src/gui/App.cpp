#include "App.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/VideoMode.hpp>

App::App() : window(sf::VideoMode({640,320}),"chip8"), sprite(texture),emulator(),texture(){
    auto buff=emulator.GetDisplay().GetBuffer();
    for(int i=0;i<4;i++){
        uint8_t b=buff[i];
        if(b){
            for(int j=0;j<4;j++){
                sfmlBuffer[i+j]=255;                
            }
        }
    }

}
void App::Run(){
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
          if (event->is<sf::Event::Closed>())
            window.close();
        }
    window.clear();

    //buffer[0]=255;
    //texture.update(sfmlBuffer);
    window.draw(sprite);
    window.display();
  }
};
