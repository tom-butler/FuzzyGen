#include <iostream>
#include <GetOpt.h>
#include <stdlib.h>
#include <cmath>
#include <sstream>
#include "shared.h"
#include <cstring>
//sims
#include "..\sims\sim.h"
#include <fstream>
using namespace std;
  //define the standard options ----------------

  //genetic
  short int POP = 1000;
  short int GENERATIONS = 1000;
  bool INCLUDE_CONTROL = false;
  bool LOGGING = true;
  bool RANDOM_START = true;
  bool ELITISM = false;

  bool FORCE_RELATIONAL = false;
  float FORCE_OVERLAP = 0.3f; //0 for none
  bool FORCE_COVERAGE = true;

  //mutation
  float MUT_CHANCE = 0.4f;
  float VARIANCE = 0.10f;

  bool MUT_COL_INITIAL = true;
  bool MUT_COL_GROW = true;
  bool MUT_COL_SLIDE = true;
  bool MUT_COL_ADD = true;

  bool MUT_SET_INITIAL = true;
  bool MUT_SET_NUM = true;
  bool MUT_SET_GROW_TOP = true;
  bool MUT_SET_GROW_BOT = true;
  bool MUT_SET_SLIDE = true;
  bool MUT_SET_SLIDE_TOP = true;
  bool MUT_SET_SLIDE_BOT = true;
  bool MUT_SET_ADD = true;

  bool MUT_RULE_RAND = true;
  bool MUT_RULE_ADDALL = true;

  //fuzzy
  short int MIN_NUM_SETS = 2;
  short int MAX_NUM_SETS = 4;
  float HEIGHT_LOW = 0.5;
  float HEIGHT_HIGH = 1;

  short int SIM = 0;

  //runtime variables ---------------------------
  short int NUM_INPUT = 0;
  short int NUM_OUTPUT = 0;
  bool GUI = false;
  short int ANCESTOR = POP/2;
  short int MAX_BEST = 0;
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
  SimCreateSim();
  //init system variables
  cont = new Controller[POP];
  if(LOGGING)
    LOG = new string[GENERATIONS];
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
    else if(key == "SIM")
      SIM = atoi(value.c_str());
  }
  file.close();
  return 0;
}

//get a random integer between two low and high
short int GetRandInt(short int low, short int high){
  return (rand() % ((high + 1) - low)) + low;
}

float GetRandFloat(float low, float high) {
  return low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
}