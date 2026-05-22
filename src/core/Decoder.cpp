#include "../../include/core/Decoder.hpp"
#include "../../include/core/CPU.hpp"
#include <cstdint>
#include <ios>
#include <iostream>
#include <stdexcept>


void Decoder::DecodeAndExecute(CPU &cpu, uint16_t opcode) {
  std::cout << "opcode: " << std::hex << opcode << std::endl;
  uint16_t nnn = opcode & 0x0FFF;
  uint8_t nn = opcode & 0x00FF;
  uint8_t n = opcode & 0x000F;
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;

  switch (opcode & 0xF000) {
  case 0x0000:
    switch (nn) {
    case 0xE0:
      cpu.display->Reset();
      break;
    case 0xEE:
      std::cout << "NOT IMPLEMENTED 0xEE" << std::endl;
      break;
    default:
      std::runtime_error("Opcode not recognized");
    }
    break;
  case 0x1000:
    cpu.PC=nnn;
    break;
  case 0x6000:
    cpu.registers[x] = nn;
    break;
  case 0x7000:
    cpu.registers[x]+=nn;
    break;
  case 0xA000:
    cpu.I = nnn;
    break;
  case 0xD000:
    cpu.registers[0xF]=0;
    for(int row=0;row<n;row++){
        uint8_t line = cpu.memory->ReadByte(cpu.I+row);
        uint8_t mask = 0b10000000;
        for(int column=0;column<8;column++){
          uint8_t bit = (line & mask) >> (7-column);
          uint8_t cx=(cpu.registers[x]%64)+column,
                  cy=(cpu.registers[y]%32)+row;
          if(cx>64 || cy > 32){
              continue;
          }
          uint8_t currPixel = cpu.display->GetPixel(cx,cy);
          if(bit && currPixel){
              cpu.registers[0xF]=1;
          }
          cpu.display->SetPixel(cx,cy,bit ^ currPixel);
          mask=mask>>1;
        }
    }
    break;
  default:
    std::cout << "I have no idea... lol" << std::endl;
  }
}
