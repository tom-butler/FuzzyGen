#include "sim.h"
#include "shared.h"
#include <iostream>
using namespace std;

//sim runtime vars
float * thrust;
float * height;
float * velocity;
int * fuel;

void tearDown();

void Kill(float **ptr)
{
  if ((*ptr) != NULL)
  {
    delete (*ptr);
  *ptr=NULL;
  }
}

void InitSim(int controller) {
  thrust =  &cont[controller].output.output;
  height =  &cont[controller].input[0].value;
  velocity = &cont[controller].input[1].value;
  fuel = &cont[controller].score;
cout << "e";
}
int NextStep(int controller) {
  cout << "3";
  if(*fuel > 0) {
    *fuel -= *thrust;
    *velocity = *velocity + FORCE;
    *velocity = *velocity - *thrust;

    if(*velocity < 0)
      *velocity = 0;
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
            *fuel = -1;

      return 0; //fail
    }
    else
      return -1; //continue
  }
  else
    return 0;
}
void tearDown() {
  delete(thrust);
  delete(height);
  delete(velocity);
  delete(fuel);
  thrust = NULL;
  height = NULL;
  velocity = NULL;
  fuel = NULL;
}
