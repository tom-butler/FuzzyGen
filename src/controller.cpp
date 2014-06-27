#include "controller.h"

//counters
static int currentController = 0;
static int currentCollection = 0;
static int currentRule = 0;

//libraries
static Controller cont[POP];
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

void updateVars(FuzzyVars newVars[NUM_VARS]) {
  vars = newVars;
}

//initialisation
void createController(FuzzyVars newVars[NUM_VARS]) {
   cont[currentController] = {0,0};

    cont[currentController].vars = newVars;
    for(i = 0; i < NUM_VARS; i++)
    {
      initSets(i, NUM_SETS);
      //if this is an output make some rules
      if(cont[currentController].vars[i].output = i)
        initRules(i);
    }
    currentController++;
}
//@TODO: REPLACE MULTIPLE ARRAY CALLS WITH POINTER
void initSets(int variable, int numSets) {
  //create initial variables
  int start = cont[currentController].vars[variable].low;
  int end = cont[currentController].vars[variable].high;
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
    cont[currentController].vars[variable].sets[j] = {centre, HEIGT, rbase, ltop, rtop, variable};
    //increment the centre for next set
    centre += space;
  }
}

//initialises all rules for a given output
void initRules(int output) {
  for(int i = 0; i < NUM_VARS; i++) {
    if(cont[currentController].vars[i].output != i) {
      for(int j = 0; j < NUM_SETS; j++) {

        for(int k = 0; k < NUM_VARS; k++) {
          if(cont[currentController].vars[k].output != k && i != k) {
            for(int l = 0; l < NUM_SETS; l++) {
              int m = getRandInt(0, NUM_SETS);
              cont[currentController].rules[currentRule] = {i, j, "AND", k, l, output, cont[currentController].vars[output].sets[m]};
              currentRule++;
            }
          }
        }
      }
    }
  }
}

//evaluate all rules that have a single output
float evaluateRules(int controller, int outputID) {
  float res1, res2, result, rcount = 0f;
  for(int i = 0; i < NUM_RULES; i++) {
    if(cont[controller].rules[i].outputvar = outputID) {
      rcount++;
      res1 = evaluateSet(rules[i].inputset);
      res2 = evaluateSet(rules[i].inputset2);

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
  return result / rcount;
}

//@TODO: repalce sets[setID] with a pointer
float evaluateSet(int controller, int inputVar, int setID) {
  float variable = cont[controller].var[inputVar];
  Set set = cont[controller].vars[var].sets[set];
  if(variable > set.leftBase - set.centreX && variable < set.rightBase + set.centreX)
    if(variable < set.centreX)
      if(variable < set.leftTop - sets[setID].centreX)
        return intersect(set.leftBase, 0, set.leftTop, set.height, variable);
      else //variable > leftTop - centreX
        return set.height;
    else //variable > centreX
      if(variable > set.rightTop + set.centreX)
        return intersect(set.rightBase, 0, set.rightTop, set.height, variable);
      else
        return set.height;
  else
    return 0;
}

//breeding
void breedController(int id1, int id2) {
  //find a random var
  int r = getRandInt(0, 4);
  int col = getRandInt(0, NUM_VARS -1);
  switch(r) {
    case 0: //swap collections
      FuzzyVar temp = cont[id2].vars[col];
      cont[id2].vars[col] = cont[id1].vars[col];
      cont[id1].vars[col] = temp;
      break;
    case 1: //swap sets
      int set = getRandInt(0, NUM_SETS -1);
      Set temp = cont[id2].vars[col].sets[set];
      cont[id2].vars[col].sets[set] = cont[id1].vars[col].sets[set];
      cont[id1].vars[col].sets[set] = temp;
      break;
    case 2:

      break;
  }

}


//mutation
float mutateCollection();

void mutateSet(int setID) {
  int mut = getRandInt(0,3);
  cont[vars[setID.variable].controller].mutations++;
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
  int mut = getRandInt(0,0);
  cont[vars[setID.variable].controller].mutations++;
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
