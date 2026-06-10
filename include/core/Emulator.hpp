#pragma once
#include "CPU.hpp"
#include "Decoder.hpp"
#include <memory>
#include <vector>


class Emulator {
public:
  Emulator();
  void LoadROM(std::vector<char> romPath);
  void Reset();
  void Tick();

  const Display &GetDisplay() const;
  Keypad* GetKeypad();
  void UpdateTimers();
  bool isBeeping();
private:
  std::unique_ptr<CPU> cpu;
  std::unique_ptr<Memory> memory;
  std::unique_ptr<Keypad> keypad;
  std::unique_ptr<Display> display;
  std::unique_ptr<Decoder> decoder;

};
