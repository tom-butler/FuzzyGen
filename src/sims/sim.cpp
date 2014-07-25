#include "sim.h"
#include "..\objects\shared.h"
//simulations
#include "sims\moon.h"

void SimCreateSim(){
  if(SIM == MOONLANDER)
    MoonCreateVars();
}

void SimInitSim(int controller) {
  if(SIM == MOONLANDER)
    MoonInitSim(controller);
}

int SimNextStep(int controller) {
  if(SIM == MOONLANDER)
    return MoonNextStep(controller);
}

void SimControlController(int controller) {
  if(SIM == MOONLANDER)
    MoonControlController(controller);
}
