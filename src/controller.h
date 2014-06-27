#ifndef FUZZY_CONTROLLER_H
#define FUZZY_CONTROLLER_H

#include <string>
#include <iostream>
#include <stdlib.h>
using namespace std;
//this file defines the structs that are used in the fuzzy logic controller

//constants
const int POP = 100;
const int NUM_VARS = 4;
const int NUM_SETS = 3;
const int NUM_RULES = NUM_VARS * NUM_SETS;

static int VARIANCE = 3;
static int HEIGHT = 1;

//classes----------------------------------------------------------------------

//rule
typedef struct {
  int inputvar;
  int inputset;
  string modifier;
  int inputvar2;
  int inputset2;
  int outputvar;
  int outputset;
} Rule;

//set
typedef struct {
  float centreX;
  float centreY;
  float height;
  float leftBase;
  float rightBase;
  float leftTop;
  float rightTop;
  int variable;
} Set;

//var
typedef struct {
  int low;
  int high;
  int value;
  int output;
  Set sets[NUM_SETS_PER_COL]
} FuzzyVar;

//Controller
typedef struct {
  int score;
  int mutations;
  FuzzyVar vars[NUM_VARS];
  Rule rules[NUM_RULES];
} Controller;

//functions--------------------------------------------------------------------

//util
void throwError(string error);
int getRandInt(int low, int high);
float intersect(float x1, float y1, float x2, float y2, float input);

//init
void createController(float vars[]);
void UpdateVars(float vars[]);
void initSets(int parentID, int numSets);
void initRules(int output);

//evaluation
float evaluateRule(int ID);
float evaluateSet(int setID, float variable);


//breeding
void breedController();
void breedCollection();
void breedSet();

//mutation
void mutate();
void mutateCollection();

void mutateSet(int setID);
void mutateSetGrowTop(int setID);
void mutateSetGrowBase(int setID);
void mutateSetSlideTop(int setID);
void mutateSetSlideBase(int setID);

void mutateRule(int ruleID);
void mutateRuleOutput(int ruleID);

#endif