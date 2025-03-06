#ifndef __CONVERSION_HPP__
#define __CONVERSION_HPP__

#include <string>
#include <string_view>
#include "parsing.hpp"

// ---------------------------------------------
//  Functions for converting to and from JSON.
// ---------------------------------------------

namespace json
{
    // conversion from json to type
    std::string to_string(std::string_view data);
    std::string to_ldouble(std::string_view data);
    std::string to_double(std::string_view data);
    std::string to_float(std::string_view data);
    std::string to_ullint(std::string_view data);
    std::string to_ulint(std::string_view data);
    std::string to_uint(std::string_view data);
    std::string to_uchar(std::string_view data);
    std::string to_llint(std::string_view data);
    std::string to_lint(std::string_view data);
    std::string to_int(std::string_view data);
    std::string to_char(std::string_view data);
    std::string to_bool(std::string_view data);

    // conversion from type to json
    std::string to_data(std::string_view line);
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
