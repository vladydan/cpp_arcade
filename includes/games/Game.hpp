#ifndef GAME_HPP
# define GAME_HPP

#include "IGame.hpp"
#include <algorithm>

class Game : public IGame
{
 public:
  Game(int const sizeX, int const sizeY);
  virtual ~Game() {};
  virtual int play(char, int const) = 0;
  virtual char **getMap() const;
  virtual int getSizeX() const;
  virtual int getSizeY() const;
  virtual std::string startGame(IDisplayManager &, std::string const &playerName) = 0;
  virtual std::list<TextElem>  *getTextElemList();
  virtual std::map<int, std::list<void *>> *getEntities() = 0;
  void setScore();
  void saveScore(std::string const playerName, std::string const &gameName) const;
 protected:
  char		**_map;
  float		_speed;
  std::list<TextElem> *_buttonList;
  int		_sizeX;
  int		_sizeY;
  int     _score;
};

template<class T, class I>
std::list<T> castList(const std::list<I> &v)
{
  std::list<T> ret(v.size());
    std::transform(v.begin(), v.end(), ret.begin(), [](I p){return (T)(p);});
  return ret;
}

#endif /* ! GAME_HPP */
