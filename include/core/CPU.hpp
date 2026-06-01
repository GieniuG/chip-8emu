#pragma once
#include "../IResettable.hpp"
#include "Decoder.hpp"
#include "Display.hpp"
#include "Keypad.hpp"
#include "Memory.hpp"
#include <array>
#include <cstdint>

class CPU : public IResettable {
public:
  void Reset() override;
  CPU(Memory *memory, Keypad *keypad, Display *display);
  
  void UpdateTimers();
  void Cycle();

private:
  std::array<uint8_t, 16> registers;
  uint16_t PC;
  uint16_t I;
  std::array<uint16_t, 16> stack;
  uint8_t SP;
  uint8_t delayTimer;
  uint8_t soundTimer;
  Memory *memory;
  Keypad *keypad;
  Display *display;

  friend class Decoder;
};
