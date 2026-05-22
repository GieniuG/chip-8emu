#include "../../include/core/Keypad.hpp"
#include <stdexcept>

void Keypad::Reset() { keys.fill(false); }

bool Keypad::IsKeyPressed(uint8_t key) const {
  if (key > 15) {
    throw std::out_of_range("Keypad read out of bounds!");
  }
  return keys[key];
}

void Keypad::SetKey(uint8_t key, bool isPressed) {
  if (key > 15) {
    throw std::out_of_range("Keypad write out of bounds!");
  }
  keys[key] = isPressed;
}

