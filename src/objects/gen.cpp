#include "controller.h"
#include "shared.h"
#include "gen.h"
#include "sim.h"

#include <iostream>
#include <fstream>

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
  BreedControllers();
}

void SaveBest() {

  ofstream output;
  output.open("logs/log.txt");
  output << BEST <<","
  << BEST_CONT<< ","
  << cont[BEST_CONT].mutations;
  for (int i = 0; i < NUM_INPUT; i++) {
    output << "#";
    output << cont[BEST_CONT].input[i].low << ","
    << cont[BEST_CONT].input[i].high << ","
    << cont[BEST_CONT].input[i].value;
    for(int s = 0; s < NUM_SETS; s++) {
      output << "#";
      output << cont[BEST_CONT].input[i].sets[s].centreX << ","
      << cont[BEST_CONT].input[i].sets[s].height << ","
      << cont[BEST_CONT].input[i].sets[s].leftBase << ","
      << cont[BEST_CONT].input[i].sets[s].rightBase << ","
      << cont[BEST_CONT].input[i].sets[s].leftTop << ","
      << cont[BEST_CONT].input[i].sets[s].rightTop << ",";
      output << "#";
    }

  }
  output << cont[BEST_CONT].output.low << ","
  << cont[BEST_CONT].output.high << ","
  << cont[BEST_CONT].output.output << ","
  << cont[BEST_CONT].output.active;
  for(int i = 0; i < cont[BEST_CONT].output.active; i++){
    output << cont[BEST_CONT].output.value[i];
    if(i == cont[BEST_CONT].output.active -1)
      output << "#";
    else
      output << ",";
  }
  for(int i = 0; i < cont[BEST_CONT].output.active; i++){
    output << cont[BEST_CONT].output.scale[i];
    if(i == cont[BEST_CONT].output.active -1)
      output << "#";
    else
      output << ",";
  }
  for( int i = 0; i < NUM_RULES; i++) {
    output << cont[BEST_CONT].rules[i].inputvar << ","
    << cont[BEST_CONT].rules[i].inputvar << ","
    << cont[BEST_CONT].rules[i].inputset << ","
    << cont[BEST_CONT].rules[i].modifier << ","
    << cont[BEST_CONT].rules[i].inputvar2 << ","
    << cont[BEST_CONT].rules[i].inputset2 << ","
    << cont[BEST_CONT].rules[i].output << ",";
  }
  output << "#";

  output.close();
}
