#ifndef FUZZY_CONTROLLER_H
#define FUZZY_CONTROLLER_H

#include "gen.h"
#include <string>
//this file defines the structs that are used in the fuzzy logic controller


//classes----------------------------------------------------------------------

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
  float value;
  float *singles;
} Singleton;

//Controller
typedef struct {
  int score;
  int mutations;
  FuzzyVar *input;
  Singleton output;
  Rule *rules;
} Controller;


//functions--------------------------------------------------------------------

int GetRandInt(int low, int high);
float Intersect(int x1, int y1, int x2, int y2, int input);
void UpdateVars(int controller, float vars[]);
void ScoreController(int controller, int score);
//init
void CreateControllers(int num, FuzzyVar input[], Singleton output);
void InitSets(int controller, int variable, int numSets);
void InitSingletons(int controller, int numSingletons);
void InitRules(int controller);

//evaluation
float EvaluateRules(int controller);
float EvaluateSet(int controller, int inputVar, int setID, int variable);
float EvaluateOutput(int controller, int singleton, float scale);

//breeding
void BreedControllers();

void ParentMutation(int id1, int id2);
void ChildMutation(int id);
void MutateCollection(int id);
void MutateSet(int controller, int var, int setID);
void MutateRule(int controller, int ruleID);

#endif