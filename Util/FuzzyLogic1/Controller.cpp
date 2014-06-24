#include "Controller.h"

//util
void throwError(string error)
{
  cout << error;
}

//initialisation
void createController(float vars[]) {
    vars = vars;
}
void createCollection(int start, int end, int outputID) {
  collection newCollection = {start, end, outputID};
  collections[currentCollection] = newCollection;
  currentCollection++;
}
void createSet(int ID, float centreX, float centreY, float height,
  float leftBase, float rightBase, float leftTop, float rightTop,
  int collection) {

  set newSet = { ID, centreX, centreY, height, leftBase, rightBase, leftTop, rightTop, collection};
  sets[currentSet] = newSet;
  currentSet++;
}

//if var1 is set1 (AND|OR) var2 is set2 then output is outputSet
void createRule(int var1, int set1, string modifier, int var2, int set2,  int output, int outputSet) {
  rule newRule = {var1, set1, modifier, var2, set2, output, outputSet};
  rules[currentRule] = newRule;
  currentRule++;
}

//evaluation
float evaluateVar(int ID, float newValue) {
  vars[ID] = newValue;
}

//evaluate all rules that have a single output
float evaluateRules(int outputID) {
  float res1, res2, variable, result, rcount;
  result, rcount = 0.0f;
  //check all rules
  for(int i = 0; i < NUM_RULES; i++) {

    //if they target the same output
    if(rules[i].output == outputID) {
      rcount++;

      //check the value for sets
      res1 = evaluateSet(rules[i].set1, rules[i].var1);
      res2 = evaluateSet(rules[i].set2, rules[i].var2);

      //decide on the value to pass to output

      if(rules[i].modifier.compare("AND") == 0) {
          if(res1 < res2)
            variable = res1;
          else
            variable = res2;
      }
      else if(rules[i].modifier.compare("OR") == 0) {
          if(res1 > res2)
            variable = res1;
          else
            variable = res2;
      }
      //add result
      result += evaluateSet(rules[i].outputSet,variable);
    }
  }
  //average result
  return result / rcount;
}
//@TODO: repalce sets[setID] with a pointer
float evaluateSet(int setID, float variable) {
  if(variable > sets[setID].leftBase - sets[setID].centreX && variable < sets[setID].rightBase + sets[setID].centreX)
    if(variable < sets[setID].centreX)
      if(variable < sets[setID].leftTop - sets[setID].centreX)
        return intersect(sets[setID].leftBase, 0, sets[setID].leftTop, sets[setID].height, variable);
      else //variable > leftTop - centreX
        return sets[setID].height;
    else //variable > centreX
      if(variable > sets[setID].rightTop + sets[setID].centreX)
        return intersect(sets[setID].rightBase, 0, sets[setID].rightTop, sets[setID].height, variable);
      else
        return sets[setID].height;
  else
    return 0;
}

//find the intersection of two lines
float intersect(float x1, float y1, float x2, float y2, float input) {
  float m, b;
  //find line equation y = mx + b
  m = (y2-y1)/(x2-x1);
  b = y1 -m*x1;
  //get the y value of the intersection
  return m * input + b; ;
}

//breeding
void breedController();
void breedCollection();
void breedSet();

//mutation
float mutateCollection();
float mutateSet();
