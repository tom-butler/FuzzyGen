#include "..\objects\shared.h"
#include "..\objects\gen.h"
#include "..\objects\controller.h"
#include "gui.h"

#include <GL\freeglut.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

int generation = 0;
int speed = 5000;
int controller = -1;
int state = 0;
int result = -1;
int tick = 0;

void ProcessNormalKeys(unsigned char key, int x, int y);
void ProcessSpecialKeys(int key, int x, int y);
void Display(void);

int main(int argc, char *argv[])
{
  //Init GLUT
  GUI = true;
  InitSystem();
  InitControllers();

  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutCreateWindow("FuzzyGen Gui");
  glutKeyboardFunc(&ProcessNormalKeys);
  glutSpecialFunc(&ProcessSpecialKeys);
  glutDisplayFunc(&Display);

  state = 1;
  glutMainLoop();

  return EXIT_SUCCESS;
  //INIT FuzzyGen
  //InitSystem(argc, argv);
  //InitControllers();
}

void ProcessNormalKeys(unsigned char key, int x, int y)
{
  switch (key)
  {
    case '\x1B': //exit key
      exit(EXIT_SUCCESS);
      break;
  }
}

void ProcessSpecialKeys(int key, int x, int y)
{
  switch (key)
  {
    case GLUT_KEY_UP:
      if(speed > 1000)
        speed -= 1000;
      else
        speed = 1;
      break;
    case GLUT_KEY_DOWN:
      if(speed == 1)
        speed = 1000;
      else if(speed < 9000)
        speed += 1000;
      break;
  }
}

void Display() {

  //end
  if(state == 5) {
    PrintFloat(-0.5f, 0.2f, "FINISHED: ",BEST);
  }

  //end
  if(state == 4) {
    if(cont[controller].score > 0)
      PrintFloat(-0.5f, 0.0f, "SUCCESS", cont[controller].score);
    else
      PrintFloat(-0.5f, 0.0f, "FAILED", cont[controller].score);

    if(tick % speed == 0) {
      if(controller < POP -1)
        state = 1;
      else
        state = 3;
    }
  }

  //breed
  if(state == 3) {
    if(generation < GENERATIONS - 1) {
      PrintFloat(-0.5f, 0.1f, "BREEDING", 0.0f);
      Breed();
      generation++;
      controller = -1;
      state = 1;
    }
    else
      state = 5;
  }

  //run
  if(state == 2) {
    if(tick % speed == 0) {
      result = RunSim(controller);
      glClear(GL_COLOR_BUFFER_BIT);
      DrawSim();
      if(result != -1) {
        if(cont[controller].score > 0) {
          if(BEST < cont[controller].score) {
            BEST = cont[controller].score;
            BEST_CONT = controller;
          }
          state = 4;
        }
        else
          state = 4;
      }
    }
  }

  //load
  if(state == 1) {
    controller++;
    InitSimulation(controller);
    result = -1;
    state = 2;
  }

  tick++;
  glutPostRedisplay();
  glutSwapBuffers();
}

//util functions
void DrawPlot(float x, float y){
  glColor3f(1.0f, 0.0f, 0.0f);
  //draw x plane
  glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x + 1, y);
  glEnd();

  //draw y plane
  glBegin(GL_LINES);
    glVertex2f(x, y + 0.5f);
    glVertex2f(x, y);
  glEnd();
}

void DrawAccumulator(float x, float y,string name, Accumulator output) {
  PrintFloat(x, y,name, output.output);
  float xScale = 1.0f / output.high;
  glColor3f(1.0f, 1.0f, 1.0f);
  //draw singletons
  for (int i = 0; i < output.active; i++) {
    float xPos = x + output.value[i] * xScale;
    glBegin(GL_LINES);
      glVertex2f(xPos, y + output.scale[i] / 2);
      glVertex2f(xPos, y);
    glEnd();
  }
  //draw output
  glColor3f(1.0f, 0.0f, 0.0f);
  float xPos = x + output.output * xScale;
  glBegin(GL_LINES);
    glVertex2f(xPos, y + 0.51f);
    glVertex2f(xPos, y + 0.5f);
  glEnd();
}

