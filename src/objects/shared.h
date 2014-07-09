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
  int inputvar;
  int inputset;
  std::string modifier;
  int inputvar2;
  int inputset2;
  int output;
} Rule;

//set
typedef struct {
  int centreX;
  int height;
  int leftBase;
  int rightBase;
  int leftTop;
  int rightTop;
  int variable;
} Set;

//var
typedef struct {
  int low;
  int high;
  float value;
  Set *sets;
} FuzzyVar;

typedef struct {
  int low;
  int high;
  float output;
  int active;
  float *value;
  float *scale;
} Accumulator;

//Controller
typedef struct {
  int score;
  int mutations;
  FuzzyVar *input;
  Accumulator output;
  Rule *rules;
} Controller;



//options-------------------------------------------

//genetic
extern int POP;                       //-p
extern int GENERATIONS;               //-g
extern int ANCESTOR;                  //-a
extern int VARIANCE;                  //-r
extern float MUT_CHANCE;              //-m
extern int BEST;
extern int BEST_CONT;

//fuzzy
extern int NUM_INPUT;
extern int NUM_OUTPUT;
extern int NUM_VARS;                   //-c
extern int NUM_SETS;                   //-s
extern int NUM_RULES;
extern int HEIGHT;                     //-h

//sim
extern int START_HEIGHT;               //-y
extern int START_FUEL;                 //-f
extern int START_VEL;                  //-v
extern int THRUST_MAX;                 //-t
extern int TERMINAL_VELOCITY;
extern int FORCE;                      //-o
extern int CRASH_SPEED;                //-x

extern FuzzyVar simInput[2];
extern Accumulator * simOutput;
extern FuzzyVar * simFitness;

//controllers
extern Controller *cont;

void InitSystem(int argc, char *argv[]);
#endif