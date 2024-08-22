#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

#include <string>

// overload to allow char* + std::string to construct a string
std::string operator+(const char* line1, const std::string& line2);

#endif//__UTILITY_HPP__
