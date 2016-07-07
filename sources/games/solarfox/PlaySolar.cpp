#include "games/Solar.hpp"
#include "games/Protocol.hpp"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "display_managers/IDisplayManager.hpp"
#include <iostream>
#include <fstream>
#include <string>

extern "C"
void  Play()
{
  Solar *solar = new Solar();
  bool entervalue = true;
  arcade::CommandType inp;

  while (entervalue)
  {
    read(0, &inp, sizeof(inp));
            std::cerr << "Commande envoyé " << (int)inp << std::endl;
    if (inp == arcade::CommandType::WHERE_AM_I)
      {
        //Déclarations
        arcade::WhereAmI *where = (arcade::WhereAmI *)malloc(sizeof(* where) + (4 * sizeof(struct arcade::Position)));
        std::map<int, std::list<void *>> *entities = solar->getEntities();
        std::map<int, std::list<void *>>::iterator it;
        std::list <void *>::iterator it2;
        int i = 0;

        //Remplissage de la structure
        for (it = entities->begin(); it != entities->end();++it)
        {
          if ((*it).first == 2)
          {
            for (it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
            {
              auto *entity = static_cast<Part *>(*it2);
              where->position[i].x = entity->x;
              where->position[i].y = entity->y;
              ++i;
            }
          }
        }
        where->type = arcade::CommandType::WHERE_AM_I;
        where->lenght = i;
//        std::cerr << where->lenght << std::endl;
  // std::ofstream outfile;

  //       outfile.open("log.txt", std::ios_base::app);
  //       outfile << std::to_string(where->lenght) << std::endl;
        //Affichage
        /*std::cerr << "lenght = " << where->lenght << std::endl;
        std::cerr << "type = " << static_cast<int>(where->type) << std::endl;
        std::cerr << "position = " << std::endl;
        for (int a = 0; a < i; a++)
        {
          std::cerr << "x" << a << " = " << where->position[a].x;
          std::cerr << " | y" << a << " = " << where->position[a].y << std::endl;
        }*/

        write(1, where, (sizeof(* where) + (1 * sizeof(struct arcade::Position))));
      }
    else if (inp == arcade::CommandType::GET_MAP)
      {
        //Déclarations
        struct arcade::GetMap *where = (struct arcade::GetMap *)(malloc(sizeof(struct arcade::GetMap) + sizeof(arcade::TileType) * solar->getSizeX() * solar->getSizeX()));
        char **map = solar->getMap();
        int x = 0, y = 0, i = 0;
        std::list <void *>::iterator it2;
        std::map<int, std::list<void *>>::iterator it;

        //Remplissage de la structure
        for (i = 0; i < (solar->getSizeX() * solar->getSizeY()); i++)
        {
          if (x >= solar->getSizeX())
          {
            x = 0;
            ++y;
          }
          if (map[y][x] == 3)
              where->tile[i] = static_cast<arcade::TileType>(7);
          else
            where->tile[i] = static_cast<arcade::TileType>(map[y][x]);
          ++x;
        }
        std::map<int, std::list<void *>> *entities = solar->getEntities();
        for (it = entities->begin(); it != entities->end();++it)
        {
          if ((*it).first == static_cast<int>(arcade::TileType::MY_SHOOT))
          {
            std::cerr << (*it).first << " good "<<  (*it).second.size() << std::endl;
            for (it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
            {
              auto *entity = static_cast<Part *>(*it2);
              std::cerr << "find missile" << std::endl;
              where->tile[static_cast<int>((entity->x * solar->getSizeX()) + entity->y)] =  arcade::TileType::MY_SHOOT;
              ++i;
            }
          }
        }
        where->type = arcade::CommandType::GET_MAP;
        where->width = static_cast<int>(solar->getSizeX());
        where->height = static_cast<int>(solar->getSizeY());
        //Write
        write(1, where, (sizeof(struct arcade::GetMap) + sizeof(arcade::TileType) * solar->getSizeX() * solar->getSizeX()));
      }
    else if (inp == arcade::CommandType::GO_UP)
      solar->play('z', -2);
    else if (inp == arcade::CommandType::GO_DOWN)
      solar->play('s', -2);
    else if (inp == arcade::CommandType::GO_LEFT)
      solar->play('q', -2);
    else if (inp == arcade::CommandType::GO_RIGHT)
      solar->play('d', -2);
    else if (inp == arcade::CommandType::GO_FORWARD)
      solar->play(0, -2);
    else if (inp == arcade::CommandType::SHOOT)
      solar->play(0, -2);
    // else if (inp == arcade::CommandType::ILLEGAL)
    //   ;
    else if (inp == arcade::CommandType::PLAY)
        solar->play(0, -3);
    else
      entervalue = false;
  }
}
