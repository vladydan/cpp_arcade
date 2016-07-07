#ifndef _SNAKE_HPP_
#define _SNAKE_HPP_

#include "Game.hpp"
#include <string>
#include <list>

#define WINDOW_HEIGHT_SNAKE 900
#define WINDOW_LENGHT_SNAKE 900

class Snake : public Game
{
 private:
  std::map<int, std::list<void *>>  _entities;
  std::list<Part *>	_body;
  int			_previousTime;
  int			_time;
  int			_size;
  char			_direction;
  bool			_cheat;
  bool      _play;

  void  InitMap();
  char  InputGestion(char);
  int   MoveSnake(char, Part *);
  void  NewFruit();
  void  setMenu();
 public:
  Snake();
  virtual ~Snake() {};
  virtual int getSize();
  virtual int play(char, int);
  virtual std::string startGame(IDisplayManager &, std::string const &playerName);
  virtual std::map<int, std::list<void *>> *getEntities();
};

#endif /* ! _GAME_HPP_ */
