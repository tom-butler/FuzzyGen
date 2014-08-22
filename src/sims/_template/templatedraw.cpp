#include "templatedraw.h"

#include <GL\freeglut.h>

#include "..\..\shared\shared.h"
#include "..\..\gui\gui.h"
#include "template.h"

void InitTemplateDraw(){
  //do init stuff
}

void DrawTemplateSets(int window) {
  glutSetWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);
  
  //draw a plot
  DrawBestCollection(-1,0, 0.5f, "input", cont[BEST_GEN_CONTROLLER].input[0]);
  DrawCollection(-1,0, 0.5f, "input", cont[controller].input[0]);
  DrawPlot(-1, 0, 0.5f);

  //rules
  DrawRules(-0.5f, 0.95f, controller, 0);
  
  //accumulator
  DrawPlot(-1, -0.999, 1);
  DrawAccumulator(-1, -0.999, "output", cont[controller].output[0]);

  }


void DrawTemplateSim(int window) {
  glutSetWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);

  //Draw the Gui info 
  PrintFloat(0.5, 0.5f ,"Generation"  ,generation);
  PrintFloat(0.5, 0.45f,"Controller"  ,controller);
  PrintFloat(0.5, 0.4f ,"Mutations"   ,cont[controller].num_mutations);
  PrintFloat(0.5, 0.35f,"Active Rules",cont[controller].output[0].active );
  PrintFloat(0.5, 0.3f ,"Fuel"        ,moon_fuel );
  PrintFloat(0.5, 0.25f,"Thrust"      ,cont[controller].output[0].output);
  PrintFloat(0.5, 0.2f ,"Sim Speed"   , 10 - (speed / 1000));
  PrintFloat(0.5, 0.15f,"BEST"        ,BEST_SCORE);
  PrintFloat(0.5, 0.1f ,"MEAN"        ,MEAN_GEN);
  PrintFloat(0.5, 0.05f,"LOW"         ,LOW_GEN);

  //DRAW SIM
  
}
