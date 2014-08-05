#include "moon.h"
#include "..\..\objects\shared.h"
#include "..\..\gui\gui.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

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
const int   SIM_HEIGHT = 200;
const int   SIM_WIDTH = 600;
const int   WIND_SPEED = 10;
const int   MAX_WIND_GUST = 20;

//harrier status
float XPos;
float YPos;
float force;
float fuel;
float harrierMass;
float XVel;

//simStatus
bool isBoom;
bool landed;
bool groundEffect;

//ship
float safeX;
float safeY;
float safeWidth;
float shipSpeed;

//controller variables
float * height;
float * YVel;
float * RelativeXVel;
float * safeDist;

float * throttle;
float * vector;

short int * score;

static FuzzyVar heightSet = {0, SIM_HEIGHT, SIM_HEIGHT, 0, 0};
static FuzzyVar YVelocitySet  = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, MAX_START_VEL, 0, 0};
static FuzzyVar XVelocitySet = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, MAX_START_VEL, 0, 0};
static FuzzyVar safeDistSet  = {-SIM_WIDTH/2, -SIM_WIDTH/2, 0, 0, 0};

//output
static Accumulator throttleAccumulator = {0, 100, 0.0f, 0, 0, 0, 0, 0, 0};
static Accumulator vectorAccumulator = {-5, 5, 0.0f, 0, 0, 0, 0, 0, 0};

void HarrierCreateVars(){
  //sim input vars
  NUM_INPUT = 4;
  simInput = new FuzzyVar[NUM_INPUT];

  simInput[0] = heightSet;
  simInput[1] = YVelocitySet;
  simInput[2] = XVelocitySet;
  simInput[3] = safeDistSet;

  //sim output vars
  NUM_OUTPUT = 2;
  simOutput = new Accumulator[NUM_OUTPUT];
  simOutput[0] = throttleAccumulator;
  simOutput[0].vars = new short int[4];
  simOutput[0].vars[0] = 0;
  simOutput[0].vars[1] = 1;
  simOutput[0].vars[2] = 2;
  simOutput[0].vars[3] = 3;
  simOutput[0].varsNum = 4;

  simOutput[1] = vectorAccumulator;
  simOutput[1].vars = new short int[4];
  simOutput[1].vars[0] = 0;
  simOutput[1].vars[1] = 1;
  simOutput[1].vars[2] = 2;
  simOutput[1].vars[3] = 3;
  simOutput[1].varsNum = 4;
}

void HarrierInitSim(int controller) {
  throttle = &cont[controller].output[0].output;
  vector = &cont[controller].output[1].output;

  height = &cont[controller].input[0].value;
  YVel = &cont[controller].input[1].value;
  RelativeXVel = &cont[controller].input[2].value;
  safeDist = &cont[controller].input[3].value;

  score = &cont[controller].score;

  if(RANDOM_START){
    //random pos
  }
  else{
    XPos = 15;
    YPos = 170;
    XVel = 46;
    YVel = 2;
    shipSpeed = 22;
    *vector = 92;
    *RelativeXVel = 22;
    *throttle = 89;
    groundEffect = true;
    safeX = 400;
    safeY = 16;
    safeWidth = 
  }
  force = 0.0f;
  fuel = START_FUEL;
  harrierMass = HARRIER_MASS + fuel;
  isBoom = false;
  landed = false;
  *safeDist = safeX - landerX;
}

int HarrierNextStep(int controller) {

  //only move if we have fuel
  if(fuel > 0) {
    if(*throttle < 0)
      *throttle = 0;
    if(*throttle > 100)
      *throttle = 100;

    //re-calculate harrier status
    harrierMass = HARRIER_MASS + fuel;
    *YVel -= GRAVITY;

    //burn fuel
    float tFuel = (MAX_FUEL_BURN - MIN_FUEL_BURN) * (*throttle /100) + MIN_FUEL_BURN;
    fuel -= tFuel;
    if(fuel < 0)
      fuel = 0;


    //adjust the velocity by thrust
    force = *throttle * MAX_FORCE / 100 * 0.95; //0.95 allows for 5% cold air bleed 
    if(fuel == 0)
      force = 0;

    float angle = DegToRad(270 - *vector);
    float xx = XPos + force * cos(angle);
    float yy = YPos + force * sin(angle);

    xx *= -1;
    yy *= -1;

    XVel += xx / harrierMass;
    *YVel += yy / harrierMass;

    //adjust velocity by ground effect
    if(groundEffect && YPos <= GROUND_EFFECT_HEIGHT + safeY){
      *YVel -= Apportion(safeY, GROUND_EFFECT_HEIGHT + safeY,YPos, 0, 0.5);
    }
    
    //adjust velocity by base wind
    XVel -= KnotsToMps(windSpeed) * HARRIER_FRICTION;

    //adjust for wind gusts
    //@TODO make gusts last a random number of ticks
    float gust = getRandomFloat(0, KnotsToMps(MAX_WIND_GUST));
    XVel -= gust * HARRIER_FRICTION;

    //adjust for ship speed
    XPos -= KnotsToMps(shipSpeed);
    *RelativeXVel = XVel - KnotsToMps(shipSpeed);

    //adjust pos
    XPos += XVel;
    YPos += *Yvel;

    //re-calculate system status
    *height = YPos;
    *safeDist = XPos - safeX;

    //check if it has landed
    if(*height <= 0){
      return 0; //crashed
    }
    else if(height <= safeX + 3){
      if(abs(safeDist) < safeWidth){
        if(RelativeXVel <= MAX_LANDING_SPEED_X && YVEL <= MAX_LANDING_SPEED_Y){
          landed = true;
          *score = fuel;
          return 1;
        }
        else{
          isBoom = true;
          *score = 0;
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
