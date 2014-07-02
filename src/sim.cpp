#include "sim.h"
#include <iostream>
using namespace std;

void InitSim() {
  height.value = START_HEIGHT;
  velocity.value = START_VEL;
  fuelRemaining.value = START_FUEL;
}
float* CheckSimValues() {
  float *vars = new float[NUM_INPUT];
vars[0] = height.value;
vars[1] = velocity.value;
return vars;
}
int NextStep(float newThrust) {
  thrust.value = newThrust;

  if(fuelRemaining.value > 0) {
    fuelRemaining.value -= thrust.value;
    velocity.value = velocity.value + FORCE;
    velocity.value = velocity.value - thrust.value;
    if(velocity.value < -TERMINAL_VELOCITY)
      velocity.value = TERMINAL_VELOCITY;
    if(velocity.value > TERMINAL_VELOCITY)
      velocity.value = TERMINAL_VELOCITY;
    height.value -= velocity.value;
    /*
    cout << height.value;
    cout << " ";
    cout << fuelRemaining.value;
    cout << " ";
    cout << velocity.value;
    cout << "\n";
    */
    if(fuelRemaining.value <= 0)
      return 0; //fail
    else if(height.value <= 0 && velocity.value < CRASH_SPEED)
      return 1; //succeed
    else if(height.value <= 0)
      return 0; //fail
    else
      return 2; //continue
  }
  else
    return 0;

}