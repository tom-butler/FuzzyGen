#include "controller.h"
#include "shared.h"
#include "gen.h"
#include "..\sims\moon.h"
#include <stdio.h>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

void InitControllers() {
  CreateControllers(POP, simInput, *simOutput);
}

void InitSimulation(int controller) {
  if(SIM == MOONLANDER)
    MoonInitSim(controller);
}

int RunSim(int controller) {
  EvaluateRules(controller);
  if(SIM == MOONLANDER)
    return MoonNextStep(controller);
}

void Breed() {
  Select();
}

void UpdateLog(int g){
  ostringstream ss;
  ss << g << "," << BEST << "," << MEAN << "," << LOW << "\n";
  string text(ss.str());
  LOG[g] = text;
}

void DrawSim(){
  if(SIM == MOONLANDER)
    DrawMoonSim();
}

void WriteLog(){

    time_t now = time(0);
    ostringstream ss;
    ofstream output;

    ss << "logs/gen/gen" << now << ".csv";
    output.open(ss.str().c_str());
    output << "Generation,Best,Mean,Low\n";
    for(int i = 0; i < GENERATIONS; i++){
      output << LOG[i];
    }
    output.close();
    ss.str("");
    ss.clear();

    ss << "logs/controller/cont" << now << ".csv";
    output.open(ss.str().c_str());
    output << "Controller,Score,Mutations\n";
    for(int controller = 0; controller < POP; controller++) {
      output << controller << ",";
      output << cont[controller].score << ",";
      output << cont[controller].mutations << "\n";
    }
    output.close();
    ss.str("");
    ss.clear();

    //input
    ss << "logs/input/input" << now << ".csv";
    output.open(ss.str().c_str());
    output << "Input,Low,High\n";
    for(int i = 0; i < NUM_INPUT; i++){
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
    for(int controller = 0; controller < POP; controller++) {
      for(int i = 0; i < NUM_INPUT; i++) {
        for(int s = 0; s < NUM_SETS; s++) {
          output << controller << ",";
          output << i << ",";
          output << s << ",";
          output << cont[controller].input[i].sets[s].height << ",";
          output << cont[controller].input[i].sets[s].centreX << ",";
          output << cont[controller].input[i].sets[s].leftBase << ",";
          output << cont[controller].input[i].sets[s].rightBase << ",";
          output << cont[controller].input[i].sets[s].leftTop << ",";
          output << cont[controller].input[i].sets[s].rightTop << "\n";
        }
      }
    }
    output.close();
    ss.str("");
    ss.clear();

    //Rules
    ss << "logs/rule/rule" << now << ".csv";
    output.open(ss.str().c_str());
    output << "Controller,Rule,Input Variable 1,Input Set 1,Modifier,Input Variable 2,Input Set 2,Output Value\n";
    for(int controller = 0; controller < POP; controller++) {
      for(int i = 0; i < NUM_RULES; i++) {
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

    //Rules
    ss << "logs/var/var" << now << ".csv";
    output.open(ss.str().c_str());
    //genetic
    output << "POP," << POP << "\n";
    output << "GENERATIONS," << GENERATIONS << "\n";
    output << "ANCESTOR," << ANCESTOR << "\n";
    output << "VARIANCE," << VARIANCE << "\n";
    output << "MUT_CHANCE," << MUT_CHANCE << "\n";
    output << "BEST," << BEST << "\n";
    output << "BEST_CONT," << BEST_CONT << "\n";
    output << "INCLUDE_CONTROL," << INCLUDE_CONTROL << "\n";
    output << "MEAN," << MEAN << "\n";
    output << "LOW," << LOW << "\n";
    output << "LOGGING," << LOGGING << "\n";

    //fuzzy
    output << "NUM_INPUT," << NUM_INPUT << "\n";
    output << "NUM_SETS," << NUM_SETS << "\n";
    output << "NUM_RULES," << NUM_RULES << "\n";
    output << "HEIGHT," << HEIGHT << "\n";

    //sim
    output << "START_HEIGHT," << START_HEIGHT << "\n";
    output << "START_FUEL," << START_FUEL << "\n";
    output << "STAR_VEL," << START_VEL << "\n";
    output << "THRUST_MAX," << THRUST_MAX << "\n";
    output << "TERMINAL_VELOCITY," << TERMINAL_VELOCITY << "\n";
    output << "FORCE," << FORCE << "\n";
    output << "CRASH_SPEED," << CRASH_SPEED << "\n";
    output.close();
    ss.str("");
    ss.clear();
}