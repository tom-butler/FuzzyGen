#include <iostream>
#include <cmath>

#include "..\..\shared\shared.h"
#include "..\..\gui\gui.h"

#include "pendulum.h"
//http://www.control.isy.liu.se/student/tsrt03/files/invpendpmenglish.pdf
using namespace std;

//sim vars
const float CART_MASS = 10.0;
const float PENDULUM_MASS = 1.0;
float ROD_LENGTH = 1.0;
const float GRAVITY = 9.8f;
const float LIN_FRICTION = 0.0;
const float ANG_FRICTION = 1.0;
const float INERTIA = 0.006;

float pendulum_currentForce = 0.0;
float pendulum_previousForce = 0.0;
float pendulum_angularPosition = 0.1;
float pendulum_angularVelocity = 0.0;
float pendulum_cartPosition = 0.0;
float pendulum_cartVelocity = 0.0;
float pendulum_timeStep = 0.1;
float pendulum_timeTag = 0.0;


const float MAX_START_VEL = 0.3;
const float MAX_START_DIST = 0.5f;
const short int MIN_PENDULUM_ANGLE = -90;
const short int MAX_PENDULUM_ANGLE = 90;
const short int MAX_THRUST = 10;
short int PENDULUM_SIM_WIDTH = 1000;
static short int TERMINAL_VELOCITY = 100;

float * thrust;
float * angle;
float * centreDist;
float * velocity;
short int * score;

short int pendulum_score;

float pendulumPos;

//throttle accumulator
static FuzzyVar pendulumAngleSet  = {MIN_PENDULUM_ANGLE, MAX_PENDULUM_ANGLE, 0.1f, 0, 0};
static FuzzyVar centreDistSet = {-PENDULUM_SIM_WIDTH/2, PENDULUM_SIM_WIDTH/2, 0, 0, 0};
static FuzzyVar cartVelocitySet = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, 0, 0, 0};

//output
static Accumulator thrustSet = {-MAX_THRUST, MAX_THRUST, 0.0f, 0, 0, 0, 0, 0, 0};
void IntegrateForwardEuler(double step);
void IntegrateForwardRungeKutta4(double step);
void PendulumCreateVars(){
  //sim input vars
  NUM_INPUT = 3;
  simInput = new FuzzyVar[NUM_INPUT];

  simInput[0] = pendulumAngleSet;
  simInput[1] = centreDistSet;
  simInput[2] = cartVelocitySet;

  //sim output vars
  NUM_OUTPUT = 1;
  simOutput = new Accumulator[NUM_OUTPUT];
  simOutput[0] = thrustSet;
  simOutput[0].vars = new short int[3];
  simOutput[0].vars[0] = 0;
  simOutput[0].vars[1] = 1;
  simOutput[0].vars[2] = 2;
  simOutput[0].varsNum = 3;
}

void PendulumInitSim(int controller) {
  thrust = &cont[controller].output[0].output ;

  angle = &cont[controller].input[0].value;
  centreDist = &cont[controller].input[1].value;
  velocity = &cont[controller].input[2].value;

  score = &cont[controller].score;
  pendulum_currentForce = 0;
  pendulum_previousForce = 0;
  pendulum_cartVelocity = 0;
  pendulum_angularPosition = 0;
  pendulum_angularVelocity = 0;
  pendulum_cartPosition = 0;
  pendulum_timeTag = 0;
  if(RANDOM_START){
    *angle = 90 + GetRandInt(-MAX_THRUST, MAX_THRUST);
    *velocity = GetRandFloat(-MAX_START_VEL, MAX_START_VEL);
    *centreDist = 0;//GetRandFloat(-MAX_START_DIST, MAX_START_DIST);
  }
  else{
    *angle = 0;// + GetRandInt(-5, 5);
    *velocity = 0;
    *centreDist = 0;
  }
  pendulum_score = 0;

      pendulum_angularPosition = DegToRad(*angle);
      pendulum_cartPosition = *centreDist ;
}

