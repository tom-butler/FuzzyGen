#include "pendulum.h"

#include <iostream>
#include <cmath>

#include "..\..\shared\shared.h"
#include "..\..\gui\gui.h"

//http://www.control.isy.liu.se/student/tsrt03/files/invpendpmenglish.pdf
using namespace std;

//sim vars
const float kCartMass = 10.0;
const float kPendulumMass = 1.0;
float kRodLength = 1.0;
const float kGravity = 9.8f;
const float kLinearFriction = 0.0;
const float kAngularFriction = 1.0;
const float kInertia = 0.006;

float pendulum_current_force = 0.0;
float pendulum_previous_force = 0.0;
float pendulum_angular_position = 0.1;
float pendulum_angular_velocity = 0.0;
float pendulum_cart_position = 0.0;
float pendulum_cart_velocity = 0.0;
float pendulum_time_step = 0.1;
float pendulum_time_tag = 0.0;


const float kMaxStartVelocity = 0.3;
const float kMaxStartDistance = 0.5f;
const short int kPendulumAngleMin = -90;
const short int kPendulumAngleMax = 90;
const short int kThrustMax = 10;
short int kPendulumSimWidth = 1000;
static short int kTerminalVelocity = 100;

float * thrust;
float * angle;
float * centre_dist;
float * velocity;
short int * score;

short int pendulum_score;

float pendulum_pos;

//throttle accumulator
static FuzzyVar pendulum_angle_set  = {kPendulumAngleMin, kPendulumAngleMax, 0.1f, 0, 0};
static FuzzyVar centre_distance_set = {-kPendulumSimWidth/2, kPendulumSimWidth/2, 0, 0, 0};
static FuzzyVar cart_velocity_set = {-kTerminalVelocity, kTerminalVelocity, 0, 0, 0};

//output
static Accumulator thrust_set = {-kThrustMax, kThrustMax, 0.0f, 0, 0, 0, 0, 0, 0};
void IntegrateForwardEuler(double step);
void IntegrateForwardRungeKutta4(double step);
void PendulumCreateVars(){
  //sim input vars
  kNumInput = 3;
  simInput = new FuzzyVar[kNumInput];

  simInput[0] = pendulum_angle_set;
  simInput[1] = centre_distance_set;
  simInput[2] = cart_velocity_set;

  //sim output vars
  kNumOutput = 1;
  simOutput = new Accumulator[kNumOutput];
  simOutput[0] = thrust_set;
  simOutput[0].vars = new short int[3];
  simOutput[0].vars[0] = 0;
  simOutput[0].vars[1] = 1;
  simOutput[0].vars[2] = 2;
  simOutput[0].num_vars = 3;
}

void PendulumInitSim(int controller) {
  thrust = &cont[controller].output[0].output ;

  angle = &cont[controller].input[0].value;
  centre_dist = &cont[controller].input[1].value;
  velocity = &cont[controller].input[2].value;

  score = &cont[controller].score;
  pendulum_current_force = 0;
  pendulum_previous_force = 0;
  pendulum_cart_velocity = 0;
  pendulum_angular_position = 0;
  pendulum_angular_velocity = 0;
  pendulum_cart_position = 0;
  pendulum_time_tag = 0;
  if(kRandomStart){
    *angle = 90 + GetRandInt(-kThrustMax, kThrustMax);
    *velocity = GetRandFloat(-kMaxStartVelocity, kMaxStartVelocity);
    *centre_dist = 0;//GetRandFloat(-kMaxStartDistance, kMaxStartDistance);
  }
  else{
    *angle = 0;// + GetRandInt(-5, 5);
    *velocity = 0;
    *centre_dist = 0;
  }
  pendulum_score = 0;

      pendulum_angular_position = DegToRad(*angle);
      pendulum_cart_position = *centre_dist ;
}

