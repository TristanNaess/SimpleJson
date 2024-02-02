#ifndef __VERIFICATION_HPP__
#define __VERIFICATION_HPP__

#include <string_view>
#include "error.hpp"

// verifies a passed string is valid JSON
json::Result verify_json(std::string_view line);

#endif//__VERIFICATION_HPP__
