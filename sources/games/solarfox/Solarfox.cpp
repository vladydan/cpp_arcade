#include "games/Solar.hpp"
#include "display_managers/IDisplayManager.hpp"
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <unistd.h>

Solar::Solar() : Game(20, 20)
{
  Part  **enemy;
  Part  *player;

  srand(time(NULL));
  enemy = new Part*[4];
  for (int i = 0; i < 4; ++i)
    enemy[i] = new Part();
  for (int i = 0; i < 4; i++)
    enemy[i]->time = (rand() % TIME_MISSILE) + 40;
  for (int i = 0; i < 4; i++)
    {
      if (i == 0 || i == 2)
	enemy[i]->x = this->_sizeX / 2;
      if (i == 1 || i == 3)
	enemy[i]->y = this->_sizeY / 2;
    }
  enemy[0]->y = 0;
  enemy[1]->x = 0;
  enemy[2]->y = this->_sizeY - 1;
  enemy[3]->x = this->_sizeX - 1;
  for (int i = 0; i < 4; i++)
    this->_enemy.push_back(enemy[i]);
  this->_speed = 0.008;
  this->_cheat = false;
  this->_direction = 'd';
  this->_rangeMax = RANGE_MAX;
  this->_time = this->_previousTime = this->_score = 0;
  this->_playerX = this->_playerY = 7;
  player = new Part();
  player->x = this->_playerX;
  player->y = this->_playerY;
  this->setMenu();
  this->_player.push_back(player);
  NewFruit();
  NewBad();
}

int	Solar::MovePlayer(char input)
{
  if ((this->_map[static_cast<int>(this->_playerY + 0.5)][static_cast<int>(this->_playerX + 0.5)] == 3) && !this->_cheat)
    return (-1);
  if (input == 'z')
    {
      if (this->_playerY > 1)
        this->_playerY -= this->_speed * this->_deltaSeconde;
      else
      {
        if (!this->_cheat)
          return (-1);
      }
    }
  else if (input == 'q')
   {
    if (this->_playerX > 1)
     this->_playerX -= this->_speed * this->_deltaSeconde;
    else
    {
     if (!this->_cheat)
       return (-1);
      }
    }
  else if (input == 's')
   {
    if (this->_playerY <= this->_sizeY -2)
     this->_playerY += this->_speed * this->_deltaSeconde;
    else
      {
        if (!this->_cheat)
          return (-1);
      }
    }
  else if (input == 'd')
   {
    if (this->_playerX < this->_sizeX -2)
     this->_playerX += this->_speed * this->_deltaSeconde;
    else
      {
        if (!this->_cheat)
          return (-1);
      }
    }
  return (0);
}

std::map<int, std::list<void *>> *Solar::getEntities()
{
  std::list<Part*> save;
  std::list<Part*> savePlayer;

  for (std::list<Part *>::iterator p = this->_enemy.begin(); p != this->_enemy.end(); ++p)
  {
      for (std::list<Part *>::iterator p2 = (*p)->rocket.begin(); p2 != (*p)->rocket.end(); ++p2)
        save.push_back(*p2);
  }
  std::list<Part *>::iterator p = this->_player.begin();
  {
      for (std::list<Part *>::iterator p2 = (*p)->rocket.begin(); p2 != (*p)->rocket.end(); ++p2)
        savePlayer.push_back(*p2);
  }
  this->_entities[8] = castList<void *, Part *>(this->_enemy);
  this->_entities[1] = castList<void *, Part *>(savePlayer);
  (*this->_player.begin())->x = this->_playerX;
  (*this->_player.begin())->y = this->_playerY;
  this->_entities[2] = castList<void *, Part *>(this->_player);
  this->_entities[5] = castList<void *, Part *>(save);
  return (&this->_entities);
}

char	Solar::InputGestion(char input)
{
  if (input == 0)
    return (this->_direction);
  // else if (input == 'd' && this->_direction == 'q')
  //   return ('q');
  // else if (input == 'q' && this->_direction == 'd')
  //   return ('d');
  // else if (input == 'z' && this->_direction == 's')
  //   return ('s');
  // else if (input == 's' && this->_direction == 'z')
  //   return ('z');
  this->_direction = input;
  return (input);
}


void  Solar::NewFruit()
{
  int x, y, fruit;

  fruit = NBR_FRUITS;
  x = (rand() % static_cast<int>(this->_sizeX - 4)) + 2;
  y = (rand() % static_cast<int>(this->_sizeY - 4)) + 2;
  while (fruit > 0)
    {
      while (this->_map[y][x] != 0)
      {
        x = (rand() % static_cast<int>(this->_sizeX - 4)) + 2;
        y = (rand() % static_cast<int>(this->_sizeY - 4)) + 2;
      }
      this->_map[y][x] = 6;
      fruit--;
    }
}

void  Solar::NewBad()
{
  int x, y, fruit;

  fruit = NBR_BAD;
  x = (rand() % static_cast<int>(this->_sizeX - 4)) + 2;
  y = (rand() % static_cast<int>(this->_sizeY - 4)) + 2;
  while (fruit > 0)
    {
      while (this->_map[y][x] != 0)
      {
        x = (rand() % static_cast<int>(this->_sizeX - 4)) + 2;
        y = (rand() % static_cast<int>(this->_sizeY - 4)) + 2;
      }
      this->_map[y][x] = 3;
      fruit--;
    }
}

