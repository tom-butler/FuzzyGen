#include "pendulum.h"

#include <iostream>
#include <cmath>

#include "..\..\shared\shared.h"
#include "..\..\gui\gui.h"

//https://github.com/222464/ERL/blob/master/ERL/experiments/polebalancing.lua
using namespace std;

const float PI = 3.14159;

const float kPixelsPerMeter = 128.0;
const float kPoleLength = 1.0;
const float kGravity = -2.8;
const float kMassMass = 20.0;
const float kCartMass = 2.0;
const float kDT = 0.017;
const float kCartFriction = 0.02;
const float kMaxSpeed = 3.0;
const float kCartMoveRadius = 1.8;
const float kPoleRotationalFriction = 0.008;
const int   kMaxTime = 500;
const int kMaxPoleAngle = 91;
const int kMaxPoleVel = 50;
int ticks;
float cart_accel_x;
float pole_angle_accel;
float score_count;
//AI interface
float * agent_force;
float * cart_x;
float * cart_vel;
float * pole_angle;
float * pole_vel;
short int * pendulum_score;
//Input Sets
static FuzzyVar cart_x_set = {-kCartMoveRadius * 10, kCartMoveRadius * 10, 0.0f, 0, 0};
static FuzzyVar cart_vel_x = {-kMaxSpeed, kMaxSpeed, 0.0f, 0, 0};
static FuzzyVar pole_angle_set = {-kMaxPoleAngle, kMaxPoleAngle, 0.0f, 0, 0};
static FuzzyVar pole_angle_vel_set = {-kMaxPoleVel, kMaxPoleVel, 0.0f, 0, 0};

//Output Sets
static Accumulator agent_force_set = {-1, 1, 0.0f, 0, 0, 0, 0, 0, 0};

/**
 * Initialise the simulation
 * Called once each test
 */
void PendulumCreateVars(){
  //sim input vars
  kNumInput = 4;
  simInput = new FuzzyVar[kNumInput];

  simInput[0] = cart_x_set;
  simInput[1] = cart_vel_x;
  simInput[2] = pole_angle_set;
  simInput[3] = pole_angle_vel_set;

  //sim output vars
  kNumOutput = 1;
  simOutput = new Accumulator[kNumOutput];
  simOutput[0] = agent_force_set;
  simOutput[0].vars = new short int[4];
  simOutput[0].vars[0] = 0;
  simOutput[0].vars[1] = 1;
  simOutput[0].vars[2] = 2;
  simOutput[0].vars[3] = 3;
  simOutput[0].num_vars = 4;

}

/**
 * Initialise the simulation for this controller
 * Called for each controller each generation
 * @param controller controller to be tested
 */
void PendulumInitSim(int controller) {

  agent_force = &cont[controller].output[0].output;
  cart_x = &cont[controller].input[0].value;
  cart_vel = &cont[controller].input[1].value;
  pole_angle = &cont[controller].input[2].value;
  pole_vel = &cont[controller].input[3].value;
  pendulum_score = &cont[controller].score;

  //random
  if(kRandomStart) {
    //give us a random location in the outer quarters 
    //to minimise the system landing on 0 and getting a high score doing nothing
    *cart_x = GetRandFloat((kCartMoveRadius * 10)* 0.5, (kCartMoveRadius * 10));
    if(GetRandInt(0, 1) == 0)
      *cart_x *= -1;

    *pole_angle = GetRandFloat(-kMaxPoleAngle * 0.3,kMaxPoleAngle * 0.3 );
  }
  else {
    //not random
    *cart_x = 0.0;
  }


  *cart_vel = 0.0;
  *pole_angle = 0.0;
  *pole_vel = 0.0;  
  *pendulum_score = 0;
  ticks = 0;
  cart_accel_x = 0.0;
  pole_angle_accel = 0.0;
  score_count = 0.0f;
}

/**
 * Takes the next step in testing the controller
 * @param  controller controller being tested
 * @return            -1: continue, 0: success, 1: failed
 */
