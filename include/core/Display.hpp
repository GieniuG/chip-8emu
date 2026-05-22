#pragma once
#include "../IResettable.hpp"
#include <array>
#include <cstdint>

class Display : public IResettable {
public:
  void Reset() override;
  uint8_t GetPixel(uint8_t x, uint8_t y);
  void SetPixel(uint8_t x, uint8_t y, uint8_t value);

  const std::array<uint8_t, 64 * 32> &GetBuffer() const;

private:
  std::array<uint8_t, 64 * 32> buffer;
};
