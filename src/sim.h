#ifndef SIM_H
#define SIM_H

#include "controller.h"

//constants
static FuzzyVar thrust = {0, 10, 0, 0};
static FuzzyVar height = {0, 1000, 1000, 0};
static FuzzyVar velocity = {0, 50, 0, 0};
static FuzzyVar fuelRemaining = {0, 1000, 0 , 4};

void InitSim();
int NextStep();
#endif