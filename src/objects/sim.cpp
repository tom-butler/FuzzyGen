#include "sim.h"
#include "shared.h"
#include <iostream>
using namespace std;

//sim runtime vars
float * thrust;
float * height;
float * velocity;
short int * fuel;

void InitSim(int controller) {
  thrust =  &cont[controller].output.output;
  height =  &cont[controller].input[0].value;
  velocity = &cont[controller].input[1].value;
  fuel = &cont[controller].score;
}
int NextStep(int controller) {
  if(*fuel > 0) {
    *fuel -= *thrust;
    *velocity = *velocity + FORCE;
    *velocity = *velocity - *thrust;
    if(*velocity < -TERMINAL_VELOCITY)
      *velocity = -TERMINAL_VELOCITY;
    if(*velocity > TERMINAL_VELOCITY)
      *velocity = TERMINAL_VELOCITY;

    *height -= *velocity;
/*
    cout << *height;
    cout << " " << *fuel;
    cout << " " << *velocity;
    cout << " " << *thrust;
    cout << "\n";
*/
    if(*fuel <= 0.0f) {
      return 0; //fail
    }
    else if(*height <= 0.0f && *velocity < CRASH_SPEED) {
      return 0; //succeed
    }
    else if(*height <= 0.0f) {
      *fuel = 0;
      return 0; //fail
    }
    else {
      return -1; //continue
    }
  }
  else {
    return 0;
  }
}