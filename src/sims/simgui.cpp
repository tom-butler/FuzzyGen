#include "..\objects\shared.h"
#include "..\gui\gui.h"
#include "sims\moon.h"
#include <GL\freeglut.h>

void DrawMoonSets(int window);
void DrawMoonSim(int window);
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

}

void DrawMoonSets(int window) {
  glutSetWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);
  //DRAW PLOTS


  DrawBestCollection(0,0, 0.5f, "Velocity", cont[BEST_CONT].input[1]);
  DrawCollection(0,0, 0.5f, "Velocity", cont[controller].input[1]);
  DrawPlot(0, 0, 0.5f);

  DrawBestCollection(0,0.5f, 0.5f, "Height", cont[BEST_CONT].input[0]);
  DrawCollection(0,0.5f, 0.5f, "Height", cont[controller].input[0]);
  DrawPlot(0, 0.5f, 0.5f);

  //rules
  DrawRules(0.5f, 0.95f, controller, 0);
  //draw accumulator
  DrawPlot(0, -0.999, 1);
  DrawAccumulator(0, -0.999, "Output", cont[controller].output[0]);
}


void DrawMoonSim(int window) {
  glutSetWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);

  PrintFloat(0.5, 0.5f,"Generation",generation);
  PrintFloat(0.5, 0.45f,"Controller",controller);
  PrintFloat(0.5, 0.4f,"Mutations",cont[controller].mutations);
  PrintFloat(0.5, 0.35f,"Active Rules",cont[controller].output[0].active );
  PrintFloat(0.5, 0.3f,"Fuel",fuel );
  PrintFloat(0.5, 0.25f,"Thrust", cont[controller].output[0].output);
  PrintFloat(0.5, 0.2f, "Sim Speed", 10 - (speed / 1000));
  PrintFloat(0.5, 0.15f, "BEST", BEST);
  PrintFloat(0.5, 0.1f, "MEAN", MEAN);
  PrintFloat(0.5, 0.05f, "LOW", LOW);

  //DRAW SIM
  float y = cont[controller].input[0].value - cont[controller].input[0].low;
  y /= cont[controller].input[0].high;

  float thrust = cont[controller].output[0].output;

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
  thrust /= 100;
  thrust -= 0.05f;
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_TRIANGLES);
    glVertex2f(-0.52f, y+0.05f);
    glVertex2f(-0.5f, y- thrust);
    glVertex2f(-0.48f, y+0.05f);
  glEnd();

}