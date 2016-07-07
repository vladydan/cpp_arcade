#ifndef CACADISPLAY_MANAGER_HPP
#define CACADISPLAY_MANAGER_HPP

#include "IDisplayManager.hpp"
#include <vector>
#include "extern/CACA/caca.h"

struct CACAShape
{
  char character;
  uint16_t color;
};

class CACADisplayManager : public IDisplayManager
{
  public:
    CACADisplayManager();
    virtual ~CACADisplayManager();
    virtual int createWindow(size_t const & length, size_t const &height, std::string const &name);
    virtual int setShape(IGame &game, int const tileType, std::string const &shape, unsigned int const color, unsigned int const size = 0,  std::string const &textureName = "");
    virtual std::string startGame(IGame &game, std::string const &musicName = "");
    virtual void displayMap(char **map, int const sizeX, int const sizeY);
    virtual const std::string displayMenu(IGame &game, GroupButton grp);
    virtual void displayEntities(std::map<int, std::list<void *>> *entities, int const sizeX, int const sizeY);
    void setButton(TextElem const &button);
    virtual void replay(IGame &game, GameInfos &info);
    virtual void closeWindow();
    virtual t_data    getData(){return _data;}
    virtual void      loadData(t_data &data){(void)data;}
    char handleEvents(caca_event_t &ev);
  private:
    size_t _windowLength;
    size_t _windowHeight;
    cucul_canvas_t *_canvas;
    caca_display_t *_window;
    std::vector <CACAShape> _shapes;
    t_data _data;
};

#endif
