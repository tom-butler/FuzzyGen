#include <iostream>
#include <stdlib.h>
#include <cmath>

#include "..\..\objects\shared.h"
#include "..\..\gui\gui.h"

#include "harrier.h"

using namespace std;

float DegToRad(float deg);
float KnotsToMps(float knots);
float Apportion(float inLow, float inHigh, float in, float outLow, float outHigh);

//harrier
const float   MIN_FUEL_BURN = 0.1f;
const float   MAX_FUEL_BURN = 0.5f;
const int   MAX_FORCE = 106000; //(newtons)
const float MAX_LANDING_SPEED_Y = 3.4f;
const float MAX_LANDING_SPEED_X = 3.4f;
const int   TERMINAL_VELOCITY = 100;
const int   START_FUEL = 80;

const int   HARRIER_MASS = 5655 + 3500; //mass + missles
const float HARRIER_FRICTION = 0.013f;
const int   GROUND_EFFECT_HEIGHT = 16;

//environment
const float SIM_GRAVITY = 9.8f;
int HARRIER_SIM_HEIGHT = 200;
int HARRIER_SIM_WIDTH = 600;
const int   WIND_SPEED = 10;
const int   MAX_WIND_GUST = 20;

//extern
float harrier_YPos;
float harrier_XPos;
float harrier_fuel;
float harrier_mass;
float harrier_safeX;
float harrier_safeY;
float harrier_safeWidth;

float harrier_force;
float harrier_XVel;

//simStatus
bool harrier_isBoom;
bool harrier_landed;
bool harrier_groundEffect;

//ship

float harrier_shipSpeed;

//controller variables
float * harrier_height;
float * harrier_YVel;
float * harrier_RelativeXVel;
float * harrier_safeDist;

float * harrier_throttle;
float * harrier_vector;

short int * harrier_score;

static FuzzyVar harrier_heightSet = {0, HARRIER_SIM_HEIGHT, HARRIER_SIM_HEIGHT, 0, 0};
static FuzzyVar harrier_YVelocitySet  = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, 0, 0, 0};
static FuzzyVar harrier_XVelocitySet = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, 0, 0, 0};
static FuzzyVar harrier_safeDistSet  = {-HARRIER_SIM_WIDTH/2, -HARRIER_SIM_WIDTH/2, 0, 0, 0};

//output
static Accumulator harrier_throttleAccumulator = {0, 100, 0.0f, 0, 0, 0, 0, 0, 0};
static Accumulator harrier_vectorAccumulator = {-5, 5, 0.0f, 0, 0, 0, 0, 0, 0};

void HarrierCreateVars(){
  //sim input vars
  NUM_INPUT = 4;
  simInput = new FuzzyVar[NUM_INPUT];

  simInput[0] = harrier_heightSet;
  simInput[1] = harrier_YVelocitySet;
  simInput[2] = harrier_XVelocitySet;
  simInput[3] = harrier_safeDistSet;

  //sim output vars
  NUM_OUTPUT = 2;
  simOutput = new Accumulator[NUM_OUTPUT];
  simOutput[0] = harrier_throttleAccumulator;
  simOutput[0].vars = new short int[4];
  simOutput[0].vars[0] = 0;
  simOutput[0].vars[1] = 1;
  simOutput[0].vars[2] = 2;
  simOutput[0].vars[3] = 3;
  simOutput[0].varsNum = 4;

  simOutput[1] = harrier_vectorAccumulator;
  simOutput[1].vars = new short int[4];
  simOutput[1].vars[0] = 0;
  simOutput[1].vars[1] = 1;
  simOutput[1].vars[2] = 2;
  simOutput[1].vars[3] = 3;
  simOutput[1].varsNum = 4;
}