void	Solar::MoveX(int const r, std::list<Part *>::iterator &p)
{
  if (r == 0)
    {
      if (((*p)->x -1) > 0)
        (*p)->x -= this->_speed * this->_deltaSeconde;
    }
    else if (r == 1)
    {
      if (((*p)->x +1) < this->_sizeX -1)
        (*p)->x += this->_speed * this->_deltaSeconde;
    }
}

void	Solar::MoveY(int const r, std::list<Part *>::iterator &p)
{
  if (r == 0)
  {
    if (((*p)->y -1) > 0)
      (*p)->y -= this->_speed * this->_deltaSeconde;
  }
  else if (r == 1)
  {
    if (((*p)->y +1) < this->_sizeY -1)
      (*p)->y += this->_speed * this->_deltaSeconde;
  }
}

void  Solar::CreateMissils()
{
  int i = 0;

  for (std::list<Part *>::iterator p = this->_enemy.begin(); p != this->_enemy.end(); ++p)
    {
      --(*p)->time;
      if ((*p)->time <= 0 && ((*p)->rocket.size() == 0))
      {
        (*p)->time = (rand() % TIME_MISSILE) + 20;
        Part *body = new Part();
        body->x = (*p)->x;
        body->y = (*p)->y;
        (*p)->rocket.push_front(body);
        ++i;
      }
    }
}

void  Solar::CreateMissilPlayer()
{
  std::list<Part *>::iterator p;

  p = this->_player.begin();
  --(*p)->time;
  if ((*p)->time <= 0 && ((*p)->rocket.size() == 0))
  {
    (*p)->time = TIME_MISSILE / 4;
    Part *body = new Part();
    body->x = (*p)->x;
    body->distance = 0;
    body->y = (*p)->y;
    body->direction = this->_direction;
    (*p)->rocket.push_front(body);
  }
}

bool  Solar::CheckColision(std::list<Part *>::iterator &p2)
{
  if (this->_cheat == true)
    return (true);
  if ((this->_playerY < (*p2)->y + 0.75 && this->_playerY > ((*p2)->y - 0.75))
    && (this->_playerX < (*p2)->x + 0.75 && this->_playerX > ((*p2)->x - 0.75)))
    return (false);
  return (true);
}

bool  Solar::CheckFruit()
{
  int x, y;

  y = -1;
  while (++y < this->_sizeY)
  {
    x = -1;
    while (++x < this->_sizeX)
    {
      if (this->_map[y][x] == 6)
        return (true);
    }
  }
  return (false);
}

bool  Solar::CheckColisionPlayer(std::list<Part *>::iterator &p2)
{
  bool flag = true;
  if (static_cast<int>((*p2)->y + 0.5) > 19 || static_cast<int>((*p2)->x + 0.5) > 19 || static_cast<int>((*p2)->y + 0.5) < 1 || static_cast<int>((*p2)->x + 0.5) < 1)
    return (true);
  if (this->_map[static_cast<int>((*p2)->y + 0.5)][static_cast<int>((*p2)->x + 0.5)] == 6)
  {
    this->_map[static_cast<int>((*p2)->y + 0.5)][static_cast<int>((*p2)->x + 0.5)] = 0;
    ++this->_score;
    if (CheckFruit() == false)
    {
      this->_speed += 0.001;
      NewFruit();
      NewBad();
    }
    return (false);
  }
  if (this->_map[static_cast<int>((*p2)->y + 0.5)][static_cast<int>((*p2)->x + 0.5)] == 3)
  {
    this->_map[static_cast<int>((*p2)->y + 0.5)][static_cast<int>((*p2)->x + 0.5)] = 0;
    return (false);
  }
  for (std::list<Part *>::iterator p = this->_enemy.begin(); p != this->_enemy.end(); ++p)
    {
      for (std::list<Part *>::iterator p3 = (*p)->rocket.begin(); p3 != (*p)->rocket.end(); ++p3)
	{
	  if (static_cast<int>((*p3)->y + 0.5) == static_cast<int>((*p2)->y + 0.5)
	    && static_cast<int>((*p3)->x + 0.5) == static_cast<int>((*p2)->x + 0.5))
	  {
	    flag = false;
	    (*p)->rocket.erase(p3);
	    break;
	    //return (false);
	  }
	}
      }
  //Colision boulle
  return (flag);
}

int Solar::MoveMissils()
{
  int i = 0;

  for (std::list<Part *>::iterator p = this->_enemy.begin(); p != this->_enemy.end(); ++p)
    {
      for (std::list<Part *>::iterator p2 = (*p)->rocket.begin(); p2 != (*p)->rocket.end(); ++p2)
      {
	if (CheckColision(p2) == false)
	  return (-1);
	if (i == 0 && ((*p2)->y) < this->_sizeY)
	  (*p2)->y += (this->_speed)* this->_deltaSeconde;
	else if (i == 1 && ((*p2)->x) < this->_sizeX)
	  (*p2)->x += (this->_speed) * this->_deltaSeconde;
	else if (i == 2 && ((*p2)->y) > 0)
	  (*p2)->y -= (this->_speed) * this->_deltaSeconde;
	else if (i == 3 && ((*p2)->x) > 0)
	  (*p2)->x -= (this->_speed) * this->_deltaSeconde;
	else
	  {
	    (*p)->rocket.erase(p2);
	    break;
	   }
      }
    ++i;
  }
  return (0);
}

