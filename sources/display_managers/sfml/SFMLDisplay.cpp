#include "display_managers/SFMLDisplayManager.hpp"
#include "games/IGame.hpp"
#include "games/Game.hpp"
#include <string>
#include <iostream>
#include "extern/SFML/Audio/Music.hpp"

int SFMLDisplayManager::createWindow(size_t const & length, size_t const &height, std::string const &name)
{
  this->_windowLength = length;
  this->_windowHeight = height;
  this->_window = new sf::RenderWindow(sf::VideoMode(length, height), name);
  if (this->_window != NULL)
  {
    this->_window->setFramerateLimit(30);
    return 0;
  }
  return 1;
}

void SFMLDisplayManager::closeWindow()
{
  this->_window->close();
}

const std::string SFMLDisplayManager::displayMenu(IGame &game, GroupButton group)
{
  (void)game;
  sf::Event event;
  std::string ret;
  sf::Text userText;

  if (group == MENUBTN)
  {
    userText.setCharacterSize(30);
    userText.setFont(this->_font);
    userText.setPosition(350, 150);
    userText.setColor(sf::Color::Red);
  }
  if (group == INGAME)
  {
    this->displayButtons(game, group);
    return "";
  }
  while (this->_window->isOpen())
  {
    while (this->_window->pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        this->_window->close();
      this->getUserInput(event, this->_playerName);
      if (*this->_playerName.begin() > '1' && *this->_playerName.begin() < '6')
        return "noGame " + this->_playerName;
    }
    if ((ret = this->checkMouseInput(game)) != "")
    {
      if (game.getSizeX() == -1)
        ret += " " + this->_playerName;
      return ret;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
      return "exit";
    this->_window->clear(sf::Color(0xFF, 0xFF, 0xFF));
    userText.setString(this->_playerName);
    this->displayMap(game.getMap(), game.getSizeX(), game.getSizeY());
    this->displayEntities(game.getEntities(), game.getSizeX(), game.getSizeY());
    this->displayButtons(game, group);
    if (game.getSizeX() == -1)
      this->_window->draw(userText);
    this->_window->display();
  }
  return "";
}

void SFMLDisplayManager::displayMap(char **map, int const sizeX, int const sizeY)
{
  int i;
  int j;

  i = 0;
  while (i < sizeY)
  {
    j = 0;
    while (j < sizeX)
    {
      this->_shapeList[0].rectangleShape->setPosition(j * this->_shapeList[0]._tileSize, i * this->_shapeList[0]._tileSize);
      this->_window->draw(*this->_shapeList[0].rectangleShape);
      if (this->_shapeList[map[i][j]].rectangleShape != NULL)
      {
        this->_shapeList[map[i][j]].rectangleShape->setPosition(j * this->_shapeList[map[i][j]]._tileSize, i * this->_shapeList[map[i][j]]._tileSize);
        this->_window->draw(*this->_shapeList[map[i][j]].rectangleShape);
      }
      else if (this->_shapeList[map[i][j]].circleShape != NULL)
       {
        this->_shapeList[map[i][j]].circleShape->setPosition(j * (this->_windowLength / sizeX), i * (this->_windowHeight / sizeY));
        this->_window->draw(*this->_shapeList[map[i][j]].circleShape);
       }
      else
      {
        this->_shapeList[map[i][j]].rectangleShape = new sf::RectangleShape(sf::Vector2f(j * this->_shapeList[map[i][j]]._tileSize,
          i * this->_shapeList[map[i][j]]._tileSize));
        this->_window->draw(*this->_shapeList[map[i][j]].rectangleShape);
      }
      j++;
    }
    i++;
  }
}

void SFMLDisplayManager::displayEntities(std::map<int, std::list<void *>> *entities, int const sizeX, int const sizeY)
{
  std::map<int, std::list<void *>>::iterator it;
  std::list <void *>::iterator it2;
  int i;

  if (entities == NULL)
    return ;
  (void)sizeX;
  (void)sizeY;
  i = 0;
  for (it = entities->begin(); it != entities->end();++it)
  {
    for (it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
    {
      entity *entite = static_cast<entity *>(*it2);
      if (this->_shapeList[(*it).first].rectangleShape != NULL)
      {
        this->_shapeList[(*it).first].rectangleShape->setPosition(entite->x * this->_shapeList[(*it).first]._tileSize, entite->y * this->_shapeList[(*it).first]._tileSize);
        this->_window->draw(*this->_shapeList[(*it).first].rectangleShape);
      }
      else
      {
        this->_shapeList[(*it).first].circleShape->setPosition(entite->x * this->_shapeList[0]._tileSize , entite->y * this->_shapeList[0]._tileSize);
        this->_window->draw(*this->_shapeList[(*it).first].circleShape);
      }
    }
    i++;
  }
}


void SFMLDisplayManager::displayButtons(IGame &game, GroupButton group)
{
  std::list<TextElem>  *buttons;

  buttons = game.getTextElemList();
  for (std::list<TextElem>::iterator it = buttons->begin(); it != buttons->end(); ++it)
  {
    if (group == (*it).group)
      this->displayButton(*it);
  }
}

void SFMLDisplayManager::displayButton(TextElem const &button)
{
  sf::RectangleShape buttonShape(sf::Vector2f(button.sizeX, button.sizeY));
  sf::Text shapeText;
  ConvertColor convert;

  convert.color = button.color;
  buttonShape.setFillColor(sf::Color(convert.colors.r, convert.colors.g, convert.colors.b, convert.colors.t));
  buttonShape.setPosition(button.pos.x , button.pos.y);
  this->_window->draw(buttonShape);
  if (button.text != "")
  {
    shapeText.setCharacterSize(button.FontSize);
    shapeText.setFont(this->_font);
    shapeText.setPosition(button.pos.x, button.pos.y);
    shapeText.setColor(sf::Color::Red);
    shapeText.setString(button.text);
    this->_window->draw(shapeText);
  }
}
