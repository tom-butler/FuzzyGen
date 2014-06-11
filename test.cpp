#include "Util/FuzzyLogic1/FuzzySet.h"
#include "Util/FuzzyLogic1/FuzzyCollection.h"

FuzzyCollection c();
FuzzySet s();
int main() {

    FuzzyCollection c("name", 0, 100);
  pair <float, float> p(10.5f, 10.3f);
  FuzzySet s(p,10.0f,10.0f,10.0f,10.0f,10.0f, c);


}