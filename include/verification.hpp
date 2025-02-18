#ifndef __VERIFICATION_HPP__
#define __VERIFICATION_HPP__

#include <string_view>
#include <utility>
#include "error.hpp"

// expects whitespace to be removed
// at some point change the verify functions to take a index into the full string and return when a passed delimiter is reached
// this allows for a single pass through the string, rather than finding beginning and end points for the substrings

// possibly put the declarations behind a testing ifdef. Apart from verify_json() should not be called from outside verification.cpp

// verifies a passed string is valid JSON
bool verify_json(std::string_view line);

// for checking the type of verified JSON
bool is_object(std::string_view line);
bool is_array(std::string_view line);
bool is_number(std::string_view line);
bool is_string(std::string_view line);
bool is_bool(std::string_view line);
bool is_null(std::string_view line);

#endif//__VERIFICATION_HPP__
