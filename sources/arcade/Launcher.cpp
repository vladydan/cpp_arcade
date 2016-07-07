#include <dirent.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include "arcade/Launcher.hpp"

Launcher::Launcher(std::string const &libraryPath)
{
  std::string libName;

  libName = libraryPath;
  this->_games = this->readDirectory("games/");
  this->_libs = readDirectory("lib/");
  this->loadScore();
  this->setMenu(*this->_games, *this->_libs);
  if(this->_displayLoader.loadDL(libraryPath, "entryPoint") == 1)
    return;
  this->_displayManager = this->_displayLoader.getInstance();
  libName.erase(0, 4);
  this->_currentLib = libName;
  this->_displayManager->createWindow(WINDOW_LENGHT, WINDOW_HEIGHT, "Arcade");
  this->startMenu();
}

void Launcher::startMenu()
{
  this->_displayManager->startGame(*this, "");
}

Launcher::~Launcher()
{

}

char **Launcher::getMap() const
{
  return NULL;
}

int Launcher::getSizeX() const
{
  return -1;
}

int Launcher::getSizeY() const
{
  return -1;
}

bool compareScore(const std::string& first, const std::string& second)
{
  std::stringstream ss(first);
  std::stringstream ss2(second);
  std::vector<std::string> tokensFirst;
  std::vector<std::string> tokensSecond;
  std::string buf;

  while (ss >> buf)
    tokensFirst.push_back(buf);
  while (ss2 >> buf)
    tokensSecond.push_back(buf);
  if (std::stoi(tokensFirst.back()) < std::stoi(tokensSecond.back()))
    return false;
  return true;
}

void Launcher::loadScore()
{
  std::ifstream infile("ressources/score.txt");
  std::string line;
  while (std::getline(infile, line))
  {
    this->_playerScore.push_back(line);
  }
  this->_playerScore.sort(compareScore);
}

void Launcher::loadNextLib()
{
  for(std::list<std::string>::iterator it = this->_libs->begin(); it != this->_libs->end(); ++it)
  {
    if (*it == this->_currentLib)
    {
      if (++it == this->_libs->end())
        it = this->_libs->begin();
      this->_displayManager->closeWindow();
      delete this->_displayManager;
      this->_displayLoader.loadDL("lib/" + *it, "entryPoint");
      this->_displayManager = this->_displayLoader.getInstance();
      this->_displayManager->createWindow(WINDOW_LENGHT, WINDOW_HEIGHT, "Arcade");
      this->_currentLib = *it;
      return;
    }
  }
}

void Launcher::loadBackLib()
{
  std::list<std::string>::iterator it = this->_libs->begin();
  std::advance(it, this->_libs->size() - 1);
  while(it != this->_libs->begin())
  {
    if (*it == this->_currentLib)
    {
      if (--it == this->_libs->begin())
      {
        it = this->_libs->end();
        it--;
      }
      this->_displayManager->closeWindow();
      delete this->_displayManager;
      this->_displayLoader.loadDL("lib/" + *it, "entryPoint");
      this->_displayManager = this->_displayLoader.getInstance();
      this->_currentLib = *it;
      this->_displayManager->createWindow(WINDOW_LENGHT, WINDOW_HEIGHT, "Arcade");
      return;
    }
    --it;
  }
  std::advance(it, this->_libs->size() - 1);
  this->_displayManager->closeWindow();
  delete this->_displayManager;
  this->_displayLoader.loadDL("lib/" + *it, "entryPoint");
  this->_displayManager = this->_displayLoader.getInstance();
  this->_currentLib = *it;
  this->_displayManager->createWindow(WINDOW_LENGHT, WINDOW_HEIGHT, "Arcade");
}

int Launcher::play(char, int)
{
  std::string menuRet;
  IGame *game;
  std::string buf;
  t_data &nullReference = *(t_data*)0;
  std::string ret;
  std::string currentGame;

  menuRet = this->_displayManager->displayMenu(*this, MENUBTN);
  if (menuRet == "exit")
  {
    this->_displayManager->closeWindow();
    delete this->_displayManager;
    exit (0);
  }
  else
  {
    std::stringstream ss(menuRet);
    std::vector<std::string> tokens;
    while (ss >> buf)
      tokens.push_back(buf);
    tokens.push_back("Unnamed_Player");
    if (tokens[1] == "3")
    {
      this->loadNextLib();
      return this->play(0, 0);
    }
    else if (tokens[1] == "2")
    {
      this->loadBackLib();
      return this->play(0, 0);
    }
    currentGame = *tokens.begin();
    if (this->_gameLoader.loadDL("games/" + currentGame, "entryPoint") != 0)
      return play(0, 0);
    game = this->_gameLoader.getInstance();
    while ((ret = game->startGame(*this->_displayManager, tokens[1])) != "exit")
      {
        if (ret == "4")
          this->loadBackGame(currentGame);
        if (ret == "5")
          this->loadNextGame(currentGame);
        if (ret == "3")
          this->loadNextLib();
        else if (ret == "2")
          this->loadBackLib();
        else
          game = this->_gameLoader.getInstance();
        this->_displayManager->loadData(nullReference);
      }
  }
  return this->play(0, 0);
}

std::string Launcher::startGame(IDisplayManager &displayManager,  std::string const &playerName)
{
  (void)displayManager;
  (void)playerName;
  return "";
}

std::map<int, std::list<void *>> *Launcher::getEntities()
{
  return (NULL);
}

std::list<TextElem>  *Launcher::getTextElemList()
{
  return this->_buttonList;
}

