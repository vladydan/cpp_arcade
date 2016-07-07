#ifndef _SOLAR_HPP_
#define _SOLAR_HPP_

#define TIME_MISSILE 40
#define NBR_FRUITS   15
#define NBR_BAD      5
#define RANGE_MAX    2

#include "Game.hpp"

#define WINDOW_HEIGHT_SOLAR 1000
#define WINDOW_LENGHT_SOLAR 1000

class Solar : public Game
{
 public:
  Solar();
  virtual ~Solar() {};
  virtual int play(char, int);
  virtual std::string startGame(IDisplayManager &, std::string const &playerName);
  virtual std::map<int, std::list<void *>> *getEntities();

 private:
  char    InputGestion(char);
  void    MoveX(int const, std::list<Part *>::iterator &);
  void    MoveY(int const, std::list<Part *>::iterator &);
  bool    CheatRange(char const input);
  bool    CheckColision(std::list<Part *>::iterator &);
  bool    CheckColisionPlayer(std::list<Part *>::iterator &);
  int     MovePlayer(char);
  void    NewBad();
  int     CheckBad();
  void    CreateMissils();
  void    CreateMissilPlayer();
  void    MoveMissilsPlayer();
  void    NewFruit();
  bool    CheckFruit();
  int     MoveMissils();
  void    MoveEnemy();
  void    setMenu();
 protected:
  std::map<int, std::list<void *>>  _entities;
  std::list<Part *>                 _enemy;
  std::list<Part *>                 _player;
  int                               _previousTime;
  float                               _deltaSeconde;
  int                               _time;
  int                               _rangeMax;
  char                              _direction;
  float                             _playerX;
  float                             _playerY;
  bool                              _cheat;
};

#endif /* ! _GAME_HPP_ */
