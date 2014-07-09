#ifndef SHARED_H
#define SHARED_H

#include <string>

//singleton
typedef struct {
  float value;
  float scale;
} singleton;

//rule
typedef struct {
  short int inputvar;
  short int inputset;
  std::string modifier;
  short int inputvar2;
  short int inputset2;
  short int output;
} Rule;

//set
typedef struct {
  short int height;
  short int centreX;
  short int leftBase;
  short int rightBase;
  short int leftTop;
  short int rightTop;
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
extern short int POP;                       //-p
extern short int GENERATIONS;               //-g
extern short int ANCESTOR;                  //-a
extern short int VARIANCE;                  //-r
extern float MUT_CHANCE;              //-m
extern short int BEST;
extern short int BEST_CONT;

//fuzzy
extern short int NUM_INPUT;
extern short int NUM_VARS;                   //-c
extern short int NUM_SETS;                   //-s
extern short int NUM_RULES;
extern short int HEIGHT;                     //-h

//sim
extern short int START_HEIGHT;               //-y
extern short int START_FUEL;                 //-f
extern short int START_VEL;                  //-v
extern short int THRUST_MAX;                 //-t
extern short int TERMINAL_VELOCITY;
extern short int FORCE;                      //-o
extern short int CRASH_SPEED;                //-x

extern FuzzyVar simInput[2];
extern Accumulator * simOutput;
extern FuzzyVar * simFitness;

//controllers
extern Controller *cont;

void InitSystem(int argc, char *argv[]);
#endif