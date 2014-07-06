#ifndef SETTINGS_H
#define SETTINGS_H


#include <string>
//options-------------------------------------------
//system
extern bool DEBUG = true;                    //-d

//genetic
extern int POP = 10000;                       //-p
extern int GENERATIONS = 10000;               //-g
extern int ANCESTOR = POP/2;                  //-a
extern int VARIANCE = 3;                      //-r
extern float MUT_CHANCE = 0.3f;               //-m
extern int BEST = 0;
extern int BEST_CONT = 0;

//fuzzy
extern int NUM_INPUT = 2;
extern int NUM_OUTPUT = 1;
extern int NUM_VARS = 4;                      //-c
extern int NUM_SETS = 3;                      //-s
extern int NUM_RULES = 9;
extern int HEIGHT = 1;                        //-h

//sim
extern int START_HEIGHT = 100;               //-y
extern int START_FUEL = 1000;                 //-f
extern int START_VEL = 3;                     //-v
extern int THRUST_MAX = 50;                   //-t
extern int TERMINAL_VELOCITY = START_VEL * 10;
extern int FORCE = 3;                         //-o
extern int CRASH_SPEED = 5;                   //-x

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

extern Controller *cont;

#endif