#include "gen.h"

#include <ctime>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>


#include "controller\controller.h"
#include "shared.h"
#include "..\sims\sim.h"

using namespace std;

void InitControllers() {
  if(kIncludeControl)
    SimControlController(0, simInput, simOutput);
  Create(kNumPop, simInput, simOutput);
}

void InitSimulation(int controller) {
  SimInitSim(controller);
}

int RunSim(int controller) {
  Run(controller);
  return SimNextStep(controller);
}

void Breed() {
  Select();
}

void UpdateLog(int g) {
  ostringstream ss;

  //gen
  ss << g << "," << BEST_SCORE << "," << BEST_GEN_SCORE << "," << AVG_GEN << "," << LOW_GEN << "\n";
  string text(ss.str());
  GEN_LOG[g] = text;
  ss.str("");
  ss.clear();
  
}

void WriteLog() {

    time_t now = time(0);
    ostringstream ss;
    ofstream output;

    ss << "logs/gen/" << kTestFile << "-" << kTest << "-" << kRunNum << "-gen-" << now << ".csv";
    output.open(ss.str().c_str());
    output << "Generation,Best Max,Gen Best,Mean,Low\n";
    for(int i = 0; i < kNumGenerations; i++){
      output << GEN_LOG[i];
    }
    output.close();
    ss.str("");
    ss.clear();

    ss << "logs/controller/" << kTestFile << "-" << kTest << "-" << kRunNum << "-cont-" << now << ".csv";
    output.open(ss.str().c_str());
    output << "Controller,Score,Mutations,Rule Number\n";
    for(int controller = 0; controller < kNumPop; controller++) {
      output << controller << ",";
      output << cont[controller].score << ",";
      output << cont[controller].num_mutations << "\n";
    }
    output.close();
    ss.str("");
    ss.clear();

    //Sets
    ss << "logs/set/" << kTestFile << "-" << kTest << "-" << kRunNum << "-set-" << now << ".csv";
    output.open(ss.str().c_str());
    output << "Controller,Input,Set,Height,CentreX,Left Base,Right Base,Left Top,Right Top\n";
    for(int controller = 0; controller < kNumPop; controller++) {
      for(int i = 0; i < kNumInput; i++) {
        for(int s = 0; s < cont[controller].input[i].num_sets; s++) {
          output << controller << ",";
          output << i << ",";
          output << s << ",";
          output << cont[controller].input[i].sets[s].height << ",";
          output << cont[controller].input[i].sets[s].centre_x << ",";
          output << cont[controller].input[i].sets[s].left_base << ",";
          output << cont[controller].input[i].sets[s].right_base << ",";
          output << cont[controller].input[i].sets[s].left_top << ",";
          output << cont[controller].input[i].sets[s].right_top << "\n";
        }
      }
    }
    output.close();
    ss.str("");
    ss.clear();

    //Rules
    
    ss << "logs/rule/" << kTestFile << "-" << kTest << "-" << kRunNum << "-rule-" << now << ".csv";
    output.open(ss.str().c_str());
    output << "Controller,Accumulator,Rule,Inputs,Output Value\n";
    for(int controller = 0; controller < kNumPop; controller++) {
      for(int accumulator = 0; accumulator < kNumOutput; accumulator++)
      for(int i = 0; i < cont[controller].output[accumulator].num_rules; i++) {
        output << controller << ",";
        output << accumulator << ",";
        output << i << ",";
        output << cont[controller].output[accumulator].rules[i].sets[0];
        for(int v = 1; v < kNumInput; v++) {
          output << " " << cont[controller].output[accumulator].rules[i].sets[v];
        }
        output << "," << cont[controller].output[accumulator].rules[i].output << "\n";
      }
    }
    output.close();
    ss.str("");
    ss.clear();

    //settings (this has been designed to be able to be loaded as a test)
    ss << "logs/settings/" << kTestFile << "-" << kTest << "-" << kRunNum << "-settings-"  << now << ".csv";
    output.open(ss.str().c_str());
    //heading
    output << "TestNo" << ",";
    output << "NumPop" << ",";
    output << "NumGenerations" << ",";
    output << "NumAncestor" << ",";
    output << "IncludeControl" << ",";
    output << "Logging" << ",";
    output << "RandomStart" << ",";
    output << "NumTests" << ",";
    output << "Elitism" << ",";
    output << "ForceSetOverlap" << ",";
    output << "ForceSetCoverage" << ",";
    output << "MutationChance" << ",";
    output << "Variance" << ",";
    output << "BreedPercent" << ",";
    output << "InitialMutation" << ",";
    output << "CollectionInitialMutaion" << ",";
    output << "SetInitialMutation" << ",";
    output << "RuleInitialMutation" << ",";
    output << "GrowMutation" << ",";
    output << "CollectionGrowMutation" << ",";
    output << "SetGrowTopMutation" << ",";
    output << "SetGrowBottomMutation" << ",";
    output << "RuleGrowMutation" << ",";
    output << "SlideMutation" << ",";
    output << "CollectionSlideMutation" << ",";
    output << "SetSlideMutation" << ",";
    output << "SetSlideTopMutation" << ",";
    output << "SetSlideBottomMutation" << ",";
    output << "RuleSlideMutation" << ",";
    output << "NumInput" << ",";
    output << "NumOutput" << ",";
    output << "NumSetsMin" << ",";
    output << "NumSetsMax" << ",";
    output << "SetNumberMutation" << ",";
    output << "SetHeightMin" << ",";
    output << "SetHeightMax" << ",";
    output << "Sim" << ",";
    output << "Select" << ",";
    output << "Breed" << "\n";

    //values
    output << kTest << ",";
    output << kNumPop << ",";
    output << kNumGenerations << ",";
    output << kNumAncestor << ",";
    output << kIncludeControl << ",";
    output << kLogging << ",";
    output << kRandomStart << ",";
    output << kNumTests << ",";
    output << kElitism << ",";
    output << kForceSetOverlap << ",";
    output << kForceSetCoverage << ",";
    output << kMutationChance << ",";
    output << kVariance << ",";
    output << kBreedPercent << ",";
    output << kInitialMutation << ",";
    output << kCollectionInitialMutaion << ",";
    output << kSetInitialMutation << ",";
    output << kRuleInitialMutation << ",";
    output << kGrowMutation << ",";
    output << kCollectionGrowMutation << ",";
    output << kSetGrowTopMutation << ",";
    output << kSetGrowBottomMutation << ",";
    output << kRuleGrowMutation << ",";
    output << kSlideMutation << ",";
    output << kCollectionSlideMutation << ",";
    output << kSetSlideMutation << ",";
    output << kSetSlideTopMutation << ",";
    output << kSetSlideBottomMutation << ",";
    output << kRuleSlideMutation << ",";
    output << kNumInput << ",";
    output << kNumOutput << ",";
    output << kNumSetsMin << ",";
    output << kNumSetsMax << ",";
    output << kSetNumberMutation << ",";
    output << kSetHeightMin << ",";
    output << kSetHeightMax << ",";
    output << kSim << ",";
    output << kSelect << ",";
    output << kBreed << "\n";

    output.close();
    ss.str("");
    ss.clear();
}