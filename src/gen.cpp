#include <iostream>
#include "gen.h"
#include "sim.h"
#include "controller.h"
#include <GetOpt.h>

using namespace std;



int main(int argc, char *argv[])
{
  InitSystem(argc, argv);
  InitControllers();
  GALoop();
}

void InitSystem(int argc,char *argv[]) {
  //loop through and set any defined options
  int c;
  while((c = getopt(argc, argv, "pgarmcshyfvt0x:")) != -1) {
    switch(c){
      //GENETIC
      case 'p':
        POP = atoi(optarg);
        ANCESTOR = POP/2;
        break;
      case 'g':
        GENERATIONS = atoi(optarg);
        break;
      case 'a':
        ANCESTOR = atoi(optarg);
        break;
      case 'r':
        VARIANCE = atoi(optarg);
        break;
      case 'm':
        MUT_CHANCE = atoi(optarg);
        break;
      //fuzzy
      case 'c':
        NUM_VARS = atoi(optarg);
        break;
      case 's':
        NUM_SETS = atoi(optarg);
        break;
      case 'h':
        HEIGHT = atoi(optarg);
        break;

      //sim
      case 'y':
        START_HEIGHT = atoi(optarg);
        break;
      case 'f':
        START_FUEL = atoi(optarg);
        break;
      case 't':
        THRUST_MAX = atoi(optarg);
        break;
      case 'v':
        START_VEL = atoi(optarg);
        break;
      case 'o':
        FORCE = atoi(optarg);
        break;
      case 'x':
        CRASH_SPEED = atoi(optarg);
        break;
      case '?':
        cout << "Bad option -" << optopt;
        break;
      default:
        cout << "Option -" << optopt << " does not exist";
        break;
    }
    //re-caclulate after change
    TERMINAL_VELOCITY = START_VEL * 10;
    NUM_RULES = NUM_VARS * NUM_SETS;
  }
}

//@TODO: THIS SHIT IS WEIRD
void InitControllers() {
    FuzzyVar variables[] = {thrust, height, velocity, fuelRemaining};
    CreateControllers(POP, variables);
}
//Runs the GA until requirements met
void GALoop() {
  for(int i = 0; i < GENERATIONS; i++) {
    ScoreFitness();
    SelectController();
  }
}

//Scores each genotype
void ScoreFitness() {
  for(int i = 0; i < POP; i++) {
    InitSim();
    int result = 2;
    while(result == 2) {
      int variables[] = {thrust.value, height.value, velocity.value, fuelRemaining.value};
      UpdateVars(i, variables);
      thrust.value = EvaluateRules(i, 0);
      result = NextStep();
    }
    if(result == 0) //failed
      ScoreController(i, fuelRemaining.value /2);
    if(result == 1)
      ScoreController(i, fuelRemaining.value);
  }
}
