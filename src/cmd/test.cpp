#include <iostream>

#include controller.h
#include sim.h
#include gen.h

int TestGetRandInt();

int main ()
{
  cout << "Testing controller\n";
  cout << "------------------\n\n";
  int controllerErrors = 0;
  controllerErrors += TestGetRandInt();
  controllerErrors += TestIntersect();
  controllerErrors += TestController();
  controllerErrors += TestSets();
  controllerErrors += TestEvaluateSet();
  cout << "Testing Controller Complete\n";

  cout << "Testing Genetics\n";
  cout << "------------------\n\n";
  int genErrors = 0;
  cout << "Testing Genetics Complete";

    cout << "Testing Simulation\n";
  cout << "------------------\n\n";
  int simErrors = 0;
  cout << "Testing Simulation Complete";

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

  result = Intersect(5, 0, 35, 30, 20);
  expected = 15;
  if(result != expected) {
    cout << "Failed at 0\n";
    cout << "Expected " << expected << "Actual Result " << result;
    return 1;
  }
  result = Intersect(5, 0, 45, 30, 20);
  expected = 11.5f;
  if(result != expected) {
    cout << "Failed at 1\n";
    cout << "Expected " << expected << "Actual Result " << result;
    return 1;
  }
  result = Intersect(0, 25, 50, 0, 30);
  expected = 10;
  if(result != expected) {
    cout << "Failed at 2\n";
    cout << "Expected " << expected << "Actual Result " << result;
    return 1;
  }
  result = Intersect(0, 14, 40, 0, 25);
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
  Singleton out = {-10, 10, 0, 0};
  int Num = 100;

  CreateControllers(Num, input, output);
  if(cont[0].score != 0)
    cout << "FAILED at 0\n";
    return 1;
  }
  if(cont[99].score != 0)
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
  if(cont[0].output.value != 0){
    cout << "FAILED at 7\n";
    return 1;
  }
  cout << "OK\n";
  return 0;
}

int TestSets()
{
  cout << "Testing Sets                  ";
  for(int i = 0; i < 2; i++) {
    int centre = cont[0].input[0].sets[0].centreX;
    int lBase = cont[0].input[0].sets[0].leftBase;
    int lTop = cont[0].input[0].sets[0].leftTop;
    int rTop = cont[0].input[0].sets[0].rightTop;
    int rBase = cont[0].input[0].sets[0].rightBase;
    //is lbase < ltop
    if(( centre - lBase) > (centre - lTop)) {
      cout << "FAILED at " << i;
      cout << "-0\n";
      return 1;
    }
    //is ltop < var.low
    if(( centre - lTop) < cont[0].input[0].low) {
      cout << "FAILED at " << i;
      cout << "-1\n";
      return 1;
    }
    //is centre inside low and high
    if(centre < cont[0].input[0].low) {
      cout << "FAILED at " << i;
      cout << "-2\n";
      return 1;
    }
    if(centre > cont[0].input[0].high) {
      cout << "FAILED at " << i;
      cout << "-3\n";
      return 1;
    }
    if((centre + rTop) < (centre + rBase)) {
      cout << "FAILED at " << i;
      cout << "-4\n";
      return 1;
    }
    if((centre + rTop) > cont[0].input[0].high) {
      cout << "FAILED at " << i;
      cout << "-5\n";
      return 1;
    }
    cout << "OK\n";
    return 0;
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