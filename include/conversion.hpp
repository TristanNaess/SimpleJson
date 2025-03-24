#ifndef __CONVERSION_HPP__
#define __CONVERSION_HPP__

#include <string>
#include <string_view>
#include "parsing.hpp"

// ---------------------------------------------
//  Functions for converting to and from JSON.
// ---------------------------------------------

namespace conversion
{
    // conversion from json to type
    std::string to_string(std::string_view data);
    long double to_ldouble(std::string_view data);
    double to_double(std::string_view data);
    float to_float(std::string_view data);
    unsigned long long int to_ullint(std::string_view data);
    unsigned long int to_ulint(std::string_view data);
    unsigned int to_uint(std::string_view data);
    unsigned char to_uchar(std::string_view data);
    long long int to_llint(std::string_view data);
    long int to_lint(std::string_view data);
    int to_int(std::string_view data);
    char to_char(std::string_view data);
    bool to_bool(std::string_view data);

    // conversion from type to json
    std::string to_data(std::string_view line);
    std::string to_data(const char* line);
    std::string to_data(long double d);
    std::string to_data(double d);
    std::string to_data(float f);
    std::string to_data(unsigned long long int i);
    std::string to_data(unsigned long int i);
    std::string to_data(unsigned int i);
    std::string to_data(unsigned char c);
    std::string to_data(long long int i);
    std::string to_data(long int i);
    std::string to_data(int i);
    std::string to_data(char c);
    std::string to_data(bool b);
}

#endif//__CONVERSION_HPP__
