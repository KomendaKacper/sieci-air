//
// Created by Fujitsu on 14.12.2023.
//

#ifndef SERWERY_AIR_HELPERS_HPP
#define SERWERY_AIR_HELPERS_HPP

#include "types.hpp"

#include <functional>
#include <random>
#include <cstdlib>
#include <vector>
#include <string>


using ProbabilityGenerator = std::function<double()>;

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;

extern std::vector<std::string> splitString(const std::string& line, char c);

extern std::string removeWhitespaces(std::string data);

#endif //SERWERY_AIR_HELPERS_HPP
