#include <Common.h>
#include <random>

int pick_random_index(int start, int end)
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(start, end);
    return distrib(gen);
}