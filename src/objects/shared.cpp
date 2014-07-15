#include <iostream>
#include <GetOpt.h>
#include <stdlib.h>

#include "shared.h"

using namespace std;
  //define the global variables

  //genetic
  short int POP = 10000;                       //-p
  short int GENERATIONS = 1000;               //-g
  short int ANCESTOR = POP/2;                  //-a
  float VARIANCE = 0.10f;                      //-r
  float MUT_CHANCE = 0.4f;               //-m
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
  short int NUM_VARS = 4;                      //-c
  short int NUM_SETS = 3;                      //-s
  short int NUM_RULES = 9;
  float HEIGHT = 1;                        //-h

  //sim
  short int START_HEIGHT = 1000;               //-y
  short int START_FUEL = 500;                 //-f
  short int START_VEL = 3;                     //-v
  short int THRUST_MAX = 10;                   //-t
  short int TERMINAL_VELOCITY = START_VEL * 10;
  float FORCE = 2.0f;                         //-o
  short int CRASH_SPEED = 10;                   //-x

  //sim init vars
  static Accumulator thrustSet = {0, THRUST_MAX, 0.f, 0, 0, 0};
  static FuzzyVar heightSet = {0, START_HEIGHT, START_HEIGHT, 0};
  static FuzzyVar velocitySet = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, START_VEL, 0};
  static FuzzyVar fuelSet = {0, START_FUEL, START_FUEL, 0};

  FuzzyVar simInput[2] = {heightSet, velocitySet};
  Accumulator * simOutput = &thrustSet;
  FuzzyVar * simFitness = &fuelSet;

  Controller *cont;

void InitSystem(int argc,char *argv[]) {
  //init system variables
  if(LOGGING)
    LOG = new string[GENERATIONS];

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
}