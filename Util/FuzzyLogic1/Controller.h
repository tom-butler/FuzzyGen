#ifndef FUZZY_CONTROLLER_H
#define FUZZY_CONTROLLER_H

#include <string>
#include <iostream>
using namespace std;
//this file defines the structs that are used in the fuzzy logic controller

//constants
const int NUM_VARS = 3;
const int NUM_COL = 3;
const int NUM_SETS_PER_COL = 3;
const int NUM_SETS = NUM_SETS_PER_COL * NUM_COL;
const int NUM_RULES = NUM_COL * NUM_SETS;

//classes----------------------------------------------------------------------

//collection
typedef struct {
	int start;
	int end;
	int output;
} collection;

//set
typedef struct {
	int id;
	float centreX;
	float centreY;
  float height;
  float leftBase;
  float rightBase;
  float leftTop;
  float rightTop;
	int collection;
} set;

//rule
typedef struct {
  int var1;
	int set1;
  string modifier;
  int var2;
	int set2;
	int output;
  int outputSet;
} rule;


//variables--------------------------------------------------------------------

//globals
int currentCollection = 0;
int currentSet = 0;
int currentRule = 0;

collection collections[NUM_COL];
float vars[NUM_RULES];
set sets[NUM_SETS];
rule rules[NUM_RULES];


//functions--------------------------------------------------------------------

//util
void throwError(string error);

//init
void createCollection(int start, int end, int outputID);
void createSet(int ID, float centreX, float centreY, float height,
  float leftBase, float rightBase, float leftTop, float rightTop,
  collection &parent);
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
float mutateCollection();
float mutateSet();


#endif