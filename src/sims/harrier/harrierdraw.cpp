#include <GL\freeglut.h>
#include <GL\glut.h>
#include <vector>

#include "..\..\objects\shared.h"
#include "..\..\gui\gui.h"
#include "harrier.h"

#include "harrierdraw.h"

using namespace std;
//textures


void InitHarrierDraw(){

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
  glColor3f(1.0f,0.0f,0.0f);
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
  glColor3f(24/256.0f,64/256.0f,64/256.0f); //blue
  glBegin(GL_POLYGON);
    glVertex2f(-1,-0.95);
    glVertex2f( 1,-0.95);
    glVertex2f( 1,-1);
    glVertex2f(-1,-1);
  glEnd();

  //ship

  float DrawSafeX = ConvertToSimScale(harrier_safeX, 0, HARRIER_SIM_WIDTH);
  float DrawSafeY = ConvertToSimScale(harrier_safeY, 0, HARRIER_SIM_HEIGHT);
  float DrawSafeWidth = ConvertToSimScale(harrier_safeWidth, 0, HARRIER_SIM_WIDTH);

  //ship
  glColor3f(0.3514f,0.3514f, 0.3514f); //dark grey
  glBegin(GL_POLYGON);
    glVertex2f(DrawSafeX - DrawSafeWidth/2      , -1);
    glVertex2f(DrawSafeX - DrawSafeWidth/2 *1.01, DrawSafeY);
    glVertex2f(DrawSafeX + DrawSafeWidth/2, DrawSafeY);
    glVertex2f(DrawSafeX + DrawSafeWidth/2 *0.01, -1);
  glEnd();
  //landing
  glColor3f(1.0f,1.0f,0); //red
  glBegin(GL_LINES);
    glVertex2f(DrawSafeX - DrawSafeWidth/2, DrawSafeY);
    glVertex2f(DrawSafeX + DrawSafeWidth/2, DrawSafeY);
  glEnd();

  float x = ConvertToSimScale(harrier_XPos, 0, HARRIER_SIM_WIDTH);
  float y = ConvertToSimScale(harrier_YPos, 0, HARRIER_SIM_HEIGHT);

  //harrier
  glColor3f(1,1,1);
  //hull
  glColor3f(0.3514f,0.3514f, 0.3514f); //dark grey
  glBegin(GL_POLYGON);
    glVertex2f(x - 0.15f, y + 0.03f);
    glVertex2f(x + 0.07f, y + 0.03f);
    glVertex2f(x + 0.15f, y - 0.03f);
    glVertex2f(x - 0.12f, y - 0.03f);
  glEnd();
  //tail
  glBegin(GL_POLYGON);
    glVertex2f(x - 0.15f, y + 0.03f);
    glVertex2f(x - 0.17f, y + 0.08f);
    glVertex2f(x - 0.15f, y + 0.08f);
    glVertex2f(x - 0.10f, y + 0.03f);
  glEnd();
  //wing
  glColor3f(0.3f,0.3f, 0.3f);
  glBegin(GL_POLYGON);
    glVertex2f(x - 0.05f, y);
    glVertex2f(x - 0.04f, y + 0.02f);
    glVertex2f(x + 0.04f, y + 0.02f);
    glVertex2f(x + 0.05f, y);
    glVertex2f(x + 0.04f, y - 0.03f);
    glVertex2f(x - 0.02f, y - 0.04f);
    glVertex2f(x - 0.04f, y - 0.03f);
  glEnd();
  //cockpit
  glColor3f(0.0f,0.0f, 0.5f);
  glBegin(GL_POLYGON);
    glVertex2f(x + 0.05f, y +0.03f);
    glVertex2f(x + 0.07f, y +0.03f);
    glVertex2f(x + 0.08f, y +0.03f);
    glVertex2f(x + 0.09f, y +0.03f);
    glVertex2f(x + 0.1f , y  +0.03f);
    glVertex2f(x + 0.13f, y +0.02f);
    glVertex2f(x + 0.14f, y -0.02f);
    glVertex2f(x + 0.15f, y -0.03f);
  glEnd();
  //centre X
  glColor3f(1.0f,0.0f,0.0f);
  glBegin(GL_LINES);
    glVertex2f(x - 0.01f, y - 0.01);
    glVertex2f(x + 0.01f, y + 0.01);
  glEnd();
  glBegin(GL_LINES);
    glVertex2f(x - 0.01f, y + 0.01);
    glVertex2f(x + 0.01f, y - 0.01);
  glEnd();
}