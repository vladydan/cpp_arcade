#include "display_managers/CACADisplayManager.hpp"
#include <iostream>
#include <chrono>

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

CACADisplayManager::CACADisplayManager()
{
  this->_shapes.resize(256);
}

CACADisplayManager::~CACADisplayManager()
{
  cucul_free_canvas(this->_canvas);
  caca_free_display(this->_window);
}

void findBackButton(std::list<TextElem>  *buttons, int &flag, GroupButton group)
{
  int i;

  i = buttons->size();
  for (std::list<TextElem>::iterator it = buttons->end(); it != buttons->begin(); --it, --i)
  {
    if (group == (*it).group && i < flag && (*it).isButton)
    {
      flag = i;
      return ;
    }
  }
  std::list<TextElem>::iterator it = buttons->begin();
  if (group == (*it).group && i < flag && (*it).isButton)
    flag = 0;
}

void findNextButton(std::list<TextElem>  *buttons, int &flag, GroupButton group)
{
  int i;

  i = 0;
  for (std::list<TextElem>::iterator it = buttons->begin(); it != buttons->end(); ++it, ++i)
  {
    if (group == (*it).group && i > flag && (*it).isButton)
    {
      flag = i;
      return ;
    }
  }
}

std::string &getButtonValue(std::list<TextElem>  *buttons, int &flag)
{
  int i;
  std::list<TextElem>::iterator it;
  i = 0;
  for (it = buttons->begin(); it != buttons->end(); ++it, ++i)
  {
    if (i == flag)
    {
      flag = i;
      return (*it).value;
    }
  }
  return (*it).value;
}

const std::string       CACADisplayManager::displayMenu(IGame &game, GroupButton group)
{
  std::list<TextElem>  *buttons;
  caca_event_t ev;
  int x;
  int y;
  int displayFlag;
  int i;
  std::string playerName("");

  x = 0;
  buttons = game.getTextElemList();
  displayFlag = buttons->size();
  findBackButton(buttons, displayFlag, group);
  while (1)
  {
    y = 0;
    caca_refresh_display(this->_window);
    caca_clear_canvas(this->_canvas);
    i = 0;
    for (std::list<TextElem>::iterator it = buttons->begin(); it != buttons->end(); ++it, ++i)
    {
      if (group == (*it).group)
      {
        x = (((*it).pos.x / 8) * 8) / 8;
        y = (((*it).pos.y / 16) * 16) / 16;
        if (i == displayFlag)
          caca_set_color_argb(this->_canvas, 0xFF, 0xFFFF);
        else
          caca_set_color_argb(this->_canvas, 0x00, 0xFFFF);
        caca_printf(this->_canvas, x, y, (*it).text.c_str());
        y += 1;
      }
    }
    while (caca_get_event(this->_window, CACA_EVENT_KEY_PRESS, &ev, 0))
    {
      if (caca_get_event_key_ch(&ev) == CACA_KEY_UP)
        findNextButton(buttons, displayFlag, group);
      if (caca_get_event_key_ch(&ev) == CACA_KEY_DOWN)
        findBackButton(buttons, displayFlag, group);
      if (caca_get_event_key_ch(&ev) == CACA_KEY_RETURN)
        return getButtonValue(buttons, displayFlag);
      if (caca_get_event_type(&ev) == CACA_EVENT_KEY_PRESS && caca_get_event_key_utf32(&ev) == 27 && game.getSizeX() == -1)
      {
        exit(0);
      }
      if (caca_get_event_key_ch(&ev) > 48 && caca_get_event_key_ch(&ev) < 58)
      {
        playerName += caca_get_event_key_ch(&ev);
        return "noGame " + playerName;
      }
      if (caca_get_event_type(&ev) == CACA_EVENT_KEY_PRESS && caca_get_event_key_utf32(&ev) == 27)
        return "exit";
    }
  }
  return "";
}

void CACADisplayManager::setButton(TextElem const &button)
{
  (void)button;
}

char CACADisplayManager::handleEvents(caca_event_t &ev)
{
  if (caca_get_event_key_utf32(&ev) >= 97 && caca_get_event_key_utf32(&ev) <= 122)
    return caca_get_event_key_utf32(&ev);
  if (caca_get_event_key_utf32(&ev) >= 48 && caca_get_event_key_utf32(&ev) <= 56)
    return caca_get_event_key_utf32(&ev);
  return (0);
}

