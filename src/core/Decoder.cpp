#include "../../include/core/Decoder.hpp"
#include <iostream>

void Decoder::DecodeAndExecute(CPU &cpu, uint16_t opcode) {
  std::cout << "opcode: " << std::hex << opcode << std::endl;
  //switch(opcode) {
  //  //clear screen
  //  case 0x00E0:
  //    std::cout << "clearing the screen" << std::endl;
  //    break;
  //}

}
