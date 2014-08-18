#include <iostream>
#include <stdio.h>
#include <string.h>

#include "../shared/shared.h"
#include "../shared/controller.h"
#include "../shared/controller.cpp"
#include "../shared/gen.h"
#include "../shared/gen.cpp"
#include "../sims/moon.h"
#include "../sims/moon.cpp"

int TestGetRandInt();
int TestIntersect();
int TestController();
int TestSets();
int TestEvaluateSet();
int TestEvaluateRule();
int TestInitSim();
int TestNextStep();
int TestSelectMean();
int TestSelectHalf();

int main ()
{
  cout << "Init System\n";
  InitSystem();

  cout << "Testing controller\n";
  cout << "------------------\n\n";
  int controllerErrors = 0;
  controllerErrors += TestGetRandInt();
  controllerErrors += TestIntersect();
  controllerErrors += TestController();
  controllerErrors += TestSets();
  controllerErrors += TestEvaluateSet();
  controllerErrors += TestEvaluateRule();
  cout << "Testing Controller Complete\n";

  cout << "Testing Simulation\n";
  cout << "------------------\n\n";
  int simErrors = 0;
  simErrors += TestInitSim();
  simErrors += TestNextStep();
  cout << "Testing Simulation Complete";

  cout << "Testing Genetics\n";
  cout << "------------------\n\n";
  int genErrors = 0;
  genErrors += TestSelectMean();
  genErrors += TestSelectHalf();
  cout << "Testing Genetics Complete";

  cout << "Testing Complete";
  cout << "------------------\n\n";
  if(controllerErrors > 0)
    cout << "Found: " << controllerErrors  << " Controller Errors\n";
  if(genErrors > 0)
    cout << "Found: " << genErrors  << " Genetic Errors\n";
  if(simErrors > 0)
    cout << "Found: " << simErrors  << " Simulation Errors\n";
}


//Test controller
//-------------------------------------------------------

//Check GetRand Int
int TestGetRandInt()
{
  cout << "Testing GetRandInt            ";
  for(int i = 0; i < 100; i++)
  {
    int test = GetRandInt(i, (100-i));
    if(test < i || test > (100-i))
    {
      cout << "FAILED at " << i;
      return 1;
    }
  }
  cout << "OK";
  return 0;
}

//Check Intersect
int TestIntersect()
{
  cout << "Testing Intersect             ";
  float result = 0;
  float expected = 0;

  result = Lerp(5.0f, 0, 35.0f, 30.0f, 20.0f);
  expected = 15;
  if(result != expected) {
    cout << "Failed at 0\n";
    cout << "Expected " << expected << "Actual Result " << result;
    return 1;
  }
  result = Lerp(5, 0, 45, 30, 20);
  expected = 11.5f;
  if(result != expected) {
    cout << "Failed at 1\n";
    cout << "Expected " << expected << "Actual Result " << result;
    return 1;
  }
  result = Lerp(0, 25, 50, 0, 30);
  expected = 10;
  if(result != expected) {
    cout << "Failed at 2\n";
    cout << "Expected " << expected << "Actual Result " << result;
    return 1;
  }
  result = Lerp(0, 14, 40, 0, 25);
  expected = 5.5f;
  if(result != expected) {
    cout << "Failed at 2\n";
    cout << "Expected " << expected << "Actual Result " << result;
    return 1;
  }

  cout << "OK\n";
  return 0;
}

int TestController()
{
  cout << "Testing CreateController      ";
  //create some input FuzzyVars
  FuzzyVar in1 = {0, 1000, 0, 0};
  FuzzyVar in2 = {-50, 0, -50, 0};
  FuzzyVar input[2] = {in1, in2};
  //create an output singleton set
  Accumulator out = {-10, 10, 0.0f, 0, 0 , 0};
  int Num = 100;

  CreateControllers(Num, input, out);
  if(cont[0].score != START_FUEL){
    cout << "FAILED at 0\n";
    return 1;
  }
  if(cont[99].score != START_FUEL){
    cout << "FAILED at 1\n";
    return 1;
  }
  if(cont[0].input[0].low != 0){
    cout << "FAILED at 2\n";
    return 1;
  }
  if(cont[0].input[1].value != -50){
    cout << "FAILED at 3\n";
    return 1;
  }
  if(cont[0].input[1].high != 0){
    cout << "FAILED at 4\n";
    return 1;
  }

  if(cont[0].output.low != -10){
    cout << "FAILED at 5\n";
    return 1;
  }
  if(cont[0].output.high != 10){
    cout << "FAILED at 6\n";
    return 1;
  }
  if(cont[0].output.output != 0){
    cout << "FAILED at 7\n";
    return 1;
  }
  for(int i = 0; i < NUM_RULES; i++){
    if(cont[0].rules[i].inputvar < 0){
      cout << "FAILED at 8\n";
      return 1;
    }
    if(cont[0].rules[i].inputvar2 > 1){
      cout << "FAILED at 9\n";
      return 1;
    }
    if(cont[0].rules[i].inputset < 0){
      cout << "FAILED at 10\n";
      return 1;
    }
    if(cont[0].rules[i].inputset >= NUM_SETS){
      cout << "FAILED at 11\n";
      return 1;
    }
    if(cont[0].rules[i].inputset2 < 0){
      cout << "FAILED at 12\n";
      return 1;
    }
    if(cont[0].rules[i].inputset2 >= NUM_SETS){
      cout << "FAILED at 13\n";
      return 1;
    }
    if(cont[0].rules[i].output < 0){
      cout << "FAILED at 14\n";
      return 1;
    }
    if(cont[0].rules[i].output > THRUST_MAX){
      cout << "FAILED at 15\n";
      return 1;
    }
    if(strcmp(cont[0].rules[i].modifier.c_str(),"AND") == 0 || strcmp(cont[0].rules[0].modifier.c_str(),"OR") == 0){
      cout << "FAILED at 16\n";
      return 1;
    }
  }
  cout << "OK\n";
  return 0;
}

