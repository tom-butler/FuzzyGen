#include "moon.h"
#include "..\..\objects\shared.h"
#include "..\..\gui\gui.h"
#include <iostream>
#include <stdlib.h>
//http://www.control.isy.liu.se/student/tsrt03/files/invpendpmenglish.pdf
using namespace std;

//sim vars
const float CART_MASS = 0.5;
const float PENDULUM_MASS = 0.2;
const float FRICTION = 0.1;
const float INERTIA = 0.006;
const float PENDULUM_LENGTH = 0.3

const float GRAVITY = 9.81f;
const short int MAX_START_VEL = 3;
const short int MAX_START_DIST = 20;
const short int MAX_THRUST = 5;
const short int SIM_WIDTH = 1000;
static short int TERMINAL_VELOCITY = 10;

float * thrust;
float * angle;
float * centreDist;
float * velocity;
short int * score;

float pendulumPos;

//throttle accumulator
static FuzzyVar pendulumAngle  = {0, 180, 90, 0, 0};
static FuzzyVar centreDist = {-SIM_WIDTH/2, SIM_WIDTH/2, 0, 0, 0};
static FuzzyVar cartVelocity = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, 0, 0, 0};

//output
static Accumulator thrustSet = {-MAX_THRUST, MAX_THRUST, 0.0f, 0, 0, 0, 0, 0, 0};

void PendulumCreateVars(){
  //sim input vars
  NUM_INPUT = 3;
  simInput = new FuzzyVar[NUM_INPUT];

  simInput[0] = pendulumAngle;
  simInput[1] = centreDist;

  simInput[2] = cartVelocity;

  //sim output vars
  NUM_OUTPUT = 1;
  simOutput = new Accumulator[NUM_OUTPUT];
  simOutput[0] = thrustSet;
  simOutput[0].vars = new short int[3];
  simOutput[0].vars[0] = 0;
  simOutput[0].vars[1] = 1;
  simOutput[0].vars[1] = 2;
  simOutput[0].varsNum = 3;
}

void PendulumInitSim(int controller) {
  thrust = &cont[controller].output[0].output;

  angle = &cont[controller].input[0].value;
  velocity = &cont[controller].input[1].value;
  centreDist = &cont[controller].input[2].value;

  score = &cont[controller].score;

  if(RANDOM_START){
    *angle = 90 + GetRandInt(-10, 10);
    *velocity = GetRandFloat(-MAX_START_VEL, MAX_START_VEL);
    *centreDist = GetRandFloat(-MAX_START_DIST, MAX_START_DIST);
  }
  else{
    *angle = 90;
    *velocity = 0;
    *centreDist = 0;
  }
  *score = 0;
}

int PendulumNextStep(int controller) {

  if(angle > 0 && angle < 180) {
    float offset = GetRandFloat(0.0f,1.0f);
    *velocity -= FRICTION;
    *velocity += (*thrust) / CART_WEIGHT;

    //ensure it is within bounds
    if(*velocity < -TERMINAL_VELOCITY)
      *velocity = -TERMINAL_VELOCITY;
    if(*velocity > TERMINAL_VELOCITY)
      *velocity = TERMINAL_VELOCITY;

    //move
    *centreDist += velocity;
    

    *score += SIM_WIDTH - abs(centreDist);
    return -1
  }
  else {
    return 0;
  }
}

//manually created controller to prove the system works
void PendulumControlController(int controller) {

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
