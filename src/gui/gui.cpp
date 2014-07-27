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
  glutCreateWindow("Sim");
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
      DrawSim();
      if(result != -1) {
        if(cont[controller].score > 0) {
          if(cont[controller].score > MAX_BEST){
            MAX_BEST = cont[controller].score;
          }
          if(cont[controller].score > BEST){
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
void DrawPlot(float x, float y, float width){
  glColor3f(1.0f, 0.0f, 0.0f);
  //draw x plane
  glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
  glEnd();

  //draw y plane
  glBegin(GL_LINES);
    glVertex2f(x, y + 0.5f);
    glVertex2f(x, y);
  glEnd();
}

void DrawAccumulator(float x, float y, string name, Accumulator output) {
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

void DrawCollection(float x, float y, float width, string name, FuzzyVar collection) {
  //draw current value
  glColor3f(1.0f, 0.0f, 0.0f);
  PrintFloat(x, y,name, collection.value);
  //find the appropriate scale
  float scale = (collection.high - collection.low) / width;
  // for height

  for(int i = 0; i < collection.setNum;i++) {
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
    float height = collection.sets[i].height / 2.5f;

    float value = collection.value - collection.low;
    value /= scale;
    glColor3f(1.0f, 0.0f, 0.0f);
     glBegin(GL_LINES);
      glVertex2f(x, y + 1 / 2.5f);
      glVertex2f(x + width, y + 1 / 2.5f);
     glEnd();
    //draw the lines
    glBegin(GL_LINES);
      glVertex2f(x + (centre - lBase), y);
      glVertex2f(x + (centre - lTop), y + height);
      glVertex2f(x + (centre - lTop), y + height);
      glVertex2f(x + (centre + rTop), y + height);
      glVertex2f(x + (centre + rTop), y + height);
      glVertex2f(x + (centre + rBase), y);
    glEnd();
  glColor3f(1.0f, 1.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
      glVertex2f(x + value, y);
      glVertex2f(x + value, y + 0.5f);
    glEnd();

  }

}
void DrawBestCollection(float x, float y, float width, string name, FuzzyVar collection) {
  //find the appropriate scale
  float scale = (collection.high - collection.low) / width;
  // for height

  for(int i = 0; i < collection.setNum;i++) {
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
    float height = collection.sets[i].height / 2.5f;

    float value = collection.value - collection.low;
    value /= scale;
    //draw the lines
  glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);
      glVertex2f(x + (centre - lBase), y);
      glVertex2f(x + (centre - lTop), y + height);
      glVertex2f(x + (centre - lTop), y + height);
      glVertex2f(x + (centre + rTop), y + height);
      glVertex2f(x + (centre + rTop), y + height);
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

void DrawRules(float x, float y, int controller, int accumulator) {
  //draw current
  for(int i = 0; i < cont[controller].output[accumulator].ruleNum; i++) {
    if(cont[controller].output[accumulator].rules[i].output != 0){
      if(cont[controller].output[accumulator].rules[i].isActive)
        glColor3f(1.0f, 1.0f, 0.0f);
      else
        glColor3f(1.0f, 0.0f, 0.0f);
      ostringstream ss;
      ss << "IF ";
      for(int s = 0; s < cont[controller].output[accumulator].varsNum; s++){
        ss << cont[controller].output[accumulator].rules[i].sets[s] << " ";
      }
      ss << "THEN " << cont[controller].output[accumulator].rules[i].output;
      string text(ss.str());
      glRasterPos2f(x, y + (0.05f * -i));
      glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *) text.c_str());
    }
  }
  //draw best
  for(int i = 0; i < cont[BEST_CONT].output[accumulator].ruleNum; i++){
    ostringstream ss;
    ss << cont[BEST_CONT].output[accumulator].rules[i].output;
    string text(ss.str());
    glColor3f(0.2f, 0.2f, 0.2f);
    glRasterPos2f(x + 0.4f, y + (0.05f * -i));
    glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *) text.c_str());
  }
}