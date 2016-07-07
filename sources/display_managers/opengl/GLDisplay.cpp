#include "display_managers/GLDisplayManager.hpp"
#include "extern/GL/freeglut.h"
#include "games/IGame.hpp"
#include <cmath>
#include <iostream>

void   RenderMenu();
void RenderFunc();
extern int displayFlag;
std::string &getButtonValue(std::list<TextElem>  *buttons, int &flag);

void displayText(std::string const& text, float x, float y, int flag)
{
  int j = text.size();

  if (flag == 1)
    glColor3f(0.2, 0.5, 0.7);
  else
    glColor3f(1, 1, 1);

  x = x / (float)900;
  y = y / (float)900;
  x -= 0.5;
  y -= 0.5;
  y *= -1;
  glRasterPos2f(x, y);
  for(int i = 0; i < j; i++)
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
}

void setColor(int i, const std::vector<GLShapes*>& _shapeList, GLfloat *color)
{
  color[0] = _shapeList[i]->color.r / 255.0;
  color[1] = _shapeList[i]->color.g / 255.0;
  color[2] = _shapeList[i]->color.b / 255.0;
  glColor3fv(color);
}

void drawSquare(int sizeX, int sizeY, int i, int j)
{
  glBegin(GL_QUADS);
  glVertex2f((1.0 - ((static_cast<float>(2.0 / sizeX)) * j)) * -1,
	     (1.0 - ((static_cast<float>(2.0 / sizeY)) * (i + 1))));
  glVertex2f((1.0 - ((static_cast<float>(2.0 / sizeX)) * (j + 1))) * -1,
	     (1.0 - ((static_cast<float>(2.0 / sizeY)) * (i + 1))));
  glVertex2f((1.0 - ((static_cast<float>(2.0 / sizeX)) * (j + 1))) * -1,
	     (1.0 - ((static_cast<float>(2.0 / sizeY)) * i)));
  glVertex2f((1.0 - ((static_cast<float>(2.0 / sizeX)) * j)) * -1,
	     (1.0 - ((static_cast<float>(2.0 / sizeY)) * i)));
  glEnd();
}

void drawCircle(int sizeX, int sizeY, int i, int j)
{
  int k;
  double angle1 = 0.0;

  glBegin(GL_POLYGON);
  glVertex2d((0.05 * cos(0.0) +
	      (1.0 - ((static_cast<float>(2.0 / sizeX)) * j) -
	       ((static_cast<float>(2.0 / sizeX)) / 2.0))) * -1,
	     (0.05 * sin(0.0) +
	      (1.0 - ((static_cast<float>(2.0 / sizeY)) * i) -
	       ((static_cast<float>(2.0 / sizeY)) / 2.0))));
  for (k = 0; k < 100; k++)
    {
      glVertex2d((0.05 * cos(angle1) +
		  (1.0 - ((static_cast<float>(2.0 / sizeX)) * j) -
		   ((static_cast<float>(2.0 / sizeX)) / 2.0))) * -1,
		 (0.05 * sin(angle1) +
		  (1.0 - ((static_cast<float>(2.0 / sizeY)) * i) -
		   ((static_cast<float>(2.0 / sizeY)) / 2.0))));
      angle1 += 2.0f * 3.1416f / 100;
    }
  glEnd();
}

const std::string GLDisplayManager::displayMenu(IGame &game, GroupButton grp)
{
  std::string gameName;

  this->_group = grp;
  this->_game = &game;
  glutDisplayFunc(RenderMenu);
  glutIdleFunc(RenderMenu);
  try
  {
    glutMainLoop();
  }
  catch (const std::string msg)
  {
    glutDisplayFunc(RenderFunc);
    glutIdleFunc(RenderFunc);
    return msg;
  }
  return gameName;
}

void GLDisplayManager::displayMap(char **map, int const sizeX, int const sizeY)
{
  GLfloat color[3];
  int i;
  int j;

  i = 0;
  while (i < sizeY)
    {
      j = 0;
      while (j < sizeX)
	{
	  setColor(0, this->_shapeList, color);
	  drawSquare(sizeX, sizeY, i, j);
	  setColor(map[i][j], this->_shapeList, color);
	  if (this->_shapeList[map[i][j]]->name == "circle")
	    drawCircle(sizeX, sizeY, i, j);
	  else
	    drawSquare(sizeX, sizeY, i, j);
	  j++;
	}
      i++;
    }
}

void GLDisplayManager::displayEntities(std::map<int, std::list<void *>> *entities, int const sizeX, int const sizeY)
{
  GLfloat color[3];
  std::map<int, std::list<void *>>::iterator it;
  std::list <void *>::iterator it2;
  int i;

  i = 0;
  if (entities == NULL)
    return ;
  for (it = entities->begin(); it != entities->end(); ++it)
    {
      for (it2 = (*it).second.begin(); it2 != (*it).second.end(); it2++)
	{
	  auto *entity = static_cast<Part *>(*it2);
	  setColor((*it).first, this->_shapeList, color);
	  if (this->_shapeList[(*it).first]->name == "circle")
	    drawCircle(sizeX, sizeY, entity->y, entity->x);
	  else
	    drawSquare(sizeX, sizeY, entity->y, entity->x);
	}
      i++;
    }
}
