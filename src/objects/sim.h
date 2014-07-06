#ifndef SIM_H
#define SIM_H

#include "controller.h"

//sim init vars
static Accumulator thrustSet = {-THRUST_MAX, THRUST_MAX, 0, 0 , 0};
static FuzzyVar heightSet = {0, START_HEIGHT, START_HEIGHT, 0};
static FuzzyVar velocitySet = {TERMINAL_VELOCITY, TERMINAL_VELOCITY, START_VEL, 0};
static FuzzyVar fuelSet = {0, START_FUEL, START_FUEL, 0};

static FuzzyVar simInput[] = {heightSet, velocitySet};
static Accumulator * simOutput = &thrustSet;
static FuzzyVar * simFitness = &fuelSet;

//sim runtime vars
static float thrust *;
static float height *;
static float velocity *;
static float fuel *;

void InitSim();
int NextStep(float newThrust);
#endif