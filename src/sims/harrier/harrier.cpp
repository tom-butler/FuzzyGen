#include "harrier.h"

#include <cmath>
#include <stdlib.h>
#include <iostream>

#include "..\..\shared\shared.h"
#include "..\..\gui\gui.h"

using namespace std;

float KnotsToMps(float knots);
float Apportion(float inLow, float inHigh, float in, float outLow, float outHigh);

//harrier
const float   kFuelBurnMin = 0.1f;
const float   kFuelBurnMax = 0.5f;
const int   kForceMax = 106000; //(newtons)
const float kLandingSpeedYMax = 3.4f;
const float kLandingSpeedXMax = 3.4f;
const int   kTerminalVelocity = 100;
const int   kStartFuel = 80;

const int   kHarrierMass = 5655 + 3500; //mass + missles
const float kHarrierFriction = 0.013f;
const int   kGroundEffectHeight = 16;

//environment
const float kGravity = 9.8f;
int kHarrierSimHeight = 200;
int kHarrierSimWidth = 600;
const int   kWindSpeed = 10;
const int   kWindGustMax = 20;
const int   kVectorMax = 5;


const float kHarrierScoreMax = (kHarrierSimWidth/10) + kTerminalVelocity + kTerminalVelocity + kStartFuel;
//extern
float harrier_y_position;
float harrier_x_position;
float harrier_fuel;
float harrier_mass;
float harrier_safe_x_position;
float harrier_safe_y_position;
float harrier_safe_width;

float harrier_force;
float harrier_x_velocity;

//simStatus
bool harrier_is_boom;
bool harrier_landed;
bool harrier_ground_effect;

//ship

float harrier_ship_speed;

//controller variables
float * harrier_height;
float * harrier_y_velocity;
float * harrier_relative_x_velocity;
float * harrier_safe_distance;

float * harrier_throttle;
float * harrier_vector;

short int * harrier_score;

static FuzzyVar harrier_height_set = {0, kHarrierSimHeight, kHarrierSimHeight, 0, 0};
static FuzzyVar harrier_y_velocity_set  = {-kTerminalVelocity, kTerminalVelocity, 0, 0, 0};
static FuzzyVar harrier_x_velocity_set = {-kTerminalVelocity, kTerminalVelocity, 0, 0, 0};
static FuzzyVar harrier_safe_distance_set  = {-kHarrierSimWidth/2, kHarrierSimWidth/2, 0, 0, 0};

//output
static Accumulator harrier_throttle_accumulator = {0, 100, 0.0f, 0, 0, 0, 0, 0, 0};
static Accumulator harrier_vector_accumulator = {-kVectorMax, kVectorMax, 0.0f, 0, 0, 0, 0, 0, 0};

void HarrierCreateVars(){
  //sim input vars
  kNumInput = 4;
  simInput = new FuzzyVar[kNumInput];

  simInput[0] = harrier_height_set;
  simInput[1] = harrier_y_velocity_set;
  simInput[2] = harrier_x_velocity_set;
  simInput[3] = harrier_safe_distance_set;

  //sim output vars
  kNumOutput = 2;
  simOutput = new Accumulator[kNumOutput];
  simOutput[0] = harrier_throttle_accumulator;
  simOutput[0].vars = new short int[4];
  simOutput[0].vars[0] = 0;
  simOutput[0].vars[1] = 1;
  simOutput[0].vars[2] = 2;
  simOutput[0].vars[3] = 3;
  simOutput[0].num_vars = 4;

  simOutput[1] = harrier_vector_accumulator;
  simOutput[1].vars = new short int[4];
  simOutput[1].vars[0] = 0;
  simOutput[1].vars[1] = 1;
  simOutput[1].vars[2] = 2;
  simOutput[1].vars[3] = 3;
  simOutput[1].num_vars = 4;
}

void HarrierInitSim(int controller) {
  harrier_throttle = &cont[controller].output[0].output;
  harrier_vector = &cont[controller].output[1].output;

  harrier_height = &cont[controller].input[0].value;
  harrier_y_velocity = &cont[controller].input[1].value;
  harrier_relative_x_velocity = &cont[controller].input[2].value;
  harrier_safe_distance = &cont[controller].input[3].value;

  harrier_score = &cont[controller].score;

  if(kRandomStart){
    //random pos
  }
  else{
    harrier_x_position = 15;
    harrier_y_position = 170;
    harrier_x_velocity = 46;
    *harrier_y_velocity = 2;
    harrier_ship_speed = 22;
    *harrier_vector = 0;
    *harrier_relative_x_velocity = 22;
    //*harrier_throttle = 89;
    harrier_ground_effect = true;
    harrier_safe_x_position = 400;
    harrier_safe_y_position = 16;
    harrier_safe_width = 20;
  }
  harrier_force = 0.0f;
  harrier_fuel = kStartFuel;
  harrier_mass = kHarrierMass + harrier_fuel;
  harrier_is_boom = false;
  harrier_landed = false;
  *harrier_safe_distance = harrier_safe_x_position - harrier_x_position;
}

