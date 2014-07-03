#include "gen.h"
#include "sim.h"
#include "controller.h"
#include <iostream>
#include <GetOpt.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
  InitSystem(argc, argv);
  InitControllers();
  GALoop();
}

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
  cout << "Initialising Controllers...\n";
    CreateControllers(POP, simInput, *simOutput);
  cout << "Controllers Initialised\n";
}
//Runs the GA until requirements met
void GALoop() {
  for(int i = 0; i < GENERATIONS; i++) {
    cout << "\nGeneration " << i;
    cout << "\n";
    cout << "Scoring controllers...\n";
    ScoreFitnesses();
    cout << "Controllers Scored\n";
    cout << "Best Score " << BEST;
    cout << "\n";
    cout << "Breeding Controllers...\n";
    BreedControllers();
    cout << "Controllers Bred \n";
  }
  cout << "Complete\n";
}

//Scores each genotype
void ScoreFitnesses() {
  for(int i = 0; i < POP; i++) {
    InitSim();
    int result = -1;
    while(result == -1) {
      float f[] = {GetInputValue(1), GetInputValue(2)};
      UpdateVars(i, f);
      result = NextStep(EvaluateRules(i));
    }
    //cout << "\n\n";
    if(result > BEST)
      BEST = result;
    ScoreController(i, result);
  }
}
