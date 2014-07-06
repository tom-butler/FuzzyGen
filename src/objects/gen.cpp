#include "controller.h"
#include "..\settings.h"
#include "gen.h"
#include "sim.h"

#include <iostream>
#include <fstream>
#include <GetOpt.h>
#include <stdlib.h>

using namespace std;

bool DEBUG = true;                    //-d

//genetic
int POP = 10000;                       //-p
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

Controller *cont;
void InitSystem(int argc,char *argv[]) {
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
}

//@TODO: THIS SHIT IS WEIRD
void InitControllers() {
    cont = new Controller[POP];
    CreateControllers(POP, simInput, *simOutput);
    cout << cont[0].score;
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

void SaveBest() {

  ofstream output;
  output.open("logs/log.txt");
  output << BEST <<","
  << BEST_CONT<< ","
  << cont[BEST_CONT].mutations;
  for (int i = 0; i < NUM_INPUT; i++) {
    output << "#";
    output << cont[BEST_CONT].input[i].low << ","
    << cont[BEST_CONT].input[i].high << ","
    << cont[BEST_CONT].input[i].value;
    for(int s = 0; s < NUM_SETS; s++) {
      output << "#";
      output << cont[BEST_CONT].input[i].sets[s].centreX << ","
      << cont[BEST_CONT].input[i].sets[s].height << ","
      << cont[BEST_CONT].input[i].sets[s].leftBase << ","
      << cont[BEST_CONT].input[i].sets[s].rightBase << ","
      << cont[BEST_CONT].input[i].sets[s].leftTop << ","
      << cont[BEST_CONT].input[i].sets[s].rightTop << ","
      << cont[BEST_CONT].input[i].sets[s].variable << ",";
      output << "#";
    }

  }
  output << cont[BEST_CONT].output.low << ","
  << cont[BEST_CONT].output.high << ","
  << cont[BEST_CONT].output.output << ","
  << cont[BEST_CONT].output.active;
  for(int i = 0; i < cont[BEST_CONT].output.active; i++){
    output << cont[BEST_CONT].output.value[i];
    if(i == cont[BEST_CONT].output.active -1)
      output << "#";
    else
      output << ",";
  }
  for(int i = 0; i < cont[BEST_CONT].output.active; i++){
    output << cont[BEST_CONT].output.scale[i];
    if(i == cont[BEST_CONT].output.active -1)
      output << "#";
    else
      output << ",";
  }
  for( int i = 0; i < NUM_RULES; i++) {
    output << cont[BEST_CONT].rules[i].inputvar << ","
    << cont[BEST_CONT].rules[i].inputvar << ","
    << cont[BEST_CONT].rules[i].inputset << ","
    << cont[BEST_CONT].rules[i].modifier << ","
    << cont[BEST_CONT].rules[i].inputvar2 << ","
    << cont[BEST_CONT].rules[i].inputset2 << ","
    << cont[BEST_CONT].rules[i].output << ",";
  }
  output << "#";

  output.close();
}
