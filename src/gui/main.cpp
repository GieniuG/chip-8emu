#include <iostream>
#include "../../include/core/Emulator.hpp"
#include "App.hpp"


int main(int argc, char** argv) {
    App app;

    if(argc > 1){
        std::string romPath = argv[1];
        app.LoadRom(romPath);
    }
    app.Run();
    return 0;
}
