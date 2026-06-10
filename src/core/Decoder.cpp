#include "../../include/core/Decoder.hpp"
#include "../../include/core/CPU.hpp"
#include <cstdint>
#include <iostream>
#include <stdexcept>

void Decoder::DecodeAndExecute(CPU &cpu, uint16_t opcode) {
  uint16_t nnn = opcode & 0x0FFF;
  uint8_t nn = opcode & 0x00FF;
  uint8_t n = opcode & 0x000F;
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;

  auto vx = cpu.registers[x], vy = cpu.registers[y];
  switch (opcode & 0xF000) {
  case 0x0000:
    switch (nn) {
    case 0xE0:
      // clear screen
      cpu.display->Reset();
      break;
    case 0xEE:
      // return from subroutine
      cpu.SP--;
      cpu.PC = cpu.stack[cpu.SP];
      break;
    default:
        ThrowUnknownOpcode(cpu.PC,opcode);
    }
    break;
  case 0x1000:
    // jump
    cpu.PC = nnn;
    break;
  case 0x2000:
    // call subroutine
    cpu.stack[cpu.SP] = cpu.PC;
    if (cpu.SP < 16)
      cpu.SP++;
    cpu.PC = nnn;
    break;
  case 0x3000:
    // skip conditionally
    if (vx == nn)
      cpu.PC += 2;
    break;
  case 0x4000:
    // skip conditionally
    if (vx != nn)
      cpu.PC += 2;
    break;
  case 0x5000:
    // skip conditionally
    if (vx == vy)
      cpu.PC += 2;
    break;
  case 0x6000:
    // set register vx
    cpu.registers[x] = nn;
    break;
  case 0x7000:
    // add value to vx
    cpu.registers[x] += nn;
    break;
  case 0x8000:
    // logical and arithmetic instructions
    switch (n) {
    case 0x0:
      cpu.registers[x] = vy;
      break;
    case 0x1:
      cpu.registers[x] = vx | vy;
      break;
    case 0x2:
      cpu.registers[x] = vx & vy;
      break;
    case 0x3:
      cpu.registers[x] = vx ^ vy;
      break;
    case 0x4: {
      uint16_t sum = vx + vy;
      cpu.registers[x] = sum & 0xFF;
      cpu.registers[0xf] = sum > 255 ? 1 : 0;
    } break;
    case 0x5: {
      uint16_t diff = vx - vy;
      cpu.registers[x] = diff & 0xFF;
      cpu.registers[0xf] = diff > 255 ? 0 : 1;
    } break;
    case 0x6:
      cpu.registers[x] = vx >> 1;
      cpu.registers[0xf] = vx & 0x001;
      break;
    case 0x7: {
      uint16_t diff = vy - vx;
      cpu.registers[x] = diff & 0xFF;
      cpu.registers[0xf] = diff > 255 ? 0 : 1;
    } break;
    case 0xe:
      cpu.registers[x] = vx << 1;
      cpu.registers[0xf] = (vx & 0x80)>>7;
      break;
    default:
        ThrowUnknownOpcode(cpu.PC,opcode);
    }
    break;
  case 0x9000:
    // skip conditionally
    if (vx != vy)
      cpu.PC += 2;
    break;
  case 0xA000:
    // set index register
    cpu.I = nnn;
    break;
  case 0xB000:
    // jump with offset
    cpu.PC = nnn + cpu.registers[x];
    break;
  case 0xC000:
    // random
    cpu.registers[x] = (rand() % 255) & nn;
    break;
  case 0xD000:
    // display/draw
    cpu.registers[0xF] = 0;
    for (int row = 0; row < n; row++) {
      uint8_t line = cpu.memory->ReadByte(cpu.I + row);
      uint8_t mask = 0b10000000;
      for (int column = 0; column < 8; column++) {
        uint8_t bit = (line & mask) >> (7 - column);
        uint8_t cx = (vx % 64) + column, cy = (vy % 32) + row;
        if (cx >= 64 || cy >= 32) {
          continue;
        }
        uint8_t currPixel = cpu.display->GetPixel(cx, cy);
        if (bit && currPixel) {
          cpu.registers[0xF] = 1;
        }
        cpu.display->SetPixel(cx, cy, bit ^ currPixel);
        mask = mask >> 1;
      }
    }
    break;
  case 0xE000:
    // skip if key
    switch (nn) {
    case 0x9E:
      if (cpu.keypad->IsKeyPressed(vx)) {
        cpu.PC += 2;
      }
      break;
    case 0xA1:
      if (!cpu.keypad->IsKeyPressed(vx)) {
        cpu.PC += 2;
      }
      break;
    default:
        ThrowUnknownOpcode(cpu.PC,opcode);
    }
    break;
  case 0xF000:
    switch (nn) {
    case 0x07:
        //set vx to timer
        cpu.registers[x]=cpu.delayTimer;
        break;
    case 0x15:
        //set timer to vx
        cpu.delayTimer = vx;
        break;
    case 0x18:
        //set sound timer to vx
        cpu.soundTimer = vx;
        break;
    case 0x1E:
      // add vx to index register
      {
        uint16_t sum = cpu.I + vx;
        cpu.registers[0xf] = sum > 0x0FFF ? 1 : 0;
        cpu.I = sum & 0x0FFF;
        break;
      }
    case 0x0A:
      // wait for key
      {
      bool anyKey=false;
      for(int i=0;i<16;i++){
          if(cpu.keypad->IsKeyPressed(i)){
              cpu.registers[x]=i;
              anyKey=true;
              break;
          }
      }
      if (!anyKey)
        cpu.PC -= 2;
      }
      break;
    case 0x29:
      // get font char
      cpu.I = vx*5;
      break;
    case 0x33: 
      //binary coded dec conversion
      {
        uint8_t temp = vx;
        for (int i = 0; i < 3; i++) {
          cpu.memory->WriteByte(cpu.I+2-i,temp%10);
          temp /= 10;
        }
        break;
      }
     case 0x55:
      //load to memory
      for(int i=0;i<=x;i++){
          cpu.memory->WriteByte(cpu.I+i,cpu.registers[i]);
      }
      break;
     case 0x65:
      //read from memory
      for(int i=0;i<=x;i++){
          cpu.registers[i]=cpu.memory->ReadByte(cpu.I+i);
      }
      break;
    default:
        ThrowUnknownOpcode(cpu.PC,opcode);
    }
    break;

  default:
    ThrowUnknownOpcode(cpu.PC,opcode);
  }
}

void Decoder::ThrowUnknownOpcode(uint8_t PC, uint16_t opcode){
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Unknown opcode: 0x%04X pod adresem 0x%04X", opcode,(uint16_t)(PC-2));
    throw std::runtime_error(buffer);
}
