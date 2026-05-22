#pragma once
#include "CPU.hpp"
#include "Decoder.hpp"
#include <memory>


class Emulator {
public:
  Emulator();
  void LoadROM(std::string romPath);
  void Reset();
  void Tick();

  const Display &GetDisplay() const;
  Keypad GetKeypad();
private:
  std::unique_ptr<CPU> cpu;
  std::unique_ptr<Memory> memory;
  std::unique_ptr<Keypad> keypad;
  std::unique_ptr<Display> display;
  std::unique_ptr<Decoder> decoder;

};
