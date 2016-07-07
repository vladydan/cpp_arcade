#include "games/IGame.hpp"
#include "display_managers/IDisplayManager.hpp"
#include "arcade/DLLoader.hpp"
#include "arcade/Launcher.hpp"
#include <iostream>
#include <string>

int     main(int ac, char **av)
{
  // DLLoader<IDisplayManager>* DisplayLoader;
  // DLLoader<IGame>* GameLoader;
  if (ac == 2)
  {
    IGame *launcher = new Launcher(av[1]);
  // IGame *perso;
  // IDisplayManager *displayManager;
  // std::string ret;
  (void)launcher;
}
  // if (ac == 3)
  //   {
  //     DisplayLoader = new DLLoader<IDisplayManager>();
  //     GameLoader = new DLLoader<IGame>();
  //     DisplayLoader->loadDL(av[1], "entryPoint");
  //     GameLoader->loadDL(av[2], "entryPoint");
  //     displayManager = DisplayLoader->getInstance();
  //     perso = GameLoader->getInstance();
  //     while ((ret = perso->startGame(*displayManager)) == "replay")
  //     {
  //       perso = GameLoader->getInstance();
  //       displayManager = DisplayLoader->getInstance();
  //     }
  //     delete perso;
  //     delete displayManager;
  //     delete GameLoader;
  //     delete DisplayLoader;
  //   }
   else
     std::cout << av[0] << ": usage: " << av[0] << " lib.so" << std::endl;
  return (0);
}
