#ifndef FUZZYCOL_H
#define FUZZYCOL_H

#include <vector>
#include <string>

using namespace std;

class FuzzySet;
class FuzzyCollection {
  public:
    //vars
    string name;
    int min;
    int max;
    vector<FuzzySet> sets;
    vector <FuzzySet>::iterator it;
    //functions
    FuzzyCollection();
    FuzzyCollection(string name, int min, int max);
    //Adds a new set to the collection
    void AddSet(FuzzySet newSet);
    //Force Set overlap by Mutating Sets
    void ForceOverlap(float overlap);
    //Force Sets to be within a relationship theshold
    void RelationshipThreshold(float threshold);
    //Mutate entire genome by adjusting sets
    void GrowCollection(float size);
  private:
};

#endif