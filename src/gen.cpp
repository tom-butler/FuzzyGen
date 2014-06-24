#include <iostream>
#include <vector>
#include "sim.h"
#include "controller.h"
using namespace std;

//variables
vector <FuzzyCollection> collections;
vector <FuzzyCollection>::iterator it;

//functions
void init();
void GALoop();
void ScoreFitness(int id);
void Selection();
void Breed(int id);
void Mutate(int id);

int main(int argc, char *argv[])
{
  init();
  GALoop();
}

//sets up the initial variables
void init() {
  it = collections.begin();
  collections.insert(it,FuzzyCollection("test",0,100) );
}

//Runs the GA until requirements met
void GALoop() {

}

//Scores each genotype
void ScoreFitness(int id) {

}

//Selects best genotypes
void Selection() {

}

//Breeds best genotypes
void Breed(int id) {

}

//Mutate genotype for random variance
void Mutate(int id) {

}