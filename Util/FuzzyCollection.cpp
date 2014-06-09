#ifndef FUZZYCOL_H
#define FUZZYCOL_H

#include <vector>
#include "FloatPoint.cpp"
#include "FuzzySet.cpp"

using namespace std;

class FuzzyCollection {
  public:
    //vars
    char *name;
    int min;
    int max;
    vector<FuzzySet> sets;
    vector <FuzzySet>::iterator it;
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
private:


};

//empty constructor
FuzzyCollection::FuzzyCollection() {}
//constructor
FuzzyCollection::FuzzyCollection(char *name, int min, int max) : name(name), min(min), max(max) {

}
//

void AddSet(FuzzySet newSet) {

    sets.insert(0, newSet);
}

#endif