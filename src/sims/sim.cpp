#include "sim.h"
#include "..\objects\shared.h"
//simulations
#include "sims\moon.h"
#include "sims\harrier.h"

void SimCreateSim(){
  if(SIM == MOONLANDER)
    MoonCreateVars();
  else if(SIM == HARRIER)
    HarrierCreateVars();
}

void SimInitSim(int controller) {
  if(SIM == MOONLANDER)
    MoonInitSim(controller);
  else if(SIM == HARRIER)
    HarrierInitSim(controller);
}

int SimNextStep(int controller) {
  if(SIM == MOONLANDER)
    return MoonNextStep(controller);
  else if(SIM == HARRIER)
    return HarrierNextStep(controller);
}

void SimControlController(int controller) {
  if(SIM == MOONLANDER)
    MoonControlController(controller);
  else if(SIM == HARRIER)
    HarrierControlController(controller);
}
