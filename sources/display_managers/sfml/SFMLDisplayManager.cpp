#include "display_managers/SFMLDisplayManager.hpp"
#include "games/IGame.hpp"
#include "games/Game.hpp"
#include <iostream>
#include <string>
#include "extern/SFML/Audio/Music.hpp"

SFMLDisplayManager::SFMLDisplayManager()
{
  if (!this->_font.loadFromFile("ressources/font.ttf"))
  {
    std::cerr << "Cannot load font.ttf" << std::endl;
  }
  this->_shapeList.resize(NUMBER_SHAPES);
  this->_playerName = "";
}

SFMLDisplayManager::~SFMLDisplayManager()
{
  this->_window->close();
  delete this->_window;
}

void SFMLDisplayManager::replay(IGame &game, GameInfos &info)
{
  sf::Event event;
  std::list<Button>::const_iterator it;
  std::string ret;

  while (this->_window->isOpen())
  {
    while (this->_window->pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        this->_window->close();
      this->getUserInput(event, info.playerName);
    }
    if ((ret = this->checkMouseInput(game)) != "")
    {
      info.buttonPressed = ret;
      return ;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
      info.buttonPressed = "escape";
      return ;
    }
    this->_window->clear(sf::Color(0xFF, 0xFF, 0xFF));
    this->displayMap(game.getMap(), game.getSizeX(), game.getSizeY());
    this->displayEntities(game.getEntities(), game.getSizeX(), game.getSizeY());
    this->displayButtons(game, ENDGAME);
    this->_window->display();
  }
}

std::string SFMLDisplayManager::startGame(IGame &game, std::string const &musicName)
{
  sf::Event event;
  sf::Music music;
  sf::Clock frameClock;
  std::string retInfos;
  std::string scoreRet;
  char gameEvent = 0;
  int ret;

  (void)game;
  if (musicName != "")
  {
    if (music.openFromFile(musicName))
    {
      music.setVolume(50);
      music.play();
    }
  }
  while (this->_window->isOpen())
  {
    while (this->_window->pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        this->_window->close();
      if (event.type == sf::Event::TextEntered && gameEvent == 0)
      {
        gameEvent = event.text.unicode;
        if (gameEvent > 48 && gameEvent < 55)
        {
          retInfos += gameEvent;
          return retInfos;
        }
      }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
      {
        this->_window->close();
        exit(0);
      }
    ret = game.play(gameEvent, frameClock.getElapsedTime().asMilliseconds());
    if (ret == -1)
      return "";
    else if (ret != -2)
    {
      gameEvent = 0;
    }
    this->_window->clear(sf::Color(0xFF, 0xFF, 0xFF));
    this->displayMap(game.getMap(), game.getSizeX(), game.getSizeY());
    this->displayEntities(game.getEntities(), game.getSizeX(), game.getSizeY());
    this->displayMenu(game, INGAME);
    this->_window->display();
  }
  return "";
}

extern "C"
IDisplayManager *entryPoint()
{
  return new SFMLDisplayManager();
}
