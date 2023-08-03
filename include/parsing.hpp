#ifndef __PARSING_HPP__
#define __PARSING_HPP__

#include <string>
#include <utility>

/*
   Functions for locating delimited data within JSON structure
*/

// Given initial quote, return's ending quote location or throws json::parsing_error
std::size_t match_quote(const std::string& line, std::size_t index) const;

// Given opening bracket/brace, finds matching closing bracket/brace or throws json::parsing_error
// Matches '{' or '[' only
std::size_t match_bracket(const std::string& line, std::size_t index) const;

// find beginning and end of next field, searching from index to end
// throws if no whole field after index
std::pair<std::size_t, std::size_t> next_field(const std::string& line, std::size_t index) const;

// Possibly want to add functions for finding the start of the next field in object or array

#endif//__PARSING_HPP__
