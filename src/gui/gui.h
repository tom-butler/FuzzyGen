#ifndef GUI_H
#define GUI_H

#include <string>
#include <vector>

//Forward Declarations
#include "..\shared\shared.h"

typedef unsigned int  GLuint;

extern int speed;
extern int controller;
extern int generation;

void DrawPlot(float x, float y, float width);
void DrawAccumulator(float x, float y, std::string name, Accumulator output);
void DrawCollection(float x, float y, float width, std::string name, FuzzyVar collection);
void DrawBestCollection(float x, float y, float width, std::string name, FuzzyVar collection);
void PrintFloat(float x, float y, std::string name, float value);
void DrawRules(float x, float y, int controller, int accumulator);
float ConvertToSimScale(float var, float min, float max);
void DrawCircle(float x, float y, float radius);
void DrawSetValues(int input, float x, float y);
#endif