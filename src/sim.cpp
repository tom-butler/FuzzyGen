#include "sim.h"

using namespace std;

void initSim(int height, int vel, int fuel) {
  yPos = height;
  velocity = vel;
  fuelRemaining = fuel;
}

int nextStep(int thrust) {
  if(fuelRemaining > 0) {
    fuelRemaining -= thrust;
    velocity = velocity + FORCE;
    velocity = velocity - thrust;
    yPos += velcocity;
    return 0;
  }
  else
    return 1;
}