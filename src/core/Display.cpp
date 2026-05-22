#include "../../include/core/Display.hpp"
#include <stdexcept>

void Display::Reset() { buffer.fill(0); }

uint8_t Display::GetPixel(uint8_t x, uint8_t y) {
  if (x > 63 || y > 31) {
    throw std::out_of_range("Display read out of bounds!");
  }
  return buffer[x + y * 64];
};

void Display::SetPixel(uint8_t x, uint8_t y, uint8_t value) {
  if (x > 63 || y > 31) {
    throw std::out_of_range("Display write out of bounds!");
  }
  buffer[x + y * 64] = value;
};

const std::array<uint8_t, 64 * 32> &Display::GetBuffer() const {
  return buffer;
};
