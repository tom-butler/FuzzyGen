#ifndef GUI_H
#define GUI_H
#include <string>
#include "..\objects\controller.h"

static float height = 1.0f;
static int CONT = 0;
static int STATE = 0; //0 init, 1 load, 2 draw, 3 breed
static int result = -1;
void keyboard(unsigned char key, int x, int y);
void display(void);
//draw functions
void drawSim(float y, float thrust);
void drawPlot(float x, float y, std::string text);
void drawSingleton(float x, float y);
void drawCollection(float x, float y, FuzzyVar collection);

#endif