int CACADisplayManager::createWindow(size_t const & length, size_t const &height, std::string const &name)
{
  this->_canvas = cucul_create_canvas(200, 70);
  (void)length;
  (void)height;
  this->_window = caca_create_display(this->_canvas);
  caca_set_display_title(this->_window, name.c_str());
  caca_set_display_time(this->_window, 1500);
  return (0);
}

int CACADisplayManager::setShape(IGame &game, int const tileType, std::string const &shape, unsigned int const color, unsigned int const size, std::string const &textureName)
{
  this->_shapes[tileType].color = color;
  this->_shapes[tileType].character = '*';
  (void)shape;
  (void)game;
  (void)size;
  (void)textureName;
  return (0);
}

void CACADisplayManager::replay(IGame &game, GameInfos &info)
{
  (void)game;
  (void)info;
}

void CACADisplayManager::closeWindow()
{
}

std::string CACADisplayManager::startGame(IGame &game, std::string const &musicName)
{
  caca_event_t ev;
  std::string info;
  char gameEvent;
  int ret;
  auto startTime = Time::now();

  static_cast<void>(musicName);
  while (1)
  {
    caca_refresh_display(this->_window);
    auto currentTime = Time::now();
    while (caca_get_event(this->_window, CACA_EVENT_KEY_PRESS, &ev, 0))
    {
      if (gameEvent == 0)
        gameEvent = this->handleEvents(ev);
      if (gameEvent >= 48 && gameEvent <= 66)
      {
        info += gameEvent;
        return info;
      }
      if (caca_get_event_type(&ev) == CACA_EVENT_KEY_PRESS && caca_get_event_key_utf32(&ev) == 27)
      {
        exit(0);
      }
    }
    fsec fs = currentTime - startTime;
    ret = game.play(gameEvent, std::chrono::duration_cast<ms>(fs).count());
    if (ret == -1)
      return "";
    if (ret != -2)
    {
      gameEvent = 0;
    }
    caca_clear_canvas(this->_canvas);
    this->displayMap(game.getMap(), game.getSizeX(), game.getSizeY());
    this->displayEntities(game.getEntities(), game.getSizeX(), game.getSizeY());
  }
  return "";
}

void CACADisplayManager::displayMap(char **map, int const sizeX, int const sizeY)
{
  int i;
  int j;
  int e;
  int r;
  int currentI;
  int currentJ;

  i = 0;
  currentJ = 0;
  currentI = 0;
  while (i < sizeY)
  {
    j = 0;
    currentJ = 0;
    while (j < sizeX)
    {
      e = currentI;
      while (e < currentI + 2)
      {
        r = currentJ;
        while (r < currentJ + 4)
        {
          if (map[i][j] != 0)
          {
            if (map[i][j] == 2)
              caca_set_color_ansi(this->_canvas, 0, CACA_MAGENTA);
            else if (map[i][j] == 3)
              caca_set_color_ansi(this->_canvas, 0, CACA_RED);
            caca_put_char(this->_canvas, r, e, map[i][j] + 48);
          }
          r++;
        }
        e++;
      }
      currentJ += 4;
      j++;
    }
    currentI += 2;
    i++;
  }
}

void CACADisplayManager::displayEntities(std::map<int, std::list<void *>> *entities, int const sizeX, int const sizeY)
{
  if (entities == NULL)
    return;
  std::map<int, std::list<void *>>::iterator it;
  std::list <void *>::iterator it2;
  int i;
  int e;
  int r;

  (void)sizeX;
  (void)sizeY;
  i = 0;
  for (it = entities->begin(); it != entities->end();++it)
  {
    for (it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
    {
      auto *entity = static_cast<Part *>(*it2);
      e = entity->y * 2;
      while (e < entity->y * 2 + 2)
      {
        r = entity->x * 4;
        while (r < entity->x *4 + 2)
        {
          if ((*it).first == 0)
            caca_set_color_ansi(this->_canvas, 0, CACA_WHITE);
          else if ((*it).first == 1)
            caca_set_color_ansi(this->_canvas, CACA_GREEN, 0);
          else if ((*it).first == 7)
            caca_set_color_ansi(this->_canvas, CACA_BROWN, 0);
          else if ((*it).first == 5)
            caca_set_color_ansi(this->_canvas, CACA_YELLOW, 0);
           caca_put_char(this->_canvas, r, e, (*it).first + 48);
          r++;
        }
        e++;
      }
    }
    i++;
  }
}

extern "C"
IDisplayManager *entryPoint()
{
  return new CACADisplayManager();
}
