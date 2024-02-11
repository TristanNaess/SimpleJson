#ifndef __PARSING_HPP__
#define __PARSING_HPP__

#include <string>
#include <cstring>
#include <utility>
#include <string_view>

#include "mutable_view.hpp"

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

// Removes all whitespace characters that are not inside a quote surrounded string
// Whitespace includes: " \n\r\t" as specified by JSON
// Creates duplicate of string without whitespace, leaving original unchanged
std::string remove_whitespace(const std::string& line);

// Adds whitespace to json to make more readable, tree makes one line per field
std::string pretty_json(const std::string& line); // TODO: implement using parsing functions
//std::string pretty_json_tree(const std::string& line);

std::string_view extract_field(std::string_view key) const;
std::string_view extract_index(std::size_t index) const;

mutable_view extract_field(std::string_view key);
mutable_view extract_index(std::size_t index);

#endif//__PARSING_HPP__
