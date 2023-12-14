//
// Created by Fujitsu on 14.12.2023.
//

#ifndef SERWERY_AIR_HELPERS_HPP
#define SERWERY_AIR_HELPERS_HPP

#include <functional>
#include <random>
#include "types.hpp"

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;

#endif //SERWERY_AIR_HELPERS_HPP
