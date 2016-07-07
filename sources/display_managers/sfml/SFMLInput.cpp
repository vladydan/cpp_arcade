#include "display_managers/SFMLDisplayManager.hpp"
#include "games/IGame.hpp"
#include "games/Game.hpp"
#include <string>
#include <iostream>
#include "extern/SFML/Audio/Music.hpp"

std::string SFMLDisplayManager::checkButtonPosition(sf::Vector2i mousePosition, IGame &game) const
{
  std::list<Button>::const_iterator it;
  std::list<TextElem>  *buttons;

  buttons = game.getTextElemList();
  for (std::list<TextElem>::iterator it = buttons->begin(); it != buttons->end(); ++it)
  {
    if (mousePosition.x >= it->pos.x && mousePosition.x <= it->pos.x + it->sizeX && mousePosition.y >= it->pos.y && mousePosition.y <= it->pos.y + it->sizeY && it->isButton)
    {
      std::cout << (*it).value << std::endl;
      return (*it).value;
    }
  }
  return "";
}

std::string SFMLDisplayManager::checkMouseInput(IGame &game) const
{
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->_window);
    return this->checkButtonPosition(mousePosition, game);
  }
  return "";
}

void SFMLDisplayManager::getUserInput(sf::Event event, std::string &userInput)
{
  if (event.type == sf::Event::TextEntered)
  {
    if (event.text.unicode == 8) // delete character
      userInput = userInput.substr(0, userInput.size() - 1);
    else if (userInput.size() < 15)
      userInput += event.text.unicode;
  }
}
