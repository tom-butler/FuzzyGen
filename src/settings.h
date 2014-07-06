#ifndef SETTINGS_H
#define SETTINGS_H


#include <string>
//options-------------------------------------------
//system
static bool DEBUG = true;                    //-d

//genetic
static int POP = 10000;                       //-p
static int GENERATIONS = 10000;               //-g
static int ANCESTOR = POP/2;                  //-a
static int VARIANCE = 3;                      //-r
static float MUT_CHANCE = 0.3f;               //-m
static int BEST = 0;
static int BEST_CONT = 0;

//fuzzy
static int NUM_INPUT = 2;
static int NUM_OUTPUT = 1;
static int NUM_VARS = 4;                      //-c
static int NUM_SETS = 3;                      //-s
static int NUM_RULES = 9;
static int HEIGHT = 1;                        //-h

//sim
static int START_HEIGHT = 100;               //-y
static int START_FUEL = 1000;                 //-f
static int START_VEL = 3;                     //-v
static int THRUST_MAX = 50;                   //-t
static int TERMINAL_VELOCITY = START_VEL * 10;
static int FORCE = 3;                         //-o
static int CRASH_SPEED = 5;                   //-x

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

static Controller *cont;

#endif