#ifndef FUZZYGEN_SHARED_SHARED_H
#define FUZZYGEN_SHARED_SHARED_H

#include <string>

//rule
typedef struct {
  short int *sets;
  float output;
  bool is_active;
} Rule;

//set
typedef struct {
  float height;
  float centre_x;
  float left_base;
  float right_base;
  float left_top;
  float right_top;
} Set;

//var
typedef struct {
  short int low;
  short int high;
  float value;
  Set *sets;
  short int num_sets;
} FuzzyVar;

typedef struct {
  short int low;
  short int high;
  float output;
  short int num_active;
  float *value;
  float *scale;
  Rule *rules;
  short int num_rules;
  short int *vars;
  short int num_vars;
} Accumulator;

//Controller
typedef struct {
  short int score;
  short int num_mutations;
  FuzzyVar *input;
  Accumulator *output;
} Controller;



//options-------------------------------------------

//genetic
extern short int  kNumPop;
extern short int  kNumGenerations;
extern short int  kNumAncestor;
extern bool       kIncludeControl;
extern bool       kLogging;
extern bool       kRandomStart;
extern short int  kNumTests;
extern bool       kElitism;

extern bool kForceSetRelationship;
extern float kForceSetOverlap;
extern bool kForceSetCoverage;

//Mutations
extern float      kMutationChance;
extern float      kVariance;
extern float      kBreedPercent;

extern bool       kCollectionInitialMutaion;
extern bool       kCollectionGrowMutation;
extern bool       kCollectionSlideMutation;
extern bool       kCollectionAddMutation;

extern bool       kSetInitialMutation;
extern bool       kSetNumberMutation;
extern bool       kSetGrowTopMutation;
extern bool       kSetGrowBottomMutation;
extern bool       kSetSlideMutation;
extern bool       kSetSlideTopMutation;
extern bool       kSetSlideBottomMutation;
//extern bool       kSetAddMutation;

extern bool       kRuleRandomMutation;
extern bool       kRuleAddAllMutation;

//fuzzy
extern short int  kNumInput;
extern short int  kNumOutput;
extern short int  kNumSetsMin;
extern short int  kNumSetsMax;
extern float      kSetHeightMin;
extern float      kSetHeightMax;

//sims
extern short int  kSim;
const short int   kMoonLanderSim = 0;
const short int   kPendulumCartSim = 1;
const short int   kHarrierSim = 2;

//selection method
extern short int kSelect;
const short int kSelectHalf = 0;
const short int kSelectAvg = 1;
const short int kSelectMean = 2;

extern short int kBreed;
const short int kAsexual = 0;
const short int kBisexual = 1;

//runtime --------------------------------
extern short int  BEST_SCORE;
extern Controller BEST_CONTROLLER;
extern short int  BEST_GEN;
extern short int  BEST_GEN_SCORE;
extern short int  BEST_GEN_CONTROLLER;
extern float      AVG_GEN;
extern int        LOW_GEN;

extern int        random;

extern std::string *LOG;
extern FuzzyVar *simInput;
extern Accumulator *simOutput;
extern FuzzyVar *simFitness;
extern Controller *cont;

void InitSystem();
int InitTest(int test);

short int GetRandInt(short int low, short int high);
float GetRandFloat(float low, float high);
float Lerp(float value, float low, float high);

float Intersect(float x1, float y1, float x2, float y2, float value);
void ResetAccumulator(int controller, int accumulator);
void ForceVarBounds(int controller, int var);
float DegToRad(float deg);
float RadToDeg(float rad);
double sqr(double value);
void ForceBounds(float &var, float min, float max);

void CleanController(Controller &controller);
void CleanAccumulators(Controller &controller);
void CleanSets(Controller &controller);
void CleanRules(Controller &controller, int output);

void CopyController(Controller parent, Controller &child);
#endif