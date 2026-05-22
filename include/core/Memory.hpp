#pragma once
#include "../IResettable.hpp"
#include <array>
#include <cstdint>

class Memory : public IResettable {
public:
  uint8_t ReadByte(uint16_t);
  void WriteByte(uint16_t, uint8_t);
  void Reset() override;

private:
  std::array<uint8_t, 4096> mem;
};
