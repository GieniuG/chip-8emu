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

void Emulator::LoadROM(std::string romPath) {
  std::ifstream file(romPath,std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: "+romPath);
  }
  auto size = file.tellg();
  if(size>3584){
    throw std::runtime_error("File too big: "+romPath);
  }
  file.seekg(0,std::ios::beg);
  std::vector<char> buffer(size);

  file.read(buffer.data(),size);
  for(int i=0;i<size;i++){
    memory->WriteByte(0x200+i,static_cast<uint8_t>(buffer[i]));
  }

  file.close();
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
Keypad Emulator::GetKeypad() { return *keypad; }
