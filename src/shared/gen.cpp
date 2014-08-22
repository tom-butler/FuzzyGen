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
  Create(kNumPop, simInput, simOutput);
  if(kIncludeControl)
    SimControlController(0);
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

void UpdateLog(int g){
  ostringstream ss;
  ss << g << "," << BEST_SCORE << "," << BEST_GEN << "," << MEAN_GEN << "," << LOW_GEN << "\n";
  string text(ss.str());
  LOG[g] = text;
}

void WriteLog(){

    time_t now = time(0);
    ostringstream ss;
    ofstream output;

    ss << "logs/gen/gen" << now << ".csv";
    output.open(ss.str().c_str());
    output << "Generation,Best Max,Gen Best,Mean,Low\n";
    for(int i = 0; i < kNumGenerations; i++){
      output << LOG[i];
    }
    output.close();
    ss.str("");
    ss.clear();

    ss << "logs/controller/cont" << now << ".csv";
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

    //input
    ss << "logs/input/input" << now << ".csv";
    output.open(ss.str().c_str());
    output << "Input,Low,High\n";
    for(int i = 0; i < kNumInput; i++){
      output << i << ",";
      output << cont[0].input[i].low << ",";
      output << cont[0].input[i].high << "\n";
    }
    output.close();
    ss.str("");
    ss.clear();

    //Sets
    ss << "logs/set/set" << now << ".csv";
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
    /*
    ss << "logs/rule/rule" << now << ".csv";
    output.open(ss.str().c_str());
    output << "Controller,Rule,Input Variable 1,Input Set 1,Modifier,Input Variable 2,Input Set 2,Output Value\n";
    for(int controller = 0; controller < POP; controller++) {
      for(int i = 0; i < cont[controller].ruleNum; i++) {
        output << controller << ",";
        output << i << ",";
        output << cont[controller].rules[i].inputvar << ",";
        output << cont[controller].rules[i].inputset << ",";
        output << cont[controller].rules[i].modifier << ",";
        output << cont[controller].rules[i].inputvar2 << ",";
        output << cont[controller].rules[i].inputset2 << ",";
        output << cont[controller].rules[i].output << "\n";
      }
    }
    output.close();
    ss.str("");
    ss.clear();
*/
    //Rules
    ss << "logs/var/var" << now << ".csv";
    output.open(ss.str().c_str());
    //genetic
    output << "POP," << kNumPop << "\n";
    output << "GENERATIONS," << kNumGenerations << "\n";
    output << "ANCESTOR," << kNumAncestor << "\n";
    output << "VARIANCE," << kVariance << "\n";
    output << "MUT_CHANCE," << kMutationChance << "\n";
    output << "BEST," << BEST_GEN << "\n";
    output << "BEST_CONT," << BEST_GEN_CONTROLLER << "\n";
    output << "INCLUDE_CONTROL," << kIncludeControl << "\n";
    output << "MEAN," << MEAN_GEN << "\n";
    output << "LOW," << LOW_GEN << "\n";

    //fuzzy
    output << "NUM_INPUT," << kNumInput << "\n";

    output.close();
    ss.str("");
    ss.clear();
}