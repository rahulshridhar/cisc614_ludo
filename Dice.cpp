#include "Dice.h"
#include "Common.h"
#include <random>

int Dice::roll()
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(1, 6);
    return distrib(gen);
}
