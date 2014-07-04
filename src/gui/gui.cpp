#include "gui.h"
#include "..\objects\gen.h"
#include <GL\glut.h>
using namespace std;

void keyboard(unsigned char key, int x, int y);
void display(void);

int main(int argc, char *argv[])
{
  //Init GLUT
  glutInit(&argc, argv);
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

  glColor3f(1.0f, 0.0f, 0.0f);

  glBegin(GL_POLYGON);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f( 0.5f, -0.5f);
    glVertex2f( 0.5f,  0.5f);
    glVertex2f(-0.5f,  0.5f);
  glEnd();

  glFlush();
}
