#ifndef SERWERYAIR_TYPES_HPP
#define SERWERYAIR_TYPES_HPP
#include <functional>
using ElementID = unsigned int;

using TimeOffset = unsigned long long;

using Time = unsigned long long;

using ProbabilityGenerator = std::function<double()>;

#endif //SERWERYAIR_TYPES_HPP
