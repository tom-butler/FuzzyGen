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
const int NUM_SETS_PER_COL = 3;
const int NUM_SETS = NUM_VARS * NUM_SETS_PER_COL;
const int NUM_RULES = NUM_VARS * NUM_SETS;

static int VARIANCE = 3;
static int HEIGHT = 1;

//classes----------------------------------------------------------------------
typedef struct {
  int score;
  int mutations;
} Controller;


//var
typedef struct {
  int low;
  int high;
  int value;
  int output;
  int controller;
} FuzzyVar;

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

//rule
typedef struct {
	int input1;
  string modifier;
	int input2;
  int output;
} Rule;


//functions--------------------------------------------------------------------

//util
void throwError(string error);
int getRandInt(int low, int high);

//init
void createController(float vars[]);
void createCollection(int start, int end, int outputID);
void initSets(int parentID, int numSets);
void createSet(float centreX, float height, float leftBase, float rightBase, float leftTop, float rightTop, int collection);
void initRules(int output);
void createRule(set *set1, set *set2, int *var1, int *var2, int *output);

//evaluation
float evaluateVar(int ID, float newValue);
float evaluateRule(int ID);
float evaluateSet(int setID, float variable);
float intersect(float x1, float y1, float x2, float y2, float input);

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