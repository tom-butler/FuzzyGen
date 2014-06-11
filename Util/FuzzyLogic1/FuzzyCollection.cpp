#include "FuzzyCollection.h"
#include "FuzzySet.h"

//empty constructor
FuzzyCollection::FuzzyCollection() {}
//constructor
FuzzyCollection::FuzzyCollection(string name, int min, int max) : name(name), min(min), max(max) {

}
//

void FuzzyCollection::AddSet(FuzzySet newSet) {
  it = sets.begin();
  sets.insert(it, newSet);
}

void FuzzyCollection::ForceOverlap(float overlap) {
  //check overlap
}

void FuzzyCollection::RelationshipThreshold(float threshold) {

}

void GrowCollection(float size) {

}