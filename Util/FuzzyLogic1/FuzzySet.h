#ifndef FUZZYSET_H
#define FUZZYSET_H


#include <utility> //pair
using namespace std;

class FuzzyCollection;
class FuzzySet {
public:
    //vars
    pair <float, float> centre;
    float height;
    float leftBase;
    float rightBase;
    float leftTop;
    float rightTop;
    FuzzyCollection & parent;

    //functions
    FuzzySet();
    FuzzySet(pair <float, float> centre,float height, float leftBase, float rightBase, float leftTop, float rightTop, FuzzyCollection parent);
    //interpolate the fuzzy value
    float Evaluate(float rangeVariable);
    //grow/shrink the set around the centre
    void GrowSet(float size);
    //slide the set by adjusting the centre
    void MutateSlide(float amount);
private:
};

#endif