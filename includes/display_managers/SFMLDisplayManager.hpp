#ifndef SFMLDISPLAY_MANAGER_HPP
#define SFMLDISPLAY_MANAGER_HPP

#include "IDisplayManager.hpp"
#include "extern/SFML/Graphics.hpp"
#include <list>

struct SFMLShapes
{
  sf::RectangleShape *rectangleShape;
  sf::CircleShape *circleShape;
  float _tileSize;
};

#define NUMBER_SHAPES 30

struct  Colors
{
  unsigned char b;
  unsigned char g;
  unsigned char r;
  unsigned char t;
};

union ConvertColor
{
  unsigned int color;
  Colors       colors;
};

struct Button
{
  sf::RectangleShape *shape;
  sf::Text *gameText;
  std::string value;
  GroupButton group;
};

class SFMLDisplayManager : public IDisplayManager
{
  public:
    SFMLDisplayManager();
    virtual ~SFMLDisplayManager();
    virtual int createWindow(size_t const & length, size_t const &height, std::string const &);
    virtual int setShape(IGame &game, int const tileType, std::string const &shape, unsigned int const color, unsigned int const size = 0,  std::string const &textureName = "");
    virtual std::string startGame(IGame &game, std::string const &musicName = "");
    virtual void displayMap(char **map, int const sizeX, int const sizeY);
    virtual void displayEntities(std::map<int, std::list<void *>> *entities, int const sizeX, int const sizeY);
    std::string checkButtonPosition(sf::Vector2i mousePosition, IGame &game) const;
    virtual const std::string displayMenu(IGame &game, GroupButton group);
    void getUserInput(sf::Event event, std::string &userInput);
    virtual void replay(IGame &game, GameInfos &info);
    virtual void closeWindow();
    virtual t_data getData();
    virtual void loadData(t_data &data);
    void displayButtons(IGame &game, GroupButton group);
    void displayButton(TextElem const &button);
    std::string checkMouseInput(IGame &game) const;
  private:
    sf::RenderWindow        *_window;
    std::vector<SFMLShapes> _shapeList;
    sf::Font                _font;
    size_t                 _windowLength;
    size_t                  _windowHeight;
    t_data                  _data;
    std::string             _playerName;
};

#endif
