#include "Controller.h"

//initialisation
void createController(float vars[]) {
  controller.vars = vars;
}
void createCollection(int start, int end, int output, controller &parent) {
  collection newCollection = {start, end, output, &parent};
  collections[currentCollection] = newCollection;
  currentCollection++;
}
void createSet(int ID, float centreX, float centreY. float height,
  float leftBase, float rightBase, float leftTop, float rightTop,
  collection &parent) {

  set newSet = {int ID, float centreX, float centreY. float height,
    float leftBase, float rightBase, float leftTop, float rightTop,
    collection &parent};
  sets[currentSet] = newSet;
  currentSet++;

}
void createRule(set *set1, set *set2, int *var1, int *var2, int *output);

//evaluation
float evaluateVar(int ID, float newValue) {
  vars[ID] = newValue;
}
float evaluateRule(int ID) {

}
float evaluateSet(int collectionID, int setID, float variable) {

}

//breeding
void breedController();
void breedCollection();
void breedSet();

//mutation
float mutateCollection();
float mutateSet();
