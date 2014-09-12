#include "gui.h"

#include <cmath>
#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <GL\freeglut.h>

#include "..\shared\shared.h"
#include "..\shared\gen.h"
#include "..\shared\controller\controller.h"


#define LOAD 1
#define RUN 2
#define BREED 3
#define FINISH 4

float r[5] = {1.0f,0.2f,0.1f,0.3f,0.8f};
float g[5] = {0.1f,0.3f,0.0f,1.0f,0.4f};
float b[5] = {0.3f,0.5f,1.0f,0.0f,0.5f};
using namespace std;

bool bestOnly = false;
bool isbreed = false;

int generation = 0;
int speed = 5000;
int controller = -1;
int state = 1;
int result = -1;
int tick = 0;

int multi_run_score = 0;
int run = kNumTests;
void ProcessNormalKeys(unsigned char key, int x, int y);
void ProcessSpecialKeys(int key, int x, int y);
void Display(void);
void InitNextController();
void Idle();
void RunAll();
int main(int argc, char *argv[])
{
  //Init GLUT
  InitSystem();
  InitControllers();

  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Sim");
  glutKeyboardFunc(&ProcessNormalKeys);
  glutSpecialFunc(&ProcessSpecialKeys);
  glutDisplayFunc(&Display);
  glutIdleFunc(&Idle);

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
    case 'b':
      if(bestOnly)
        bestOnly = false;
      else
        bestOnly = true;
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

  if(state == RUN)
    DrawSim();
  if(result == 0)
    PrintFloat(-0.5f, 0.0f, "SUCCESS", cont[controller].score);
  if(result == 1)
    PrintFloat(-0.5f, 0.0f, "FAILED", cont[controller].score);
  //end
  if(state == FINISH)
    PrintFloat(-0.5f, 0.2f, "FINISHED: ",BEST_SCORE);
  //breed
  if(state == BREED)
    PrintFloat(-0.5f, 0.1f, "BREEDING", 0.0f);
  
  //swap to idle
  glutSwapBuffers();
}

//sim logic
void Idle() {

  //Breed
  if(state == BREED) {
    if(generation < kNumGenerations - 1) {
      Breed();
      generation++;
      controller = -1;
      state = LOAD;
    }
    else
      state = FINISH;
  }

  //Run
  if(state == RUN) {
    if(result != -1)
      state = LOAD;
    if(tick % speed == 0) {
      result = RunSim(controller);
    }
  }

  //Load
  if(state == LOAD) {
    if(bestOnly){
      if(!isbreed){
        isbreed = true;
        PrintFloat(-0.5f, 0.0f, "RUNNING", generation);
        RunAll();
        controller = BEST_GEN_CONTROLLER;
        InitNextController();
      }
      else{
        isbreed = false;
        state = BREED;
      }
    }
    else{
      if(controller < 0) {
        controller++;
        InitNextController();
        if(kRandomStart)
          run = 0;
      }
      else if(run < kNumTests) {
        multi_run_score += cont[controller].score;
        InitNextController();
        run++;
      }
      else if(controller < kNumPop - 1){
        if(kRandomStart) {
          cont[controller].score = multi_run_score / kNumTests;
          multi_run_score = 0;
          run = 0;
        }
        if(cont[controller].score > BEST_SCORE){
          BEST_SCORE = cont[controller].score;
          CleanController(BEST_CONTROLLER);
          CopyController(cont[controller], BEST_CONTROLLER);
        }
        if(cont[controller].score > BEST_GEN_SCORE){
          BEST_GEN_SCORE = cont[controller].score;
          BEST_GEN_CONTROLLER = controller;
        }
        controller++;
        InitNextController();
      }
      else{
        state = BREED;
      }
    }
  }
  tick+= 100;

  //set flag to draw
  glutPostRedisplay();
}

void InitNextController() {
  InitSimulation(controller);
  result = -1;
  state = 2;
}
void RunAll() {
  BEST_GEN_SCORE = 0;
  for(int c = 0; c < kNumPop; ++c) {
    int multiscore = 0;
    for(int t = 0; t < kNumTests; t++) {
      InitSimulation(c);
      int result = -1;
      while(result == -1) {
        result = RunSim(c);
      }
      multiscore += cont[c].score;
    }
    if(kRandomStart)
      cont[c].score = multiscore / kNumTests;

    if(cont[c].score > BEST_SCORE) {
      BEST_SCORE = cont[c].score;
    }

    if(cont[c].score > BEST_GEN_SCORE){
      BEST_GEN_SCORE = cont[c].score;
      BEST_GEN_CONTROLLER = c;
    }
  }
  if(BEST_GEN_SCORE > BEST_SCORE) {
    CleanController(BEST_CONTROLLER);
    CopyController(cont[BEST_GEN_CONTROLLER], BEST_CONTROLLER);
  }
}


/**
 *util functions
 */

