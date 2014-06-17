#ifndef FUZZY_CONTROLLER_H
#define FUZZY_CONTROLLER_H
//this file defines the structs that are used in the fuzzy logic controller

//predefine--------------------------------------------------------------------
struct controller;
struct collection;
struct set;
struct rule;

//variables--------------------------------------------------------------------

//constants
const int NUM_VAR = 3;
const int NUM_COL = 3;
const int NUM_SETS = 3;
const int NUM_RULE = NUM_COL * NUM_SETS;

//globals
int currentCollection = 0;
int currentSet = 0;
int currentRule = 0;

controller controller;
collection collections[];
set sets[];
rule rules[];



//classes----------------------------------------------------------------------

//controller
typedef struct{
	float vars[NUM_VAR];
} controller;

//collection
typedef struct {
	int start;
	int end;
	int output;
	controller *parent;
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
	collection *parent;
} set;

//rule
typedef struct {
	set *set1;
	set *set2;
	int *var1;
	int *var2;
	int *output;
} rule;


//functions--------------------------------------------------------------------

//init
void createController();
void createCollection(int start, int end, int output, controller &parent);
void createSet(int ID, float centreX, float centreY. float height,
  float leftBase, float rightBase, float leftTop, float rightTop,
  collection &parent);
void createRule(set *set1, set *set2, int *var1, int *var2, int *output);

//evaluation
float evaluateVar(int ID, float newValue);
float evaluateRule(int ID);
float evaluateSet(int setID);

//breeding
void breedController();
void breedCollection();
void breedSet();

//mutation
float mutateCollection();
float mutateSet();



#endif