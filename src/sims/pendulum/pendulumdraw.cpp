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
  
  //Y
  DrawBestCollection(-1,0.5, 0.5f, "Angle", cont[BEST_GEN_CONTROLLER].input[0]);
  DrawCollection(-1,0.5, 0.5f, "Angle", cont[controller].input[0]);
  DrawPlot(-1, 0.5, 0.5f);
  DrawBestCollection(-1,0, 0.5f, "Centre Distance", cont[BEST_GEN_CONTROLLER].input[1]);
  DrawCollection(-1,0, 0.5f, "Centre Distance", cont[controller].input[1]);
  DrawPlot(-1, 0, 0.5f);
  DrawBestCollection(-1,-0.5, 0.5f, "Cart Velocity", cont[BEST_GEN_CONTROLLER].input[2]);
  DrawCollection(-1,-0.5, 0.5f, "Cart Velocity", cont[controller].input[2]);
  DrawPlot(-1, -0.5, 0.5f);
  //rules
  DrawRules(-0.5f, 0.95f, controller, 0);
  //draw accumulator
  DrawPlot(-1, -0.999, 1);
  PrintFloat(-0.3,-0.99,"Rules",cont[controller].output[0].num_rules);
  DrawAccumulator(-1, -0.999, "Force", cont[controller].output[0]);

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
  PrintFloat(0.5, 0.15f,"MEAN"         , MEAN_GEN);
  PrintFloat(0.5, 0.1f ,"LOW"          , LOW_GEN);

  PrintFloat(0.5, 0.05f ,"Score"       , pendulum_score);
  PrintFloat(0.5, 0.0f  ,"Angle"       , cont[controller].input[0].value );
  PrintFloat(0.5, -0.05f,"Centre Dist" , cont[controller].input[1].value);
  PrintFloat(0.5, -0.1f,"Cart Velocity", cont[controller].input[2].value);
  PrintFloat(0.5, -0.15f,"Thrust"       , cont[controller].output[0].output);

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
  float x = cont[controller].input[1].value;//ConvertToSimScale(cont[controller].input[1].value, -1, 1) ;
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
  float rod_length = kRodLength / 5;
  float angle = DegToRad(90 - cont[controller].input[0].value);
  float xx = x + rod_length * cos(angle);
  float yy = -0.8 + rod_length * sin(angle);
  //thrust
  glBegin(GL_LINES);
    glVertex2f(x, -0.8);
    glVertex2f(xx, yy);
  glEnd();
}
