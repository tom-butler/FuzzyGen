#include "sim.h"

#include "..\shared\shared.h"
#include "moon\moon.h"
#include "harrier\harrier.h"
#include "pendulum\pendulum.h"

void SimCreateSim(){
  if(kSim == kMoonLanderSim)
    MoonCreateVars();
  else if(kSim == kHarrierSim)
    HarrierCreateVars();
  else if(kSim == kPendulumCartSim)
    PendulumCreateVars();
}

void SimInitSim(int controller) {
  if(kSim == kMoonLanderSim)
    MoonInitSim(controller);
  else if(kSim == kHarrierSim)
    HarrierInitSim(controller);
  else if(kSim == kPendulumCartSim)
    PendulumInitSim(controller);
}

int SimNextStep(int controller) {
  if(kSim == kMoonLanderSim)
    return MoonNextStep(controller);
  else if(kSim == kHarrierSim)
    return HarrierNextStep(controller);
  else if(kSim == kPendulumCartSim)
    return PendulumNextStep(controller);
}

void SimControlController(int controller, FuzzyVar input[], Accumulator output[]) {
  if(kSim == kMoonLanderSim)
    MoonControlController(controller,input,output);
  else if(kSim == kHarrierSim)
    HarrierControlController(controller,input,output);
  else if(kSim == kPendulumCartSim)
    PendulumControlController(controller,input,output);
}
