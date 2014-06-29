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
void ThrowError(string error)
{
  cout << error;
}

int GetRandInt(int low, int high){
  return rand() % (high - low) + low;
}

//find the intersection of two lines
//@TODO: check if parallel lines picks the highest point
float Intersect(float x1, float y1, float x2, float y2, float input) {
  float m, b;
  //find line equation y = mx + b
  m = (y2-y1)/(x2-x1);
  b = y1 -m*x1;
  //get the y value of the intersection
  return m * input + b; ;
}

void UpdateVars(FuzzyVars newVars[NUM_VARS]) {
  vars = newVars;
}

//initialisation
void CreateController(FuzzyVars newVars[NUM_VARS]) {
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
void InitSets(int variable, int numSets) {
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
void InitRules(int output) {
  for(int i = 0; i < NUM_VARS; i++) {
    if(cont[currentController].vars[i].output != i) {
      for(int j = 0; j < NUM_SETS; j++) {

        for(int k = 0; k < NUM_VARS; k++) {
          if(cont[currentController].vars[k].output != k && i != k) {
            for(int l = 0; l < NUM_SETS; l++) {
              random = getRandInt(0, NUM_SETS);
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
float EvaluateRules(int controller, int outputID) {
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
float EvaluateSet(int controller, int inputVar, int setID) {
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

void SelectController() {
  //select highest half and breed them
  Controller parents[NUM_PARENT];
  int count, avg = 0;

  //find the average score
  for(int i = 0; i < POP; i++)
    avg += cont[i].score;

  avg /= i;

  //get all controllers with >= avg score
  for(int i = 0; i < POP; i++) {
    if(cont[i].score >= avg && count < NUM_PARENT) {
      parents[count] = cont[i];
      count++
    }
  }

  //if the average didn't give enough parents, add some more randomly
  if(count < NUM_PARENT){
    for(i = count; i < NUM_PARENT, i++){
      parents[i] = cont[getRandInt(0,POP)];
    }
  }


  //breed the parents
  currentController = 0;
  for(i = 0; i < POP/2, i += 2) {
    BreedController(i, i+1);
  }
}
//breeding
void BreedController(Controller cont1, Controller cont2) {

  //save the parents
  cont[currentController] = cont1;
  currentController++;
  cont[currentController] = cont2;
  currentController++;

  //create two children from two parents
  cont[currentController] = cont1;
  currentController++;
  cont[currentController] = cont2;
  currentController++;

  //get the id's of the children
  int id1 = currentController -1;
  int id2 = currentController;


  //parent mutation, twice for extra variation
  ParentMutation(id1, id2);
  ParentMutation(id1, id2);

  //self mutation
  ChildMutation(id1);
  ChildMutation(id2);

  }
}


//mutation
void ParentMutation(int id1, int id2) {
  //randomly merge two genes

  //find a random var
  random = getRandInt(0, 4);
  int col = getRandInt(0, NUM_VARS -1);

  switch(random) {
    case 0: //swap collections
      FuzzyVar temp = cont[id2].vars[col];
      cont[id2].vars[col] = cont[id1].vars[col];
      cont[id1].vars[col] = temp;
      break;
    case 1: //copy collection
      cont[id2].vars[col] = cont[id1].vars[col];
      break;
    case 2: //swap set
      int set = getRandInt(0, NUM_SETS -1);
      Set temp = cont[id2].vars[col].sets[set];
      cont[id2].vars[col].sets[set] = cont[id1].vars[col].sets[set];
      cont[id1].vars[col].sets[set] = temp;
      break;
    case 3: //copy set
      int set = getRandInt(0, NUM_SETS -1);
      cont[id2].vars[col].sets[set] = cont[id1].vars[col].sets[set];
      break;
    case 4: //change rule output
      int rule = getRandInt(0,NUM_RULES -1);
      Rule temp = cont[id2].rules[rule];
      int rule2 = getRandInt(0,NUM_RULES -1);
      cont[id2].rules[rule].output = cont[id1].rules[rule2].output;
      break;
    }
}

void ChildMutation(int id) {
  random = getRandInt(0, 1);
  if(random == 0)
    mutateSet(id, getRandInt(0,NUM_VARS), getRandInt(0,NUM_SETS))
  else
    mutateRule(id, getRandInt(0, NUM_RULES));
}

void MutateSet(int controller, int var, int setID) {
  random = getRandInt(0,3);
  cont[controller].mutations++;
  switch(random){
    case 0:
      mutateSetGrowTop(controller, var, setID);
      break;
    case 1:
      mutateSetGrowBase(controller, var, setID);
      break;
    case 2:
      mutateSetSlideTop(controller, var, setID);
      break;
    case 3:
      mutateSetSlideBase(controller, var, setID);
      break;
    default:
      return;
      break;
  }
}

void MutateSetGrowTop(int controller,int var,int setID) {
  random = getRandInt(-VARIANCE, VARIANCE);
  Set set = cont[controller].vars[var].sets[setID];
  set.leftTop -= random;
  set.rightTop += random;

  if(set.leftTop < cont[controller].vars[var].start)
    set.leftTop = cont[controller].vars[var].start;
  if(set.rightTop > cont[controller].vars[var].end)
    set.rightTop = cont[controller].vars[var].end;

  cont[controller].vars[var].sets[setID] = set;
}
void MutateSetGrowBase(int controller,int var,int setID) {
  random = getRandInt(-VARIANCE, VARIANCE);
  Set set = cont[controller].vars[var].sets[setID];
  set.leftBase -=random;
  set.rightBase +=random;

  if(set.leftBase < cont[controller].vars[var].start)
    set.leftBase = cont[controller].vars[var].start;
  if(set.rightBase > cont[controller].vars[var].end)
    set.rightBase = cont[controller].vars[var].end;

  cont[controller].vars[var].sets[setID] = set;
}
void MutateSetSlideTop(int controller,int var,int setID) {
  random = getRandInt(-VARIANCE, VARIANCE);
  Set set = cont[controller].vars[var].sets[setID];
  set.leftTop +=random;
  set.rightTop +=random;

  if(set.leftTop < cont[controller].vars[var].start)
    set.leftTop = cont[controller].vars[var].start;
  if(set.rightTop > cont[controller].vars[var].end)
    set.rightTop = cont[controller].vars[var].end;

  cont[controller].vars[var].sets[setID] = set;
}
void MutateSetSlideBase(int controller,int var,int setID) {
  random = getRandInt(-VARIANCE, VARIANCE);
  Set set = cont[controller].vars[var].sets[setID];
  set.leftBase +=random;
  set.rightBase +=random;

  if(set.leftBase < cont[controller].vars[var].start)
    set.leftBase = cont[controller].vars[var].start;
  if(set.rightBase > cont[controller].vars[var].end)
    set.rightBase = cont[controller].vars[var].end;
}

void MutateRule(int controller, int ruleID) {
  random = getRandInt(0,0);
  cont[controller].mutations++;
  switch(random){
    case 0:
      mutateRuleOutput(int controller, ruleID);
      break;
    default:
      return;
      break;
  }
}

void MutateRuleOutput(int controller, int ruleID) {
  int var = cont[controller].rules[ruleID].outputvar;
  do {
    random = getRandInt(0, NUM_SETS);
    if(random != cont[controller].rules[ruleID].outputSet) {
      rules[ruleID].output = random;
      return;
    }
  } while(1);
}
