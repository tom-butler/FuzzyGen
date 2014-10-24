#include "shared.h"

#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <GetOpt.h>
#include <stdlib.h>

#include "..\sims\sim.h"

using namespace std;
  //define the standard options ----------------


  //test info
  short int kTestFile             = 0;
  short int kTest                 = 0;
  short int kRunNum               = 0;

  //genetic
  short int kNumPop               = 200; 
  short int kNumGenerations       = 200;
  bool kIncludeControl            = false;
  bool kLogging                   = true;
  bool kRandomStart               = true;
  short int kNumTests             = 3;

  bool kElitism                   = true;
  float kForceSetOverlap          = 0.3f; //0 for none
  bool kForceSetCoverage          = true;

  //mutation
  float kMutationChance           = 0.03f;
  float kVariance                 = 0.3f;
  float kBreedPercent             = 0.3f;

  bool  kInitialMutation             = true;
  bool    kCollectionInitialMutaion  = true;
  bool    kSetInitialMutation        = true;
  bool    kRuleInitialMutation       = true;

  bool  kGrowMutation                = true;
  bool    kCollectionGrowMutation    = true;
  bool    kSetGrowTopMutation        = true;
  bool    kSetGrowBottomMutation     = true;
  bool    kRuleGrowMutation          = true;

  bool  kSlideMutation               = true;    
  bool    kCollectionSlideMutation   = true;
  bool    kSetSlideMutation          = true;
  bool    kSetSlideTopMutation       = true;
  bool    kSetSlideBottomMutation    = true;
  bool    kRuleSlideMutation         = true;

  //fuzzy
  short int kNumSetsMin           = 2;   //0-5
  short int kNumSetsMax           = 3;   //0-5 (higher than min)
  bool kSetNumberMutation         = true;


  float     kSetHeightMin         = 0.5; //0 - 1
  float     kSetHeightMax         = 1;   //0 - 1 (higher than min)

  short int kSim                  = 1;
  short int kSelect               = kSelectAvg;
  short int kBreed                = kBisexual;

  //runtime variables ---------------------------
  short int kNumInput             = 0;
  short int kNumOutput            = 0;
  short int kNumAncestor          = kNumPop / 2;

  short int BEST_SCORE            = 0;
  short int BEST_GEN              = 0;
  short int BEST_GEN_SCORE        = 0;
  short int BEST_GEN_CONTROLLER   = 0;
  float     AVG_GEN               = 0.0f;
  int       LOW_GEN               = 0;

  int random                      = 0;

  string *GEN_LOG;

  Controller BEST_CONTROLLER;
  FuzzyVar  *simInput;
  Accumulator *simOutput;
  FuzzyVar *simFitness;
  Controller *cont;
