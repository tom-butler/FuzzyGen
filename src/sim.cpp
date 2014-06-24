#include "sim.h"

using namespace std;

void initSim(int height, int y, int vel, int fuel) {
  xPos = height;
  yPos = y;
  velocity = vel;
  fuelRemaining = fuel;
}

int nextStep(int thrust) {
  if(fuelRemaining > 0) {
    fuelRemaining -= thrust;
    velocity = velocity + FORCE;
    velocity = velocity - thrust;
    xPos += velcocity;
    return 0;
  }
  else
    return 1;
}