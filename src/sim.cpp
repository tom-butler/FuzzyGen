#include "sim.h"
#include "controller.h"
using namespace std;

void initSim(int height, int vel, int fuel) {
  yPos = height;
  velocity = vel;
  fuelRemaining = fuel;
}

void nextStep(int thrust) {
  if(fuelRemaining > 0) {
    fuelRemaining -= thrust;
    velocity = velocity + FORCE;
    velocity = velocity - thrust;
    yPos += velcocity;

    if(fuelRemaining <= 0)
      return 0; //fail
    else if(ypos <= 0 && velocity < CRASH_SPEED)
      return 1; //succeed
    else
      return 2; //continue
  }
  else

}