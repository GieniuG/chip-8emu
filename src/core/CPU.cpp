#include "../../include/core/CPU.hpp"
#include <cstdint>

void CPU::Reset() {
  registers.fill(0);
  PC = 0x200;
  I = 0;
  stack.fill(0);
  SP = 0;
  delayTimer = 0;
  soundTimer = 0;
};


CPU::CPU(Memory *memory, Keypad *keypad, Display *display){
    this->memory = memory;
    this->keypad = keypad;
    this->display = display;
};

void CPU::Cycle() {
    uint8_t firstByte=memory->ReadByte(PC);
    uint8_t secondByte=memory->ReadByte(PC+1);
    uint16_t opcode = firstByte << 8 | secondByte;

    PC+=2;

    Decoder::DecodeAndExecute(*this, opcode);
};

void CPU::UpdateTimers(){
    if(delayTimer>0) delayTimer--;
    if(soundTimer>0) soundTimer--;
}
