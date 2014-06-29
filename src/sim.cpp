#include "sim.h"

using namespace std;

void InitSim() {
  height.value = START_HEIGHT;
  velocity.value = START_VEL;
  fuelRemaining.value = START_FUEL;
}

int NextStep() {
  if(fuelRemaining.value > 0) {
    fuelRemaining.value -= thrust.value;
    velocity.value = velocity.value + FORCE;
    velocity.value = velocity.value - thrust.value;
    height.value += velocity.value;

    if(fuelRemaining.value <= 0)
      return 0; //fail
    else if(height.value <= 0 && velocity.value < CRASH_SPEED)
      return 1; //succeed
    else
      return 2; //continue
  }
  else
    return 0;

}