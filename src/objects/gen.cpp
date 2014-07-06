#include "controller.h"
#include "gen.h"
#include "sim.h"

#include <iostream>
#include <GetOpt.h>
#include <stdlib.h>

using namespace std;

void InitSystem(int argc,char *argv[]) {
  cout << "Initialising System...\n";
  //loop through and set any defined options
  int c;
  while((c = getopt(argc, argv, "pgarmcshyfvt0x:")) != -1) {
    switch(c){
      //system
      case 'd':
        DEBUG = true;
        break;
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
        cout << "Bad option -" << c;
        break;
      default:
        cout << "Option -" << c << " does not exist";
        break;
    }
    //re-caclulate after change
    TERMINAL_VELOCITY = START_VEL * 10;
    NUM_RULES = NUM_VARS * NUM_SETS;
  }
  cout << "System Initialised\n";
}

//@TODO: THIS SHIT IS WEIRD
void InitControllers() {
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