void DrawPlot(float x, float y, float width) {
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
  float xScale = 1.0f / (output.high - output.low);
  glColor3f(1.0f, 1.0f, 1.0f);
  //draw singletons
  for (int i = 0; i < output.num_active; i++) {
    float xPos = x + (output.value[i] - output.low) * xScale;
    glBegin(GL_LINES);
      glVertex2f(xPos, y + output.scale[i]/ 2);
      glVertex2f(xPos, y);
    glEnd();
  }
  //draw output
  glColor3f(1.0f, 1.0f, 0.0f);
  float xPos = x + (output.output - output.low) * xScale;
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
  for(int i = 0; i < collection.num_sets;i++) {
    //scale the points
    float centre = collection.sets[i].centre_x - collection.low;
    centre /= scale;

    float l_base = collection.sets[i].left_base / scale;
    //lBase /= scale;
    float l_top = collection.sets[i].left_top / scale;
    //lTop /= scale;
    float r_top = collection.sets[i].right_top / scale;
    //rTop /= scale;
    float r_base = collection.sets[i].right_base / scale;
    //rBase /= scale;
    float height = collection.sets[i].height / 2.5f;

    float value = collection.value - collection.low;
    value /= scale;
    glColor3f(1.0f, 0.0f, 0.0f);
     glBegin(GL_LINES);
      glVertex2f(x, y + 1 / 2.5f);
      glVertex2f(x + width, y + 1 / 2.5f);
     glEnd();
    glColor3f(r[i],g[i],b[i]);
    //draw the lines
    glBegin(GL_LINES);
      glVertex2f(x + (centre - l_base), y);
      glVertex2f(x + (centre - l_top), y + height);
      glVertex2f(x + (centre - l_top), y + height);
      glVertex2f(x + (centre + r_top), y + height);
      glVertex2f(x + (centre + r_top), y + height);
      glVertex2f(x + (centre + r_base), y);
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

  for(int i = 0; i < collection.num_sets;i++) {
    //scale the points
    float centre = collection.sets[i].centre_x - collection.low;
    centre /= scale;

    float l_base = collection.sets[i].left_base / scale;
    //lBase /= scale;
    float l_top = collection.sets[i].left_top / scale;
    //lTop /= scale;
    float r_top = collection.sets[i].right_top / scale;
    //rTop /= scale;
    float r_base = collection.sets[i].right_base / scale;
    //rBase /= scale;
    float height = collection.sets[i].height / 2.5f;

    //draw the lines
  glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);
      glVertex2f(x + (centre - l_base), y);
      glVertex2f(x + (centre - l_top), y + height);
      glVertex2f(x + (centre - l_top), y + height);
      glVertex2f(x + (centre + r_top), y + height);
      glVertex2f(x + (centre + r_top), y + height);
      glVertex2f(x + (centre + r_base), y);
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
  for(int i = 0; i < cont[controller].output[accumulator].num_rules; i++) {
    if(cont[controller].output[accumulator].rules[i].is_active)
      glColor3f(1.0f, 1.0f, 0.0f);
    else
      glColor3f(1.0f, 0.0f, 0.0f);
    ostringstream ss;
    for(int s = 0; s < cont[controller].output[accumulator].num_vars; s++){
      ss << cont[controller].output[accumulator].rules[i].sets[s] << "";
    }
    ss << " " <<  setprecision(2) << cont[controller].output[accumulator].rules[i].output;
    string text(ss.str());
    glRasterPos2f(x, y + (0.04f * -i));
    glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *) text.c_str());
  }
  //draw best
  for(int i = 0; i < cont[BEST_GEN_CONTROLLER].output[accumulator].num_rules; i++){
    ostringstream ss;
    ss << setprecision(2) << cont[BEST_GEN_CONTROLLER].output[accumulator].rules[i].output;
    string text(ss.str());
    glColor3f(0.2f, 0.2f, 0.2f);
    glRasterPos2f(x + 0.25f, y + (0.04f * -i));
    glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *) text.c_str());
  }
}

float ConvertToSimScale(float var, float min, float max) {
  return (((var - min) * 2) / (max - min)) - 1.0f;
}
void DrawCircle(float x, float y, float radius) {
  glBegin(GL_POLYGON);
    float cx = x + (float)radius * cos(359 * 3.14159f/180.0f);
    float cy = y + (float)radius * sin(359 * 3.14159f/180.0f);
    for(int j = 0; j < 360; j++)
    {
      glVertex2f(cx,cy);
      cx = x + (float)radius * cos(j * 3.14159f/180.0f);
      cy = y + (float)radius * sin(j * 3.14159f/180.0f);
      glVertex2f(cx,cy);
    }
  glEnd();
}

void DrawSetValues(int input, float x, float y) {
    PrintFloat(x,y, "",cont[controller].input[input].low);
    PrintFloat(x + 0.21,y, "",cont[controller].input[input].high);
  for(int set = 0; set < cont[controller].input[input].num_sets; set++) {
    glColor3f(r[set],g[set],b[set]);
    PrintFloat(x,y - (set * 0.3) - 0.04, "Height",cont[controller].input[input].sets[set].height);
    PrintFloat(x,y - (set * 0.3) - 0.08f, "LBase",cont[controller].input[input].sets[set].left_base);
    PrintFloat(x + 0.2,y - (set * 0.3) - 0.08f, "LTop",cont[controller].input[input].sets[set].left_top);
    PrintFloat(x,y - (set * 0.3) - 0.12f, "C X   ",cont[controller].input[input].sets[set].centre_x);
    PrintFloat(x,y - (set * 0.3) - 0.16f, "RTop",cont[controller].input[input].sets[set].right_top);
    PrintFloat(x + 0.2,y - (set * 0.3) - 0.16f, "RBase",cont[controller].input[input].sets[set].right_base);
  }
}