void  Solar::MoveMissilsPlayer()
{
  std::list<Part *>::iterator p;
  p = this->_player.begin();
  for (std::list<Part *>::iterator p2 = (*p)->rocket.begin(); p2 != (*p)->rocket.end(); ++p2)
    {
      if (CheckColisionPlayer(p2) == false)
      {
	(*p)->rocket.erase(p2);
	break;
      }
      if ((*p2)->direction == 's' && ((*p2)->y) < this->_sizeY)
       (*p2)->y += (this->_speed + 0.01)* this->_deltaSeconde;
      else if ((*p2)->direction == 'd' && ((*p2)->x) < this->_sizeX)
	(*p2)->x += (this->_speed + 0.01) * this->_deltaSeconde;
      else if ((*p2)->direction == 'z' && ((*p2)->y) > 0)
	(*p2)->y -= (this->_speed + 0.01) * this->_deltaSeconde;
      else if ((*p2)->direction == 'q' && ((*p2)->x) > 0)
	(*p2)->x -= (this->_speed + 0.01) * this->_deltaSeconde;
      else
	{
	  (*p)->rocket.erase(p2);
	  break;
	}
      (*p2)->distance += this->_speed * this->_deltaSeconde;
      if ((*p2)->distance > this->_rangeMax)
	{
	  (*p)->rocket.erase(p2);
	  break;
	}
    }
}

int     Solar::CheckBad()
{
  return (0);
}

void    Solar::MoveEnemy()
{
  int   i, r;

  i = -1;
  srand(time(NULL));
  for (std::list<Part *>::iterator p = this->_enemy.begin(); p != this->_enemy.end(); ++p)
    {
      ++i;
      r = rand() % 2;
      if (i == 0 || i == 2)
	this->MoveX(r, p);
      else if (i == 1 || i == 3)
	this->MoveY(r, p);
    }
}

bool    Solar::CheatRange(char const input)
{
  if (input == 'r')
    this->_rangeMax = this->_playerX;
  else if (input == 't')
    this->_rangeMax = 1;
  else if (input == 'y')
    this->_rangeMax = RANGE_MAX;
  else if (input == 'c')
    this->_cheat = !this->_cheat;
  else
    return (false);
  return (true);
}

int     Solar::play(char input, int const time)
{
  int   save;

  this->setScore();
  save = this->_time;
  this->_time += time - this->_time;
  this->_deltaSeconde = time - save;
  if (time == -2)
  {
     this->_deltaSeconde = 126;
  }
  if (CheatRange(input) == true)
    input = this->_direction;
  input = this->InputGestion(input);
  if (this->MovePlayer(input) == -1)
    return (-1);
  if (this->CheckBad() == -1)
    return (-1);
  this->MoveEnemy();
  this->CreateMissils();
  this->CreateMissilPlayer();
  if (this->MoveMissils() == -1)
    return (-1);
  this->MoveMissilsPlayer();
  return (this->_score);
}

std::string	Solar::startGame(IDisplayManager &displayManager, std::string const &playerName)
{
  std::string info;

  usleep(10000);
  displayManager.setShape(*this, 0, "square", 0x000000, WINDOW_HEIGHT / this->_sizeX , "ressources/space.png");
  displayManager.setShape(*this, 1, "circle", 0x0000FF, WINDOW_HEIGHT / this->_sizeX / 2, "ressources/missileblue.png");
  displayManager.setShape(*this, 6, "circle", 0xFFFFFF, WINDOW_HEIGHT / this->_sizeX / 2, "ressources/star2.png");
  displayManager.setShape(*this, 3, "circle", 0xFF0000, WINDOW_HEIGHT / this->_sizeX / 2, "ressources/badstar.png");
  displayManager.setShape(*this, 5, "circle", 0x009FFF, WINDOW_HEIGHT / this->_sizeX / 2, "ressources/missile2.png");
  displayManager.setShape(*this, 2, "circle", 0x008000, WINDOW_HEIGHT / this->_sizeX / 2, "ressources/ship.png");
  displayManager.setShape(*this, 8, "circle", 0xFFFF00, WINDOW_HEIGHT / this->_sizeX , "ressources/voltorbe.png");
  // if (displayManager.displayMenu(*this, MENUBTN) == "exit")
  //   return "exit";
  info = displayManager.startGame(*this, "ressources/SadViolin.ogg");
  if (*info.begin() > 48 && *info.begin() < 56)
    return info;
  this->saveScore(playerName, "SolarFox");
  return displayManager.displayMenu(*this, ENDGAME);
}

extern "C"
IGame *entryPoint()
{
  return new Solar();
}
