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


//functions--------------------------------------------------------------------

void CreateControllers(int num, FuzzyVar input[], Accumulator output);

void EvaluateRules(int controller);

void BreedControllers();


#endif