#include "controller.h"
#include "shared.h"
#include "gen.h"
#include "..\sims\moon.h"

#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

void InitControllers() {
  CreateControllers(POP, simInput, *simOutput);
}

void InitSimulation(int controller) {
  if(SIM == MOONLANDER)
    MoonInitSim(controller);
}

int RunSim(int controller) {
  EvaluateRules(controller);
  if(SIM == MOONLANDER)
    return MoonNextStep(controller);
}

void Breed() {
  Select();
}