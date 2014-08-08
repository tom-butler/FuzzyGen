#include <GL\freeglut.h>

#include "..\objects\shared.h"
#include "sims\harrier.h"
#include "sims\moon.h"
#include "..\gui\gui.h"



void DrawMoonSets(int window);
void DrawMoonSim(int window);

void DrawHarrierSets(int window);
void DrawHarrierSim(int window);

bool isInit = false;
int sets;
int vars;
int sim;

void DrawSim() {
  if(!isInit){
    sim = glutGetWindow();
    sets = glutCreateWindow("Sets");
    isInit = true;
  }
  if(SIM == MOONLANDER){
    DrawMoonSets(sets);
    DrawMoonSim(sim);
  }
  else if(SIM == HARRIER){
    DrawHarrierSets(sets);
    DrawHarrierSim(sim);
  }

}

void DrawMoonSets(int window) {
  glutSetWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);
  
  //Y
  DrawBestCollection(-1,0, 0.5f, "Y Velocity", cont[BEST_CONT].input[1]);
  DrawCollection(-1,0, 0.5f, "Y Velocity", cont[controller].input[1]);
  DrawPlot(-1, 0, 0.5f);
  DrawBestCollection(-1,0.5f, 0.5f, "Height", cont[BEST_CONT].input[0]);
  DrawCollection(-1,0.5f, 0.5f, "Height", cont[controller].input[0]);
  DrawPlot(-1, 0.5f, 0.5f);
  //rules
  DrawRules(-0.5f, 0.95f, controller, 0);
  //draw accumulator
  DrawPlot(-1, -0.999, 1);
  DrawAccumulator(-1, -0.999, "Throttle", cont[controller].output[0]);

  //X
  DrawBestCollection(0,0, 0.5f, "X Velocity", cont[BEST_CONT].input[2]);
  DrawCollection(0,0, 0.5f, "X Velocity", cont[controller].input[2]);
  DrawPlot(0, 0, 0.5f);
  DrawBestCollection(0,0.5f, 0.5f, "SafeDist", cont[BEST_CONT].input[3]);
  DrawCollection(0,0.5f, 0.5f, "SafeDist", cont[controller].input[3]);
  DrawPlot(0, 0.5f, 0.5f);
  //rules
  DrawRules(0.5f, 0.95f, controller, 1);
  //draw accumulator
  DrawPlot(0, -0.999, 1);
  DrawAccumulator(0, -0.999, "Thrust", cont[controller].output[1]);
}


void DrawMoonSim(int window) {
  glutSetWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);

  PrintFloat(0.5, 0.5f,"Generation",generation);
  PrintFloat(0.5, 0.45f,"Controller",controller);
  PrintFloat(0.5, 0.4f,"Mutations",cont[controller].mutations);
  PrintFloat(0.5, 0.35f,"Active Rules",cont[controller].output[0].active );
  PrintFloat(0.5, 0.3f,"Fuel",moon_fuel );
  PrintFloat(0.5, 0.25f,"Thrust", cont[controller].output[0].output);
  PrintFloat(0.5, 0.2f, "Sim Speed", 10 - (speed / 1000));
  PrintFloat(0.5, 0.15f, "BEST", BEST);
  PrintFloat(0.5, 0.1f, "MEAN", MEAN);
  PrintFloat(0.5, 0.05f, "LOW", LOW);

  //DRAW SIM
  float y = cont[controller].input[0].value - cont[controller].input[0].low;
  y /= cont[controller].input[0].high;
  float x = moon_landerX - 500;
  x /= 1000;

  float throttle = cont[controller].output[0].output;
  float thrust = cont[controller].output[1].output;

  //draw ground
  glColor3f(140.0f/255, 140.0f/255, 140.0f/255);

  glBegin(GL_POLYGON);
    glVertex2f(-1, -0.9f);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, -0.9f);
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
    glVertex2f(x,  y+0.2f);
    glVertex2f(x + 0.1f, y);

    glVertex2f(x,  y+0.07f);
    glVertex2f(x + 0.1f, y);

  //left
    glVertex2f(x,  y+0.2f);
    glVertex2f(x - 0.1f, y);

    glVertex2f(x,  y+0.07f);
    glVertex2f(x - 0.1f, y);
  glEnd();

  //hull

  glColor3f(98.0f/255, 98.0f/255, 98.0f/255);
  glBegin(GL_POLYGON);
    glVertex2f(x + 0.02f , y + 0.2f);
    glVertex2f(x - 0.02f , y + 0.2f);
    glVertex2f(x - 0.05f , y + 0.15f);
    glVertex2f(x - 0.03f , y + 0.05f);
    glVertex2f(x + 0.02f , y + 0.05f);
    glVertex2f(x + 0.05f , y + 0.15f);
  glEnd();
  glColor3f(90.0f/255, 90.0f/255, 90.0f/255);
  glBegin(GL_POLYGON);
    glVertex2f(x + 0.015f, y + 0.19f);
    glVertex2f(x - 0.015f, y + 0.19f);
    glVertex2f(x - 0.04f , y + 0.055f);
    glVertex2f(x + 0.06f , y + 0.055f);
  glEnd();
  glColor3f(184.0f/255, 134.0f/255, 11.0f/255);
  glBegin(GL_POLYGON);
    glVertex2f(x - 0.04f , y + 0.12f);
    glVertex2f(x + 0.04f , y + 0.12f);
    glVertex2f(x + 0.04f , y + 0.05f);
    glVertex2f(x - 0.04f , y + 0.05f);
  glEnd();


  //draw the thrust
  throttle /= 100;
  throttle -= 0.05f;
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.02f, y+0.05f);
    glVertex2f(x, y- throttle);
    glVertex2f(x + 0.02f, y+0.05f);
  glEnd();

  //draw the side thrusters
  float offset = 0.05f;
  if(thrust > 0.0f)
    offset = -offset;
  thrust /= 50.0f;
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_TRIANGLES);
    glVertex2f(x + offset, y + 0.05f);
    glVertex2f(x + offset - thrust, y);
    glVertex2f(x + offset, y - 0.05f);
  glEnd();


  //draw landing zone
  glBegin(GL_LINES);
    glVertex2f((moon_safeX - 500) / 1000, -1);
    glVertex2f((moon_safeX - 500) / 1000, -0.9);
  glEnd();
}

