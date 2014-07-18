#include "moon.h"
#include "..\objects\shared.h"
#include "..\gui\gui.h"
#include <iostream>
#include <GL\freeglut.h>
using namespace std;

//sim runtime vars
float * thrust;
float * height;
float * velocity;
short int * fuel;
static Accumulator thrustSet = {0, THRUST_MAX, 0.f, 0, 0, 0};
static FuzzyVar heightSet  = {0, START_HEIGHT, START_HEIGHT, 0};
static FuzzyVar velocitySet = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, START_VEL, 0};
static FuzzyVar fuelSet  = {0, START_FUEL, START_FUEL, 0};

void MoonCreateVars(){
    //sim init vars
  simInput[0] = heightSet;
  simInput[1] = velocitySet;
  simOutput = &thrustSet;
  simFitness = &fuelSet;
}

void MoonInitSim(int controller) {
  thrust = &cont[controller].output.output;
  height = &cont[controller].input[0].value;
  velocity = &cont[controller].input[1].value;
  fuel = &cont[controller].score;
}
int MoonNextStep(int controller) {
  if(*fuel > 0) {
    *fuel -= *thrust;
    *velocity = *velocity + FORCE;
    *velocity = *velocity - *thrust;
    if(*velocity < -TERMINAL_VELOCITY)
      *velocity = -TERMINAL_VELOCITY;
    if(*velocity > TERMINAL_VELOCITY)
      *velocity = TERMINAL_VELOCITY;


    *height -= *velocity;

    if(*height < 0.0f)
      *height = 0.0f;
    if(*height > START_HEIGHT)
      *height = START_HEIGHT;
    if(*fuel < 0)
      *fuel = 0.0f;

    if(*fuel == 0.0f) {
      *fuel = (START_HEIGHT - *height) / 10;
      return 0; //fail
    }
    else if(*height <= 0.0f && *velocity < CRASH_SPEED) {
      return 0; //succeed
    }
    else if(*height <= 0.0f) {
      *fuel = (START_HEIGHT - *height)/ 10 + (TERMINAL_VELOCITY - *velocity);
      return 0; //semi-fail
    }
    else {
      return -1; //continue
    }
  }
  else {
    return 0;
  }
}

void DrawMoonSim() {

  //DRAW PLOTS
  DrawPlot(0, 0);
  DrawBestCollection(0,0,"Height", cont[BEST_CONT].input[0]);
  DrawCollection(0,0,"Height", cont[controller].input[0]);
  DrawPlot(0, 0.5f);
  DrawBestCollection(0,0.5f,"Velocity", cont[BEST_CONT].input[1]);
  DrawCollection(0,0.5f,"Velocity", cont[controller].input[1]);

  //Draw diagnostics
  PrintFloat(0, -0.5f,"Generation",generation);
  PrintFloat(0, -0.55f,"Controller",controller);
  PrintFloat(0, -0.6f,"Mutations",cont[controller].mutations);
  PrintFloat(0, -0.65f,"Active Rules",cont[controller].output.active );
  PrintFloat(0, -0.7f,"Fuel",cont[controller].score );
  PrintFloat(0, -0.75f,"Thrust", cont[controller].output.output);
  PrintFloat(0, -0.8f, "Sim Speed", 10 - (speed / 1000));
  PrintFloat(0, -0.85f, "BEST", BEST);
  PrintFloat(0, -0.9f, "MEAN", MEAN);
  PrintFloat(0, -0.95f, "LOW", LOW);
  //rules
  DrawRules(0.3f, -0.45f, controller);
  //draw accumulator
  DrawPlot(0, -0.999);
  DrawAccumulator(0, -0.999, "Output", cont[controller].output);

  //DRAW SIM
  float y = cont[controller].input[0].value - cont[controller].input[0].low;
  y /= cont[controller].input[0].high;

  float thrust = cont[controller].output.output;

  //draw ground
  glColor3f(140.0f/255, 140.0f/255, 140.0f/255);

  glBegin(GL_POLYGON);
    glVertex2f(-1, -0.9f);
    glVertex2f(-1, -1);
    glVertex2f(-0.006f, -1);
    glVertex2f(-0.006f, -0.9f);
  glEnd();

  //draw the moonlander

  //we need to calculate the position from the 0-100 value
  y *= 2.0f;
  y -= 1.0f;

  //create the moonlander object

  //feet
    glColor3f(90.0f/255, 90.0f/255, 90.0f/255);
  glBegin(GL_LINES);
  //right
    glVertex2f(-0.5f,  y+0.2f);
    glVertex2f(-0.4f, y);

    glVertex2f(-0.5f,  y+0.07f);
    glVertex2f(-0.4f, y);

  //left
    glVertex2f(-0.5f,  y+0.2f);
    glVertex2f(-0.6f, y);

    glVertex2f(-0.5f,  y+0.07f);
    glVertex2f(-0.6f, y);
  glEnd();

  //hull

  glColor3f(98.0f/255, 98.0f/255, 98.0f/255);
  glBegin(GL_POLYGON);
    glVertex2f(-0.48f,  y+0.2f);
    glVertex2f(-0.52f,  y+0.2f);
    glVertex2f(-0.55f, y+0.15f);
    glVertex2f(-0.53f, y+0.05f);
    glVertex2f(-0.48f, y+0.05f);
    glVertex2f(-0.45f, y+0.15f);
  glEnd();
  glColor3f(90.0f/255, 90.0f/255, 90.0f/255);
  glBegin(GL_POLYGON);
    glVertex2f(-0.485f, y+0.19f);
    glVertex2f(-0.515f, y+0.19f);
    glVertex2f(-0.54f, y+0.055f);
    glVertex2f(-0.46f, y+0.055f);
  glEnd();
  glColor3f(184.0f/255, 134.0f/255, 11.0f/255);
  glBegin(GL_POLYGON);
    glVertex2f(-0.54f, y+0.12f);
    glVertex2f(-0.46f, y+0.12f);
    glVertex2f(-0.46f, y+0.05f);
    glVertex2f(-0.54f, y+0.05f);
  glEnd();


  //draw the thrust
  thrust /= THRUST_MAX;
  thrust -= 0.05f;
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_TRIANGLES);
    glVertex2f(-0.52f, y+0.05f);
    glVertex2f(-0.5f, y- thrust);
    glVertex2f(-0.48f, y+0.05f);
  glEnd();


}