void CheckSet(int controller, int var, int set);
void InitSystem() {
  //seed random
  srand(static_cast <unsigned>(time(0)));

  SimCreateSim();
  //init system variables
  cont = new Controller[kNumPop];
  if(kLogging) {
    GEN_LOG = new string[kNumGenerations];
  }
  if(!kRandomStart)
    kNumTests = 1;
  if(kNumSetsMin > kNumSetsMax) {
    kNumSetsMin = kNumSetsMax;
    kSetNumberMutation = false;
  }

}
int InitTest(int filename, int test) {

  int num_vars = 0;

  //open the file
  ifstream file;
  ostringstream ss;
  ss << "tests/" << filename << ".csv";
  file.open(ss.str().c_str());
  if(!file){
    file.close();
    return 1;
  }

  //read the variable names into the header
  vector <string> header;
  string line;
  getline(file,line);
  //waste the first word
  char* nextword = strtok( (char*) line.c_str(), ",");
  while (nextword != NULL) {
    nextword = strtok( NULL, ",");
    if(nextword != NULL) {
      header.push_back(nextword);
      num_vars++;
    }
  }

  //waste the unnecesary lines
  for(int i = 0; i < test - 1; ++i){
    getline(file, line);
  }
  //get the values we want
  vector <string> values;
  getline(file, line);
  file.close();
  nextword = strtok( (char*) line.c_str(), ",");

  while (nextword != NULL) {
    nextword = strtok( NULL, ",");
    if(nextword != NULL)
      values.push_back(nextword);
  }
  //@TODO: Make this a switch
  //assign the values to their variable
  for(int i = 0; i < num_vars; i++){

    //genetic
    if(header[i] == "NumPop") {
      kNumPop = atoi(values[i].c_str());
      kNumAncestor = kNumPop/2;
    }
    else if(header[i] == "NumGenerations")
      kNumGenerations = atoi(values[i].c_str());
    else if(header[i] == "NumAncestor")
      kNumAncestor = atoi(values[i].c_str());
    else if(header[i] == "IncludeControl")
      kIncludeControl = atoi(values[i].c_str());
    else if(header[i] == "Logging")
      kLogging = atoi(values[i].c_str());
    else if(header[i] == "RandomStart")
      kRandomStart = atoi(values[i].c_str());
    else if(header[i] == "NumTests")
      kNumTests = atoi(values[i].c_str());
    else if(header[i] == "Elitism")
      kElitism = atoi(values[i].c_str());
    else if(header[i] == "ForceSetOverlap")
      kForceSetOverlap = atof(values[i].c_str());
    else if(header[i] == "ForceSetCoverage")
      kForceSetCoverage = atoi(values[i].c_str());

    //Mutations
    else if(header[i] == "MutationChance")
      kMutationChance = atof(values[i].c_str());
    else if(header[i] == "Variance")
      kVariance = atof(values[i].c_str());
    else if(header[i] == "BreedPercent")
      kBreedPercent = atof(values[i].c_str());
    else if(header[i] == "InitialMutation")
      kInitialMutation = atoi(values[i].c_str());
    else if(header[i] == "CollectionInitialMutaion")
      kCollectionInitialMutaion = atoi(values[i].c_str());
    else if(header[i] == "SetInitialMutation")
      kSetInitialMutation = atoi(values[i].c_str());
    else if(header[i] == "RuleInitialMutation")
      kRuleInitialMutation = atoi(values[i].c_str());
    else if(header[i] == "GrowMutation")
      kGrowMutation = atoi(values[i].c_str());
    else if(header[i] == "CollectionGrowMutation")
      kCollectionGrowMutation = atoi(values[i].c_str());
    else if(header[i] == "SetGrowTopMutation")
      kSetGrowTopMutation = atoi(values[i].c_str());
    else if(header[i] == "SetGrowBottomMutation")
      kSetGrowBottomMutation = atoi(values[i].c_str());
    else if(header[i] == "RuleGrowMutation")
      kRuleGrowMutation = atoi(values[i].c_str());
    else if(header[i] == "SlideMutation")
      kSlideMutation = atoi(values[i].c_str());
    else if(header[i] == "CollectionSlideMutation")
      kCollectionSlideMutation = atoi(values[i].c_str());
    else if(header[i] == "SetSlideMutation")
      kSetSlideMutation = atoi(values[i].c_str());
    else if(header[i] == "SetSlideTopMutation")
      kSetSlideTopMutation = atoi(values[i].c_str());
    else if(header[i] == "SetSlideBottomMutation")
      kSetSlideBottomMutation = atoi(values[i].c_str());
    else if(header[i] == "RuleSlideMutation")
      kRuleSlideMutation = atoi(values[i].c_str());

    //fuzzy
    else if(header[i] == "NumInput")
      kNumInput = atoi(values[i].c_str());
    else if(header[i] == "NumOutput")
      kNumOutput = atoi(values[i].c_str());
    else if(header[i] == "NumSetsMin")
      kNumSetsMin = atoi(values[i].c_str());
    else if(header[i] == "NumSetsMax")
      kNumSetsMax = atoi(values[i].c_str());
    else if(header[i] == "SetNumberMutation")
      kSetNumberMutation = atoi(values[i].c_str());
    else if(header[i] == "SetHeightMin")
      kSetHeightMin = atof(values[i].c_str());
    else if(header[i] == "SetHeightMax")
      kSetHeightMax = atof(values[i].c_str());

    //sims
    else if(header[i] == "Sim")
      kSim = atoi(values[i].c_str());
    else if(header[i] == "Select")
      kSelect = atoi(values[i].c_str());
    else if(header[i] == "Breed")
      kBreed = atoi(values[i].c_str());

  }
  kTestFile = filename;
  kTest = test;
  return 0;
}