int TestSets()
{
  cout << "Testing Sets                  ";
  for(int c = 0; c < POP; c++) {
    for(int i = 0; i < NUM_INPUT; i++) {
      for(int s = 0; s < NUM_SETS;s++) {
        int centre = cont[c].input[i].sets[0].centreX;
        int lBase = cont[c].input[i].sets[0].leftBase;
        int lTop = cont[c].input[i].sets[0].leftTop;
        int rTop = cont[c].input[i].sets[0].rightTop;
        int rBase = cont[c].input[i].sets[0].rightBase;
        //is lbase < ltop
        if(( centre - lBase) > (centre - lTop)) {
          cout << "FAILED at " << c << "-" << i << "-" << s;
          cout << "-0\n";
          return 1;
        }
        //is ltop < var.low
        if(( centre - lTop) < cont[0].input[0].low) {
          cout << "FAILED at " << c << "-" << i << "-" << s;
          cout << "-1\n";
          return 1;
        }
        //is centre inside low and high
        if(centre < cont[0].input[0].low) {
          cout << "FAILED at " << c << "-" << i << "-" << s;
          cout << "-2\n";
          return 1;
        }
        if(centre > cont[0].input[0].high) {
          cout << "FAILED at " << c << "-" << i << "-" << s;
          cout << "-3\n";
          return 1;
        }
        if((centre + rTop) < (centre + rBase)) {
          cout << "FAILED at " << c << "-" << i << "-" << s;
          cout << "-4\n";
          return 1;
        }
        if((centre + rTop) > cont[0].input[0].high) {
          cout << "FAILED at " << c << "-" << i << "-" << s;
          cout << "-5\n";
          return 1;
        }
        cout << "OK\n";
        return 0;
      }
    }
  }
}

int TestEvaluateSet(){
  cout << "Testing Fuzzyfication         ";
  float last = -1;
  for(int i = 0; i < 2; i++) {
    for(int s = 0; s < NUM_SETS; s++) {
      int low = cont[0].input[i].low;
      int high = cont[0].input[i].high;
      for(int v = 0; v < (high - low); v++ ) {
        if(last = -1);
          last = EvaluateSet(0,i,s,v);

        int cur = EvaluateSet(0,i,s,v);
        if(cur - last > 0.1 || cur - last < -0.1) {
          cout << "FAILED at " << i;
          cout << "-" << s;
          cout << "-" << s;
          cout << "-5\n";

          cout << "Expected " << last;
          cout << " +- 0.1\n";
          cout << "Instead got " << cur << "\n";
          return 1;
        }
      }
    }
  }
  cout << "OK\n";
  return 0;
}

int TestEvaluateRule(){
  cout << "Testing Rule Evaluation       ";
  for(int i = 0; i < POP; i++){
    EvaluateRules(i);
    if(cont[i].output.output < cont[i].output.low) {
      cout << "FAILED at " << i << "-1\n";
      return 1;
    }
    if(cont[i].output.output > cont[i].output.high) {
      cout << "FAILED at " << i << "-2\n";
      return 1;
    }
    int active = 0;
    for(int r = 0; r < NUM_RULES; r++){
      if(cont[i].rules[r].isActive){
        active++;
      }
    }
    if(cont[i].output.active != active) {
      cout << "FAILED at " << i << "-3\n";
      return 1;
    }
  }
  cout << "OK\n";
  return 0;
}


