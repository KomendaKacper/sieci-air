#include "package.hpp"



int freed_IDs::counter_ = 1;
std::set<unsigned int> freed_IDs::set_;


int assigned_IDs::counter_ = 1;
std::set<unsigned int> assigned_IDs::set_;

unsigned int counter = 1;