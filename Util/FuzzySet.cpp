#ifndef FUZZYSET_H
#define FUZZYSET_H

#include "FloatPoint.cpp"
using namespace std;


class FuzzySet {
  public:
    //vars
    FloatPoint centre;
    float height;
    float leftBase;
    float rightBase;
    float leftTop;
    float rightTop;
    //FuzzyCollection& parent;

    //functions
    FuzzySet(FloatPoint centre,float height, float leftBase, float rightBase, float leftTop, float rightTop);
    //interpolate the fuzzy value
    float Evaluate(float rangeVariable);
    //grow/shrink the set around the centre
    void MutateGrow(float size);
    //slide the set by adjusting the centre
    void MutateSlide(float amount);

  private:
};

FuzzySet::FuzzySet(FloatPoint centre,float height, float leftBase, float rightBase, float leftTop, float rightTop) : centre(centre), height(height), leftBase(leftBase), rightBase(rightBase), leftTop(leftTop), rightTop(rightTop) { }

#endif