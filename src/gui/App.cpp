#include "App.hpp"
#include "../../include/core/AudioBeeper.hpp"
#include "../../include/core/RomLoader.hpp"
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdint>
#include <filesystem>
#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

enum class EmulatorState { Running, Paused, Menu };
EmulatorState currentState = EmulatorState::Menu;

App::App() : window(), sprite(texture), emulator(), texture() {

  if (!texture.create(64, 32)) {
    throw std::runtime_error("Could not create screen texture");
  };
  sprite.setTexture(texture, true);
}

bool App::LoadRom(const std::string &path) {
  try {
    RomLoader loader;
    auto romData = loader.Load(path);
    emulator.Reset();
    emulator.LoadROM(romData);
  } catch (const std::exception &e) {
    std::cerr << "Błąd krytyczny: " << e.what() << std::endl;
    return false;
  }
  return true;
}

void App::Run() {
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

  std::string errorMessage = "";

  Keypad *keypad = emulator.GetKeypad();

  sf::Image screenImage;
  screenImage.create(64, 32, sf::Color::Black);

  AudioBeeper beeper;

  window.create(sf::VideoMode(1024, 512), "chip8");
  window.setFramerateLimit(60);
  sf::View initialView(sf::FloatRect(0, 0, 64, 32));
  window.setView(initialView);

  ImGui::SFML::Init(window);
  sf::Clock deltaClock;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);

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
        } else {
          if (event.type == sf::Event::KeyPressed) {
            switch (event.key.scancode) {
            case sf::Keyboard::P:
              if (currentState != EmulatorState::Menu) {
                currentState = (currentState == EmulatorState::Paused)
                                   ? EmulatorState::Running
                                   : EmulatorState::Paused;
              }
            case sf::Keyboard::Scan::Escape:
              currentState = (currentState == EmulatorState::Menu)
                                 ? EmulatorState::Running
                                 : EmulatorState::Menu;
            }
          }
        }
      }
      if (event.type == sf::Event::Resized) {
        float viewRatio = 64.f / 32.f;
        float windowRatio = static_cast<float>(event.size.width) /
                            static_cast<float>(event.size.height);

        float sizeX = 1.0f;
        float sizeY = 1.0f;
        float posX = 0.0f;
        float posY = 0.0f;

        if (windowRatio < viewRatio) {
          sizeX = viewRatio / windowRatio;
          posX = (1.0f - sizeX) / 2.0f;
        } else {
          sizeY = windowRatio / viewRatio;
          posY = (1.0f - sizeY) / 2.0f;
        }

        sf::View view(sf::FloatRect(0, 0, 64, 32));
        view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
        window.setView(view);
      }
    }
    if (currentState == EmulatorState::Running) {
      try {
        for (int i = 0; i < 10; i++)
          emulator.Tick();
        emulator.UpdateTimers();
        if (emulator.isBeeping()) {
          if (!beeper.isPlaying()) {
            beeper.play();
          }
        } else {
          if (beeper.isPlaying()) {
            beeper.stop();
          }
        }
      } catch (const std::exception &e) {
        errorMessage = e.what();
        currentState = EmulatorState::Menu;
      }
    } else {
      beeper.stop();
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

    ImGui::SFML::Update(window, deltaClock.restart());
    if (currentState == EmulatorState::Menu) {
      ImGui::Begin("CHIP-8 ROM Loader");
      ImGui::Text("Wybierz plik z grami:");
      ImGui::Separator();

      try {
        for (const auto &entry : std::filesystem::directory_iterator("roms")) {
          if (entry.is_regular_file() && entry.path().extension() == ".ch8") {

            std::string filename = entry.path().filename().string();
            std::string fullPath = entry.path().string();

            if (ImGui::Button(filename.c_str())) {
              LoadRom(fullPath);
              currentState = EmulatorState::Running;
            }
          }
        }
      } catch (const std::filesystem::filesystem_error &e) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                           "Brak folderu roms w katalogu programu");
      }
      if (!errorMessage.empty()) {
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 2.0f, 0.2f, 1.0f), "EMULATOR CRASH:");
        ImGui::TextWrapped("%s", errorMessage.c_str());
      }

      ImGui::End();
    }

    window.clear(sf::Color::Black);

    texture.update(screenImage);
    window.draw(sprite);
    ImGui::SFML::Render(window);
    window.display();
  }
  ImGui::SFML::Shutdown();
};
