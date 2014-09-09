#include <GL\freeglut.h>

#include "..\shared\shared.h"
#include "harrier\harrierdraw.h"
#include "moon\moondraw.h"
#include "pendulum\pendulumdraw.h"


bool isInit = false;
int sets;
int vars;
int sim;

void DrawSim() {
  if(!isInit){
    sim = glutGetWindow();
    sets = glutCreateWindow("Sets");
    isInit = true;
    if(kSim == kMoonLanderSim)
      InitMoonDraw();
    else if(kSim == kHarrierSim)
      InitHarrierDraw();
    else if(kSim == kPendulumCartSim)
      InitPendulumDraw();
  }
  if(kSim == kMoonLanderSim){
    DrawMoonSets(sets);
    DrawMoonSim(sim);
  }
  else if(kSim == kHarrierSim){
    DrawHarrierSets(sets);
    DrawHarrierSim(sim);
  }
  else if(kSim == kPendulumCartSim){
    DrawPendulumSets(sets);
    DrawPendulumSim(sim);
  }
}
