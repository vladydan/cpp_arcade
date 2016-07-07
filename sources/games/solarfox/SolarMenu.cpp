#include "games/Solar.hpp"
#include "display_managers/IDisplayManager.hpp"

void    Solar::setMenu()
{
  TextElem buttons[40];
  this->_buttonList = new std::list<TextElem>;

 buttons[0].pos.x = 300;
  buttons[0].pos.y = WINDOW_HEIGHT / 3.5;
  buttons[0].sizeX = 200;
  buttons[0].sizeY = 50;
  buttons[0].text = "Game Over";
  buttons[0].fontName = "ressources/font.ttf";
  buttons[0].FontSize = 30;
  buttons[0].color = 0;
  buttons[0].isButton = false;
  buttons[0].group = ENDGAME;
  buttons[0].value = "";

  buttons[1].pos.x = 300;
  buttons[1].pos.y = WINDOW_HEIGHT / 2.8;
  buttons[1].sizeX = 200;
  buttons[1].sizeY = 50;
  buttons[1].text = "Score : 0";
  buttons[1].fontName = "ressources/font.ttf";
  buttons[1].FontSize = 30;
  buttons[1].color = 0;
  buttons[1].isButton = false;
  buttons[1].group = ENDGAME;
  buttons[1].value = "score";

  //big black box
  buttons[2].pos.x = 200;
  buttons[2].pos.y = 200;
  buttons[2].sizeX = 500;
  buttons[2].sizeY = 500;
  buttons[2].text = "";
  buttons[2].fontName = "ressources/font.ttf";
  buttons[2].FontSize = 30;
  buttons[2].color = 0x34000000;
  buttons[2].isButton = false;
  buttons[2].group = ENDGAME;
  buttons[2].value = "";

  buttons[3].pos.x = 300;
  buttons[3].pos.y = 450;
  buttons[3].sizeX = 350;
  buttons[3].sizeY = 50;
  buttons[3].text = "Play Again";
  buttons[3].fontName = "ressources/font.ttf";
  buttons[3].FontSize = 30;
  buttons[3].color = 0x556770F4;
  buttons[3].isButton = true;
  buttons[3].group = ENDGAME;
  buttons[3].value = "replay";

  buttons[4].pos.x = 300;
  buttons[4].pos.y = 520;
  buttons[4].sizeX = 350;
  buttons[4].sizeY = 50;
  buttons[4].text = "Back to the menu";
  buttons[4].fontName = "ressources/font.ttf";
  buttons[4].FontSize = 30;
  buttons[4].color = 0x556770F4;
  buttons[4].isButton = true;
  buttons[4].group = ENDGAME;
  buttons[4].value = "exit";

  buttons[5].pos.x = 10;
  buttons[5].pos.y = 10;
  buttons[5].sizeX = 200;
  buttons[5].sizeY = 50;
  buttons[5].text = "Score : 0";
  buttons[5].fontName = "ressources/font.ttf";
  buttons[5].FontSize = 30;
  buttons[5].color = 0xF0F5F0F0;
  buttons[5].isButton = false;
  buttons[5].group = INGAME;
  buttons[5].value = "score";

  buttons[6].pos.x = 400;
  buttons[6].pos.y = 400;
  buttons[6].sizeX = 200;
  buttons[6].sizeY = 50;
  buttons[6].text = "Play";
  buttons[6].fontName = "ressources/font.ttf";
  buttons[6].FontSize = 30;
  buttons[6].color = 0x556770F4;
  buttons[6].isButton = true;
  buttons[6].group = MENUBTN;
  buttons[6].value = "play";

  buttons[7].pos.x = 400;
  buttons[7].pos.y = 500;
  buttons[7].sizeX = 200;
  buttons[7].sizeY = 50;
  buttons[7].text = "Exit";
  buttons[7].fontName = "ressources/font.ttf";
  buttons[7].FontSize = 30;
  buttons[7].color = 0x556770F4;
  buttons[7].isButton = true;
  buttons[7].group = MENUBTN;
  buttons[7].value = "exit";

 for(int i  = 0; i < 8 ; i++)
  this->_buttonList->push_front(buttons[i]);
}
