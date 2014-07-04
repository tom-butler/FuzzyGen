#include "gui.h"
#include "..\objects\gen.h"
#include <GL\freeglut.h>
using namespace std;



int main(int argc, char *argv[])
{
  //Init GLUT
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutCreateWindow("FuzzyGen Gui");
  glutKeyboardFunc(&keyboard);
  glutDisplayFunc(&display);
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
  glClear(GL_COLOR_BUFFER_BIT);

  //DRAW PLOTS

  //red for plots
  glColor3f(1.0f, 0.0f, 0.0f);

  //draw plot
  drawPlot(0, 0.5f, "Velocity:");
  drawPlot(0, 0, "Height:");
  drawPlot(0, -0.5f, "Thrust:");
  drawPlot(0, -0.999, "Output:");
  //fill plot

  //DRAW SIM
  drawSim(0, 0);
  glFlush();
}
//y is 0-100 marking the percentage between low and high
void drawSim(int y, int thrust) {

  //draw ground
  float grey  = 96.0f;
  glColor3f(grey/255, grey/255, grey/255);

  glBegin(GL_POLYGON);
    glVertex2f(-1, -0.9f);
    glVertex2f(-1, -1);
    glVertex2f(-0.006f, -1);
    glVertex2f(-0.006f, -0.9f);
  glEnd();
}
void drawPlot(float x, float y, string text){

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

void drawSingleton(float x, float y) {
  glBegin(GL_LINES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(-0.5f, 0.5f);
  glEnd();
}

void drawCollection(float x, float y, FuzzyVar collection) {

}