#ifndef FUZZY_CONTROLLER_H
#define FUZZY_CONTROLLER_H

#include <string>
#include <iostream>
#include <stdlib.h>
#include "gen.h"
using namespace std;
//this file defines the structs that are used in the fuzzy logic controller


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
  Set *sets;
} FuzzyVar;

//Controller
typedef struct {
  int score;
  int mutations;
  FuzzyVar *vars;
  Rule *rules;
} Controller;


//functions--------------------------------------------------------------------

//util
void ThrowError(string error);
int GetRandInt(int low, int high);
float Intersect(int x1, int y1, int x2, int y2, int input);
void UpdateVars(int controller, int vars[]);
void ScoreController(int controller, int score);
//init
void CreateControllers(int num, FuzzyVar vars[]);
void InitSets(int variable, int numSets);
void InitRules(int output);

//evaluation
float EvaluateRules(int controller, int ID);
float EvaluateSet(int controller, int inputVar, int setID);

//breeding
void SelectController();
void BreedController(Controller id1, Controller id2);

void ParentMutation(int id1, int id2);
void ChildMutation(int id);
void MutateCollection(int id);
void MutateSet(int controller, int var, int setID);
void MutateRule(int controller, int ruleID);

#endif