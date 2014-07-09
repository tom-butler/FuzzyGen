#include "..\objects\shared.h"
#include "..\objects\gen.h"
#include "..\objects\controller.h"

#include <GL\freeglut.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

static int controller = 0;
static int state = 0;
static int result = -1;
int tick = 0;

void keyboard(unsigned char key, int x, int y);
void display(void);

//draw functions
void draw();
void drawSim(float y, float thrust);
void drawPlot(float x, float y);
void drawAccumulator(float x, float y, string name, Accumulator output);
void drawCollection(float x, float y, string name, FuzzyVar collection);
void PrintFloat(float x, float y, string name, float value);
int main(int argc, char *argv[])
{
  //Init GLUT
  InitSystem(argc, argv);
  InitControllers();

  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutCreateWindow("FuzzyGen Gui");
  glutKeyboardFunc(&keyboard);
  glutDisplayFunc(&display);


  state = 1;
  glutMainLoop();

  return EXIT_SUCCESS;
  //INIT FuzzyGen
  //InitSystem(argc, argv);
  //InitControllers();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case '\x1B': //exit key
      exit(EXIT_SUCCESS);
      break;
  }
}

void display() {

  //load
  if(state == 1) {
    InitSimulation(controller);
    result = -1;
    state = 2;
  }

  //run
  if(state == 2) {
    if(tick % 10000 == 0){
      result = RunSim(controller);
      draw();
      if(result != -1){
        controller++;
        state = 1;
      }
    }
  }
  tick++;
  glutPostRedisplay();
  glutSwapBuffers();
}
void draw() {
  glClear(GL_COLOR_BUFFER_BIT);
    //DRAW PLOTS
  drawPlot(0, 0);
  drawCollection(0,0,"Height", cont[controller].input[0]);

  drawPlot(0, 0.5f);
  drawCollection(0,0.5f,"Velocity", cont[controller].input[1]);

  //drawPlot(0, -0.5f, "Thrust:");
  //drawCollection(0,0.5f, cont[CONT].input[0]);
  PrintFloat(0, -0.5f,"Fuel",cont[controller].score );
  PrintFloat(0, -0.55f,"Active Rules",cont[controller].output.active );
  PrintFloat(0, -0.6f,"Controller",controller);

  drawPlot(0, -0.999);
  drawAccumulator(0, -0.999, "Output", cont[controller].output);

  //DRAW SIM
  float height = cont[controller].input[0].value - cont[controller].input[0].low;
  height /= cont[controller].input[0].high;

  drawSim(height, cont[controller].output.output);
}

//y is 0-1 marking the percentage between low and high
void drawSim(float y, float thrust) {

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
void drawPlot(float x, float y){
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

void drawAccumulator(float x, float y,string name, Accumulator output) {
  PrintFloat(x, y,name, output.output);
  float xScale = 1.0f / output.high;
  glColor3f(1.0f, 1.0f, 1.0f);

  for (int i = 0; i < output.active; i++) {
    float xPos = x + output.value[i] * xScale;
    glBegin(GL_LINES);
      glVertex2f(xPos, y + output.scale[i] / 2);
      glVertex2f(xPos, y);
    glEnd();
  }
}

void drawCollection(float x, float y,string name, FuzzyVar collection) {
  //draw current value

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
    glBegin(GL_LINES);
      glVertex2f(x + value, y);
      glVertex2f(x + value, y + 0.4f);
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