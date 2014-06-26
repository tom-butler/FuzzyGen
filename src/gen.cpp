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
  initGen();
  GALoop();
}

//sets up the initial variables
void initGen() {
  initSim(1000, 3, 300);

}

void initController() {
  //create vars
  createController(thrust, height, velocity, fuelRemaining);
}
//Runs the GA until requirements met
void GALoop() {
  for(i = 0; i < GENERATIONS; i++) {
    Update();
    ScoreFitness();
    Selection();
    Breed();
  }

}
void Update() {

}
//Scores each genotype
void ScoreFitness() {
  nextStep(thrust);
}

//Selects best genotypes
void Selection() {

}

//Breeds best genotypes
void Breed(int id) {

}

//Mutate genotype for random variance
void Mutate(int id) {

}