#ifndef FUZZY_CONTROLLER_H
#define FUZZY_CONTROLLER_H

#include <string>
#include <iostream>
#include <stdlib.h>
using namespace std;
//this file defines the structs that are used in the fuzzy logic controller

//constants
const int POP = 100;
const int NUM_PARENT = POP/2;
const int NUM_VARS = 4;
const int NUM_SETS = 3;
const int NUM_RULES = NUM_VARS * NUM_SETS;

static int VARIANCE = 3;
static int HEIGHT = 1;
static float MUT_CHANCE = 0.3f;

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
void ThrowError(string error);
int GetRandInt(int low, int high);
float Intersect(float x1, float y1, float x2, float y2, float input);
void UpdateVars(FuzzyVar vars[]);

//init
void CreateController(FuzzyVar vars[]);
void InitSets(int variable, int numSets);
void InitRules(int output);

//evaluation
float EvaluateRule(int ID);
float EvaluateSet(int controller, int inputVar, int setID);

//breeding
void BreedController(int id1, int id2);
void MutateCollection(int id);

void MutateSet(int controller, int var, int setID);
void MutateSetGrowTop(int controller, int var, int setID);
void MutateSetGrowBase(int controller, int var, int setID);
void MutateSetSlideTop(int controller, int var, int setID);
void MutateSetSlideBase(int controller, int var, int setID);

void MutateRule(int controller, int ruleID);
void MutateRuleOutput(int controller, int ruleID);

#endif