#include <iostream>
#include "sim.h"
#include "controller.h"

using namespace std;

const int GENERATIONS = 100;

//functions
void InitGen();
void GALoop();
void ScoreFitness(int id);
void Selection();
void Breed(int id);
void Mutate(int id);

int Main(int argc, char *argv[])
{
  initControllers();
  GALoop();
}
//@TODO: THIS SHIT IS WEIRD
void InitControllers() {
  //create vars
  for(i = 0; i < POP; i++) {
    thrust.controller = i;
    height.controller = i;
    velocity.controller = i;
    fuelRemaining.controller = i;
    createController({thrust, height, velocity, fuelRemaining});
  }
}
//Runs the GA until requirements met
void GALoop() {
  for(i = 0; i < GENERATIONS; i++) {
    ScoreFitness();
    SelectController();
  }

}

//Scores each genotype
void ScoreFitness() {
  for(i = 0; i < POP; i++) {
    initSim(1000, 0, 1000);
    int result = 3;
    while(var != 3) {
      UpdateVars({thrust, height, velocity, fuelRemaining});
      thrust.value = evaluateRules(0);
      result = nextStep(thrust.value);
      cont[i].score = fuelRemaining;
    }
  }
}
