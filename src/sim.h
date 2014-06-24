#ifndef SIM_H
#define SIM_H

//constants
const int START_FUEL = 1000;
const int THRUST_MAX = 50;
const int TERMINAL_VELOCITY = 50;
const int FORCE = 5;

//globals
static Lander lander;
static int xPos;
static int velocity;
static int fuelRemaining;

initSim(int height, int vel, int fuel);
int nextStep();
#endif