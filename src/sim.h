#ifndef SIM_H
#define SIM_H

#include "controller.h"

//constants
static Singleton thrust = {0, 10, 0};
static FuzzyVar height = {0, 1000, 1000, 0};
static FuzzyVar velocity = {0, 50, 0, 0};
static FuzzyVar fuelRemaining = {0, 1000, 0, 0};

static FuzzyVar simInput[] = {height, velocity};
static Singleton * simOutput = &thrust;
static FuzzyVar * simFitness = &fuelRemaining;


void InitSim();
float *CheckSimValues();
int NextStep(float newThrust);
#endif