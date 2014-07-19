#include <iostream>
#include <GetOpt.h>
#include <stdlib.h>
#include <cmath>
#include <sstream>
#include "shared.h"
#include <cstring>
//sims
#include "..\sims\moon.h"
#include <fstream>
using namespace std;
  //define the standard options ----------------

  //genetic
  short int POP = 500;
  short int GENERATIONS = 1000;
  float VARIANCE = 0.10f;
  float MUT_CHANCE = 0.4f;
  bool INCLUDE_CONTROL = false;
  bool LOGGING = true;

  bool MUT_COL_INITIAL = true;
  bool MUT_COL_GROW = true;
  bool MUT_COL_SLIDE = true;
  bool MUT_COL_ADD = true;

  bool MUT_SET_INITIAL = true;
  bool MUT_SET_GROW = true;
  bool MUT_SET_SLIDE = true;
  bool MUT_SET_ADD = true;

  bool MUT_RULE_RAND = true;
  bool MUT_RULE_ADDALL = true;


  //fuzzy
  short int NUM_INPUT = 2;
  short int NUM_SETS = 3;
  float HEIGHT = 1;

  short int SIM = 0;

  //moonlander
  short int START_HEIGHT = 1000;
  short int START_FUEL = 500;
  short int START_VEL = 3;
  short int THRUST_MAX = 10;
  short int TERMINAL_VELOCITY = START_VEL * 10;
  float FORCE = 5.0f;
  short int CRASH_SPEED = 3;

  //runtime variables ---------------------------
  bool GUI = false;
  short int ANCESTOR = POP/2;
  short int NUM_RULES = pow(NUM_SETS, NUM_INPUT) ;
  short int BEST = 0;
  short int BEST_CONT = 0;
  float MEAN = 0.0f;
  int LOW = 0;

  string *LOG;
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
int InitTest(int test) {
  ifstream file;
  ostringstream ss;
  ss << "tests/" << test << ".csv";
  file.open(ss.str().c_str());
  if(!file){
    file.close();
    return 1;
  }
  string line;
  while(getline(file,line)){
    string key = strtok((char *) line.c_str(),",");
    string value = strtok(NULL,",");
    if(key == "POP")
      POP = atoi(value.c_str());
    else if(key == "PARENTS")
      ANCESTOR = POP * atof(value.c_str());
    else if(key == "GENERATIONS")
      GENERATIONS = atoi(value.c_str());
    else if(key == "VARIANCE")
      VARIANCE = atof(value.c_str());
    else if(key == "MUT_CHANCE")
      MUT_CHANCE = atof(value.c_str());
    else if(key == "INCLUDE_CONTROL")
      INCLUDE_CONTROL = atoi(value.c_str());
    else if(key == "LOGGING")
      LOGGING = atoi(value.c_str());
    else if(key == "NUM_INPUT")
      NUM_INPUT = atoi(value.c_str());
    else if(key == "NUM_SETS")
      NUM_SETS = atoi(value.c_str());
    else if(key == "HEIGHT")
      HEIGHT = atoi(value.c_str());
    else if(key == "SIM")
      SIM = atoi(value.c_str());
    //moonlander
    else if(key == "START_HEIGHT")
      START_HEIGHT = atoi(value.c_str());
    else if(key == "START_FUEL")
      START_FUEL = atoi(value.c_str());
    else if(key == "START_VEL")
      START_VEL = atoi(value.c_str());
    else if(key == "THRUST_MAX")
      THRUST_MAX = atoi(value.c_str());
    else if(key == "TERMINAL_VELOCITY")
      TERMINAL_VELOCITY = atoi(value.c_str());
    else if(key == "FORCE")
      FORCE = atoi(value.c_str());
    else if(key == "CRASH_SPEED")
      CRASH_SPEED = atoi(value.c_str());
  }
  file.close();
  return 0;
}