#ifndef SIM_H
#define SIM_H

//constants
const int START_FUEL = 1000;
const int THRUST_MAX = 50;
const int TERMINAL_VELOCITY = 50;
const int FORCE = 5;

//Lander
//
typedef struct {
    int x;
    int y;
    int vel;
    int fuelRemaining;
}Lander ;

initSim(int x, int y, int vel, int fuel);
void checkInput(int argc, char *argv[]);
void nextStep();

#endif