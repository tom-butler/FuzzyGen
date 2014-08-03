#include "moon.h"
#include "..\..\objects\shared.h"
#include "..\..\gui\gui.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

const int MAX_FORCE = 106000; //(newtons)
const int MAX_FUEL_BURN;
const int MIN_FUEL_BURN;
const float MAX_LANDING_SPEED_Y = 3.4f;
const float MAX_LANDING_SPEED_X = 3.4f;
const int SIM_HEIGHT = 200;
const int SIM_WIDTH = 600;
const int TERMINAL_VELOCITY = 100;
const int HARRIER_MASS = 5700 + 3500; //mass + missles
const int START_FUEL = 80;
const float GRAVITY = 9.8f;


//harrier status
float XPos;
float YPos;
float force;
float fuel;
float harrierMass;
bool isBoom;
bool landed;
bool groundEffect;
//ship
float safeX;
float safeWidth;
float shipSpeed;
//environment
float windSpeed;
float maxWindGust;
float windGustTarget;

//controller variables
float * height;
float * YVel;
float * XVel;
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
  XVel = &cont[controller].input[2].value;
  safeDist = &cont[controller].input[3].value;

  score = &cont[controller].score;

  if(RANDOM_START){
    //random pos
  }
  else{
    XPos = 15;
    YPos = 170;
    *vector = 92;
    *xVel = 23;
  }
  fuel = START_FUEL;
  *safeDist = safeX - landerX;
}

int HarrierNextStep(int controller) {

  //only move if we have fuel
  if(fuel > 0) {
    if(*throttle < 0)
      *throttle = 0;
    if(*throttle > 100)
      *throttle = 100;

    harrierMass = HARRIER_MASS + fuel;
    *YVel -= GRAVITY;

    float tFuel = (MAX_FUEL_BURN - MIN_FUEL_BURN) * (*throttle /100) + MIN_FUEL_BURN;
    fuel -= tFuel;

    if(fuel < 0)
      fuel = 0;

    force = *throttle * MAX_FORCE / 100 * 0.95; //0.95 allows for 5% cold air bleed
    if(fuel == 0)
      force = 0;

    float angle = 270  - vector;
    float xx;
    float yy;
    
  }
  else {
    return 0;
  }
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
