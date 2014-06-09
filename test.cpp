#include "Util/FloatPoint.cpp"
#include "Util/FuzzySet.cpp"
#include "Util/FuzzyCollection.cpp"

FuzzyCollection c();
FuzzySet s();
int main() {
  FloatPoint p(10.5f, 10.3f);
  FuzzySet s(p,10.0f,10.0f,10.0f,10.0f,10.0f);

  FuzzyCollection c("name", 0, 100);
}