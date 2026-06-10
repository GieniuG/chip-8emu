#include "../../include/core/RomLoader.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::ifstream;

std::vector<char> RomLoader::Load(const std::string& romPath) {
  ifstream file(romPath, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    throw std::runtime_error("File not found: "+romPath);
  }
  auto size = file.tellg();
  if(size>3584){
    throw std::runtime_error("File too big: "+romPath);
  }
  file.seekg(0,std::ios::beg);
  std::vector<char> buffer(size);

  file.read(buffer.data(),size);

  file.close();
  return buffer;
};
