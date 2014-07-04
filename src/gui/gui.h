#ifndef GUI_H
#define GUI_H
#include <string>
#include "..\objects\controller.h"


void keyboard(unsigned char key, int x, int y);
void display(void);
//draw functions
void drawSim(int y, int thrust);m
void drawPlot(float x, float y, std::string text);
void drawSingleton(float x, float y);
void drawCollection(float x, float y, FuzzyVar collection);

#endif