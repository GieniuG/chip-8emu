#pragma once
#include "../IResettable.hpp"
#include <array>
#include <cstdint>

class Keypad : public IResettable {
public:
  void Reset() override;
  bool IsKeyPressed(uint8_t key) const;
  void SetKey(uint8_t key, bool isPressed);

private:
  std::array<bool, 16> keys;
};
