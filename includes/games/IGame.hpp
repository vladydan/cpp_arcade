#ifndef _IGAME_HPP_
#define _IGAME_HPP_

#include <map>
#include <list>
#include <string>


struct entity
{
  float x;
  float y;
};


struct Part
{
  float x;
  float y;
  float speed;
  int   time;
  char  direction;
  float distance;
  std::list<Part *> rocket;
};

struct Position
{
  int x;
  int y;
};

enum GroupButton
{
  DEFAULT       = 0,
  MENUBTN       = 1,
  INGAME        = 2,
  ENDGAME       = 3
};

struct   TextElem
{
  Position         pos; //position of button, orgin in center
  float            sizeX; //size X of the box
  float            sizeY; // size Y of the box
  std::string      text; //text inside the button, can be null if just want to display a box
  std::string      fontName;
  size_t           FontSize;
  unsigned int     color; //color of the box
  bool             isButton;
  GroupButton      group;
  std::string      value; // value returned if button is pressed
};

struct GameInfos
{
  std::string       playerName;
  int               score;
  std::string       buttonPressed;
};

class IDisplayManager;

class IGame
{
public:
  virtual ~IGame() {};
  virtual int getSizeX() const = 0;
  virtual int getSizeY() const = 0;
  virtual int play(char, int) = 0;
  virtual char  **getMap() const = 0;
  virtual std::string startGame(IDisplayManager &, std::string const &playerName) = 0;
  virtual std::list<TextElem>  *getTextElemList() = 0;
  virtual std::map<int, std::list<void *>> *getEntities() = 0;
};
#endif /* ! _IGAME_HPP_ */
