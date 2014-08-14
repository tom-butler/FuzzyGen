#include <GL\freeglut.h>

#include "..\objects\shared.h"
//sims
#include "harrier\harrierdraw.h"
#include "moon\moondraw.h"
#include "pendulum\pendulumdraw.h"


void DrawMoonSets(int window);
void DrawMoonSim(int window);

bool isInit = false;
int sets;
int vars;
int sim;

void DrawSim() {
  if(!isInit){
    sim = glutGetWindow();
    sets = glutCreateWindow("Sets");
    isInit = true;
    if(SIM == MOONLANDER)
      InitMoonDraw();
    else if(SIM == HARRIER)
      InitHarrierDraw();
    else if(SIM == PENDULUM)
      InitPendulumDraw();
  }
  if(SIM == MOONLANDER){
    DrawMoonSets(sets);
    DrawMoonSim(sim);
  }
  else if(SIM == HARRIER){
    DrawHarrierSets(sets);
    DrawHarrierSim(sim);
  }
  else if(SIM == PENDULUM){
    DrawPendulumSets(sets);
    DrawPendulumSim(sim);
  }
}