int TestInitSim() {
  cout << "Testing Sim Init              ";
  float * lastThrustLoc;
  float * lastHeightLoc;
  float * lastVelocityLoc;
  short int * lastFuelLoc;
  for(int i = 0; i < POP; i++){
    InitSim(i);
    if(i > 0){
      if(lastThrustLoc == thrust) {
        cout << "FAILED at " << i << "-1\n";
        return 1;
      }
      if(lastHeightLoc == height) {
        cout << "FAILED at " << i << "-1\n";
        return 1;
      }
      if(lastVelocityLoc == velocity) {
        cout << "FAILED at " << i << "-2\n";
        return 1;
      }
      if(lastFuelLoc == fuel) {
        cout << "FAILED at " << i << "-3\n";
        return 1;
      }
    }
    lastThrustLoc = thrust;
    lastHeightLoc = height;
    lastVelocityLoc = velocity;
    lastFuelLoc = fuel;
  }
  cout << "OK\n";
  return 0;
}

int TestNextStep() {
  cout << "Testing Running Sim           ";
  for(int i = 0; i < POP; i++){
    InitSim(i);
    int result = -1;
    while(result == -1){
      EvaluateRules(i);
      result = NextStep(i);
      if(*fuel < 0) {
        cout << "FAILED at " << i << "-1\n";
        return 1;
      }
      if(*fuel > START_FUEL) {
        cout << "FAILED at " << i << "-2\n";
        return 1;
      }
      if(*height < 0) {
        cout << "FAILED at " << i << "-3\n";
        return 1;
      }
      if(*height > START_HEIGHT) {
        cout << "FAILED at " << i << "-4\n";
        return 1;
      }
      if(*thrust < 0) {
        cout << "FAILED at " << i << "-5\n";
        return 1;
      }
      if(*thrust > THRUST_MAX) {
        cout << "FAILED at " << i << "-6\n";
        return 1;
      }
      if(*velocity < -TERMINAL_VELOCITY) {
        cout << "FAILED at " << i << "-7\n";
        return 1;
      }
      if(*velocity > TERMINAL_VELOCITY) {
        cout << "FAILED at " << i << "-8\n";
        return 1;
      }
    }
  }
  cout << "OK\n";
  return 0;
}

int TestSelectMean(){
  cout << "Testing Select Mean           ";
  int totalmut = 0;
  int totalscore = 0;
  int totalmean = 0;
  for(int i = 0; i < POP; i++){
    totalmut += cont[i].mutations;
    totalscore += cont[i].score;
  }
  totalmean = totalscore / POP;
  SelectMean(totalmean);
  for(int i = 0; i < POP; i++) {
    int newmut = 0;
    int newscore = 0;
    int newmean = 0;
    for(int j = 0; j < POP; j++){
      newmut += cont[j].mutations;
      newscore += cont[j].score;
    }
    newmean = newscore / POP;
    //miniscle chance these will false negative
    if(newmut == totalmut) {
      cout << "FAILED at " << i << "-1\n";
      return 1;
    }
    if(newscore == totalscore) {
      cout << "FAILED at " << i << "-2\n";
      return 1;
    }
    if(newmean == totalmean) {
      cout << "FAILED at " << i << "-3\n";
      return 1;
    }

    if(cont[i].score == -2) {
      cout << "FAILED at " << i << "-4\n";
      return 1;
    }
    if(cont[i].score != START_FUEL) {
      cout << "FAILED at " << i << "-5\n";
      return 1;
    }
    if(cont[i].score != START_FUEL) {
      cout << "FAILED at " << i << "-6\n";
      return 1;
    }
  }
  cout << "\n";
  return 0;
}

int TestSelectHalf(){
  cout << "Testing Select Half           ";
  int totalmut = 0;
  int totalscore = 0;
  int totalmean = 0;
  for(int i = 0; i < POP; i++){
    totalmut += cont[i].mutations;
    totalscore += cont[i].score;
  }
  totalmean = totalscore / POP;
  SelectHalf();
  for(int i = 0; i < POP; i++) {
    int newmut = 0;
    int newscore = 0;
    int newmean = 0;
    for(int j = 0; j < POP; j++){
      newmut += cont[j].mutations;
      newscore += cont[j].score;
    }
    newmean = newscore / POP;
    //miniscle chance these will false negative
    if(newmut == totalmut) {
      cout << "FAILED at " << i << "-1\n";
      return 1;
    }
    if(newscore == totalscore) {
      cout << "FAILED at " << i << "-2\n";
      return 1;
    }
    if(newmean == totalmean) {
      cout << "FAILED at " << i << "-3\n";
      return 1;
    }

    if(cont[i].score == -2) {
      cout << "FAILED at " << i << "-4\n";
      return 1;
    }
    if(cont[i].score != START_FUEL) {
      cout << "FAILED at " << i << "-5\n";
      return 1;
    }
    if(cont[i].score != START_FUEL) {
      cout << "FAILED at " << i << "-6\n";
      return 1;
    }
  }
  cout << "\n";
  return 0;
}