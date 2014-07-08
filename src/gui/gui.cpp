#include "..\objects\shared.h"
#include "..\objects\gen.h"
#include "..\objects\controller.h"

#include <GL\freeglut.h>
#include <stdio.h>
#include <string>

using namespace std;

static float height = 1.0f;
static int CONT = 0;
static int STATE = 0;
static int result = -1;

void keyboard(unsigned char key, int x, int y);
void display(void);

//draw functions
void draw();
void drawSim(float y, float thrust);
void drawPlot(float x, float y, string text);
void drawAccumulator(float x, float y, Accumulator output);
void drawCollection(float x, float y, FuzzyVar collection);

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


  STATE = 1;
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
  if(STATE == 1) {
    InitSimulation(BEST);
    result = -1;
    STATE = 2;
  }

  //run
  if(STATE == 2) {
    result = RunSim(BEST);
    glClear(GL_COLOR_BUFFER_BIT);
    draw();
    glutSwapBuffers();
    if(result != -1)
      STATE = 3;
  }
}
void draw() {
    //DRAW PLOTS
  drawPlot(0, 0.5f, "Velocity:");
  drawCollection(0,0.5f, cont[CONT].input[0]);

  drawPlot(0, 0, "Height:");
  drawCollection(0,0.5f, cont[CONT].input[1]);

  //drawPlot(0, -0.5f, "Thrust:");
  //drawCollection(0,0.5f, cont[CONT].input[0]);

  drawPlot(0, -0.999, "Output:");
  drawAccumulator(0, -0.999, cont[CONT].output);

  //DRAW SIM
  drawSim(height, 0.0f);
  height -= 0.0001f;
  glutPostRedisplay();
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
  thrust /= 10;
  thrust -= 0.05f;
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_TRIANGLES);
    glVertex2f(-0.52f, y+0.05f);
    glVertex2f(-0.5f, y- thrust);
    glVertex2f(-0.48f, y+0.05f);
  glEnd();


}
void drawPlot(float x, float y, string text){
  glColor3f(1.0f, 0.0f, 0.0f);
  //write title
  glRasterPos2f(x + 0.002f, y + 0.45f);
  glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *) text.c_str());

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

void drawAccumulator(float x, float y, Accumulator output) {
  float xScale = 1.0f / output.high;
  glColor3f(1.0f, 1.0f, 1.0f);

  for (int i = 0; i < output.active; i++) {
    float xPos = x + output.value[i] * xScale;
    glBegin(GL_LINES);
      glVertex2f(xPos, y + output.scale[i]);
      glVertex2f(xPos, y);
    glEnd();
  }
}

void drawCollection(float x, float y, FuzzyVar collection) {
  //find the appropriate scale
  float scale = collection.low - collection.high;
  scale /= 100.0f;

  for(int i = 0; i < NUM_SETS;i++) {
    //scale the points
    float centre = collection.sets[i].centreX;
    centre -= collection.low;
    centre*scale;

    float lBase = collection.sets[i].leftBase * scale;
    float lTop = collection.sets[i].leftTop * scale;

    float rTop = collection.sets[i].rightTop * scale;
    float rBase = collection.sets[i].rightBase * scale;

    float height = collection.sets[i].height *scale;

    //draw the lines
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
      glVertex2f(x + (centre - lBase), y);
      glVertex2f(x + (centre - lTop), y + height);
      glVertex2f(x + (centre + rTop), y + height);
      glVertex2f(x + (centre - rBase), y);
    glEnd();
  }

}