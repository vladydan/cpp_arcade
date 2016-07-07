#include "display_managers/GLDisplayManager.hpp"
#include "extern/GL/freeglut.h"
#include "games/IGame.hpp"
#include <iostream>
#include <chrono>
#include <unistd.h>

static GLDisplayManager *g_instance;
int displayFlag;
bool leave;
bool leaveChangeKey;
std::string leaveKey;

static std::chrono::time_point<std::chrono::high_resolution_clock>  startTime;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

void displayText(std::string const& text, float x, float y, int flag);
void findBackButton(std::list<TextElem>  *buttons, int &flag, GroupButton group);
void findNextButton(std::list<TextElem>  *buttons, int &flag, GroupButton group);
std::string &getButtonValue(std::list<TextElem>  *buttons, int &flag);

void   RenderFunc()
{
  int	ret;
  std::string retString;
  auto currentTime = Time::now();

  glClear(GL_COLOR_BUFFER_BIT);
  fsec fs = currentTime - startTime;
  ret = g_instance->getGame()->play(g_instance->getKey(), std::chrono::duration_cast<ms>(fs).count());
  if (ret == -1)
  {
    throw retString;
  }
  else if (ret != -2)
    {
      g_instance->setKey(0);
    }
  g_instance->displayMap(g_instance->getGame()->getMap(),
  g_instance->getGame()->getSizeX(), g_instance->getGame()->getSizeY());
  g_instance->displayEntities(g_instance->getGame()->getEntities(),
			      g_instance->getGame()->getSizeX(), g_instance->getGame()->getSizeY());
  if (leaveChangeKey)
  {
    leaveChangeKey = false;
    throw leaveKey;
  }
  usleep(20000);
  leave = false;
  glFlush();
}

void   RenderMenu()
{
  std::list<TextElem>  *buttons;
  buttons = g_instance->getGame()->getTextElemList();
  int i;
  static bool first;

  GroupButton group = g_instance->getGroup();
  if (!first)
  {
    findBackButton(buttons, displayFlag, group);
    first = true;
  }
  glClear(GL_COLOR_BUFFER_BIT);
  i = 0;
  for (std::list<TextElem>::iterator it = buttons->begin(); it != buttons->end(); ++it, ++i)
  {
    if (group == (*it).group)
    {
      if (i == displayFlag)
        displayText((*it).text, (*it).pos.x, (*it).pos.y, 1);
      else
        displayText((*it).text, (*it).pos.x, (*it).pos.y, 0);
    }
  }
  if (leave)
  {
    leave = false;
    g_instance->setKey(0);
    throw getButtonValue(buttons, displayFlag);
  }
  if (leaveChangeKey)
  {
    leaveChangeKey = false;
    throw "noGame " + leaveKey;
  }
  glFlush();
}

void InputFunc(unsigned char key, int x, int y)
{
  (void)x;
  (void)y;
  if (g_instance->getMenu())
    {
      if (((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z')) &&
	  g_instance->getName().size() <= 10)
	{
	  g_instance->appendName(key);
	  return ;
	}
  else if (key == 8)
	{
	  g_instance->removeName();
	  return ;
	}
    }
  g_instance->setKey(key);
  if (key == 13)
  {
    leave = true;
//    glutLeaveMainLoop();
   }
   // if (key > '1' && key < '6')
   // {
   //  leaveKey = "";
   //  leaveKey += key;
   //  leaveChangeKey = true;
   // }
  if (key == 27)
    {
      g_instance->closeWindow();
    }
  else if (key == 32)
    {
      // REPLAY
    }
}

void ReshapeFunc(int width, int height)
{
  if (height == 0)
    height = 1;
  GLfloat aspect = (GLfloat)width / (GLfloat)height;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (width >= height)
    gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
  else
    gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
  g_instance->setWheigth(height);
  g_instance->setWlength(width);
}

GLDisplayManager::GLDisplayManager()
{
  char	**argv = { NULL };
  int	argc;

  this->_wlength = 1;
  this->_wheight = 1;
  this->_window = 0;
  this->_key = 0;
  this->_menu = false;
  this->_game = NULL;
  this->_infos = NULL;
  this->_shapeList.resize(NUMBER_SHAPES);
  g_instance = this;
  argc = 0;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
}

GLDisplayManager::~GLDisplayManager()
{
}

void SpecialInput(int key, int x, int y)
{
  GroupButton group = g_instance->getGroup();
  std::list<TextElem>  *buttons;
  buttons = g_instance->getGame()->getTextElemList();

  (void)x;
  (void)y;
  if (key == GLUT_KEY_UP)
  {
    findNextButton(buttons, displayFlag, group);
  }
  if (key == GLUT_KEY_DOWN)
  {
    findBackButton(buttons, displayFlag, group);
  }
}

int GLDisplayManager::createWindow(size_t const & length, size_t const &height, std::string const &name)
{
  this->_wlength = length;
  this->_wheight = height;
  glutInitWindowSize(length, height);
  glutInitWindowPosition(0, 0);
  this->_window = glutCreateWindow(name.c_str());
  glutDisplayFunc(RenderFunc);
  glutIdleFunc(RenderFunc);
  glutKeyboardFunc(InputFunc);
  glutSpecialFunc(SpecialInput);
  glutReshapeFunc(ReshapeFunc);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  return (this->_window);
}

std::string GLDisplayManager::startGame(IGame &game, std::string const &musicName)
{
  std::string retInfos;

  this->_key = 0;
  (void)musicName;
  this->_game = &game;
  startTime = Time::now();
  try
  {
    glutMainLoop();
  }
  catch(std::string str)
  {
    retInfos = str;
  }
  return (retInfos);
}

void GLDisplayManager::closeWindow()
{
  glutLeaveMainLoop();
  glutDestroyWindow(glutGetWindow());
}

void GLDisplayManager::replay(IGame &game, GameInfos &info)
{
  glutLeaveMainLoop();
  this->_game = &game;
  this->_infos = &info;
  this->setMenu(false);
  this->setKey(0);
  glutMainLoop();
}

extern "C"
IDisplayManager *entryPoint()
{
  return new GLDisplayManager();
}
