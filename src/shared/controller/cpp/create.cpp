#include "..\h\create.h"

#include <algorithm>
#include <iostream>

#include "..\..\shared.h"


using namespace std;


//initialisation
void CreateControllers(int num_controllers, FuzzyVar input[], Accumulator output[]) {

  for(int i = 0; i < num_controllers;i++) {
    if(kIncludeControl && i == 0){
      continue;
    }
    //create input sets
    cont[i].input = new FuzzyVar[kNumInput];
    copy(input,input + kNumInput, cont[i].input);
    for(int j = 0; j < kNumInput; j++)
    {
      cont[i].input[j].sets = new Set[kNumSetsMax];
      if(kSetNumberMutation){
        CreateSets(i, j, GetRandInt(kNumSetsMin, kNumSetsMax));
      }
      else{
        CreateSets(i, j ,kNumSetsMin);
      }
    }
    //create accumulators
    cont[i].output = new Accumulator[kNumOutput];
    copy(output,output + kNumOutput, cont[i].output);
    for(int o = 0; o < kNumOutput; o++){
      cont[i].output[o].num_rules = 1;
      for(int p = 0; p < cont[i].output[o].num_vars; p++){
        cont[i].output[o].num_rules *= cont[i].input[cont[i].output[o].vars[p]].num_sets;
      }
      cont[i].output[o].value = new float[cont[i].output[o].num_rules];
      cont[i].output[o].scale = new float[cont[i].output[o].num_rules];
      cont[i].output[o].rules = new  Rule[cont[i].output[o].num_rules];
      CreateRules(i, o);
    }
  }
  //initialise the BEST CONTROLLER 
  BEST_CONTROLLER.output = new Accumulator[kNumOutput];
  copy(output,output + kNumOutput, BEST_CONTROLLER.output);
  CopyController(cont[0], BEST_CONTROLLER);

}

void CreateSets(int controller, int variable, short int numSets) {
  //create initial variables
  short int start = cont[controller].input[variable].low;
  short int end = cont[controller].input[variable].high;

  short int space = (end - start)  / (numSets - 1);
  short int centre = start;
  for(int j = 0; j < numSets; j++) {

    short int lbase = (0.7 * space) + GetRandInt(0.0f, (end - start)* kVariance);
    short int rbase = 0.7 * space + GetRandInt(0.0f, (end - start)* kVariance);
    short int ltop = 0.3 * space + GetRandInt(0.0f, (end - start)* kVariance);
    short int rtop = 0.3 * space + GetRandInt(0.0f, (end - start)* kVariance);

    //build the set
    Set s = {GetRandFloat(kSetHeightMin, kSetHeightMax), centre, lbase, rbase, ltop, rtop};

    cont[controller].input[variable].sets[j] = s;
    //increment the centre for next set
    centre += space;
  }
  cont[controller].input[variable].num_sets = numSets;
  ForceVarBounds(controller,variable);
}

void CreateRules(int controller, int accumulator) {
  short int * num_vars;
  num_vars = &cont[controller].output[accumulator].num_vars;
  int *var = new int[*num_vars];

  for(int i = 0; i < *num_vars; i++) {
    var[i] = 0;
  }

  for(int r = 0; r < cont[controller].output[accumulator].num_rules; ++r){
    //create the rule
    Rule rule = {0, GetRandFloat(cont[controller].output[accumulator].low,cont[controller].output[accumulator].high), 0};
    cont[controller].output[accumulator].rules[r] = rule;
    cont[controller].output[accumulator].rules[r].sets = new short int[*num_vars];
    
    //iterate the sets
    for(int v = *num_vars - 1; v > 0; v--){
      if(var[v] >= cont[controller].input[cont[controller].output[accumulator].vars[v]].num_sets) {
        var[v] = 0;
        if(v > 0){
          var[v - 1]++;
        }
      }
    }

    //add the sets to the rule
    for(int v = 0; v < *num_vars; v++){
      cont[controller].output[accumulator].rules[r].sets[v] = var[v];
    }

    var[*num_vars - 1]++;
  }

  delete [] var;
}