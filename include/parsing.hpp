#ifndef __PARSING_HPP__
#define __PARSING_HPP__

#include <string>
//#include <string_view> // may use if static strings accessed

/*
   Functions for locating delimited data within JSON structure
*/

// Given initial quote, return's ending quote location or throws json::parsing_error
std::size_t match_quote(const std::string& line, std::size_t index);

// Given opening bracket/brace, finds matching closing bracket/brace or throws json::parsing_error
// Matches '{' or '[' only
std::size_t match_bracket(const std::string& line, std::size_t index);

// Possibly want to add functions for finding the start of the next field in object or array

#endif//__PARSING_HPP__
