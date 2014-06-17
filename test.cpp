#include "Util/FuzzyLogic1/FuzzySet.h"
#include "Util/FuzzyLogic1/FuzzyCollection.h"
#include "Util/FuzzyLogic1/FuzzyController.h"

FuzzyCollection c();
FuzzySet s();
int main() {

//create a controller
FuzzyController controller;

//create collection
  FuzzyCollection c("name", 0, 100);

  pair <float, float> p(10.5f, 10.3f);
  FuzzySet s("low"p,10.0f,10.0f,10.0f,10.0f,10.0f, c);

  pair <float, float> p(10.5f, 10.3f);
  FuzzySet s("med",p,10.0f,10.0f,10.0f,10.0f,10.0f, c);

  pair <float, float> p(10.5f, 10.3f);
  FuzzySet s("high",p,10.0f,10.0f,10.0f,10.0f,10.0f, c);

  controller.AddCollection(c);
}