#include "moon.h"
#include "..\..\objects\shared.h"
#include "..\..\gui\gui.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

//sim vars
const short int MAX_START_VEL = 3;
const short int MAX_THRUST = 100;
const short int MAX_FUEL = 8845;
const short int LANDER_WEIGHT = 5655;
const short int SIM_HEIGHT = 5000;
const short int SIM_WIDTH = 1000;
const short int SAFE_WIDTH = 200;
const float GRAVITY = 1.633f;
const float Y_CRASH_SPEED = 3.05f;
const float X_CRASH_SPEED = 1.22f;

//relative
static short int SAFE_X = 100 + GetRandInt(0, 260);
static float MAX_FUEL_BURN = MAX_FUEL / 15 / 60.0; //maximum fuel to be burnt in one sec (tic)
static short int START_HEIGHT = SIM_HEIGHT - (SIM_HEIGHT * 0.1); //10 % down
static float START_FUEL = MAX_FUEL * 0.2; //70 % of fuel burnt on entry
static short int TERMINAL_VELOCITY = 100;


const float MAX_SCORE = (SIM_HEIGHT)/ 10 + SIM_WIDTH / 10 + (TERMINAL_VELOCITY - -TERMINAL_VELOCITY)  + TERMINAL_VELOCITY + START_FUEL;

//sim runtime vars
float landerMass;
float fuel;
float landerX;
float safeX;

float * throttle;
float * sideThrust;
float * height;
float * YVelocity;
float * XVelocity;
float * safeDist;
short int * score;


//throttle accumulator
static FuzzyVar heightSet  = {0, SIM_HEIGHT, START_HEIGHT, 0, 0};
static FuzzyVar YVelocitySet = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, MAX_START_VEL, 0, 0};

//x thrust accumulator
static FuzzyVar XVelocitySet = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, 0, 0};
static FuzzyVar safeDistSet = {-SIM_WIDTH, SIM_WIDTH,0,0};

//fitness
static FuzzyVar fuelSet  = {0, START_FUEL, START_FUEL, 0, 0};

//output
static Accumulator sideThrustSet = {-5, 5, 0.0f, 0, 0, 0, 0, 0, 0};
static Accumulator thrustSet = {0, MAX_THRUST, 0.0f, 0, 0, 0, 0, 0, 0};

void MoonCreateVars(){
  //sim input vars
  NUM_INPUT = 3;
  simInput = new FuzzyVar[NUM_INPUT];

  simInput[0] = heightSet;
  simInput[1] = YVelocitySet;

  simInput[2] = XVelocitySet;
  simInput[3] = safeDistSet;

  //sim output vars
  NUM_OUTPUT = 2;
  simOutput = new Accumulator[NUM_OUTPUT];
  simOutput[0] = thrustSet;
  simOutput[0].vars = new short int[2];
  simOutput[0].vars[0] = 0;
  simOutput[0].vars[1] = 1;
  simOutput[0].varsNum = 2;

  simOutput[1] = sideThrustSet;
  simOutput[1].vars = new short int[1];
  simOutput[1].vars[0] = 2;
  simOutput[1].vars[0] = 3;
  simOutput[1].varsNum = 2;

  simFitness = &fuelSet;
}

void MoonInitSim(int controller) {
  throttle = &cont[controller].output[0].output;
  sideThrust = &cont[controller].output[1].output;

  height = &cont[controller].input[0].value;
  YVelocity = &cont[controller].input[1].value;
  XVelocity = &cont[controller].input[2].value;
  safeDist = &cont[controller].input[3].value;
  score = &cont[controller].score;


  landerMass = LANDER_WEIGHT + START_FUEL;
  if(RANDOM_START){
    *height = START_HEIGHT + GetRandInt(0, 250);
    *YVelocity = GetRandFloat(-MAX_START_VEL, MAX_START_VEL);
    *XVelocity = GetRandFloat(-MAX_START_VEL, MAX_START_VEL);
    safeX = 250 + GetRandInt(0, 250);
    landerX = 100 + GetRandInt(0, 150);
  }
  else{
    *height = START_HEIGHT + 250;
    *YVelocity = 0;
    *XVelocity = 0;
    safeX = 500;
    landerX = 200;
  }
  fuel = START_FUEL;
  *safeDist = safeX - landerX;
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
    fuel -= MAX_FUEL_BURN * (abs(*sideThrust) /10);
    landerMass = LANDER_WEIGHT + fuel;
    *YVelocity += GRAVITY;
    *YVelocity -= (*throttle * 440) / landerMass;
    *XVelocity += (*sideThrust * 100) / landerMass;

    //ensure it is within bounds
    if(*YVelocity < -TERMINAL_VELOCITY)
      *YVelocity = -TERMINAL_VELOCITY;
    if(*YVelocity > TERMINAL_VELOCITY)
      *YVelocity = TERMINAL_VELOCITY;

    if(*XVelocity < -TERMINAL_VELOCITY)
      *XVelocity = -TERMINAL_VELOCITY;
    if(*XVelocity > TERMINAL_VELOCITY)
      *XVelocity = TERMINAL_VELOCITY;
    if(fuel < 0)
      fuel = 0.0f;

    //move
    *height -= *YVelocity;
    landerX += *XVelocity;

    *safeDist = safeX - landerX;

    //check the height
    if(*height < 0.0f)
      *height = 0.0f;
    if(*height > SIM_HEIGHT)
      *height = SIM_HEIGHT;

    //score
    else if(*height <= 0.0f && *YVelocity < Y_CRASH_SPEED && abs(*XVelocity) < X_CRASH_SPEED) { //safe
      *score = (SIM_HEIGHT - *height)/ 10 + (SIM_WIDTH - abs(*safeDist)) / 10 + (TERMINAL_VELOCITY - *YVelocity) + (TERMINAL_VELOCITY - abs(*XVelocity)) + fuel;
      *score = (*score / MAX_SCORE) * 1000;
      return 0; //fail
    }
    else if(*height <= 0.0f) { //crashed
      *score = (SIM_HEIGHT - *height)/ 10 + (SIM_WIDTH - abs(*safeDist)) / 10 + (TERMINAL_VELOCITY - *YVelocity) + (TERMINAL_VELOCITY - abs(*XVelocity));
      *score = (*score / MAX_SCORE) * 1000;
      return 0; //semi-fail
    }
    if(fuel == 0.0f) { //if it ran out of fuel
      *score = (SIM_HEIGHT - *height) / 10 + (SIM_WIDTH - abs(*safeDist)) / 10;
      *score = (*score / MAX_SCORE) * 1000;
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

//manually created controller to prove the system works
void MoonControlController(int controller) {

  /*
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
*/
}
