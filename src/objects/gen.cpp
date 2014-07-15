#include "controller.h"
#include "shared.h"
#include "gen.h"
#include "sim.h"

#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

void InitControllers() {
  cont = new Controller[POP];
  CreateControllers(POP, simInput, *simOutput);
}

void InitSimulation(int controller) {
  InitSim(controller);
}

int RunSim(int controller) {
  EvaluateRules(controller);
  return NextStep(controller);
}

void Breed() {
  Select();
}