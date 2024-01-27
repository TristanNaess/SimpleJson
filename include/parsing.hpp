#ifndef __PARSING_HPP__
#define __PARSING_HPP__

#include <string>
#include <utility>

/*
   Functions for locating delimited data within JSON structure
*/

// Given initial quote, return's ending quote location or throws json::parsing_error
std::size_t match_quote(const std::string& line, std::size_t index);

// Given opening bracket/brace, finds matching closing bracket/brace or throws json::parsing_error
// Matches '{' or '[' only
std::size_t match_bracket(const std::string& line, std::size_t index);

// Locates next non-quote surrounded occurence of char starting at index (inclusive)
// If character is '"', will return quote location rather than skip
// Throws if unmatched quotes are reached
// Undefined behavior if search starts inside quotes
// returns npos if not located
std::size_t find_unquoted(const std::string& line, char character, std::size_t index = 0);

// Removes all whitespace characters that are not inside a quote surrounded string
// Whitespace includes: " \n\r\t" as specified by JSON
// Creates duplicate of string without whitespace, leaving original unchanged
std::string remove_whitespace(const std::string& line);

// Adds whitespace to json to make more readable, tree makes one line per field
std::string pretty_json(const std::string& line);
//std::string pretty_json_tree(const std::string& line);


#endif//__PARSING_HPP__
