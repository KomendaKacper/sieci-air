//
// Created by Fujitsu on 14.12.2023.
//

#include "helpers.hpp"
#include <sstream>
#include <random>


std::random_device rd;
std::mt19937 rng(rd());

double default_probability_generator() {

    return std::generate_canonical<double, 10>(rng);
}

std::function<double()> probability_generator = default_probability_generator;

std::vector<std::string> splitString(const std::string& line, char c)
{
    std::istringstream ss(line);

    std::string token;
    std::vector<std::string> ret;
    while(getline(ss, token, c))
        ret.push_back(token);

    return ret;
}

std::array<char, 2> whitespaceChars = {'\r', '\0'};
std::string removeWhitespaces(std::string data)
{
    for(size_t i = 0;i<data.size();i++)
        for(char whitespaceChar : whitespaceChars)
            if(data[i] == whitespaceChar)
            {
                data.erase(i--);
                break;
            }

    return data;
}