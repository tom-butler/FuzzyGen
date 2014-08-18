#include "sim.h"
#include "..\shared\shared.h"
//simulations
#include "moon\moon.h"
#include "harrier\harrier.h"
#include "pendulum\pendulum.h"

void SimCreateSim(){
  if(SIM == MOONLANDER)
    MoonCreateVars();
  else if(SIM == HARRIER)
    HarrierCreateVars();
  else if(SIM == PENDULUM)
    PendulumCreateVars();
}

void SimInitSim(int controller) {
  if(SIM == MOONLANDER)
    MoonInitSim(controller);
  else if(SIM == HARRIER)
    HarrierInitSim(controller);
  else if(SIM == PENDULUM)
    PendulumInitSim(controller);
}

int SimNextStep(int controller) {
  if(SIM == MOONLANDER)
    return MoonNextStep(controller);
  else if(SIM == HARRIER)
    return HarrierNextStep(controller);
  else if(SIM == PENDULUM)
    return PendulumNextStep(controller);
}

void SimControlController(int controller) {
  if(SIM == MOONLANDER)
    MoonControlController(controller);
  else if(SIM == HARRIER)
    HarrierControlController(controller);
  else if(SIM == PENDULUM)
    PendulumControlController(controller);
}
