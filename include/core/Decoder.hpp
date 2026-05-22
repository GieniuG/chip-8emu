#pragma once
#include <cstdint>
class CPU;

class Decoder {
public:
  static void DecodeAndExecute(CPU &cpu, uint16_t opcode);
};