int PendulumNextStep(int controller) {
  if(*angle > -90 && *angle < 90 ) {
    if(pendulum_timeTag < 10){

      pendulum_currentForce = *thrust;
      IntegrateForwardEuler(pendulum_timeStep);

      //pendulum_angularPosition += DegToRad(GetRandFloat(-1,1));
      *angle = RadToDeg(pendulum_angularPosition);
      *centreDist = pendulum_cartPosition;
      *velocity = pendulum_cartVelocity ;

      //cap it at the bounds
      ForceBounds(*angle,MIN_PENDULUM_ANGLE,MAX_PENDULUM_ANGLE);
      ForceBounds(*centreDist, -1, 1);
      ForceBounds(*velocity, -TERMINAL_VELOCITY, TERMINAL_VELOCITY);

      //move 
      pendulum_score += (PENDULUM_SIM_WIDTH/2 - abs(*centreDist)) /100;
      return -1;
    }
    else {
      *score = pendulum_score;
      return 0;
    }
  }
  else {
    *score = pendulum_score;
    return 1;
  }
}
void IntegrateForwardEuler(double step)
{
  double m=PENDULUM_MASS, M=CART_MASS, l=ROD_LENGTH, f_lin=LIN_FRICTION, f_ang=ANG_FRICTION, g=GRAVITY, h=step;
  double z1=0.0, z2=0.0, z3=0.0, z4=0.0;

  // Integration using Forward Euler.
  z1 = pendulum_angularPosition + h*pendulum_angularVelocity;
  z2 = pendulum_angularVelocity + h*(pendulum_previousForce*cos(pendulum_angularPosition) - (M+m)*g*sin(pendulum_angularPosition) + m*l*cos(pendulum_angularPosition)*sin(pendulum_angularPosition)*sqr(pendulum_angularVelocity) + f_lin*cos(pendulum_angularPosition)*pendulum_cartVelocity + (M+m)*f_ang/m*pendulum_angularVelocity)/(m*l*sqr(cos(pendulum_angularPosition)) - (M+m)*l);
  z3 = pendulum_cartPosition + h*pendulum_cartVelocity;
  z4 = pendulum_cartVelocity + h*(pendulum_previousForce + m*l*sin(pendulum_angularPosition)*sqr(pendulum_angularVelocity) - m*g*cos(pendulum_angularPosition)*sin(pendulum_angularPosition) + cos(pendulum_angularPosition)*f_ang*pendulum_angularVelocity)/(M + m - m*sqr(cos(pendulum_angularPosition)));

  pendulum_angularPosition = z1;
  pendulum_angularVelocity = z2;
  pendulum_cartPosition = z3;
  pendulum_cartVelocity = z4;
  pendulum_previousForce = pendulum_currentForce;
  pendulum_timeTag += pendulum_timeStep;
}

