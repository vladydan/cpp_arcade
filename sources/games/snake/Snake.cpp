#include "games/Snake.hpp"
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include "display_managers/IDisplayManager.hpp"
#include <iostream>
#include <unistd.h>

void	Snake::InitMap()
{
  int x, y;

  y = -1;
  while (++y < this->_sizeY)
    {
      x = -1;
      while (++x < this->_sizeX)
      {
	if (y == 0 || y == this->_sizeY - 1 || x == 0 || x == this->_sizeX - 1)
	  this->_map[y][x] = 1;
	else
	  this->_map[y][x] = 0;
      }
    }
  this->setMenu();
}

std::map<int, std::list<void *>> *Snake::getEntities()
{
  this->_entities[7] = castList<void *, Part *>(this->_body);
  return (&this->_entities);
}

int Snake::getSize()
{
  return (this->_size);
}

Snake::Snake() : Game(20, 20)
{
  Part *head;
  Part *bodyOne;
  Part *bodyTwo;
  Part *bodyTree;

  head = new Part();
  bodyOne = new Part();
  bodyTwo = new Part();
  bodyTree = new Part();
  head->x = (this->_sizeX / 2);
  bodyOne->x = (this->_sizeX / 2) - 1;
  bodyTwo->x = (this->_sizeX / 2) - 2;
  bodyTree->x = (this->_sizeX / 2) - 3;
  head->y = bodyOne->y = bodyTwo->y = bodyTree->y = (this->_sizeY / 2);
  this->_body.push_back(head);
  this->_body.push_back(bodyOne);
  this->_body.push_back(bodyTwo);
  this->_body.push_back(bodyTree);
  this->_size = 4;
  this->_direction = 'd';
  this->_score = 0;
  this->_previousTime = 0;
  this->_time = 0;
  this->_speed = 75;
  this->_cheat = false;
  this->_play = false;
  this->setMenu();
  this->InitMap();
  this->NewFruit();
}

int	Snake::MoveSnake(char input, Part *newbody)
{
  if (input == 'z')
    {
      newbody->y--;
      if (newbody->y <= 0)
        return (1);
    }
  else if (input == 'q')
    {
      newbody->x--;
      if (newbody->x <= 0)
        return (1);
    }
  else if (input == 's')
    {
      newbody->y++;
      if (newbody->y >= this->_sizeY -1)
        return (1);
    }
  else if (input == 'd')
    {
      newbody->x++;
      if (newbody->x >= this->_sizeX -1)
        return (1);
    }
  return (0);
}

char  Snake::InputGestion(char input)
{
  if (input == 0)
    return (this->_direction);
  else if (input == 'd' && this->_direction == 'q')
    return ('q');
  else if (input == 'q' && this->_direction == 'd')
    return ('d');
  else if (input == 'z' && this->_direction == 's')
    return ('s');
  else if (input == 's' && this->_direction == 'z')
    return ('z');
  else if (input == 'c')
    {
      this->_cheat = !this->_cheat;
      return (this->_direction);
    }
  this->_direction = input;
  return (input);
}

void  Snake::NewFruit()
{
  int x, y, ok;

  srand(time(NULL));
  y = ok = -1;
  while (++y < this->_sizeY)
    {
      x = -1;
      while (++x < this->_sizeX)
	{
	  if (this->_map[x][y] == 6)
	    return;
	}
    }
  x = (rand() % this->_sizeX);
  y = (rand() % this->_sizeY);
  while (ok == -1)
  {
    std::list<Part *>::iterator p2 = this->_body.begin();
    while (p2 != this->_body.end())
      {
        if (((x == static_cast<int>((*p2)->x)) && (y == static_cast<int>((*p2)->y))) || this->_map[y][x] == 1)
          break;
        ++p2;
        if (p2 == this->_body.end())
          ok = 1;
      }
    if (ok == -1)
    {
      x = (rand() % this->_sizeX);
      y = (rand() % this->_sizeY);
    }
  }
  this->_map[y][x] = 6;
}

int	Snake::play(char input, int time)
{
  Part
   *newbody;
  int fruit;
  if (time == -2)
  {
    InputGestion(input);
    return (0);
  }
  if (time != -3)
  {
    this->_time += time - this->_previousTime;
    this->_previousTime = time;
    if (!this->_cheat && (this->_time > (this->_speed - (this->_score % 75))))
      this->_time = 0;
    else if (this->_cheat && (this->_time > (this->_speed - (0 % 75))))
      this->_time = 0;
    else
      return (-2);
  }
  fruit = 0;
  NewFruit();
  newbody = new Part();
  input = InputGestion(input);
  std::list<Part *>::iterator savefirst = this->_body.begin();
  newbody->x = (*savefirst)->x;
  newbody->y = (*savefirst)->y;
  if (MoveSnake(input, newbody) == 1)
    return (-1);
  if (this->_map[static_cast<int>(newbody->y)][static_cast<int>(newbody->x)] == 6)
    {
      this->_map[static_cast<int>(newbody->y)][static_cast<int>(newbody->x)] = 0;
      this->_score++;
      fruit = 1;
    }
  for (std::list<Part *>::iterator p2 = this->_body.begin(); p2 != this->_body.end(); ++p2)
    {
      if ((static_cast<int>(newbody->x) == static_cast<int>((*p2)->x)) && (static_cast<int>(newbody->y) == static_cast<int>((*p2)->y)))
        return (-1);
    }
  this->_body.push_front(newbody);
  std::list<Part *>::iterator psave = this->_body.begin();
  std::advance(psave, this->_body.size() - 1);
  if (fruit == 0)
    {
      this->_map[static_cast<int>(((*psave)->y))][static_cast<int>(((*psave)->x))] = 0;
      this->_body.pop_back();
    }
  this->setScore();
  return (this->_score);
}

std::string Snake::startGame(IDisplayManager &displayManager, std::string const &playerName)
{
  std::string info;

  usleep(100000);
  displayManager.setShape(*this, 0, "square", 0x008000, WINDOW_HEIGHT / this->getSizeX(), "ressources/grass.png");
  displayManager.setShape(*this, 1, "square", 0xA52A2A, WINDOW_HEIGHT / this->getSizeY(), "ressources/wall2.png");
  displayManager.setShape(*this, 7, "circle", 0xFFFF00, WINDOW_HEIGHT / this->getSizeX() / 2, "ressources/boulay_b.png");
  displayManager.setShape(*this, 6, "circle", 0xFF0000, WINDOW_HEIGHT / this->getSizeX() / 2, "ressources/kebab2.png");
  // if (displayManager.displayMenu(*this, MENUBTN) == "exit")
  //   return "exit";
  info = displayManager.startGame(*this, "ressources/SadViolin.ogg");
  if (*info.begin() > 48 && *info.begin() < 56)
    return info;
  this->saveScore(playerName, "Snake");
  return displayManager.displayMenu(*this, ENDGAME);
}

extern "C"
IGame *entryPoint()
{
  return new Snake();
}
