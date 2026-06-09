#include "App.hpp"
#include <iostream>

int main(int argc, char **argv) {
  App app;

  std::string romDir = "roms/";
  std::string startRom = "";
  for (int i = 1; i < argc; i++) {
    std::string_view arg = argv[i];
    if (arg == "-p") {
      if (i + 1 < argc) {
        romDir = argv[++i];
      } else {
        std::cout << "Nie podano ścieżki" << std::endl;
      }
    } else if (arg == "-r") {
      if (i + 1 < argc) {
        app.LoadRom(argv[++i]);
      } else {
        std::cout << "Nie podano ścieżki" << std::endl;
      }
    } else if (arg == "-h") {
      std::cout << "Użycie: ./chip8 [opcje]" << std::endl
                << "Opcje:" << std::endl
                << "  -p <scieżka> Zmienia folder z grami (domyślnie ./roms/)"
                << std::endl
                << "  -r <scieżka> Od razu uruchamia wybrany ROM" << std::endl
                << "  -h wyświetla pomoc" << std::endl;
      return 0;
    }
  }
  app.setRootPath(romDir);
  app.Run();
  return 0;
}
