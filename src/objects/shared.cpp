#include <iostream>
#include <GetOpt.h>
#include <stdlib.h>

#include "shared.h"
//sims
#include "..\sims\moon.h"

using namespace std;
  //define the global variables

  //genetic
  short int POP = 500;
  short int GENERATIONS = 1000;
  short int ANCESTOR = POP/2;
  float VARIANCE = 0.10f;
  float MUT_CHANCE = 0.4f;
  short int BEST = 0;
  short int BEST_CONT = 0;
  bool INCLUDE_CONTROL = false;
  float MEAN = 0.0f;
  int LOW = 0;
  bool LOGGING = true;
  string *LOG;

  //fuzzy
  short int NUM_INPUT = 2;
  short int NUM_OUTPUT = 1;
  short int NUM_VARS = 4;
  short int NUM_SETS = 3;
  short int NUM_RULES = 9;
  float HEIGHT = 1;


  short int SIM = 0;

  //moonlander
  short int MOONLANDER = 0;
  short int START_HEIGHT = 1000;
  short int START_FUEL = 500;
  short int START_VEL = 3;
  short int THRUST_MAX = 10;
  short int TERMINAL_VELOCITY = START_VEL * 10;
  float FORCE = 2.0f;
  short int CRASH_SPEED = 10;

  FuzzyVar  *simInput;
  Accumulator * simOutput;
  FuzzyVar * simFitness;

  Controller *cont;

void InitSystem() {
  //init system variables
  cont = new Controller[POP];
  simInput = new FuzzyVar[NUM_INPUT];
  if(LOGGING)
    LOG = new string[GENERATIONS];
  if(SIM == MOONLANDER)
    MoonCreateVars();

}