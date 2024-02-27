#ifndef __VERIFICATION_HPP__
#define __VERIFICATION_HPP__

#include <string_view>
#include "error.hpp"

// verifies a passed string is valid JSON
json::result verify_json(std::string_view line);

json::result verify_object(std::string_view line);
json::result verify_array(std::string_view line);
json::result verify_number(std::string_view line);
json::result verify_string(std::string_view line);
json::result verify_bool(std::string_view line);
json::result verify_null(std::string_view line);

// for checking the type of verified JSON
bool is_object(std::string_view line);
bool is_array(std::string_view line);
bool is_number(std::string_view line);
bool is_string(std::string_view line);
bool is_bool(std::string_view line);
bool is_null(std::string_view line);

#endif//__VERIFICATION_HPP__
