#include "pendulumdraw.h"

#include <GL\freeglut.h>
#include <cmath>

#include "..\..\shared\shared.h"
#include "..\..\gui\gui.h"
#include "pendulum.h"

using namespace std;

void InitPendulumDraw(){
  //init draw variables
}

void DrawPendulumSets(int window) {
  glutSetWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);

  DrawSetValues(0, -1, 0.5);
  DrawBestCollection(-0.8,0.5, 0.5f, "CartX", BEST_CONTROLLER.input[0]);
  DrawCollection(-0.8,0.5, 0.5f, "CartX", cont[controller].input[0]);
  DrawPlot(-0.8, 0.5, 0.5f);

  DrawSetValues(1, -1, -0.5);
  DrawBestCollection(-0.8,0, 0.5f, "Cart Vel", BEST_CONTROLLER.input[1]);
  DrawCollection(-0.8,0, 0.5f, "Cart Vel", cont[controller].input[1]);
  DrawPlot(-0.8, 0, 0.5f);

  DrawSetValues(2, -0.2, 0.5);
  DrawBestCollection(0, 0.5, 0.5f, "Pole Angle", BEST_CONTROLLER.input[2]);
  DrawCollection(0,0.5, 0.5f, "Pole Angle", cont[controller].input[2]);
  DrawPlot(0, 0.5, 0.5f);

  DrawSetValues(3, -0.2, -0.5);
  DrawBestCollection(0, 0, 0.5f, "Pole Vel", BEST_CONTROLLER.input[3]);
  DrawCollection(0,0, 0.5f, "Pole Vel", cont[controller].input[3]);
  DrawPlot(0, 0, 0.5f);
  //rules
  DrawRules(0.5f, 0.95f, controller, 0);
  //draw accumulator
  DrawPlot(-0.8, -1, 1);
  DrawAccumulator(-0.8, -1, "Force", cont[controller].output[0]);
  PrintFloat(0.2,-1,"Rules",cont[controller].output[0].num_rules);
  
}

void DrawPendulumSim(int window) {
  glutSetWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);

  //HUD
  glColor3f(1.0f,0.0f,0.0f);
  PrintFloat(0.5, 0.5f ,"Generation"   , generation);
  PrintFloat(0.5, 0.45f,"Controller"   , controller);
  PrintFloat(0.5, 0.4f ,"Mutations"    , cont[controller].num_mutations);
  PrintFloat(0.5, 0.35f,"Active Rules" , cont[controller].output[0].num_active );  
  PrintFloat(0.5, 0.3f ,"Sim Speed"    , 10 - (speed / 1000));
  PrintFloat(0.5, 0.25f,"MAX BEST"     , BEST_SCORE);
  PrintFloat(0.5, 0.2f ,"BEST"         , BEST_GEN_SCORE);
  PrintFloat(0.5, 0.15f,"Average"      , AVG_GEN);
  PrintFloat(0.5, 0.1f ,"LOW"          , LOW_GEN);

  //PrintFloat(0.5, 0.05f ,"Score"       , pendulum_score);
  PrintFloat(0.5, 0.05f,"Centre Dist" , cont[controller].input[0].value);
  PrintFloat(0.5, 0.0f,"Cart Velocity", cont[controller].input[1].value);
  PrintFloat(0.5, -0.05f  ,"Angle"       , cont[controller].input[2].value );
  PrintFloat(0.5, -0.1f,"Pole AngleVel", cont[controller].input[3].value);
  PrintFloat(0.5, -0.15f,"Thrust"      , cont[controller].output[0].output);
  PrintFloat(0.5, -0.2f,"Score"      , (score_count / kMaxTime) * 100 );

  //ground
  glColor3f(0.8f,0.8f,0.8f);
  glBegin(GL_POLYGON);
    glVertex2f(-1,-0.95);
    glVertex2f( 1,-0.95);
    glVertex2f( 1,-1);
    glVertex2f(-1,-1);
  glEnd();
  //centre marker
  float centre = 0;
  glColor3f(1.0f,0.0f,0.0f);
  glBegin(GL_LINES);
    glVertex2f(centre,-0.95);
    glVertex2f(centre,-1);
  glEnd();

  //cart
  float x = -1 * ConvertToSimScale(cont[controller].input[0].value, -kCartMoveRadius * 10, kCartMoveRadius * 10);
  glColor3f(0.0f,0.0f,1.0f);
  glBegin(GL_POLYGON);
    glVertex2f(x - 0.1,-0.9);
    glVertex2f(x + 0.1,-0.9);
    glVertex2f(x + 0.1,-0.8);
    glVertex2f(x - 0.1,-0.8);
  glEnd();
  //wheels
  glColor3f(0.0f,1.0f,0.0f);
  DrawCircle(x - 0.08, -0.9, 0.05);
  DrawCircle(x + 0.08, -0.9, 0.05);

  //pendulum
  float rod_length = kPoleLength / 2;
  float angle = DegToRad(90 - cont[controller].input[2].value);
  float xx = x + rod_length * cos(angle);
  float yy = -0.8 + rod_length * sin(angle);
  DrawCircle(xx,yy,kMassMass * 0.002f);
  //thrust
  glBegin(GL_LINES);
    glVertex2f(x, -0.8);
    glVertex2f(xx, yy);
  glEnd();
}