//get a random integer between two low and high
short int GetRandInt(short int low, short int high) {
  return (rand() % ((high + 1) - low)) + low;
}

float GetRandFloat(float low, float high) {
  return low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
}

//single line lerp
float Lerp(float value, float low, float high) {
  return low + (value*(high - low));
}

//two line lerp
float Intersect(float x1, float y1, float x2, float y2, float value) {
  if(x1 == x2 )
    return max(y1,y2);
  else
    return ((value - x1) * (y2 - y1) / (x2 - x1)) + y1;
}

void ForceVarBounds(int controller, int var) {

  Set last;
  for(int i = 0; i < cont[controller].input[var].num_sets; i++){
    Set set = cont[controller].input[var].sets[i];

    if(kForceSetCoverage){
      //set the first set to be adjusted to the left
      if(i == 0 && set.centre_x - set.left_top != cont[controller].input[var].low){
        set.left_top = fabs(cont[controller].input[var].low - set.centre_x);
        set.left_base = set.left_top;
      }
      //set the last set to be adjusted to the right
      if(i == cont[controller].input[var].num_sets -1 && set.centre_x + set.right_top != cont[controller].input[var].high){
        set.right_top = cont[controller].input[var].high -  set.centre_x;
        set.right_base = set.right_top;
      }
    } 

    //check overlap
    if(kForceSetOverlap > 0.0f){
      if(i == 0) {
        last = set;
      }
      else{
        //if the sets overlap less that FORCE_OVERLAP stretch them out
        if((last.centre_x + last.right_base - set.centre_x - set.left_base < (last.right_base) * kForceSetOverlap) ||
           (last.centre_x + last.right_base - set.centre_x - set.left_base < (set.left_base) * kForceSetOverlap)){
          //find the centre of the two sets
          float centre = last.centre_x + ((set.centre_x - last.centre_x)/2);
          float overlap1 = (last.right_base) * kForceSetOverlap;
          float overlap2 = (set.left_base) * kForceSetOverlap;
          last.right_base = centre - last.centre_x + overlap1;
          set.left_base = set.centre_x - centre + overlap2;
          cont[controller].input[var].sets[i-1] = last;
          CheckSet(controller, var, i - 1);
        }
        last = set;
      }
    }

    cont[controller].input[var].sets[i] = set;

    CheckSet(controller, var, i);
  }
}


void CheckSet(int controller, int var, int s) {
  Set set = cont[controller].input[var].sets[s];

  ForceBounds(set.centre_x,cont[controller].input[var].low, cont[controller].input[var].high);

  ForceBounds(set.left_top, 0, abs(cont[controller].input[var].low - set.centre_x));
  ForceBounds(set.left_base, 0, abs(cont[controller].input[var].low - set.centre_x));
  ForceBounds(set.left_top, 0, set.left_base);

  ForceBounds(set.right_top, 0, abs(cont[controller].input[var].high - set.centre_x));
  ForceBounds(set.right_base, 0, abs(cont[controller].input[var].high - set.centre_x));
  ForceBounds(set.right_top, 0, set.right_base);
  
//make sure the set is not squared if it is in the middle
    if(set.left_base <= set.left_top + (cont[controller].input[var].high - cont[controller].input[var].low) * 0.1 )
      if(set.centre_x - set.left_top - (kForceSetOverlap * set.left_top) < cont[controller].input[var].low)
        set.left_base = set.centre_x - cont[controller].input[var].low;
      else
        set.left_base = kForceSetOverlap * set.left_top + set.left_top;
    if(set.right_base <= set.right_top  + (cont[controller].input[var].high - cont[controller].input[var].low) * 0.1)
      if(set.centre_x + set.right_top + (kForceSetOverlap * set.right_top) > cont[controller].input[var].high)
        set.right_base = cont[controller].input[var].high - set.centre_x;
      else
        set.right_base = kForceSetOverlap * set.right_top + set.right_top;

  cont[controller].input[var].sets[s] = set;
}

