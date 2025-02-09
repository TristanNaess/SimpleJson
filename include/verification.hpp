#ifndef __VERIFICATION_HPP__
#define __VERIFICATION_HPP__

#include <string_view>
#include "error.hpp"

// I'm just going to use const std::string& and bool for now. I'll change to string_view after I get it working and add a mutable version
// does not expect whitespace to be removed. Actually needs to be called before white space can be removed

//  There's a lot of copying going on here in the recursive calls, change to string_view once working
bool verify_json(const std::string& line);

bool verify_object(const std::string& line);
bool verify_array(const std::string& line);
bool verify_string(const std::string& line);
bool verify_number(const std::string& line);
bool verify_bool(const std::string& line);
bool verify_null(const std::string& line);

// checks type of verified json, 
bool is_object(const std::string& line);
bool is_array(const std::string& line);
bool is_string(const std::string& line);
bool is_number(const std::string& line);
bool is_bool(const std::string& line);
bool is_null(const std::string& line);

/*
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
*/
#endif//__VERIFICATION_HPP__
