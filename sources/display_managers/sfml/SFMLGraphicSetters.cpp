#include "display_managers/SFMLDisplayManager.hpp"
#include "games/IGame.hpp"
#include "games/Game.hpp"
#include <string>
#include <iostream>
#include "extern/SFML/Audio/Music.hpp"

int SFMLDisplayManager::setShape(IGame &game, int const tileNumber, std::string const &shape, unsigned int const color, unsigned int const size, std::string const &textureName)
{
  ConvertColor convert;
  sf::Texture *texture = new sf::Texture;
  bool textureNotSet = false;

  (void)game;
  if (textureName != "")
  {
    if (!texture->loadFromFile(textureName))
    {
      std::cerr << "Texture " << textureName << "not found" << std::endl;
    }
    else
      textureNotSet = true;
    texture->setSmooth(true);
  }
  convert.color = color;
  if (shape == "square")
  {
    if (this->_shapeList[tileNumber].rectangleShape != NULL)
      delete this->_shapeList[tileNumber].rectangleShape;
    this->_shapeList[tileNumber].rectangleShape = new sf::RectangleShape(sf::Vector2f(static_cast<float>(size), static_cast<float>(size)));
    if (!textureNotSet)
      this->_shapeList[tileNumber].rectangleShape->setFillColor(sf::Color(convert.colors.r, convert.colors.g, convert.colors.b, 255));
    this->_shapeList[tileNumber].rectangleShape->setTexture(texture);
    this->_shapeList[tileNumber].rectangleShape->setTextureRect(sf::IntRect(0, 0, 100, 100));
    this->_shapeList[tileNumber]._tileSize = size;
  }
  if (shape == "circle")
  {
    if (this->_shapeList[tileNumber].circleShape != NULL)
      delete this->_shapeList[tileNumber].circleShape;
    this->_shapeList[tileNumber].circleShape = new sf::CircleShape(size);
    if (!textureNotSet)
      this->_shapeList[tileNumber].circleShape->setFillColor(sf::Color(convert.colors.r, convert.colors.g, convert.colors.b, 255));
    this->_shapeList[tileNumber].circleShape->setTexture(texture);
    this->_shapeList[tileNumber]._tileSize = size;
  }
  return (0);
}

t_data    SFMLDisplayManager::getData()
{
  return this->_data;
}

void      SFMLDisplayManager::loadData(t_data &data)
{
  if (&data == 0)
  {
    std::vector<SFMLShapes>::iterator it;
    for (it = this->_shapeList.begin(); it != this->_shapeList.end(); ++it)
    {
      delete (*it).rectangleShape;
      delete (*it).circleShape;
    }
    this->_shapeList.erase(this->_shapeList.begin(), this->_shapeList.end());
    this->_shapeList.resize(NUMBER_SHAPES);
  }
}
