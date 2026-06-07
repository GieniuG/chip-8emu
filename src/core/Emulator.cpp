#include "../../include/core/Emulator.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

Emulator::Emulator() {
  decoder = std::make_unique<Decoder>();
  display = std::make_unique<Display>();
  keypad = std::make_unique<Keypad>();
  memory = std::make_unique<Memory>();
  cpu = std::make_unique<CPU>(memory.get(), keypad.get(), display.get());

  Reset();
}

void Emulator::LoadROM(std::vector<char> romData) {
  for(int i=0;i<romData.size();i++){
    memory->WriteByte(0x200+i,static_cast<uint8_t>(romData[i]));
  }

};
void Emulator::Reset(){
  cpu->Reset();
  memory->Reset();
  keypad->Reset();
  display->Reset();
  decoder->DecodeAndExecute(*cpu, 0x200);
};
    void Emulator::Tick(){
  cpu->Cycle();
};

const Display &Emulator::GetDisplay() const { return *display; }
Keypad* Emulator::GetKeypad() { return keypad.get(); }

void Emulator::UpdateTimers(){
    cpu->UpdateTimers();
}
bool Emulator::isBeeping(){
    return cpu->GetSoundTimer() > 0;
}
