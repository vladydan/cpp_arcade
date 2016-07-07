#ifndef GLDISPLAY_MANAGER_HPP
#define GLDISPLAY_MANAGER_HPP

#include "IDisplayManager.hpp"
#include "extern/SFML/Graphics.hpp"
#include <string>

#define NUMBER_SHAPES 30

struct Colors
{
  unsigned char b;
  unsigned char g;
  unsigned char r;
  unsigned char a;
};

union ConvertColor
{
  unsigned int color;
  Colors       colors;
};

struct GLShapes
{
  std::string name;
  int size;
  Colors color;
};

class GLDisplayManager : public IDisplayManager
{
 public:
  GLDisplayManager();
  virtual ~GLDisplayManager();
  virtual int createWindow(size_t const & length, size_t const &height, std::string const &name);
  virtual int setShape(IGame &game, int const tileType, std::string const &shape, unsigned int const color, unsigned int const size = 0,  std::string const &textureName = "");
  virtual std::string startGame(IGame &game, std::string const &musicName = "");
  virtual void displayMap(char **map, int const sizeX, int const sizeY);
  void setButton(TextElem const &button);
  virtual void displayEntities(std::map<int, std::list<void *>> *entities, int const sizeX, int const sizeY);
  virtual void closeWindow();
  virtual const std::string displayMenu(IGame &game, GroupButton grp);
  virtual void replay(IGame &game, GameInfos &info);
  void setWheigth(size_t height);
  void setWlength(size_t length);
  void setMenu(bool menu);
  bool getMenu() const;
  void setScore(int score);
  int getScore() const;
  const std::string& getName() const;
  void setName(const std::string& name);
  void appendName(char c);
  void removeName();
  void setKey(char key);
  char getKey() const;
  GroupButton getGroup() const;
  IGame* getGame() const;
  virtual t_data    getData(){return _data;}
  virtual void      loadData(t_data &data){(void)data;}

 private:
  size_t _wlength;
  size_t _wheight;
  int    _window;
  std::vector<GLShapes*> _shapeList;
  char		_key;
  bool		_menu;
  IGame*	_game;
  GameInfos*	_infos;
  t_data _data;
  GroupButton _group;
};

#endif