void DrawHarrierSets(int window) {
  glutSetWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);
  
  //Y
  DrawBestCollection(-1,0, 0.5f, "Height", cont[BEST_CONT].input[0]);
  DrawCollection(-1,0, 0.5f, "Height", cont[controller].input[0]);
  DrawPlot(-1, 0, 0.5f);
  DrawBestCollection(-1,0.5f, 0.5f, "Y Velocity", cont[BEST_CONT].input[1]);
  DrawCollection(-1,0.5f, 0.5f, "Y Velocity", cont[controller].input[1]);
  DrawPlot(-1, 0.5f, 0.5f);
  //rules
  DrawRules(-0.5f, 0.95f, controller, 0);
  //draw accumulator
  DrawPlot(-1, -0.999, 1);
  DrawAccumulator(-1, -0.999, "Throttle", cont[controller].output[0]);

  //X
  DrawBestCollection(0,0, 0.5f, "X Velocity", cont[BEST_CONT].input[2]);
  DrawCollection(0,0, 0.5f, "X Velocity", cont[controller].input[2]);
  DrawPlot(0, 0, 0.5f);
  DrawBestCollection(0,0.5f, 0.5f, "SafeDist", cont[BEST_CONT].input[3]);
  DrawCollection(0,0.5f, 0.5f, "SafeDist", cont[controller].input[3]);
  DrawPlot(0, 0.5f, 0.5f);
  //rules
  DrawRules(0.5f, 0.95f, controller, 1);
  //draw accumulator
  DrawPlot(0, -0.999, 1);
  DrawAccumulator(0, -0.999, "Vector", cont[controller].output[1]);
}

void DrawHarrierSim(int window) {
  glutSetWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);

  //vars
  PrintFloat(0.5, 0.5f,"Generation",generation);
  PrintFloat(0.5, 0.45f,"Controller",controller);
  PrintFloat(0.5, 0.4f,"Mutations",cont[controller].mutations);
  PrintFloat(0.5, 0.35f,"Active Rules",cont[controller].output[0].active );
  PrintFloat(0.5, 0.15f, "BEST", BEST);
  PrintFloat(0.5, 0.1f, "MEAN", MEAN);
  PrintFloat(0.5, 0.05f, "LOW", LOW);
  PrintFloat(0.5, 0.2f, "Sim Speed", 10 - (speed / 1000));

  PrintFloat(0.5, 0.3f,"Fuel",harrier_fuel );
  PrintFloat(0.5, 0.25f,"Throttle", cont[controller].output[0].output);

  //ocean
  glColor3f(24/256,64/256,64/256); //blue
  glBegin(GL_POLYGON);
    glVertex2f(-1,-0.99);
    glVertex2f( 1,-0.99);
    glVertex2f( 1,-1);
    glVertex2f( 1,-1);
  glEnd();

  //ship

  //float DrawSafeX = ConvertToSimScale(harrier_safeX, 0, HARRIER_SIM_WIDTH);
  //float DrawSafeY = ConvertToSimScale(harrier_safeY, 0, HARRIER_SIM_HEIGHT);
  //float DrawSafeWidth = ConvertToSimScale(harrier_safeWidth, 0, HARRIER_SIM_WIDTH);
/*

  glColor3f(0.3514f,0.3514f, 0.3514f); //dark grey
  glBegin(GL_POLYGON);
    glVertex2f(DrawSafeX - DrawSafeWidth/2, DrawSafeY);
    glVertex2f(DrawSafeX - DrawSafeWidth/2 *1.01, -1);
    glVertex2f(DrawSafeX + DrawSafeWidth/2, DrawSafeY);
    glVertex2f(DrawSafeX + DrawSafeWidth/2 *0.01, -1);
  glEnd();
  //landing
  glColor3f(1.0f,0,0); //red
  glBegin(GL_LINES);
    glVertex2f(DrawSafeX - DrawSafeWidth/2, DrawSafeY);
    glVertex2f(DrawSafeX + DrawSafeWidth/2, DrawSafeY);
  glEnd();*/
}