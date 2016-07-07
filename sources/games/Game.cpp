#include "games/Game.hpp"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <fstream>

Game::Game(int const sizeX, int const sizeY) : IGame()
{
  int a;

  this->_sizeX = sizeX;
  this->_sizeY = sizeY;
  this->_map = new char*[this->_sizeY];
  for (int i = 0; i < this->_sizeY; ++i)
  {
    a = -1;
    this->_map[i] = new char[this->_sizeX];
    while (++a < this->_sizeX)
      this->_map[i][a] = 0;
  }
}

char	**Game::getMap() const
{
  return (this->_map);
}

int   Game::getSizeX() const
{
  return (this->_sizeX);
}

int   Game::getSizeY() const
{
  return (this->_sizeY);
}

std::list<TextElem> *Game::getTextElemList()
{
  return (this->_buttonList);
}

void Game::setScore()
{
  std::list<TextElem>::iterator it;

  for (it = this->_buttonList->begin(); it != this->_buttonList->end(); ++it)
  {
    if ((*it).value == "score")
    {
      (*it).text = "Score : " + std::to_string(this->_score);
    }
    if ((*it).value == "score")
    {
      (*it).text = "Score : " + std::to_string(this->_score);
    }
  }
}

void Game::saveScore(std::string const playerName, std::string const &gameName) const
{
  std::ofstream outfile;

  outfile.open("ressources/score.txt", std::ios_base::app);
  outfile << gameName << ": " << playerName << " " << std::to_string(this->_score) << std::endl;
}
