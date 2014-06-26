#include "controller.h"

//counters
static int currentCollection = 0;
static int currentSet = 0;
static int currentRule = 0;

//libraries
static FuzzyVar vars[NUM_INPUT];
static Set sets[NUM_SETS];
static Rule rules[NUM_RULES];

//util
void throwError(string error)
{
  cout << error;
}

int getRandInt(int low, int high){
  return rand() % (high - low) + low;
}

//initialisation
void createController(FuzzyVars newVars[]) {
    vars = newVars;
    for(i = 0; i < NUM_VARS; i++)
    {
      initSets(i, NUM_SETS_PER_COL);
      //if this is an output make some rules
      if(vars[i].output = i)
        initRules(i);
    }
}

void initSets(int variable, int numSets) {
  //create initial variables
  int start = vars[variable].low;
  int end = vars[variable].high;
  int space = (end - start)  / (numSets - 1);
  int centre = start;
  int bWidth = 0.7 * space;
  int tWidth = 0.3 * space

  for(j = 0; j < numSets; j++) {
    //attempt to create set variables
    int lbase = bWidth - centre;
    int rbase = bWidth + centre;
    int ltop = tWidth - centre;
    int rtop = tWidth + centre;

    //check set variables for compliance
    if(lbase  < start)
      lbase = intersect(0f, lbase, 0f, centre, start);
    if(rbase > end)
      rbase = intersect(0f, centre, 0f, rbase, end)
    if(ltop < start)
      lbase = intersect(HEIGHT, ltop, HEIGHT, centre, start);
    if(rtop > end)
      rtop = intersect(HEIGHT, centre, HEIGHT, rtop, end);

    //build the set
    createSet(centre, HEIGHT, lbase, rbase, ltop, rtop , variable);
    //increment the centre for next set
    centre += space;
  }
}

void createSet(float centreX, float height,
  float leftBase, float rightBase, float leftTop, float rightTop,
  int collection) {

  Set newSet = { centreX, centreY, height, leftBase, rightBase, leftTop, rightTop, collection};
  sets[currentSet] = newSet;
  currentSet++;
}

//initialises all rules for a given output
void initRules(int output) {
  for(int j = 0; j < NUM_SETS; j++) {
    if(vars[sets[j].variable].output != sets[j].variable) {
      for int k = 0; k < NUM_SETS; k++) {
        if(vars[sets[k].variable].output != sets[k].variable) {
          int out = 0;
          do {
            int random = getRandInt(0, NUM_SETS - 1);
            if(vars[sets[random].variable].output == sets[random].variable)
              out = random;
          } while(out == 0);
          createRule(j,"AND," k, out);
        }
      }
    }
  }
}

//if var1 is set1 (AND|OR) var2 is set2 then output is outputSet
void createRule(int set1, string modifier, int set2, int outputSet) {
  Rule newRule = { set1, modifier, set2, outputSet};
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
      res1 = evaluateSet(rules[i].set1);
      res2 = evaluateSet(rules[i].set2);

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
float evaluateSet(int setID) {
  float variable = vars[sets[setID].variable];
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
//@TODO: check if parallel lines picks the highest point
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

void mutateSet(int setID) {
  int mut = getRandInt(0,3);
  switch(mut){
    case 0:
      mutateSetGrowTop(setID);
      break;
    case 1:
      mutateSetGrowBase(setID);
      break;
    case 2:
      mutateSetSlideTop(setID);
      break;
    case 3:
      mutateSetSlideBase(setID);
      break;
    default:
      return;
      break;
  }
}

void mutateSetGrowTop(int setID) {
  int diff = getRandInt(-VARIANCE, VARIANCE);

  sets[setID].leftTop -=diff;
  sets[setID].rightTop +=diff;

  if(sets[setID].leftTop < collections[sets[setID].collection].start)
    sets[setID].leftTop = collections[sets[setID].collection].start;
  if(sets[setID].rightTop > collections[sets[setID].collection].end)
    sets[setID].rightTop = collections[sets[setID].collection].end;
}
void mutateSetGrowBase(int setID) {
  int diff = getRandInt(-VARIANCE, VARIANCE);

  sets[setID].leftBase -=diff;
  sets[setID].rightBase +=diff;

  if(sets[setID].leftBase < collections[sets[setID].collection].start)
    sets[setID].leftBase = collections[sets[setID].collection].start;
  if(sets[setID].rightBase > collections[sets[setID].collection].end)
    sets[setID].rightBase = collections[sets[setID].collection].end;
}
void mutateSetSlideTop(int setID) {
  int diff = getRandInt(-VARIANCE, VARIANCE);

  sets[setID].leftTop +=diff;
  sets[setID].rightTop +=diff;

  if(sets[setID].leftTop < collections[sets[setID].collection].start)
    sets[setID].leftTop = collections[sets[setID].collection].start;
  if(sets[setID].rightTop > collections[sets[setID].collection].end)
    sets[setID].rightTop = collections[sets[setID].collection].end;
}
void mutateSetSlideBase(int setID) {
  int diff = getRandInt(-VARIANCE, VARIANCE);

  sets[setID].leftBase +=diff;
  sets[setID].rightBase +=diff;

  if(sets[setID].leftBase < collections[sets[setID].collection].start)
    sets[setID].leftBase = collections[sets[setID].collection].start;
  if(sets[setID].rightBase > collections[sets[setID].collection].end)
    sets[setID].rightBase = collections[sets[setID].collection].end;
}

void mutateRule(int ruleID) {
  int mut = getRandInt(0,3);
  switch(mut){
    case 0:
      mutateSetGrowTop(setID);
      break;
    default:
      return;
      break;
  }
}

void mutateRuleOutput(int ruleID) {
  do {
    int test = getRandInt(0, NUM_SETS);
    if(sets[test].isOutput) {
      rules[ruleID].output = test;
      return;
    }
  } while(1);
}
