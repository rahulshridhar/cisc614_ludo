#include "Dice.h"
#include <random>

//! Generic function to return a random number between 1 to 6 for a die roll
int Dice::roll()
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(1, 6);
    return distrib(gen);
}
