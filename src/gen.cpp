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
  createController(yPos, velocity, fuelRemaining, 0);
  for(i = 0; i < NUM_COL; i++)
  {
    int cid = createCollection(0,1000,3);
  }
}
//Runs the GA until requirements met
void GALoop() {
  for(i = 0; i < GENERATIONS; i++) {

    nextStep(thrust);
  }

}

//Scores each genotype
void ScoreFitness(int id) {

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