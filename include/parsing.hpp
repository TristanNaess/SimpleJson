#ifndef __PARSING_HPP__
#define __PARSING_HPP__

#include <string>

// --------------------------------------------------------------------------------
//  For now I'm just making it work trying to keep the API clean.
//  Anything else like extracting repeat code and adding iterators can come later.
// --------------------------------------------------------------------------------

// remove non-string whitespace
std::string remove_whitespace(const std::string& s) noexcept;

#ifdef TESTING
std::string::iterator skip_quotes(std::string::iterator opening_quote) noexcept;
// no need to test const version, identical apart from const qualification
std::string::iterator skip_brackets(std::string::iterator opening_bracket) noexcept;
#endif

#endif//__PARSING_HPP__:w

