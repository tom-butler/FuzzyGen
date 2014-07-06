#include "sim.h"
#include "..\settings.h"
#include <iostream>
using namespace std;

void InitSim(int controller) {
  cout << cont[controller].input[0].value;

  thrust =  &cont[controller].output.output;

  height =  &cont[controller].input[0].value;
  velocity = &cont[controller].input[1].value;
  fuel = &cont[controller].score;
}
int NextStep(int controller) {

  if(*fuel > 0) {

    *fuel -= *thrust;

    *velocity = *velocity + FORCE;
/*
    if(thrust.value < thrust.low)
      thrust.value = thrust.low;
    if(thrust.value > thrust.high)
      thrust.value = thrust.high;
*/
    *velocity = *velocity - *thrust;

    if(*velocity < 0)
      *velocity = 0;
    if(*velocity > TERMINAL_VELOCITY)
      *velocity = TERMINAL_VELOCITY;

    *height -= *velocity;
/*
    cout << height.value;
    cout << " " << fuelRemaining.value;
    cout << " " << velocity.value;
    cout << " " << thrust.value;
    cout << "\n";
*/
    if(*fuel <= 0)
      return 0; //fail
    else if(*height <= 0 && *velocity < CRASH_SPEED)
      return 0; //succeed
    else if(*height <= 0) {
      *fuel = -1;
      return 0; //fail
    }
    else
      return -1; //continue
  }
  else
    return 0;
}
