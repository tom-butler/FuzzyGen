#include <iostream>
#include "FloatPoint"

using namespace std;

class FuzzyCollection {
  public:
    //vars
    char *name;
    int min;
    int max;
    vector <FuzzySet> sets;
    //functions
    FuzzyCollection();
    FuzzyCollection(char *name, int min, int max);
    //Adds a new set to the collection
    void AddSet(FuzzySet newSet);
    //Force Set overlap by Mutating Sets
    void ForceOverlap(float overlap);
    //Force Sets to be within a relationship theshold
    void RelationshipThreshold(float threshold);
    //Mutate entire genome by adjusting sets
    void MutateGrow(float size);
};

class FuzzySet {
  public:
    //vars
    FloatPoint centre;
    float height;
    float leftBase;
    float rightBase;
    float leftTop;
    float rightTop;
    FuzzyCollection* parent;
    //functions
    FuzzySet();
    FuzzySet(FloatPoint centre,float height, float leftBase, float rightBase, float leftTop, float rightTop, FuzzyCollection* parent);
    //interpolate the fuzzy value
    float Evaluate(float rangeVariable);
    //grow/shrink the set around the centre
    void MutateGrow(float size);
    //slide the set by adjusting the centre
    void MutateSlide(float amount);

  private:
};
