#include <iostream>
#include "../../include/core/Emulator.hpp"
#include "App.hpp"


int main() {
    App app=App();
    app.Run();



    //try {
    //    // 1. Tworzymy główną fasadę (to ona alokuje pamięć, procesor itd.)
    //    Emulator emu;
    //    
    //    // 2. Ładujemy ROM (upewnij się, że masz plik w tym miejscu!)
    //    std::cout << "Probuje zaladowac ROM...\n";
    //    emu.LoadROM("../roms/IBM Logo.ch8");
    //    std::cout << "ROM wczytany poprawnie!\n";

    //    // 3. Pętla testowa: wykonujemy 5 cykli procesora
    //    for (int i = 0; i < 21; ++i) {
    //        emu.Tick(); // Ta metoda wewnątrz wywołuje cpu->Cycle()
    //    }
    //    
    //    std::cout << "Testowe cykle zakonczone sukcesem. Maszyna zyje.\n";
    //    for(int i=0;i<64*32;i++){

    //    }

    //} catch (const std::exception& e) {
    //    // Jeśli pliku nie ma, lub jest za duży, ten kod elegancko wypisze Twój wyjątek
    //    std::cerr << "Blad krytyczny: " << e.what() << '\n';
    //    return 1;
    //}

    return 0;
}