void HarrierInitSim(int controller) {
  harrier_throttle = &cont[controller].output[0].output;
  harrier_vector = &cont[controller].output[1].output;

  harrier_height = &cont[controller].input[0].value;
  harrier_YVel = &cont[controller].input[1].value;
  harrier_RelativeXVel = &cont[controller].input[2].value;
  harrier_safeDist = &cont[controller].input[3].value;

  harrier_score = &cont[controller].score;

  if(RANDOM_START){
    //random pos
  }
  else{
    harrier_XPos = 15;
    harrier_YPos = 170;
    harrier_XVel = 46;
    *harrier_YVel = 2;
    harrier_shipSpeed = 22;
    *harrier_vector = 92;
    *harrier_RelativeXVel = 22;
    *harrier_throttle = 89;
    harrier_groundEffect = true;
    harrier_safeX = 400;
    harrier_safeY = 16;
    harrier_safeWidth = 20;
  }
  harrier_force = 0.0f;
  harrier_fuel = START_FUEL;
  harrier_mass = HARRIER_MASS + harrier_fuel;
  harrier_isBoom = false;
  harrier_landed = false;
  *harrier_safeDist = harrier_safeX - harrier_XPos;
}

int HarrierNextStep(int controller) {

  //only move if we have fuel
  if(harrier_fuel > 0) {
    if(*harrier_throttle < 0)
      *harrier_throttle = 0;
    if(*harrier_throttle > 100)
      *harrier_throttle = 100;

    //re-calculate harrier status
    harrier_mass = HARRIER_MASS + harrier_fuel;
    *harrier_YVel -= SIM_GRAVITY;

    //burn fuel
    float harrier_tFuel = (MAX_FUEL_BURN - MIN_FUEL_BURN) * (*harrier_throttle /100) + MIN_FUEL_BURN;
    harrier_fuel -= harrier_tFuel;
    if(harrier_fuel < 0)
      harrier_fuel = 0;


    //adjust the velocity by thrust
    harrier_force = *harrier_throttle * MAX_FORCE / 100 * 0.95; //0.95 allows for 5% cold air bleed 
    if(harrier_fuel == 0)
      harrier_force = 0;

    float harrier_angle = DegToRad(270 - *harrier_vector);
    float harrier_xx = harrier_XPos + harrier_force * cos(harrier_angle);
    float harrier_yy = harrier_YPos + harrier_force * sin(harrier_angle);

    harrier_xx *= -1;
    harrier_yy *= -1;

    harrier_XVel += harrier_xx / harrier_mass;
    *harrier_YVel += harrier_yy / harrier_mass;

    //adjust velocity by ground effect
    if(harrier_groundEffect && harrier_YPos <= GROUND_EFFECT_HEIGHT + harrier_safeY){
      *harrier_YVel -= Apportion(harrier_safeY, GROUND_EFFECT_HEIGHT + harrier_safeY,harrier_YPos, 0, 0.5);
    }
    
    //adjust velocity by base wind
    harrier_XVel -= KnotsToMps(WIND_SPEED) * HARRIER_FRICTION;

    //adjust for wind gusts
    //@TODO make gusts last a random number of ticks
    float harrier_gust = GetRandFloat(0, KnotsToMps(MAX_WIND_GUST));
    harrier_XVel -= harrier_gust * HARRIER_FRICTION;

    //adjust for ship speed
    harrier_XPos -= KnotsToMps(harrier_shipSpeed);
    *harrier_RelativeXVel = harrier_XVel - KnotsToMps(harrier_shipSpeed);

    //adjust pos
    harrier_XPos += harrier_XVel;
    harrier_YPos += *harrier_YVel;

    //re-calculate system status
    *harrier_height = harrier_YPos;
    *harrier_safeDist = harrier_XPos - harrier_safeX;

    //check if it has landed
    if(*harrier_height <= 0){
      return 0; //crashed
    }
    else if(*harrier_height <= harrier_safeX + 3){
      if(abs(*harrier_safeDist) < harrier_safeWidth){
        if(*harrier_RelativeXVel <= MAX_LANDING_SPEED_X && *harrier_YVel <= MAX_LANDING_SPEED_Y){
          harrier_landed = true;
          *harrier_score = harrier_fuel;
          return 1;
        }
        else{
          harrier_isBoom = true;
          *harrier_score = 0;
          return 0;
        }
      }
    }
    //@TODO: Add some functions to explode if the ship is hit 

  }
  else {
    return 0; //ran out of fuel
  }
}
float DegToRad(float deg){
  return (deg * 3.14159 / 180.0);
}
float KnotsToMps(float knots){
  return knots * 0.514;
}
float Apportion(float inLow, float inHigh, float in, float outLow, float outHigh){
  if(inHigh - inLow == 0)
    return 0;
  float value = (in - inLow) / (inHigh - inLow);
  return Lerp(1 - value, outLow, outHigh);
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