int HarrierNextStep(int controller) {

  //only move if we have fuel
  if(harrier_fuel > 0) {
    if(*harrier_throttle < 0)
      *harrier_throttle = 0;
    if(*harrier_throttle > 100)
      *harrier_throttle = 100;
    if(*harrier_vector <= -kVectorMax)
      *harrier_vector = -kVectorMax;
    if(*harrier_vector >= kVectorMax)
      *harrier_vector = kVectorMax;
    //re-calculate harrier status
    harrier_mass = kHarrierMass + harrier_fuel;
    *harrier_y_velocity -= kGravity;

    //burn fuel
    float harrier_tFuel = (kFuelBurnMax - kFuelBurnMin) * (*harrier_throttle /100) + kFuelBurnMin;
    harrier_fuel -= harrier_tFuel;
    if(harrier_fuel < 0)
      harrier_fuel = 0;


    //adjust the velocity by thrust
    harrier_force = *harrier_throttle * kForceMax / 100 * 0.95; //0.95 allows for 5% cold air bleed 
    if(harrier_fuel == 0)
      harrier_force = 0;

    float harrier_angle = DegToRad(270 - *harrier_vector);
    float harrier_xx = harrier_x_position + harrier_force * cos(harrier_angle);
    float harrier_yy = harrier_y_position + harrier_force * sin(harrier_angle);

    harrier_xx *= -1;
    harrier_yy *= -1;

    harrier_x_velocity += harrier_xx / harrier_mass;
    *harrier_y_velocity += harrier_yy / harrier_mass;

    //adjust velocity by ground effect
    if(harrier_ground_effect && harrier_y_position <= kGroundEffectHeight + harrier_safe_y_position){
      *harrier_y_velocity -= Apportion(harrier_safe_y_position, kGroundEffectHeight + harrier_safe_y_position,harrier_y_position, 0, 0.5);
    }
    
    //adjust velocity by base wind
    harrier_x_velocity -= KnotsToMps(kWindSpeed) * kHarrierFriction;

    //adjust for wind gusts
    //@TODO make gusts last a random number of ticks
    float harrier_gust = GetRandFloat(0, KnotsToMps(kWindGustMax));
    harrier_x_velocity -= harrier_gust * kHarrierFriction;

    //adjust for ship speed
    harrier_x_position -= KnotsToMps(harrier_ship_speed);
    *harrier_relative_x_velocity = harrier_x_velocity - KnotsToMps(harrier_ship_speed);

    //adjust pos
    harrier_x_position += harrier_x_velocity;
    harrier_y_position += *harrier_y_velocity;

    //re-calculate system status
    *harrier_height = harrier_y_position;
    *harrier_safe_distance = harrier_x_position - harrier_safe_x_position;

    if(*harrier_safe_distance <= -kHarrierSimWidth/2)
      *harrier_safe_distance = -kHarrierSimWidth/2;
    if(*harrier_safe_distance >= kHarrierSimWidth/2)
      *harrier_safe_distance = kHarrierSimWidth/2;
    //check if it has landed
    if(*harrier_height <= 0){
          *harrier_score = ((kHarrierSimWidth - abs(*harrier_safe_distance))/10) + (kTerminalVelocity - abs((int)*harrier_y_velocity)) + (kTerminalVelocity - abs((int)*harrier_relative_x_velocity));
          *harrier_score = (*harrier_score / kHarrierScoreMax) * 100;
          return 1; //crashed
    }
    else if(*harrier_height <= harrier_safe_y_position + 3){
      if(abs(*harrier_safe_distance) <= harrier_safe_width/2){
        if(*harrier_relative_x_velocity <= kLandingSpeedXMax && *harrier_y_velocity <= kLandingSpeedYMax){
          harrier_landed = true;
          *harrier_score = (((kHarrierSimWidth - abs(*harrier_safe_distance))/10) + (kTerminalVelocity - abs((int)*harrier_y_velocity)) + (kTerminalVelocity - abs((int)*harrier_relative_x_velocity)) + harrier_fuel);
                    *harrier_score = (*harrier_score / kHarrierScoreMax) * 100;
          return 0;
        }
        else{
          harrier_is_boom = true;
          *harrier_score = ((kHarrierSimWidth - abs(*harrier_safe_distance))/10) + (kTerminalVelocity - abs((int)*harrier_y_velocity)) + (kTerminalVelocity - abs((int)*harrier_relative_x_velocity));
                    *harrier_score = (*harrier_score / kHarrierScoreMax) * 100;
          return 1;
        }
      }
    }
    else{
      return -1;
    }
    //@TODO: Add some functions to explode if the ship is hit 

  }
  else {
    *harrier_score = 0;
    return 1; //ran out of fuel
  }
}
float KnotsToMps(float knots){
  return knots * 0.514;
}
float Apportion(float in_low, float in_high, float in, float out_low, float out_high){
  if(in_high - in_low == 0)
    return 0;
  float value = (in - in_low) / (in_high - in_low);
  return Lerp(1 - value, out_low, out_high);
}
//manually created controller to prove the system works
void HarrierControlController(int controller) {

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
