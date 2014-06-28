#include <iostream>
#include "sim.h"
#include "controller.h"

using namespace std;

const int GENERATIONS = 100;

//functions
void initGen();
void GALoop();
void ScoreFitness(int id);
void Selection();
void Breed(int id);
void Mutate(int id);

int main(int argc, char *argv[])
{
  initControllers();
  GALoop();
}
//@TODO: THIS SHIT IS WEIRD
void initControllers() {
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
    Selection();
    Breed();
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

//Selects best genotypes
void Selection() {
}

//Breeds best genotypes
void Breed(int id) {

}
