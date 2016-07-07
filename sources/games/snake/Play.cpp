#include "games/Snake.hpp"
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
  Snake *snake = new Snake();
  bool entervalue = true;
  arcade::CommandType inp;

  while (entervalue)
  {
    read(0, &inp, sizeof(inp));
    if (inp == arcade::CommandType::WHERE_AM_I)
      {
        //Déclarations
        arcade::WhereAmI *where = (arcade::WhereAmI *)malloc(sizeof(* where) + (snake->getSize() * sizeof(struct arcade::Position)));
        std::map<int, std::list<void *>> *entities = snake->getEntities();
        std::map<int, std::list<void *>>::iterator it;
        std::list <void *>::iterator it2;
        int i = 0;

        //Remplissage de la structure
        for (it = entities->begin(); it != entities->end();++it)
        {
          for (it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
          {
            auto *entity = static_cast<Part *>(*it2);
            where->position[i].x = entity->x;
            where->position[i].y = entity->y;
            ++i;
          }
        }
        where->type = arcade::CommandType::WHERE_AM_I;
        where->lenght = i;
        write(1, where, (sizeof(* where) + (snake->getSize() * sizeof(struct arcade::Position))));
      }
    else if (inp == arcade::CommandType::GET_MAP)
      {
        //Déclarations
        struct arcade::GetMap *where = (struct arcade::GetMap *)(malloc(sizeof(struct arcade::GetMap) + sizeof(arcade::TileType) * snake->getSizeX() * snake->getSizeX()));
        char **map = snake->getMap();
        int x = 0, y = 0, i = 0;

        //Remplissage de la structure
        for (i = 0; i < (snake->getSizeX() * snake->getSizeY()); i++)
        {
          if (x >= snake->getSizeX())
          {
            x = 0;
            ++y;
          }
          where->tile[i] = static_cast<arcade::TileType>(map[y][x]);
          ++x;
        }
        where->type = arcade::CommandType::GET_MAP;
        where->width = static_cast<int>(snake->getSizeX());
        where->height = static_cast<int>(snake->getSizeY());
        //Write
        write(1, where, (sizeof(struct arcade::GetMap) + sizeof(arcade::TileType) * snake->getSizeX() * snake->getSizeX()));
      }
    else if (inp == arcade::CommandType::GO_UP)
      snake->play('z', -2);
    else if (inp == arcade::CommandType::GO_DOWN)
      snake->play('s', -2);
    else if (inp == arcade::CommandType::GO_LEFT)
      snake->play('q', -2);
    else if (inp == arcade::CommandType::GO_RIGHT)
      snake->play('d', -2);
    else if (inp == arcade::CommandType::GO_FORWARD)
      snake->play(0, -2);
    /*else if (inp == arcade::CommandType::SHOOT)
      ;
    else if (inp == arcade::CommandType::ILLEGAL)
      ;*/
    else if (inp == arcade::CommandType::PLAY)
        snake->play(0, -3);
    else
      entervalue = false;
  }
}
