#include <iostream>
#include <stdlib.h>

#include "..\..\objects\shared.h"
#include "..\..\gui\gui.h"

#include "moon.h"

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
float moon_landerMass;
float moon_fuel;
float moon_landerX;
float moon_safeX;

float * moon_throttle;
float * moon_sideThrust;
float * moon_height;
float * moon_YVelocity;
float * moon_XVelocity;
float * moon_safeDist;
short int * moon_score;


//throttle accumulator
static FuzzyVar moon_heightSet  = {0, SIM_HEIGHT, START_HEIGHT, 0, 0};
static FuzzyVar moon_YVelocitySet = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, MAX_START_VEL, 0, 0};

//x thrust accumulator
static FuzzyVar moon_XVelocitySet = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, 0, 0, 0};
static FuzzyVar moon_safeDistSet = {-SIM_WIDTH, SIM_WIDTH, 0, 0, 0};


//output
static Accumulator moon_sideThrustSet = {-5, 5, 0.0f, 0, 0, 0, 0, 0, 0};
static Accumulator moon_thrustSet = {0, MAX_THRUST, 0.0f, 0, 0, 0, 0, 0, 0};

void MoonCreateVars(){
  //sim input vars
  NUM_INPUT = 4;
  simInput = new FuzzyVar[NUM_INPUT];

  simInput[0] = moon_heightSet;
  simInput[1] = moon_YVelocitySet;

  simInput[2] = moon_XVelocitySet;
  simInput[3] = moon_safeDistSet;

  //sim output vars
  NUM_OUTPUT = 2;
  simOutput = new Accumulator[NUM_OUTPUT];

  simOutput[0] = moon_thrustSet;
  simOutput[0].vars = new short int[2];
  simOutput[0].vars[0] = 0;
  simOutput[0].vars[1] = 1;
  simOutput[0].varsNum = 2;

  simOutput[1] = moon_sideThrustSet;
  simOutput[1].vars = new short int[2];
  simOutput[1].vars[0] = 2;
  simOutput[1].vars[1] = 3;
  simOutput[1].varsNum = 2;
}

void MoonInitSim(int controller) {
  moon_throttle = &cont[controller].output[0].output;
  moon_sideThrust = &cont[controller].output[1].output;

  moon_height = &cont[controller].input[0].value;
  moon_YVelocity = &cont[controller].input[1].value;
  moon_XVelocity = &cont[controller].input[2].value;
  moon_safeDist = &cont[controller].input[3].value;
  moon_score = &cont[controller].score;


  moon_landerMass = LANDER_WEIGHT + START_FUEL;
  if(RANDOM_START){
    *moon_height = START_HEIGHT + GetRandInt(0, 250);
    *moon_YVelocity = GetRandFloat(-MAX_START_VEL, MAX_START_VEL);
    *moon_XVelocity = GetRandFloat(-MAX_START_VEL, MAX_START_VEL);
    moon_safeX = 250 + GetRandInt(0, 250);
    moon_landerX = 100 + GetRandInt(0, 150);
  }
  else{
    *moon_height = START_HEIGHT + 250;
    *moon_YVelocity = 0;
    *moon_XVelocity = 0;
    moon_safeX = 500;
    moon_landerX = 200;
  }
  moon_fuel = START_FUEL;
  *moon_safeDist = moon_safeX - moon_landerX;
}

int MoonNextStep(int controller) {

  //only move if we have fuel
  if(moon_fuel > 0) {

    //throttle only works 10-100
    if(*moon_throttle < 10){
      *moon_throttle = 0;
    }
    //calculate velocity
    moon_fuel -= MAX_FUEL_BURN * (*moon_throttle / 100);
    moon_fuel -= MAX_FUEL_BURN * (abs(*moon_sideThrust) /10);
    moon_landerMass = LANDER_WEIGHT + moon_fuel;
    *moon_YVelocity += GRAVITY;
    *moon_YVelocity -= (*moon_throttle * 440) / moon_landerMass;
    *moon_XVelocity += (*moon_sideThrust * 100) / moon_landerMass;

    //ensure it is within bounds
    if(*moon_YVelocity < -TERMINAL_VELOCITY)
      *moon_YVelocity = -TERMINAL_VELOCITY;
    if(*moon_YVelocity > TERMINAL_VELOCITY)
      *moon_YVelocity = TERMINAL_VELOCITY;

    if(*moon_XVelocity < -TERMINAL_VELOCITY)
      *moon_XVelocity = -TERMINAL_VELOCITY;
    if(*moon_XVelocity > TERMINAL_VELOCITY)
      *moon_XVelocity = TERMINAL_VELOCITY;
    if(moon_fuel < 0)
      moon_fuel = 0.0f;

    //move
    *moon_height -= *moon_YVelocity;
    moon_landerX += *moon_XVelocity;

    *moon_safeDist = moon_safeX - moon_landerX;
    
    if(*moon_safeDist > SIM_WIDTH)
      *moon_safeDist = SIM_WIDTH;
    if(*moon_safeDist < -SIM_WIDTH)
      *moon_safeDist = -SIM_WIDTH;

    //check the height
    if(*moon_height < 0.0f)
      *moon_height = 0.0f;
    if(*moon_height > SIM_HEIGHT)
      *moon_height = SIM_HEIGHT;

    //score
    else if(*moon_height <= 0.0f && *moon_YVelocity < Y_CRASH_SPEED && abs(*moon_XVelocity) < X_CRASH_SPEED) { //safe
      *moon_score = (SIM_HEIGHT - *moon_height)/ 10 + (SIM_WIDTH - abs(*moon_safeDist)) / 10 + (TERMINAL_VELOCITY - *moon_YVelocity) + (TERMINAL_VELOCITY - abs(*moon_XVelocity)) + moon_fuel;
      *moon_score = (*moon_score / MAX_SCORE) * 100;
      return 0; //fail
    }
    else if(*moon_height <= 0.0f) { //crashed
      *moon_score = (SIM_HEIGHT - *moon_height)/ 10 + (SIM_WIDTH - abs(*moon_safeDist)) / 10 + (TERMINAL_VELOCITY - *moon_YVelocity) + (TERMINAL_VELOCITY - abs(*moon_XVelocity));
      *moon_score = (*moon_score / MAX_SCORE) * 100;
      return 1; //semi-fail
    }
    if(moon_fuel == 0.0f) { //if it ran out of fuel
      *moon_score = (SIM_HEIGHT - *moon_height) / 10 + (SIM_WIDTH - abs(*moon_safeDist)) / 10;
      *moon_score = (*moon_score / MAX_SCORE) * 100;
      return 1; //fail
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
