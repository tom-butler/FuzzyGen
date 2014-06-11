
#include <utility> //pair
using namespace std;

class FuzzySet
class FuzzyRule {
public:
  pair <float*, FuzzySet*>[] input;
  pair <float*, FuzzySet*> output;

  FuzzyRule();
  FuzzyRule(pair <float*, FuzzySet*>[] input, pair <float*, FuzzySet*> ouput);
  void Evaluate();
};