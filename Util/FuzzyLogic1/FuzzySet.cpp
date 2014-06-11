#include "FuzzySet.h"
#include "FuzzyCollection.h"

FuzzySet::FuzzySet() { }
FuzzySet::FuzzySet(pair <float, float> centre,float height, float leftBase, float rightBase, float leftTop, float rightTop, FuzzyCollection & parent) : centre(centre), height(height), leftBase(leftBase), rightBase(rightBase), leftTop(leftTop), rightTop(rightTop), parent(parent)  { }

float FuzzySet::Evaluate(float rangeVariable) {

}

void FuzzySet::GrowSet(float size) {
  //mutate the set
  leftBase -= size;
  leftTop -= size;
  rightBase += size;
  rightTop += size;
/*
  //check the set is in bounds
  if( leftBase < parent.min)
    leftBase = parent.min;
  if( leftTop < parent.min)
    leftTop = parent.min;
  if( rightBase > parent.max)
    rightBase = parent.max;
  if( rightTop > parent.max)
    rightTop = parent.max;
*/
  }
