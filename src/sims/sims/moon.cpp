#include "moon.h"
#include "..\..\objects\shared.h"
#include "..\..\gui\gui.h"
#include <iostream>

using namespace std;

//sim vars
const short int MAX_START_VEL = 3;
const short int MAX_THRUST = 100;
const short int MAX_FUEL = 8845;
const short int LANDER_WEIGHT = 5655;
const short int SIM_HEIGHT = 5000;
const float GRAVITY = 1.633f;
const float Y_CRASH_SPEED = 3.05f;
const float X_CRASH_SPEED = 1.22f;

//relative
static float MAX_FUEL_BURN = MAX_FUEL / 15 / 60.0; //maximum fuel to be burnt in one sec (tic)
static short int START_HEIGHT = SIM_HEIGHT - (SIM_HEIGHT * 0.1); //10 % down
static float START_FUEL = MAX_FUEL * 0.2; //70 % of fuel burnt on entry
static short int TERMINAL_VELOCITY = 100;


const float MAX_SCORE = (SIM_HEIGHT)/ 10 + (TERMINAL_VELOCITY - -TERMINAL_VELOCITY) + START_FUEL;
//sim runtime vars
float landerMass;
float fuel;

float * throttle;
float * height;
float * velocity;
short int * score;

static Accumulator thrustSet = {0, MAX_THRUST, 0.f, 0, 0, 0};
//low, high, start, sets(null)
static FuzzyVar heightSet  = {0, SIM_HEIGHT, START_HEIGHT, 0};
static FuzzyVar velocitySet = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, MAX_START_VEL, 0};
static FuzzyVar fuelSet  = {0, START_FUEL, START_FUEL, 0};

void MoonCreateVars(){
  //sim init vars
  simInput[0] = heightSet;
  simInput[1] = velocitySet;
  simOutput = &thrustSet;
  simFitness = &fuelSet;
}

void MoonInitSim(int controller) {
  throttle = &cont[controller].output.output;
  height = &cont[controller].input[0].value;
  velocity = &cont[controller].input[1].value;
  score = &cont[controller].score;

  landerMass = LANDER_WEIGHT + START_FUEL;
  *height = START_HEIGHT + GetRandInt(0, 250);
  *velocity = GetRandFloat(-MAX_START_VEL, MAX_START_VEL);
  fuel = START_FUEL;
}

int MoonNextStep(int controller) {

  //only move if we have fuel
  if(fuel > 0) {

    //throttle only works 10-100
    if(*throttle < 10){
      *throttle = 0;
    }

    //calculate velocity
    fuel -= MAX_FUEL_BURN * (*throttle / 100);
    landerMass = LANDER_WEIGHT + fuel;
    *velocity += GRAVITY;
    *velocity -= ((*throttle * 44000 / 100) / landerMass);

    //ensure it is within bounds
    if(*velocity < -TERMINAL_VELOCITY)
      *velocity = -TERMINAL_VELOCITY;
    if(*velocity > TERMINAL_VELOCITY)
      *velocity = TERMINAL_VELOCITY;
    if(fuel < 0)
      fuel = 0.0f;

    //move
    *height -= *velocity;

    //check the height
    if(*height < 0.0f)
      *height = 0.0f;
    if(*height > SIM_HEIGHT)
      *height = SIM_HEIGHT;

    //score
    if(fuel == 0.0f) {
      *score = (SIM_HEIGHT - *height) / 10;
      *score = (*score / MAX_SCORE) * 1000;
      return 0; //fail
    }
    else if(*height <= 0.0f && *velocity < Y_CRASH_SPEED) {
      *score = (SIM_HEIGHT - *height)/ 10 + (TERMINAL_VELOCITY - *velocity) + fuel;
      *score = (*score / MAX_SCORE) * 1000;
      return 0; //succeed
    }
    else if(*height <= 0.0f) {
      *score = (SIM_HEIGHT - *height)/ 10 + (TERMINAL_VELOCITY - *velocity);
      *score = (*score / MAX_SCORE) * 1000;
      return 0; //semi-fail
    }
    else {
      return -1; //continue
    }
  }
  else {
    return 0;
  }
}