int PendulumNextStep(int controller) {
  if(*angle > -90 && *angle < 90 ) {
    if(pendulum_time_tag < 10){

      pendulum_current_force = *thrust;
      IntegrateForwardEuler(pendulum_time_step);

      //pendulum_angular_position += DegToRad(GetRandFloat(-1,1));
      *angle = RadToDeg(pendulum_angular_position);
      *centre_dist = pendulum_cart_position;
      *velocity = pendulum_cart_velocity ;

      //cap it at the bounds
      ForceBounds(*angle,kPendulumAngleMin,kPendulumAngleMax);
      ForceBounds(*centre_dist, -1, 1);
      ForceBounds(*velocity, -kTerminalVelocity, kTerminalVelocity);

      //move 
      pendulum_score += (kPendulumSimWidth/2 - abs(*centre_dist)) /100;
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
  double m=kPendulumMass, M=kCartMass, l=kRodLength, f_lin=kLinearFriction, f_ang=kAngularFriction, g=kGravity, h=step;
  double z1=0.0, z2=0.0, z3=0.0, z4=0.0;

  // Integration using Forward Euler.
  z1 = pendulum_angular_position + h*pendulum_angular_velocity;
  z2 = pendulum_angular_velocity + h*(pendulum_previous_force*cos(pendulum_angular_position) - (M+m)*g*sin(pendulum_angular_position) + m*l*cos(pendulum_angular_position)*sin(pendulum_angular_position)*sqr(pendulum_angular_velocity) + f_lin*cos(pendulum_angular_position)*pendulum_cart_velocity + (M+m)*f_ang/m*pendulum_angular_velocity)/(m*l*sqr(cos(pendulum_angular_position)) - (M+m)*l);
  z3 = pendulum_cart_position + h*pendulum_cart_velocity;
  z4 = pendulum_cart_velocity + h*(pendulum_previous_force + m*l*sin(pendulum_angular_position)*sqr(pendulum_angular_velocity) - m*g*cos(pendulum_angular_position)*sin(pendulum_angular_position) + cos(pendulum_angular_position)*f_ang*pendulum_angular_velocity)/(M + m - m*sqr(cos(pendulum_angular_position)));

  pendulum_angular_position = z1;
  pendulum_angular_velocity = z2;
  pendulum_cart_position = z3;
  pendulum_cart_velocity = z4;
  pendulum_previous_force = pendulum_current_force;
  pendulum_time_tag += pendulum_time_step;
}

void IntegrateForwardRungeKutta4(double step)
{
  double K1=0.0f, K2=0.0f, K3=0.0f, K4=0.0f;
  double L1=0.0f, L2=0.0f, L3=0.0f, L4=0.0f;
  double M1=0.0f, M2=0.0f, M3=0.0f, M4=0.0f;
  double N1=0.0f, N2=0.0f, N3=0.0f, N4=0.0f;
  double m=kPendulumMass, M=kCartMass, l=kRodLength, f_lin=kLinearFriction, f_ang=kAngularFriction, g=kGravity, h=step;
  double z1=0.0, z2=0.0, z3=0.0, z4=0.0;

  // Integration using Forward Runge-Kutta.
  K1 = pendulum_angular_velocity;
  L1 = (pendulum_previous_force*cos(pendulum_angular_position) - (M+m)*g*sin(pendulum_angular_position) + m*l*cos(pendulum_angular_position)*sin(pendulum_angular_position)*sqr(pendulum_angular_velocity) + f_lin*cos(pendulum_angular_position)*pendulum_cart_velocity + (M+m)*f_ang/m*pendulum_angular_velocity)/(m*l*sqr(cos(pendulum_angular_position)) - (M+m)*l);
  M1 = pendulum_cart_velocity;
  N1 = (pendulum_previous_force + m*l*sin(pendulum_angular_position)*sqr(pendulum_angular_velocity) - m*g*cos(pendulum_angular_position)*sin(pendulum_angular_position) + cos(pendulum_angular_position)*f_ang*pendulum_angular_velocity)/(M+m - m*sqr(cos(pendulum_angular_position)));
    
  K2 = pendulum_angular_velocity + h/2.0*L1;
  L2 = ((pendulum_previous_force+pendulum_current_force)/2.0*cos(pendulum_angular_position+h/2.0*K1) - (M+m)*g*sin(pendulum_angular_position+h/2.0*K1) + m*l*cos(pendulum_angular_position+h/2.0*K1)*sin(pendulum_angular_position+h/2.0*K1)*sqr(pendulum_angular_velocity+h/2.0*L1) + f_lin*cos(pendulum_angular_position+h/2.0*K1)*(pendulum_cart_velocity+h/2.0*N1) + (M+m)*f_ang/m*(pendulum_angular_velocity+h/2.0*L1))/(m*l*sqr(cos(pendulum_angular_position+h/2.0*K1)) - (M+m)*l);
  M2 = pendulum_cart_velocity + h/2.0*N1;
  N2 = ((pendulum_previous_force+pendulum_current_force)/2.0 + m*l*sin(pendulum_angular_position+h/2.0*K1)*sqr(pendulum_angular_velocity+h/2.0*L1) - m*g*cos(pendulum_angular_position+h/2.0*K1)*sin(pendulum_angular_position+h/2.0*K1) + cos(pendulum_angular_position+h/2.0*K1)*f_ang*(pendulum_angular_velocity+h/2.0*L1))/(M+m - m*sqr(cos(pendulum_angular_position+h/2.0*K1)));
    
  K3 = pendulum_angular_velocity + h/2.0*L2;
  L3 = ((pendulum_previous_force+pendulum_current_force)/2.0*cos(pendulum_angular_position+h/2.0*K2) - (M+m)*g*sin(pendulum_angular_position+h/2.0*K2) + m*l*cos(pendulum_angular_position+h/2.0*K2)*sin(pendulum_angular_position+h/2.0*K2)*sqr(pendulum_angular_velocity+h/2.0*L2) + f_lin*cos(pendulum_angular_position+h/2.0*K2)*(pendulum_cart_velocity+h/2.0*N2) + (M+m)*f_ang/m*(pendulum_angular_velocity+h/2.0*L2))/(m*l*sqr(cos(pendulum_angular_position+h/2.0*K2)) - (M+m)*l);
  M3 = pendulum_cart_velocity + h/2.0*N2;
  N3 = ((pendulum_previous_force+pendulum_current_force)/2.0 + m*l*sin(pendulum_angular_position+h/2.0*K2)*sqr(pendulum_angular_velocity+h/2.0*L2) - m*g*cos(pendulum_angular_position+h/2.0*K2)*sin(pendulum_angular_position+h/2.0*K2) + cos(pendulum_angular_position+h/2.0*K2)*f_ang*(pendulum_angular_velocity+h/2.0*L2))/(M+m - m*sqr(cos(pendulum_angular_position+h/2.0*K2)));
    
  K4 = pendulum_angular_velocity + h*L3;
  L4 = (pendulum_previous_force *cos(pendulum_angular_position+h*K3) - (M+m)*g*sin(pendulum_angular_position+h*K3) + m*l*cos(pendulum_angular_position+h*K3)*sin(pendulum_angular_position+h*K3)*sqr(pendulum_angular_velocity+h*L3) + f_lin*cos(pendulum_angular_position+h*K3)*(pendulum_cart_velocity+h*N3) + (M+m)*f_ang/m*(pendulum_angular_velocity+h*L3))/(m*l*sqr(cos(pendulum_angular_position+h*K3)) - (M+m)*l);
  M4 = pendulum_cart_velocity + h*N3;
  N4 = (pendulum_previous_force + m*l*sin(pendulum_angular_position+h*K3)*sqr(pendulum_angular_velocity+h*L3) - m*g*cos(pendulum_angular_position+h*K3)*sin(pendulum_angular_position+h*K3) + cos(pendulum_angular_position+h*K3)*f_ang*(pendulum_angular_velocity+h*L3))/(M+m - m*sqr(cos(pendulum_angular_position+h*K3)));

  z1 = pendulum_angular_position + h*(1.0/6.0*K1 + 2.0/6.0*K2 + 2.0/6.0*K3 + 1.0/6.0*K4);
  z2 = pendulum_angular_velocity + h*(1.0/6.0*L1 + 2.0/6.0*L2 + 2.0/6.0*L3 + 1.0/6.0*L4);
  z3 = pendulum_cart_position    + h*(1.0/6.0*M1 + 2.0/6.0*M2 + 2.0/6.0*M3 + 1.0/6.0*M4);
  z4 = pendulum_cart_velocity    + h*(1.0/6.0*N1 + 2.0/6.0*N2 + 2.0/6.0*N3 + 1.0/6.0*N4);

  pendulum_angular_position = z1;
  pendulum_angular_velocity = z2;
  pendulum_cart_position = z3;
  pendulum_cart_velocity = z4;
  pendulum_previous_force = pendulum_current_force;
  pendulum_time_tag += pendulum_time_step;
}
//manually created controller to prove the system works
void PendulumControlController(int controller) {

}
