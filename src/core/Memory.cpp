#include "../../include/core/Memory.hpp"
#include <stdexcept>

uint8_t Memory::ReadByte(uint16_t addr) {
  if (addr > 4095) {
    throw std::out_of_range("Memory read out of bounds!");
  }
  return mem[addr];
}
void Memory::WriteByte(uint16_t addr, uint8_t data) {
  if (addr > 4095) {
    throw std::out_of_range("Memory read out of bounds!");
  }
  mem[addr] = data;
}
void Memory::Reset() { mem.fill(0); }
