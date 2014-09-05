#include "moon.h"

#include <stdlib.h>
#include <iostream>

#include "..\..\shared\shared.h"
#include "..\..\gui\gui.h"
#include "..\..\shared\controller\h\create.h"

using namespace std;

//sim vars
const short int kMaxStartVel = 3;
const short int kMaxThrust = 100;
const short int kMaxSideThrust = 5;
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
static Accumulator moon_side_thrust_amount_set = {-kMaxSideThrust, kMaxSideThrust, 0.0f, 0, 0, 0, 0, 0, 0};
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
    *moon_height = kSafeHeight + 150;
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
void MoonControlController(int controller, FuzzyVar input[], Accumulator output[]) {

    //set the iterators to -1 to ensure error if incorrectly used
    int var = -1;
    int set;
    int out = -1;
    int rule;

  //create input sets
  cont[controller].input = new FuzzyVar[kNumInput];
  copy(input,input + kNumInput, cont[controller].input);

   //height
  var++;
  cont[controller].input[var].sets = new Set[kNumSetsMax];
  CreateSets(controller,var,3);
  set = -1;
    //landing
    set++;
    cont[controller].input[var].sets[set].height = 1;
    cont[controller].input[var].sets[set].centre_x = 0;
    cont[controller].input[var].sets[set].left_base = 0;
    cont[controller].input[var].sets[set].right_base = kSimHeight/2;
    cont[controller].input[var].sets[set].left_top = 0;
    cont[controller].input[var].sets[set].right_top = 25;

    //low
    set++;
    cont[controller].input[var].sets[set].height = 1;
    cont[controller].input[var].sets[set].centre_x = kSimHeight/2;
    cont[controller].input[var].sets[set].left_base = kSimHeight/4;
    cont[controller].input[var].sets[set].right_base = kSimHeight/4;
    cont[controller].input[var].sets[set].left_top = 10;
    cont[controller].input[var].sets[set].right_top = 10;

    //high
    set++;
    cont[controller].input[var].sets[set].height = 1;
    cont[controller].input[var].sets[set].centre_x = kSimHeight;
    cont[controller].input[var].sets[set].left_base = kSimHeight/2;
    cont[controller].input[var].sets[set].right_base = 0;
    cont[controller].input[var].sets[set].left_top = 10;
    cont[controller].input[var].sets[set].right_top = 200;


  //Y velocity
  var++;
  cont[controller].input[var].sets = new Set[kNumSetsMax];
  CreateSets(controller,var,3);
  set = -1;
    //up
    set++;
    cont[controller].input[var].sets[set].height = 1;
    cont[controller].input[var].sets[set].centre_x = -kTerminalVelocity;
    cont[controller].input[var].sets[set].left_base = 0;
    cont[controller].input[var].sets[set].right_base = kTerminalVelocity;
    cont[controller].input[var].sets[set].left_top = 0;
    cont[controller].input[var].sets[set].right_top = kTerminalVelocity/2;

    //slow
    set++;
    cont[controller].input[var].sets[set].height = 1;
    cont[controller].input[var].sets[set].centre_x = 0;
    cont[controller].input[var].sets[set].left_base = kTerminalVelocity/4;
    cont[controller].input[var].sets[set].right_base = kTerminalVelocity/4;
    cont[controller].input[var].sets[set].left_top = kTerminalVelocity/8;
    cont[controller].input[var].sets[set].right_top = kTerminalVelocity/8;

    //fast
    set++;
    cont[controller].input[var].sets[set].height = 1;
    cont[controller].input[var].sets[set].centre_x = kTerminalVelocity;
    cont[controller].input[var].sets[set].left_base = kTerminalVelocity;
    cont[controller].input[var].sets[set].right_base = 0;
    cont[controller].input[var].sets[set].left_top = kTerminalVelocity/2;
    cont[controller].input[var].sets[set].right_top = 0;

  //X velocity
  var++;
  cont[controller].input[var].sets = new Set[kNumSetsMax];
  CreateSets(controller,var,2);
  set = -1;
    //left
    set++;
    cont[controller].input[var].sets[set].height = 1;
    cont[controller].input[var].sets[set].centre_x = -kTerminalVelocity;
    cont[controller].input[var].sets[set].left_base = 0;
    cont[controller].input[var].sets[set].right_base = kTerminalVelocity * 1.2;
    cont[controller].input[var].sets[set].left_top = 0;
    cont[controller].input[var].sets[set].right_top = kTerminalVelocity * 0.75;

    //right
    set++;
    cont[controller].input[var].sets[set].height = 1;
    cont[controller].input[var].sets[set].centre_x = kTerminalVelocity;
    cont[controller].input[var].sets[set].left_base = kTerminalVelocity * 1.2;
    cont[controller].input[var].sets[set].right_base = 0;
    cont[controller].input[var].sets[set].left_top = kTerminalVelocity * 0.75;
    cont[controller].input[var].sets[set].right_top = 0;

  //distance
  var++;
  cont[controller].input[var].sets = new Set[kNumSetsMax];
  CreateSets(controller,var,3);
  set = -1;
    //left
    set++;
    cont[controller].input[var].sets[set].height = 1;
    cont[controller].input[var].sets[set].centre_x = -kSimWidth;
    cont[controller].input[var].sets[set].left_base = 0;
    cont[controller].input[var].sets[set].right_base = kSimWidth/2;
    cont[controller].input[var].sets[set].left_top = 0;
    cont[controller].input[var].sets[set].right_top = kSimWidth/4;

    //centre
    set++;
    cont[controller].input[var].sets[set].height = 1;
    cont[controller].input[var].sets[set].centre_x = 0;
    cont[controller].input[var].sets[set].left_base = kSimWidth;
    cont[controller].input[var].sets[set].right_base = kSimWidth;
    cont[controller].input[var].sets[set].left_top = 0;
    cont[controller].input[var].sets[set].right_top = 0;

    //right
    set++;
    cont[controller].input[var].sets[set].height = 1;
    cont[controller].input[var].sets[set].centre_x = kSimWidth;
    cont[controller].input[var].sets[set].left_base = kSimWidth/2;
    cont[controller].input[var].sets[set].right_base = 0;
    cont[controller].input[var].sets[set].left_top = kSimWidth/4;
    cont[controller].input[var].sets[set].right_top = 0;

  
  //create accumulators
  cont[controller].output = new Accumulator[kNumOutput];
  copy(output,output + kNumOutput, cont[controller].output);

  //Main Thrust
  out++;
  rule = -1;

  cont[controller].output[out].num_rules = 1;
  for(int p = 0; p < cont[controller].output[out].num_vars; p++){
    cont[controller].output[out].num_rules *= cont[controller].input[cont[controller].output[out].vars[p]].num_sets;
  }
  cont[controller].output[out].value = new float[cont[controller].output[out].num_rules];
  cont[controller].output[out].scale = new float[cont[controller].output[out].num_rules];
  cont[controller].output[out].rules = new  Rule[cont[controller].output[out].num_rules];
  CreateRules(controller, out);

    //if height landing and y vel up 0, 0
    rule++;
    cont[controller].output[out].rules[rule].output = 0.0f;
    //if height landing and y vel slow 0, 1 
    rule++;
    cont[controller].output[out].rules[rule].output = 22.0f;
    //if height landing and y vel fast 0 , 2
    rule++;
    cont[controller].output[out].rules[rule].output = 40.0f;

    //if height low and y vel up 1, 0
    rule++;
    cont[controller].output[out].rules[rule].output = 0.0f;
    //if height low and y vel slow 1, 1
    rule++;
    cont[controller].output[out].rules[rule].output = 20.0f;
    //if height low and y vel fast 1, 2
    rule++;
    cont[controller].output[out].rules[rule].output = 40.0f;

    //if height high and y vel up 2 , 0
    rule++;
    cont[controller].output[out].rules[rule].output = 0.0f;
    //if height high and y vel slow 2, 1
    rule++;
    cont[controller].output[out].rules[rule].output = 10.0f;
    //if height high and y vel fast 2 , 2
    rule++;
    cont[controller].output[out].rules[rule].output = 30.0f;

  //Side Thrusters
    out++;
    rule = -1;

  cont[controller].output[out].num_rules = 1;
  for(int p = 0; p < cont[controller].output[out].num_vars; p++){
    cont[controller].output[out].num_rules *= cont[controller].input[cont[controller].output[out].vars[p]].num_sets;
  }
  cont[controller].output[out].value = new float[cont[controller].output[out].num_rules];
  cont[controller].output[out].scale = new float[cont[controller].output[out].num_rules];
  cont[controller].output[out].rules = new  Rule[cont[controller].output[out].num_rules];
  CreateRules(controller, out);

    //if dist landing and x vel left 0, 0
    rule++;
    cont[controller].output[out].rules[rule].output = -5.0f;
    //if dist landing and x vel right 0 , 2
    rule++;
    cont[controller].output[out].rules[rule].output = 5.0f;

    //if dist low and x vel left 1, 0
    rule++;
    cont[controller].output[out].rules[rule].output = -5.0f;
    //if dist low and x vel right 1, 2
    rule++;
    cont[controller].output[out].rules[rule].output = 5.0f;

    //if dist high and x vel left 2 , 0
    rule++;
    cont[controller].output[out].rules[rule].output = -5.0f;
    //if dist high and x vel right 2 , 2
    rule++;
    cont[controller].output[out].rules[rule].output = 5.0f;
}
