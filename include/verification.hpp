#ifndef __VERIFICATION_HPP__
#define __VERIFICATION_HPP__

#include <string_view>
#include "error.hpp"

// verifies a passed string is valid JSON
json::Result verify_json(std::string_view line);

json::Result verify_object(std::string_view line);
json::Result verify_array(std::string_view line);
json::Result verify_number(std::string_view line);
json::Result verify_string(std::string_view line);
json::Result verify_bool(std::string_view line);
json::Result verify_null(std::string_view line);

#endif//__VERIFICATION_HPP__
