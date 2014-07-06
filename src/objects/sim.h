#ifndef SIM_H
#define SIM_H

#include "controller.h"

//constants
static Accumulator thrust = {-THRUST_MAX, THRUST_MAX, 0, 0 , 0};
static FuzzyVar height = {0, START_HEIGHT, START_HEIGHT, 0};
static FuzzyVar velocity = {TERMINAL_VELOCITY, TERMINAL_VELOCITY, START_VEL, 0};
static FuzzyVar fuelRemaining = {0, START_FUEL, START_FUEL, 0};

static FuzzyVar simInput[] = {height, velocity};
static Accumulator * simOutput = &thrust;
static FuzzyVar * simFitness = &fuelRemaining;


void InitSim();
float GetInputValue(int i);
int NextStep(float newThrust);
#endif