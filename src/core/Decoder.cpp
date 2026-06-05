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

  auto vx=cpu.registers[x],
       vy=cpu.registers[y];
  switch (opcode & 0xF000) {
  case 0x0000:
    switch (nn) {
    case 0xE0:
      //clear screen
      cpu.display->Reset();
      break;
    case 0xEE:
      //return from subroutine
      cpu.PC=cpu.stack[cpu.SP-1];
      cpu.stack[cpu.SP]=0;
      if(cpu.SP>0)
          cpu.SP--;
      break;
    default:
      std::runtime_error("Opcode not recognized");
    }
    break;
  case 0x1000:
    //jump
    cpu.PC=nnn;
    break;
  case 0x2000:
    //call subroutine 
    cpu.stack[cpu.SP]=cpu.PC;
    if(cpu.SP<16)
        cpu.SP++;
    cpu.PC=nnn;
    break;
  case 0x3000:
    //skip conditionally
    if(vx==nn)
        cpu.PC+=2;
    break;
  case 0x4000:
    //skip conditionally
    if(vx!=nn)
        cpu.PC+=2;
    break;
  case 0x5000:
    //skip conditionally
    if(vx==vy)
        cpu.PC+=2;
    break;
  case 0x6000:
    //set register vx
    cpu.registers[x] = nn;
    break;
  case 0x7000:
    //add value to vx
    cpu.registers[x]+=nn;
    break;
  case 0x8000:
    //logical and arithmetic instructions
    switch(n){
      case 0x0:
        cpu.registers[x]=vy;
        break;
      case 0x1:
        cpu.registers[x]=vx|vy;
        break;
      case 0x2:
        cpu.registers[x]=vx&vy;
        break;
      case 0x3:
        cpu.registers[x]=vx^vy;
        break;
      case 0x4:
        {
          uint16_t sum = vx+vy;
          cpu.registers[x]=sum &0xFF;
          cpu.registers[0xf]=sum>255?1:0;
        }
        break;
      case 0x5:
        {
          uint16_t diff = vx-vy;
          cpu.registers[x]=diff &0xFF;
          cpu.registers[0xf]=diff>255?0:1;
        }
        break;
      case 0x6:
        cpu.registers[0xf]=vx&0x001;
        cpu.registers[x]=vx>>1;
        break;
      case 0x7:
        {
          uint16_t diff = vy-vx;
          cpu.registers[x]=diff &0xFF;
          cpu.registers[0xf]=diff>255?0:1;
        }
        break;
      case 0xe:
        cpu.registers[x]=vx<<1;
        break;
    }
    break;
  case 0x9000:
    //skip conditionally
    if(vx!=vy)
        cpu.PC+=2;
    break;
  case 0xA000:
    //set index register 
    cpu.I = nnn;
    break;
  case 0xB000:
    //jump with offset
    cpu.PC=nnn+cpu.registers[0];
    break;
  case 0xC000:
    //random
    cpu.registers[x]=(rand()%255)&nn;
    break;
  case 0xD000:
    //display/draw
    cpu.registers[0xF]=0;
    for(int row=0;row<n;row++){
        uint8_t line = cpu.memory->ReadByte(cpu.I+row);
        uint8_t mask = 0b10000000;
        for(int column=0;column<8;column++){
          uint8_t bit = (line & mask) >> (7-column);
          uint8_t cx=(vx%64)+column,
                  cy=(vy%32)+row;
          if(cx>=64 || cy >= 32){
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
