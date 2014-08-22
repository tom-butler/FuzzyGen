#include "moon.h"

#include <stdlib.h>
#include <iostream>

#include "..\..\shared\shared.h"
#include "..\..\gui\gui.h"


using namespace std;

//sim vars
const short int kMaxStartVel = 3;
const short int kMaxThrust = 100;
const short int kMaxFuel = 8845;
const short int kLanderWeight = 5655;
const short int kSimHeight = 5000;
const short int kSimWidth = 1000;
const short int kSafeWidth = 200;
const short int kTerminalVelocity = 100;
const float kGravity = 1.633f;
const float kYCrashSpeed = 3.05f;
const float kXCrashSpeed = 1.22f;

//relative
static short int kSafeX = 100 + GetRandInt(0, 260);
static float kMaxFuelBurn = kMaxFuel / 15 / 60.0; //maximum fuel to be burnt in one sec (tic)
static short int kSafeHeight = kSimHeight - (kSimHeight * 0.1); //10 % down
static float kStartFuel = kMaxFuel * 0.2; //70 % of fuel burnt on entry

const float kMaxScore = (kSimHeight)/ 10 + kSimWidth / 10 + (kTerminalVelocity - -kTerminalVelocity)  + kTerminalVelocity + kStartFuel;

//local sim runtime vars
float moon_lander_mass;
float moon_fuel;
float moon_lander_x_pos;
float moon_safe_x_pos;

//controller refence pointers
float * moon_throttle;
float * moon_side_thrust_amount;
float * moon_height;
float * moon_y_velocity;
float * moon_x_velocity;
float * moon_safe_distance;
short int * moon_score;


//throttle accumulator
static FuzzyVar moon_heightSet  = {0, kSimHeight, kSafeHeight, 0, 0};
static FuzzyVar moon_y_velocitySet = {-kTerminalVelocity, kTerminalVelocity, kMaxStartVel, 0, 0};

//x thrust accumulator
static FuzzyVar moon_x_velocitySet = {-kTerminalVelocity, kTerminalVelocity, 0, 0, 0};
static FuzzyVar moon_safe_distanceSet = {-kSimWidth, kSimWidth, 0, 0, 0};

//output
static Accumulator moon_side_thrust_amount_set = {-5, 5, 0.0f, 0, 0, 0, 0, 0, 0};
static Accumulator moon_thrustSet = {0, kMaxThrust, 0.0f, 0, 0, 0, 0, 0, 0};

void MoonCreateVars(){
  //sim input vars
  kNumInput = 4;
  simInput = new FuzzyVar[kNumInput];

  simInput[0] = moon_heightSet;
  simInput[1] = moon_y_velocitySet;
  simInput[2] = moon_x_velocitySet;
  simInput[3] = moon_safe_distanceSet;

  //sim output vars
  kNumOutput = 2;
  simOutput = new Accumulator[kNumOutput];
  //thrust accumulator
  simOutput[0] = moon_thrustSet;
  simOutput[0].vars = new short int[2];
  simOutput[0].vars[0] = 0;
  simOutput[0].vars[1] = 1;
  simOutput[0].num_vars = 2;
  //side thrust accumulator
  simOutput[1] = moon_side_thrust_amount_set;
  simOutput[1].vars = new short int[2];
  simOutput[1].vars[0] = 2;
  simOutput[1].vars[1] = 3;
  simOutput[1].num_vars = 2;
}

void MoonInitSim(int controller) {
  //define pointers
  moon_throttle = &cont[controller].output[0].output;
  moon_side_thrust_amount = &cont[controller].output[1].output;
  moon_height = &cont[controller].input[0].value;
  moon_y_velocity = &cont[controller].input[1].value;
  moon_x_velocity = &cont[controller].input[2].value;
  moon_safe_distance = &cont[controller].input[3].value;
  moon_score = &cont[controller].score;

  //define sim variables
  if(kRandomStart){
    *moon_height = kSafeHeight + GetRandInt(0, 250);
    *moon_y_velocity = GetRandFloat(-kMaxStartVel, kMaxStartVel);
    *moon_x_velocity = GetRandFloat(-kMaxStartVel, kMaxStartVel);
    moon_safe_x_pos = 250 + GetRandInt(0, 250);
    moon_lander_x_pos = 100 + GetRandInt(0, 150);
  }
  else{ //no random start
    *moon_height = kSafeHeight + 250;
    *moon_y_velocity = 0;
    *moon_x_velocity = 0;
    moon_safe_x_pos = 500;
    moon_lander_x_pos = 200;
  }
  //common start values 
  moon_fuel = kStartFuel;
  moon_lander_mass = kLanderWeight + moon_fuel;
  *moon_safe_distance = moon_safe_x_pos - moon_lander_x_pos;
}

int MoonNextStep(int controller) {

  //only move if we have fuel
  if(moon_fuel > 0) {

    //throttle only works 10-100
    if(*moon_throttle < 10){
      *moon_throttle = 0;
    }
    //calculate velocity
    moon_fuel -= kMaxFuelBurn * (*moon_throttle / 100);
    moon_fuel -= kMaxFuelBurn * (abs(*moon_side_thrust_amount) /10);
    moon_lander_mass = kLanderWeight + moon_fuel;
    *moon_y_velocity += kGravity;
    *moon_y_velocity -= (*moon_throttle * 440) / moon_lander_mass;
    *moon_x_velocity += (*moon_side_thrust_amount * 100) / moon_lander_mass;

    //ensure it is within bounds
    ForceBounds(*moon_y_velocity, -kTerminalVelocity, kTerminalVelocity);
    ForceBounds(*moon_x_velocity, -kTerminalVelocity, kTerminalVelocity);

    if(moon_fuel < 0){
      moon_fuel = 0.0f;
    }

    //move
    *moon_height -= *moon_y_velocity;
    moon_lander_x_pos += *moon_x_velocity;
    *moon_safe_distance = moon_safe_x_pos - moon_lander_x_pos;

    ForceBounds(*moon_safe_distance, -kSimWidth, kSimWidth);
    ForceBounds(*moon_height, 0.0f, kSimHeight);

    //score
    if(*moon_height <= 0.0f && *moon_y_velocity < kYCrashSpeed && abs(*moon_x_velocity) < kXCrashSpeed) { //safe
      *moon_score = (kSimHeight - *moon_height)/ 10 + (kSimWidth - abs(*moon_safe_distance)) / 10 + (kTerminalVelocity - *moon_y_velocity) + (kTerminalVelocity - abs(*moon_x_velocity)) + moon_fuel;
      *moon_score = (*moon_score / kMaxScore) * 100;
      return 0; //fail
    }
    else if(*moon_height <= 0.0f) { //crashed
      *moon_score = (kSimHeight - *moon_height)/ 10 + (kSimWidth - abs(*moon_safe_distance)) / 10 + (kTerminalVelocity - *moon_y_velocity) + (kTerminalVelocity - abs(*moon_x_velocity));
      *moon_score = (*moon_score / kMaxScore) * 100;
      return 1; //semi-fail
    }
    if(moon_fuel == 0.0f) { //if it ran out of fuel
      *moon_score = (kSimHeight - *moon_height) / 10 + (kSimWidth - abs(*moon_safe_distance)) / 10;
      *moon_score = (*moon_score / kMaxScore) * 100;
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