void DrawCollection(float x, float y,string name, FuzzyVar collection) {
  //draw current value
  glColor3f(1.0f, 0.0f, 0.0f);
  PrintFloat(x, y,name, collection.value);
  //find the appropriate scale
  float scale = collection.high - collection.low;
  // for height

  for(int i = 0; i < NUM_SETS;i++) {
    //scale the points
    float centre = collection.sets[i].centreX - collection.low;
    centre /= scale;

    float lBase = collection.sets[i].leftBase / scale;
    //lBase /= scale;
    float lTop = collection.sets[i].leftTop / scale;
    //lTop /= scale;
    float rTop = collection.sets[i].rightTop / scale;
    //rTop /= scale;
    float rBase = collection.sets[i].rightBase / scale;
    //rBase /= scale;
    float height = collection.sets[i].height / HEIGHT;

    float value = collection.value - collection.low;
    value /= scale;
    //draw the lines
      glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
      glVertex2f(x + (centre - lBase), y);
      glVertex2f(x + (centre - lTop), y + 0.4f);
      glVertex2f(x + (centre - lTop), y + 0.4f);
      glVertex2f(x + (centre + rTop), y + 0.4f);
      glVertex2f(x + (centre + rTop), y + 0.4f);
      glVertex2f(x + (centre + rBase), y);
    glEnd();
  glColor3f(1.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
      glVertex2f(x + value, y);
      glVertex2f(x + value, y + 0.4f);
    glEnd();

  }

}
void DrawBestCollection(float x, float y,string name, FuzzyVar collection) {
  //find the appropriate scale
  float scale = collection.high - collection.low;
  // for height

  for(int i = 0; i < NUM_SETS;i++) {
    //scale the points
    float centre = collection.sets[i].centreX - collection.low;
    centre /= scale;

    float lBase = collection.sets[i].leftBase / scale;
    //lBase /= scale;
    float lTop = collection.sets[i].leftTop / scale;
    //lTop /= scale;
    float rTop = collection.sets[i].rightTop / scale;
    //rTop /= scale;
    float rBase = collection.sets[i].rightBase / scale;
    //rBase /= scale;
    float height = collection.sets[i].height / HEIGHT;

    float value = collection.value - collection.low;
    value /= scale;
    //draw the lines
  glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);
      glVertex2f(x + (centre - lBase), y);
      glVertex2f(x + (centre - lTop), y + 0.4f);
      glVertex2f(x + (centre - lTop), y + 0.4f);
      glVertex2f(x + (centre + rTop), y + 0.4f);
      glVertex2f(x + (centre + rTop), y + 0.4f);
      glVertex2f(x + (centre + rBase), y);
    glEnd();
  }

}
void PrintFloat(float x, float y, string name, float value) {
  ostringstream ss;
  ss << name << " " << value;
  string text(ss.str());
  glRasterPos2f(x + 0.002f, y + 0.45f);
  glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *) text.c_str());
}

void DrawRules(float x, float y, int controller) {
  //draw current
  for(int i = 0; i < NUM_RULES; i++) {
    if(cont[controller].rules[i].isActive)
      glColor3f(1.0f, 1.0f, 0.0f);
    else
      glColor3f(1.0f, 0.0f, 0.0f);
    ostringstream ss;
    ss << "IF " << cont[controller].rules[i].inputset << " " << cont[controller].rules[i].modifier << " " << cont[controller].rules[i].inputset2 << " THEN " << cont[controller].rules[i].output;
    string text(ss.str());
    glRasterPos2f(x + 0.05f, y + (0.05f * i));
    glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *) text.c_str());
  }
  //draw best
  for(int i = 0; i < NUM_RULES; i++){
  ostringstream ss;
    ss << cont[BEST_CONT].rules[i].output;
    string text(ss.str());
    glColor3f(0.2f, 0.2f, 0.2f);
    glRasterPos2f(x + 0.6f, y + (0.05f * i));
    glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *) text.c_str());
  }

}