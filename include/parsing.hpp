#ifndef __PARSING_HPP__
#define __PARSING_HPP__

#include <string>

// --------------------------------------------------------------------------------
//  For now I'm just making it work trying to keep the API clean.
//  Anything else like extracting repeat code and adding iterators can come later.
// --------------------------------------------------------------------------------

// remove non-string whitespace
std::string remove_whitespace(const std::string& s) noexcept;

#endif//__PARSING_HPP__:w

