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
  InitSystem(argc, argv);
  InitControllers();
  GALoop();
}
void InitSystem(int argc,char *argv[]) {
  //loop through and set any defined options
  int c;
  while((c = getopt(argc, argv, "pacsrhmftvfx:")) != -1) {
    switch(c){
      case 'p':
        POP = atoi(optarg);
        ANCESTOR = POP/2;
        break;
      case 'a':
        ANCESTOR = atoi(optarg);
        break;
      case 'c':
        NUM_VARS = atoi(optarg);
        break;
      case 's':
        NUM_SETS = atoi(optarg);
        break;
      case 'r':
        VARIANCE = atoi(optarg);
        break;
      case 'h'
        HEIGHT = atoi(optarg);
        break;
      case 'm'
        MUT_CHANCE = atoi(optarg);
        break;
      case 'f'
        START_FUEL = atoi(optarg);
        break;
      case 't'
        THRUST_MAX = atoi(optarg);
        break;
      case 'v'
        TERMINAL_VELOCITY = atoi(optarg);
        break;
      case 'f'
        FORCE = atoi(optarg);
        break;
      case 'x'
        CHRASH_SPEED = atoi(optarg);
        break;
      case '?':
        fprintf(stderr, "Bad option -%c\n", optopt);
        break;
      default:
        fprintf(stderr, "Option -%c does not exist\n",optopt );
        break;
    }
  }
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