//manually created controller to prove the system works
void MoonControlController(int controller) {
  //height

  //landing
  cont[controller].input[0].sets[0].height = 1;
  cont[controller].input[0].sets[0].centreX = 0;
  cont[controller].input[0].sets[0].leftBase = 0;
  cont[controller].input[0].sets[0].rightBase = 50;
  cont[controller].input[0].sets[0].leftTop = 0;
  cont[controller].input[0].sets[0].rightTop = 25;

  //low
  cont[controller].input[0].sets[1].height = 1;
  cont[controller].input[0].sets[1].centreX = 60;
  cont[controller].input[0].sets[1].leftBase = 50;
  cont[controller].input[0].sets[1].rightBase = 45;
  cont[controller].input[0].sets[1].leftTop = 10;
  cont[controller].input[0].sets[1].rightTop = 10;

  //high
  cont[controller].input[0].sets[2].height = 1;
  cont[controller].input[0].sets[2].centreX = 100;
  cont[controller].input[0].sets[2].leftBase = 60;
  cont[controller].input[0].sets[2].rightBase = 900;
  cont[controller].input[0].sets[2].leftTop = 10;
  cont[controller].input[0].sets[2].rightTop = 200;

  //velocity

  //up
  cont[controller].input[1].sets[0].height = 1;
  cont[controller].input[1].sets[0].centreX = -30;
  cont[controller].input[1].sets[0].leftBase = 0;
  cont[controller].input[1].sets[0].rightBase = 28;
  cont[controller].input[1].sets[0].leftTop = 0;
  cont[controller].input[1].sets[0].rightTop = 15;

  //slow
  cont[controller].input[1].sets[1].height = 1;
  cont[controller].input[1].sets[1].centreX = 0;
  cont[controller].input[1].sets[1].leftBase = 10;
  cont[controller].input[1].sets[1].rightBase = 10;
  cont[controller].input[1].sets[1].leftTop = 5;
  cont[controller].input[1].sets[1].rightTop = 5;

  //fast
  cont[controller].input[1].sets[2].height = 1;
  cont[controller].input[1].sets[2].centreX = 30;
  cont[controller].input[1].sets[2].leftBase = 28;
  cont[controller].input[1].sets[2].rightBase = 0;
  cont[controller].input[1].sets[2].leftTop = 10;
  cont[controller].input[1].sets[2].rightTop = 0;

  //rules
  //if height landing and vel up
   Rule r0 = {0,0,"AND",1,0, 2.0f, false};
  //if height landing and vel slow
   Rule r1 = {0,0,"AND",1,1, 8.0f, false};
  //if height landing and vel fast
   Rule r2 = {0,0,"AND",1,2, 9.0f, false};

  //if height low and vel up
   Rule r3 = {0,1,"AND",1,0, 1.0f, false};
  //if height low and vel slow
   Rule r4 = {0,1,"AND",1,1, 5.0f, false};
  //if height low and vel fast
   Rule r5 = {0,1,"AND",1,2, 7.0f, false};

  //if height high and vel up
   Rule r6 = {0,2,"AND",1,0, 1.0f, false};
  //if height high and vel slow
   Rule r7 = {0,2,"AND",1,1, 5.0f, false};
  //if height high and vel fast
   Rule r8 = {0,2,"AND",1,2, 6.0f,false};

  cont[controller].rules[0] = r0;
  cont[controller].rules[1] = r1;
  cont[controller].rules[2] = r2;
  cont[controller].rules[3] = r3;
  cont[controller].rules[4] = r4;
  cont[controller].rules[5] = r5;
  cont[controller].rules[6] = r6;
  cont[controller].rules[7] = r7;
  cont[controller].rules[8] = r8;

}
