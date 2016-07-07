#ifndef LAUNCHER_HPP__
#define LAUNCHER_HPP__

#include "games/IGame.hpp"
#include "display_managers/IDisplayManager.hpp"
#include "arcade/DLLoader.hpp"
#include <string>
#include <list>

class Launcher : public IGame
{
  public:
    Launcher(std::string const&);
    ~Launcher();
    virtual int play(char, int);
    virtual char **getMap() const;
    virtual int getSizeX() const;
    virtual int getSizeY() const;
    virtual std::string startGame(IDisplayManager &, std::string const &playerBame);
    virtual std::list<TextElem>  *getTextElemList();
    virtual std::map<int, std::list<void *>> *getEntities();
    std::list<std::string> *readDirectory(std::string const &directoryName) const;
    void setMenu(std::list<std::string> gameList, std::list<std::string> libList);
    void loadNextLib();
    void startMenu();
    void loadBackLib();
    void loadScore();
    void loadNextGame(std::string &currentGame);
    void loadBackGame(std::string &currentGame);
  private:
    std::list<std::string> *_games;
    std::list<std::string> *_libs;
    DLLoader<IDisplayManager> _displayLoader;
    DLLoader<IGame> _gameLoader;
    IDisplayManager *_displayManager;
    std::list<TextElem> *_buttonList;
    std::string _currentGame;
    std::string _currentLib;
    std::list<std::string> _playerScore;
};

#endif
