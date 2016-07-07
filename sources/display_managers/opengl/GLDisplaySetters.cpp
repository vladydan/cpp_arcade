#include "display_managers/GLDisplayManager.hpp"
#include "games/IGame.hpp"



void GLDisplayManager::setWlength(size_t length)
{
  this->_wlength = length;
}

void GLDisplayManager::setWheigth(size_t height)
{
  this->_wheight = height;
}

void GLDisplayManager::setButton(TextElem const &button)
{
  (void)button;
}

int GLDisplayManager::setShape(IGame &game, int const tileNumber, std::string const &shape, unsigned int const color, unsigned int const size ,  std::string const &textureName)
{
  ConvertColor convert;

  (void)textureName;
  (void)game;
  convert.color = color;
  if (this->_shapeList[tileNumber] != NULL)
    delete this->_shapeList[tileNumber];
  this->_shapeList[tileNumber] = new GLShapes();
  this->_shapeList[tileNumber]->name = shape;
  this->_shapeList[tileNumber]->size = size;
  this->_shapeList[tileNumber]->color = convert.colors;
  return (0);
}

bool	GLDisplayManager::getMenu() const
{
  return (this->_menu);
}

void	GLDisplayManager::setMenu(bool menu)
{
  this->_menu = menu;
}

int	GLDisplayManager::getScore() const
{
  return (this->_infos->score);
}

void	GLDisplayManager::setScore(int score)
{
  this->_infos->score = score;
}

const std::string& GLDisplayManager::getName() const
{
  return (this->_infos->playerName);
}

void	GLDisplayManager::setName(const std::string& name)
{
  this->_infos->playerName = name;
}

char	GLDisplayManager::getKey() const
{
  return (this->_key);
}

void	GLDisplayManager::setKey(char key)
{
  this->_key = key;
}

IGame* GLDisplayManager::getGame() const
{
  return (this->_game);
}

GroupButton GLDisplayManager::getGroup() const
{
  return (this->_group);
}

void GLDisplayManager::appendName(char c)
{
  this->_infos->playerName += c;
}

void GLDisplayManager::removeName()
{
  this->_infos->playerName = this->_infos->playerName.substr(0, this->_infos->playerName.size() - 1);
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
