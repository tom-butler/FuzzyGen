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

  if(cont[0].input[0].low != 0){
    cout << "FAILED at 0\n";
    return 1;
  }
  if(cont[0].input[1].value != -50){
    cout << "FAILED at 1\n";
    return 1;
  }
  if(cont[0].input[1].high != 0){
    cout << "FAILED at 2\n";
    return 1;
  }

  if(cont[0].output.low != -50){
    cout << "FAILED at 3\n";
    return 1;
  }

}