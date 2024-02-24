#ifndef __PARSING_HPP__
#define __PARSING_HPP__

#include <string>
#include <cstring>
#include <utility>
#include <string_view>

//#include "mutable_view.hpp"

std::string operator+(const char* line1, const std::string& line2);
std::string operator+(const char* line1, std::string_view line2);
std::string operator+(const std::string& line1, std::string_view line2);

// Given initial quote, return's ending quote location or throws json::parsing_error
std::size_t match_quote(std::string_view line, std::size_t index);

// Given opening bracket/brace, finds matching closing bracket/brace or throws json::parsing_error
// Matches '{' or '[' only
std::size_t match_bracket(std::string_view line, std::size_t index);

// Locates next non-quote surrounded occurence of char starting at index (inclusive)
// If character is '"', will return quote location rather than skip
// Throws if unmatched quotes are reached
// Undefined behavior if search starts inside quotes
// returns npos if not located
std::size_t find_unquoted(std::string_view line, char character, std::size_t index = 0);

// Locates next delimiter character (: or ,) including start index that is not within a quoted string and is at the same bracket nesting level as the start index
// Returns std::string_view::npos if no further delimiters found
std::size_t next_delim(std::string_view line, std::size_t start);

// Removes all whitespace characters that are not inside a quote surrounded string
// Whitespace includes: " \n\r\t" as specified by JSON
// Creates duplicate of string without whitespace, leaving original unchanged
std::string remove_whitespace(const std::string& line);

// Adds whitespace to json to make more readable, tree makes one line per field
std::string pretty_json(const std::string& line); // TODO: implement using parsing functions
//std::string pretty_json_tree(const std::string& line);

// Assumes the data is pre-verified
// Throws json::out_of_range if no matching field
std::string_view extract_field(std::string_view line, std::string_view key);
std::string_view extract_index(std::string_view line, std::size_t index);

/* TO BE IMPLEMENTED LATER
mutable_view extract_field(std::string_view key);
mutable_view extract_index(std::size_t index);
*/

#endif//__PARSING_HPP__
