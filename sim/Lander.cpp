//------------
//---Lander---
//------------

//Lander object used in the moonlander sim

//constants
const int START_FUEL = 1000;
const int THRUST_MAX = 50;
const int TERMINAL_VELOCITY = 50;
const int FORCE = 5;

//Lander
//
class Lander {
  public:
    //variables
    pair<int, int> pos;
    int vel;
    int fuelRemaining;
    //functions
    Lander(pair<int, int> position, int velocity );
    void Update(int thrust);
};

//Constructor for the Lander
Lander::Lander(pair<int, int> position, int velocity) {
  pos = position;
  vel = velocity;
  fuelRemaining = START_FUEL;
}

void Update(int thrust) {
  fuelRemaining -= thrust;
  velocity = velocity + FORCE;
  velocity = velocity - thrust;
  pos.first += velcocity;
}