int PendulumNextStep(int controller) {
  ticks++;
  if(ticks < kMaxTime){

    *cart_x /= 10;
    *agent_force *= 4000.0f;
    float pendulum_cart_accel_x = cart_accel_x;
    float pole_angle_rad = DegToRad(*pole_angle);

    if (pole_angle_rad < 0.0f)
      pole_angle_rad += PI * 2.0f;
    if (pole_angle_rad > 2 * PI)
      pole_angle_rad -= 2 * PI;

    if (*cart_x < -kCartMoveRadius || *cart_x > kCartMoveRadius){
      pendulum_cart_accel_x = 0.0;
      *cart_vel = 0.0f;
    }

    

    pole_angle_accel = pendulum_cart_accel_x * cos(pole_angle_rad) + kGravity * sin(pole_angle_rad);
    *pole_vel = *pole_vel - kPoleRotationalFriction * *pole_vel + pole_angle_accel * kDT;
    pole_angle_rad = pole_angle_rad + *pole_vel * kDT;

    float force = 0.0f;

    if (*cart_vel < kMaxSpeed && *cart_vel > -kMaxSpeed)
      force = max(-4000.0f, min(4000.0f, *agent_force));

    if (*cart_x < -kCartMoveRadius){
      *cart_x = -kCartMoveRadius;

      cart_accel_x = -*cart_vel / kDT;
      *cart_vel = -0.5f * *cart_vel;;
    }
    else if (*cart_x > kCartMoveRadius) {
      *cart_x = kCartMoveRadius;

      cart_accel_x = -*cart_vel / kDT;
      *cart_vel = -0.5f * *cart_vel;
    }

    cart_accel_x = 0.25f * (force + kMassMass * kPoleLength * pole_angle_accel * cos(pole_angle_rad) - kMassMass * kPoleLength * *pole_vel * *pole_vel * sin(pole_angle_rad)) / (kMassMass + kCartMass);
    *cart_vel = *cart_vel - kCartFriction * *cart_vel + cart_accel_x * kDT;
    *cart_x = *cart_x + *cart_vel * kDT;

    pole_angle_rad = fmod(pole_angle_rad, 2.0f * PI);

    if (pole_angle_rad < 0.0f)
      pole_angle_rad += PI * 2.0f;
    if (pole_angle_rad > 2 * PI)
      pole_angle_rad -= 2 * PI;


    if(pole_angle_rad > 1.5f * PI)
      *pole_angle = RadToDeg(pole_angle_rad - (2 * PI));
    else
      *pole_angle = RadToDeg(pole_angle_rad);

    *cart_x *= 10;
    ForceBounds(*cart_x, -kCartMoveRadius * 10, kCartMoveRadius * 10);
    ForceBounds(*cart_vel, -kMaxSpeed, kMaxSpeed);
    ForceBounds(*pole_angle, -kMaxPoleAngle, kMaxPoleAngle);
    ForceBounds(*pole_vel, -kMaxPoleVel, kMaxPoleVel);
    *agent_force /= 4000.0f;
    score_count += 0.1 * (((kCartMoveRadius * 10) - abs(*cart_x)) / kCartMoveRadius) ;
    //failed
    if(pole_angle_rad > (0.5f * PI) && pole_angle_rad < (1.5f * PI)) {
      *pendulum_score = (score_count / kMaxTime) * 100;
      return 1;
    }
    //if it hits the wall it fails
    if (*cart_x <= -kCartMoveRadius * 10 || *cart_x >= kCartMoveRadius * 10){
      *pendulum_score = (score_count / kMaxTime) * 100;
      return 1;
    }
    return -1;
  }
  else {
    *pendulum_score = (score_count / kMaxTime) * 100;
    return 0;
  }
}

/**
 * Manually create a controller, to prove system works
 * @param controller controller id to be created
 * @param input      input array
 * @param output     output array
 */
void PendulumControlController(int controller, FuzzyVar input[], Accumulator output[]) {

}
