#include <iostream>
#include <GetOpt.h>
#include <stdlib.h>

#include "shared.h"

using namespace std;
  //define the global variables

  //genetic
  int POP = 1000;                       //-p
  int GENERATIONS = 10000;               //-g
  int ANCESTOR = POP/2;                  //-a
  int VARIANCE = 3;                      //-r
  float MUT_CHANCE = 0.3f;               //-m
  int BEST = 0;
  int BEST_CONT = 0;

  //fuzzy
  int NUM_INPUT = 2;
  int NUM_OUTPUT = 1;
  int NUM_VARS = 4;                      //-c
  int NUM_SETS = 3;                      //-s
  int NUM_RULES = 9;
  int HEIGHT = 1;                        //-h

  //sim
  int START_HEIGHT = 100;               //-y
  int START_FUEL = 1000;                 //-f
  int START_VEL = 3;                     //-v
  int THRUST_MAX = 50;                   //-t
  int TERMINAL_VELOCITY = START_VEL * 10;
  int FORCE = 3;                         //-o
  int CRASH_SPEED = 5;                   //-x

  //sim init vars
  static Accumulator thrustSet = {-THRUST_MAX, THRUST_MAX, 0, 0 , 0};
  static FuzzyVar heightSet = {0, START_HEIGHT, START_HEIGHT, 0};
  static FuzzyVar velocitySet = {TERMINAL_VELOCITY, TERMINAL_VELOCITY, START_VEL, 0};
  static FuzzyVar fuelSet = {0, START_FUEL, START_FUEL, 0};

  FuzzyVar simInput[2] = {heightSet, velocitySet};
  Accumulator * simOutput = &thrustSet;
  FuzzyVar * simFitness = &fuelSet;

  Controller *cont;

void InitSystem(int argc,char *argv[]) {
  //init system variables

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