#include "template.h"

#include "..\shared\shared.h"

using namespace std;

//sim runtime vars
float * in1;
float * in2;
float * out;
short int * score;
static Accumulator output = {0, THRUST_MAX, 0.f, 0, 0, 0};
static FuzzyVar in1Set  = {0, START_HEIGHT, START_HEIGHT, 0};
static FuzzyVar in2Set = {-TERMINAL_VELOCITY, TERMINAL_VELOCITY, START_VEL, 0};
static FuzzyVar fitness  = {0, START_FUEL, START_FUEL, 0};

void TemplateCreateVars(){
    //sim init vars
  simInput[0] = in1Set;
  simInput[1] = in2Set;
  simOutput = &output;
  simFitness = &fitness;
}

void TemplateInitSim(int controller) {
  thrust = &cont[controller].output.output;
  height = &cont[controller].input[0].value;
  velocity = &cont[controller].input[1].value;
  score = &cont[controller].score;
}
int TemplateNextStep(int controller) {


  if(*score <= 0.0f) {
    return 0; //fail
  }
  else {
    return -1; //continue
  }
}