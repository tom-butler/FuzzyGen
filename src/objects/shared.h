#ifndef SHARED_H
#define SHARED_H

#include <string>

//rule
typedef struct {
  short int inputvar;
  short int inputset;
  std::string modifier;
  short int inputvar2;
  short int inputset2;
  float output;
  bool isActive;
} Rule;

//set
typedef struct {
  float height;
  float centreX;
  float leftBase;
  float rightBase;
  float leftTop;
  float rightTop;
} Set;

//var
typedef struct {
  short int low;
  short int high;
  float value;
  Set *sets;
  int setNum;
} FuzzyVar;

typedef struct {
  short int low;
  short int high;
  float output;
  short int active;
  float *value;
  float *scale;
} Accumulator;

//Controller
typedef struct {
  short int score;
  short int mutations;
  FuzzyVar *input;
  Accumulator output;
  Rule *rules;
  int ruleNum;
} Controller;



//options-------------------------------------------

//genetic
extern short int POP;
extern short int GENERATIONS;
extern bool INCLUDE_CONTROL;
extern bool LOGGING;
extern bool RANDOM_START;
extern bool ELITISM;
extern bool RELATIONAL_BOUNDS;

//Mutations
extern float MUT_CHANCE;
extern float VARIANCE;

extern bool MUT_COL_INITIAL;
extern bool MUT_COL_GROW;
extern bool MUT_COL_SLIDE;
extern bool MUT_COL_ADD;

extern bool MUT_SET_INITIAL;
extern bool MUT_SET_NUM;
extern bool MUT_SET_GROW_TOP;
extern bool MUT_SET_GROW_BOT;
extern bool MUT_SET_SLIDE;
extern bool MUT_SET_SLIDE_TOP;
extern bool MUT_SET_SLIDE_BOT;
extern bool MUT_SET_ADD;

extern bool MUT_RULE_RAND;
extern bool MUT_RULE_ADDALL;

//fuzzy
extern short int NUM_INPUT;
extern short int MIN_NUM_SETS;
extern short int MAX_NUM_SETS;
extern float HEIGHT_LOW;
extern float HEIGHT_HIGH;

//sims
extern short int SIM;
const short int MOONLANDER = 0;
const short int PENDULUMCART = 1;
const short int HARRIER = 2;

//runtime --------------------------------
extern bool GUI;
extern short int ANCESTOR;
extern short int MAX_BEST;
extern short int BEST;
extern short int BEST_CONT;
extern float MEAN;
extern int LOW;

extern std::string *LOG;
extern FuzzyVar *simInput;
extern Accumulator * simOutput;
extern FuzzyVar * simFitness;
extern Controller *cont;

void InitSystem();
int InitTest(int test);
short int GetRandInt(short int low, short int high);
float GetRandFloat(float low, float high);

#endif