#ifndef IDISPLAYMANAGER_HPP
#define IDISPLAYMANAGER_HPP

#include <cstdlib>
#include <string>
#include <map>
#include "games/IGame.hpp"

#define WINDOW_SIZE_HEIGHT 768
#define WINDOW_SIZE_WIDTH 960


#define WINDOW_HEIGHT 900
#define WINDOW_LENGHT 900

typedef struct  shapes{
    int   tileType;
    std::string const &shape;
    unsigned int  color;
    unsigned int  size;
    std::string const &textureName;
}     t_shapes;

typedef struct  s_data{
    size_t  height;  // Size Window
    size_t  width;   // Size Window
    size_t  block_size;
    std::map<int, t_shapes> shapesList;
    std::map<int, TextElem> textElemList;
}       t_data;

class IDisplayManager
{
public:
  virtual ~IDisplayManager(void) { };

  virtual int createWindow(size_t const &length,
         size_t const &height,
         std::string const &name) = 0;

  //virtual int startGame(IGame &game, std::string const &musicName = "") = 0;

  virtual std::string startGame(IGame &game, std::string const &musicName) = 0;
  virtual void displayMap(char **map, int const sizeX, int const sizeY) = 0;
  virtual const std::string       displayMenu(IGame &game, GroupButton grp) = 0;
  virtual int setShape(IGame &game, int const tileType,
           std::string const &shape,
           unsigned int const color,
           unsigned int const size = 0,
           std::string const &textureName = "") = 0;

  virtual void displayEntities(std::map<int, std::list<void *> > *entities, int const sizeX, int const sizeY) = 0;
  virtual void replay(IGame &game,GameInfos & gamez) = 0;
  virtual void closeWindow() = 0;

  virtual t_data    getData() = 0;
  virtual void      loadData(t_data &data) = 0;

};

#endif