std::list<std::string> *Launcher::readDirectory(std::string const &directoryName) const
{
  auto dir = opendir(directoryName.c_str());
  std::list<std::string> *fileList = new std::list<std::string>;
  std::string noLib = "libmyopengl.so";
  if (dir == NULL)
  {
    std::cerr << "could not open directory: " << directoryName.c_str() << std::endl;
    return NULL;
  }
  auto entity = readdir(dir);
  while (entity != NULL)
  {
    if (entity->d_type == DT_REG && entity->d_name[0] != '.' && entity->d_name != noLib)
      fileList->push_back(entity->d_name);
    entity = readdir(dir);
  }
  closedir(dir);
  return fileList;
}

void Launcher::loadNextGame(std::string &currentGame)
{
  for(std::list<std::string>::iterator it = this->_games->begin(); it != this->_games->end(); ++it)
  {
    if (*it == currentGame)
    {
      if (++it == this->_games->end())
        it = this->_games->begin();
      this->_gameLoader.loadDL("games/" + *it, "entryPoint");
      currentGame = *it;
      return;
    }
  }
}

void Launcher::loadBackGame(std::string &currentGame)
{
  std::list<std::string>::iterator it = this->_games->begin();
  std::advance(it, this->_games->size() - 1);
  while(it != this->_games->begin())
  {
    if (*it == currentGame)
    {
      if (--it == this->_games->begin())
      {
        this->_gameLoader.loadDL("games/" + this->_games->back(), "entryPoint");
      }
      else
        this->_gameLoader.loadDL("games/" + *it, "entryPoint");
      currentGame = *it;
      return;
    }
    --it;
  }
  std::advance(it, this->_games->size() - 1);
  currentGame = *it;
  this->_gameLoader.loadDL("games/" + *it, "entryPoint");
}

void Launcher::setMenu(std::list<std::string> gameList, std::list<std::string> libList)
{
  TextElem buttons[40];
  this->_buttonList = new std::list<TextElem>;
  int i;
  size_t j;

  (void)libList;
  buttons[0].pos.x = WINDOW_HEIGHT / 2;
  buttons[0].pos.y = 50;
  buttons[0].sizeX = 200;
  buttons[0].sizeY = 50;
  buttons[0].text = "Arcade";
  buttons[0].fontName = "ressources/font.ttf";
  buttons[0].FontSize = 60;
  buttons[0].color = 0;
  buttons[0].isButton = false;
  buttons[0].group = MENUBTN;
  buttons[0].value = "";


  buttons[1].pos.x = 100;
  buttons[1].pos.y = 150;
  buttons[1].sizeX = 300;
  buttons[1].sizeY = 50;
  buttons[1].text = "Type your name : ";
  buttons[1].fontName = "ressources/font.ttf";
  buttons[1].FontSize = 30;
  buttons[1].color = 0;
  buttons[1].isButton = false;
  buttons[1].group = MENUBTN;
  buttons[1].value = "nameType";


  buttons[2].pos.x = 350;
  buttons[2].pos.y = 150;
  buttons[2].sizeX = 400;
  buttons[2].sizeY = 50;
  buttons[2].text = "";
  buttons[2].fontName = "ressources/font.ttf";
  buttons[2].FontSize = 30;
  buttons[2].color = 0x556770F4;
  buttons[2].isButton = false;
  buttons[2].group = MENUBTN;
  buttons[2].value = "name";


  buttons[3].pos.x = 450;
  buttons[3].pos.y = 200;
  buttons[3].sizeX = 5;
  buttons[3].sizeY = 1000;
  buttons[3].text = "";
  buttons[3].fontName = "ressources/font.ttf";
  buttons[3].FontSize = 0;
  buttons[3].color = 0xFF6770F4;
  buttons[3].isButton = false;
  buttons[3].group = MENUBTN;
  buttons[3].value = "";

  buttons[4].pos.x = 150;
  buttons[4].pos.y = 250;
  buttons[4].sizeX = 200;
  buttons[4].sizeY = 50;
  buttons[4].text = "Choose your game : ";
  buttons[4].fontName = "ressources/font.ttf";
  buttons[4].FontSize = 30;
  buttons[4].color = 0;
  buttons[4].isButton = false;
  buttons[4].group = MENUBTN;
  buttons[4].value = "chose";

  i = 5;
  auto it = gameList.begin();
  for (j = 0; j < gameList.size(); ++j, ++it)
  {
    buttons[i + j].pos.x = 150;
    buttons[i + j].pos.y += buttons[i + j - 1].pos.y + 100;
    buttons[i + j].sizeX = 250;
    buttons[i + j].sizeY = 50;
    buttons[i + j].text = *it;
    buttons[i + j].fontName = "ressources/font.ttf";
    buttons[i + j].FontSize = 30;
    buttons[i + j].color = 0x556770F4;
    buttons[i + j].isButton = true;
    buttons[i + j].group = MENUBTN;
    buttons[i + j].value = *it;
  }
  i += j;
  it = this->_playerScore.begin();
  int incr = 0;
  for (j = 0; j < 10; ++j, it++)
  {
    buttons[i + j].pos.x = 500;
    buttons[i + j].pos.y = 250 + incr;
    buttons[i + j].sizeX = 250;
    buttons[i + j].sizeY = 25;
    buttons[i + j].text = *it;
    buttons[i + j].fontName = "ressources/font.ttf";
    buttons[i + j].FontSize = 20;
    buttons[i + j].color = 0;
    buttons[i + j].isButton = false;
    buttons[i + j].group = MENUBTN;
    buttons[i + j].value = *it;
    incr += 50;
  }
  i += j;
  for(int e  = 0; e < i; e++)
    this->_buttonList->push_front(buttons[e]);
}
