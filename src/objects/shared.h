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
} Controller;



//options-------------------------------------------

//genetic
extern short int POP;
extern short int GENERATIONS;
extern float VARIANCE;
extern float MUT_CHANCE;
extern bool INCLUDE_CONTROL;
extern bool LOGGING;

//fuzzy
extern short int NUM_INPUT;
extern short int NUM_SETS;
extern float HEIGHT;

//sims
extern short int SIM;

//moonlander
extern short int START_HEIGHT;
extern short int START_FUEL;
extern short int START_VEL;
extern short int THRUST_MAX;
extern short int TERMINAL_VELOCITY;
extern float FORCE;
extern short int CRASH_SPEED;

//runtime variables -------------
const short int MOONLANDER = 0;
extern short int NUM_RULES;
extern short int ANCESTOR;
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
#endif