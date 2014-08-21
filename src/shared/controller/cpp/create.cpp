#include "..\h\create.h"

#include <algorithm>
#include <iostream>

#include "..\..\shared.h"


using namespace std;

void CreateSets(int controller, int variable,short int numSets);

//initialisation
void CreateControllers(int num_controllers, FuzzyVar input[], Accumulator output[]) {

 for(int i = 0; i < num_controllers;i++) {
    //create input sets
    cont[i].input = new FuzzyVar[NUM_INPUT];
    copy(input,input+NUM_INPUT, cont[i].input);
    for(int j = 0; j < NUM_INPUT; j++)
    {
      cont[i].input[j].sets = new Set[MAX_NUM_SETS];
      if(MUT_SET_NUM){
        CreateSets(i, j, GetRandInt(MIN_NUM_SETS, MAX_NUM_SETS));
      }
      else{
        CreateSets(i, j ,MIN_NUM_SETS);
      }
    }
    //create accumulators
    cont[i].output = new Accumulator[NUM_OUTPUT];
    copy(output,output+NUM_OUTPUT, cont[i].output);
    for(int o = 0; o < NUM_OUTPUT; o++){
      cont[i].output[o].ruleNum = 1;
      for(int p = 0; p < cont[i].output[o].varsNum; p++){
        cont[i].output[o].ruleNum *= cont[i].input[cont[i].output[o].vars[p]].setNum;
      }
      cont[i].output[o].value = new float[cont[i].output[o].ruleNum];
      cont[i].output[o].scale = new float[cont[i].output[o].ruleNum];
      cont[i].output[o].rules = new  Rule[cont[i].output[o].ruleNum];
      CreateRules(i, o);
    }
  }
}

//@TODO: REPLACE MULTIPLE ARRAY CALLS WITH POINTER
void CreateSets(int controller, int variable,short int numSets) {
  //create initial variables
  short int start = cont[controller].input[variable].low;
  short int end = cont[controller].input[variable].high;

  short int space = (end - start)  / (numSets - 1);
  short int centre = start;
  for(int j = 0; j < numSets; j++) {

  short int lbase = (0.7 * space) + GetRandInt(0.0f, (end - start)* VARIANCE);
  short int rbase = 0.7 * space + GetRandInt(0.0f, (end - start)* VARIANCE);
  short int ltop = 0.3 * space + GetRandInt(0.0f, (end - start)* VARIANCE);
  short int rtop = 0.3 * space + GetRandInt(0.0f, (end - start)* VARIANCE);

    //check set variables for compliance
    if(centre - ltop < start) {
      ltop = centre - start;
      lbase = centre - start;
    }
    if(rtop + centre > end) {
      rtop = end - centre;
      rbase = end - centre;
    }

    //build the set
    Set s = {GetRandFloat(HEIGHT_LOW, HEIGHT_HIGH), centre, lbase, rbase, ltop, rtop};

      cont[controller].input[variable].sets[j] = s;
    //increment the centre for next set
    centre += space;
  }
  cont[controller].input[variable].setNum = numSets;
}

void CreateRules(int controller, int accumulator) {
  short int * varsNum;
  varsNum = &cont[controller].output[accumulator].varsNum;
  int *var = new int[*varsNum];

  for(int i = 0; i < *varsNum; i++) {
    var[i] = 0;
  }

  for(int r = 0; r < cont[controller].output[accumulator].ruleNum; ++r){
    //create the rule
    Rule rule = {0, GetRandFloat(cont[controller].output[accumulator].low,cont[controller].output[accumulator].high), 0};
    cont[controller].output[accumulator].rules[r] = rule;
    cont[controller].output[accumulator].rules[r].sets = new short int[*varsNum];
    
    //iterate the sets
    for(int v = *varsNum - 1; v > 0; v--){
      if(var[v] >= cont[controller].input[cont[controller].output[accumulator].vars[v]].setNum) {
        var[v] = 0;
        if(v > 0){
          var[v - 1]++;
        }
      }
    }

    //add the sets to the rule
    for(int v = 0; v < *varsNum; v++){
      cont[controller].output[accumulator].rules[r].sets[v] = var[v];
    }

    var[*varsNum-1]++;
  }

  delete [] var;
}