void IntegrateForwardRungeKutta4(double step)
{
  double K1=0.0f, K2=0.0f, K3=0.0f, K4=0.0f;
  double L1=0.0f, L2=0.0f, L3=0.0f, L4=0.0f;
  double M1=0.0f, M2=0.0f, M3=0.0f, M4=0.0f;
  double N1=0.0f, N2=0.0f, N3=0.0f, N4=0.0f;
  double m=PENDULUM_MASS, M=CART_MASS, l=ROD_LENGTH, f_lin=LIN_FRICTION, f_ang=ANG_FRICTION, g=GRAVITY, h=step;
  double z1=0.0, z2=0.0, z3=0.0, z4=0.0;

  // Integration using Forward Runge-Kutta.
  K1 = pendulum_angularVelocity;
  L1 = (pendulum_previousForce*cos(pendulum_angularPosition) - (M+m)*g*sin(pendulum_angularPosition) + m*l*cos(pendulum_angularPosition)*sin(pendulum_angularPosition)*sqr(pendulum_angularVelocity) + f_lin*cos(pendulum_angularPosition)*pendulum_cartVelocity + (M+m)*f_ang/m*pendulum_angularVelocity)/(m*l*sqr(cos(pendulum_angularPosition)) - (M+m)*l);
  M1 = pendulum_cartVelocity;
  N1 = (pendulum_previousForce + m*l*sin(pendulum_angularPosition)*sqr(pendulum_angularVelocity) - m*g*cos(pendulum_angularPosition)*sin(pendulum_angularPosition) + cos(pendulum_angularPosition)*f_ang*pendulum_angularVelocity)/(M+m - m*sqr(cos(pendulum_angularPosition)));
    
  K2 = pendulum_angularVelocity + h/2.0*L1;
  L2 = ((pendulum_previousForce+pendulum_currentForce)/2.0*cos(pendulum_angularPosition+h/2.0*K1) - (M+m)*g*sin(pendulum_angularPosition+h/2.0*K1) + m*l*cos(pendulum_angularPosition+h/2.0*K1)*sin(pendulum_angularPosition+h/2.0*K1)*sqr(pendulum_angularVelocity+h/2.0*L1) + f_lin*cos(pendulum_angularPosition+h/2.0*K1)*(pendulum_cartVelocity+h/2.0*N1) + (M+m)*f_ang/m*(pendulum_angularVelocity+h/2.0*L1))/(m*l*sqr(cos(pendulum_angularPosition+h/2.0*K1)) - (M+m)*l);
  M2 = pendulum_cartVelocity + h/2.0*N1;
  N2 = ((pendulum_previousForce+pendulum_currentForce)/2.0 + m*l*sin(pendulum_angularPosition+h/2.0*K1)*sqr(pendulum_angularVelocity+h/2.0*L1) - m*g*cos(pendulum_angularPosition+h/2.0*K1)*sin(pendulum_angularPosition+h/2.0*K1) + cos(pendulum_angularPosition+h/2.0*K1)*f_ang*(pendulum_angularVelocity+h/2.0*L1))/(M+m - m*sqr(cos(pendulum_angularPosition+h/2.0*K1)));
    
  K3 = pendulum_angularVelocity + h/2.0*L2;
  L3 = ((pendulum_previousForce+pendulum_currentForce)/2.0*cos(pendulum_angularPosition+h/2.0*K2) - (M+m)*g*sin(pendulum_angularPosition+h/2.0*K2) + m*l*cos(pendulum_angularPosition+h/2.0*K2)*sin(pendulum_angularPosition+h/2.0*K2)*sqr(pendulum_angularVelocity+h/2.0*L2) + f_lin*cos(pendulum_angularPosition+h/2.0*K2)*(pendulum_cartVelocity+h/2.0*N2) + (M+m)*f_ang/m*(pendulum_angularVelocity+h/2.0*L2))/(m*l*sqr(cos(pendulum_angularPosition+h/2.0*K2)) - (M+m)*l);
  M3 = pendulum_cartVelocity + h/2.0*N2;
  N3 = ((pendulum_previousForce+pendulum_currentForce)/2.0 + m*l*sin(pendulum_angularPosition+h/2.0*K2)*sqr(pendulum_angularVelocity+h/2.0*L2) - m*g*cos(pendulum_angularPosition+h/2.0*K2)*sin(pendulum_angularPosition+h/2.0*K2) + cos(pendulum_angularPosition+h/2.0*K2)*f_ang*(pendulum_angularVelocity+h/2.0*L2))/(M+m - m*sqr(cos(pendulum_angularPosition+h/2.0*K2)));
    
  K4 = pendulum_angularVelocity + h*L3;
  L4 = (pendulum_previousForce *cos(pendulum_angularPosition+h*K3) - (M+m)*g*sin(pendulum_angularPosition+h*K3) + m*l*cos(pendulum_angularPosition+h*K3)*sin(pendulum_angularPosition+h*K3)*sqr(pendulum_angularVelocity+h*L3) + f_lin*cos(pendulum_angularPosition+h*K3)*(pendulum_cartVelocity+h*N3) + (M+m)*f_ang/m*(pendulum_angularVelocity+h*L3))/(m*l*sqr(cos(pendulum_angularPosition+h*K3)) - (M+m)*l);
  M4 = pendulum_cartVelocity + h*N3;
  N4 = (pendulum_previousForce + m*l*sin(pendulum_angularPosition+h*K3)*sqr(pendulum_angularVelocity+h*L3) - m*g*cos(pendulum_angularPosition+h*K3)*sin(pendulum_angularPosition+h*K3) + cos(pendulum_angularPosition+h*K3)*f_ang*(pendulum_angularVelocity+h*L3))/(M+m - m*sqr(cos(pendulum_angularPosition+h*K3)));

  z1 = pendulum_angularPosition + h*(1.0/6.0*K1 + 2.0/6.0*K2 + 2.0/6.0*K3 + 1.0/6.0*K4);
  z2 = pendulum_angularVelocity + h*(1.0/6.0*L1 + 2.0/6.0*L2 + 2.0/6.0*L3 + 1.0/6.0*L4);
  z3 = pendulum_cartPosition    + h*(1.0/6.0*M1 + 2.0/6.0*M2 + 2.0/6.0*M3 + 1.0/6.0*M4);
  z4 = pendulum_cartVelocity    + h*(1.0/6.0*N1 + 2.0/6.0*N2 + 2.0/6.0*N3 + 1.0/6.0*N4);

  pendulum_angularPosition = z1;
  pendulum_angularVelocity = z2;
  pendulum_cartPosition = z3;
  pendulum_cartVelocity = z4;
  pendulum_previousForce = pendulum_currentForce;
  pendulum_timeTag += pendulum_timeStep;
}
//manually created controller to prove the system works
void PendulumControlController(int controller) {

}
