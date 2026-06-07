#pragma once

#include <string>
#include <vector>



class RomLoader {
    public:
        std::vector<char> Load(const std::string& romPath);
};
