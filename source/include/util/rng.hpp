#pragma once

#include <random>

class RNG {
public:
    RNG(int seed) {set_seed(seed);};
    RNG() : RNG(0) {};

    void set_seed(int seed) {gen.seed();};
    float uniform() {return uniform_distribution(gen);};

private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> uniform_distribution {0, 1};
};