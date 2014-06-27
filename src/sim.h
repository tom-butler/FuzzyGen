#ifndef SIM_H
#define SIM_H

//constants
const int START_FUEL = 1000;
const int THRUST_MAX = 50;
const int TERMINAL_VELOCITY = 50;
const int FORCE = 5;
const int CRASH_SPEED = 5;

static FuzzyVar thrust = {0, 10, 0, 0};
static FuzzyVar height = {0, 1000, 1000, 0};
static FuzzyVar velocity = {0, 50, 0, 0};
static FuzzyVar fuelRemaining = {0, 1000, 0 , 4};

initSim(int height, int vel, int fuel);
int nextStep();
#endif