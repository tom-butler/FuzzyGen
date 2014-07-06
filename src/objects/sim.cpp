#include "sim.h"
#include <iostream>
using namespace std;

void InitSim() {
  height.value = START_HEIGHT;
  velocity.value = START_VEL;
  fuelRemaining.value = START_FUEL;
}
float GetInputValue(int i) {
  if(i == 0)
    return height.value;
  if(i == 1)
    return velocity.value;
}
int NextStep(int controller) {

  float thrust = cont[controller].output.output;
  float fuel * cont[controller].input[]
  float velocity *


  if(fuel > 0) {

    fuel -= thrust;

    velocity.value = velocity.value + FORCE;
/*
    if(thrust.value < thrust.low)
      thrust.value = thrust.low;
    if(thrust.value > thrust.high)
      thrust.value = thrust.high;
*/
    velocity.value = velocity.value - thrust;

    if(velocity.value < velocity.low)
      velocity.value = velocity.low;
    if(velocity.value > velocity.high)
      velocity.value = velocity.high;

    height.value -= velocity.value;
/*
    cout << height.value;
    cout << " " << fuelRemaining.value;
    cout << " " << velocity.value;
    cout << " " << thrust.value;
    cout << "\n";
*/
    if(fuelRemaining.value <= 0)
      return 0; //fail
    else if(height.value <= 0 && velocity.value < CRASH_SPEED)
      return fuelRemaining.value; //succeed
    else if(height.value <= 0)
      return 0; //fail
    else
      return -1; //continue
  }
  else
    return 0;

}