//clean the controller accumulator variable
void ResetAccumulator(int controller, int accumulator) {
  delete[] cont[controller].output[accumulator].value;
  delete[] cont[controller].output[accumulator].scale;

  cont[controller].output[accumulator].value = new float[cont[controller].output[accumulator].num_rules];
  cont[controller].output[accumulator].scale = new float[cont[controller].output[accumulator].num_rules];

  //force values to 0
  for(int r = 0; r < cont[controller].output[accumulator].num_rules; r++) {
    cont[controller].output[accumulator].value[r] = 0;
    cont[controller].output[accumulator].scale[r] = 0;
  }

  cont[controller].output[accumulator].output = 0.0f;
  cont[controller].output[accumulator].num_active = 0;
}

/**
 * Convert from deg to rad
 * @param  deg [angle in degrees]
 * @return     [non bounded or wrapped angle in rad]
 */
float DegToRad(float deg) {
  return (deg * 3.14159 / 180.0);
}

/**
 * Convert from radian to degree
 * @param  rad [angle in radians]
 * @return     [non bounded or wrapped angle in degrees]
 */
float RadToDeg(float rad) {
  return (rad * 180.0 / 3.14159);
}

double sqr(double value) {
  return value*value;
}

void ForceBounds(float &var, float min, float max) {
  if(var < min)
    var = min;
  if(var > max)
    var = max;
}

void CleanController(Controller &controller) {
  CleanAccumulators(controller);
  CleanSets(controller);
}

void CleanAccumulators(Controller &controller) {
  for(int o = 0; o < kNumOutput; o++) {
    CleanRules(controller, o);
  }
  delete [] controller.output;
  controller.output = NULL;
}

void CleanSets(Controller &controller) {
for(int i = 0; i < kNumInput; i++) { 
    delete [] controller.input[i].sets;
    controller.input[i].sets = NULL;
  }
  delete [] controller.input;
  controller.input = NULL;
}

void CleanRules(Controller &controller, int output) {
    for(int r = 0; r < controller.output[output].num_rules; r++) {
      delete [] controller.output[output].rules[r].sets;
      controller.output[output].rules[r].sets = NULL;
    }
    delete [] controller.output[output].rules;
    controller.output[output].rules = NULL;
    delete [] controller.output[output].scale;
    controller.output[output].scale = NULL;
    delete [] controller.output[output].value;
    controller.output[output].value = NULL;

}

void CopyController(Controller parent, Controller &child) {
    child = parent;
    child.input  = new FuzzyVar[kNumInput];
    copy(parent.input , parent.input + kNumInput, child.input);
    for(int i = 0; i < kNumInput; i++) {
      child.input[i] = parent.input[i];
      child.input[i].sets = new Set[kNumSetsMax];
      copy(parent.input[i].sets, parent.input[i].sets + kNumSetsMax, child.input[i].sets);
    }
    child.output = new Accumulator[kNumOutput];
    copy(parent.output, parent.output + kNumOutput, child.output);
    for(int o = 0; o < kNumOutput; o++){
      child.output[o] = parent.output[o];
      child.output[o].value = new float[parent.output[o].num_rules];
      copy(parent.output[o].value, parent.output[o].value + parent.output[o].num_rules, child.output[o].value);
      
      child.output[o].scale = new float[parent.output[o].num_rules];
      copy(parent.output[o].scale, parent.output[o].scale + parent.output[o].num_rules, child.output[o].scale);
      
      child.output[o].rules = new Rule[parent.output[o].num_rules];
      copy(parent.output[o].rules, parent.output[o].rules + parent.output[o].num_rules, child.output[o].rules);
      
      //child.output[o].vars = new short int[parent.output[o].num_vars];
      //copy(parent.output[o].vars , parent.output[o].vars + parent.output[o].num_vars , child.output[o].vars);
      

      for(int r = 0; r < parent.output[o].num_rules; r++){
        child.output[o].rules[r] = parent.output[o].rules[r];
        child.output[o].rules[r].sets = new short int[parent.output[o].num_vars];
        copy(parent.output[o].rules[r].sets , parent.output[o].rules[r].sets + parent.output[o].num_vars , child.output[o].rules[r].sets);
      }
    }
}
