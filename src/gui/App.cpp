#include "App.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cmath>
#include <cstdint>
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
        sfmlBuffer[i * 4 + j] = j == 3 ? 255 : 0;
      }
    }
  }
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
  // emulator.LoadROM("../roms/2-ibm-logo.ch8");
  // emulator.LoadROM("../roms/3-corax+.ch8");
  // emulator.LoadROM("../roms/4-flags.ch8");
  //emulator.LoadROM("../roms/5-quirks.ch8");
  // emulator.LoadROM("../roms/6-keypad.ch8");
  //emulator.LoadROM("../roms/7-beep.ch8");
  emulator.LoadROM("../roms/Wall [David Winter].ch8");
  Keypad *keypad = emulator.GetKeypad();
  window.setFramerateLimit(60);

  sf::Image screenImage;
  screenImage.create(64, 32, sf::Color::Black);

  // BEEP GEN
  const unsigned SAMPLE_RATE = 44100;
  const int16_t AMPLITUDE = 10000;
  const double FREQUENCY = 440.0;

  std::vector<int16_t> samples(SAMPLE_RATE);

  double samplesPerWave = SAMPLE_RATE / FREQUENCY;
  double halfWave = samplesPerWave / 2.0;

  for (unsigned i = 0; i < SAMPLE_RATE; ++i) {
    if (std::fmod(i, samplesPerWave) < halfWave) {
      samples[i] = AMPLITUDE;
    } else {
      samples[i] = -AMPLITUDE;
    }
  }
  sf::SoundBuffer beepBuffer;
  beepBuffer.loadFromSamples(samples.data(), samples.size(), 1, SAMPLE_RATE);

  sf::Sound beepSound;
  beepSound.setBuffer(beepBuffer);
  beepSound.setLoop(true);

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
            if(event.type == sf::Event::KeyPressed && event.key.scancode==sf::Keyboard::Scan::P){
                pause=pause^1;
                
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
      if (beepSound.getStatus() != sf::Sound::Playing) {
        beepSound.play();
      }
    } else {
      if (beepSound.getStatus() == sf::Sound::Playing) {
        beepSound.stop();
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
