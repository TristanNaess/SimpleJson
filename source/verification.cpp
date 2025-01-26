#include "verification.hpp"

bool verify_json(const std::string& line)
{
    if (line.size() == 0) return false; // cannot be empty

    switch (line[0])
    {
        case '{':
            return verify_object(line);
            break;
        case '[':
            return verify_array(line);
            break;
        default:
            return false; // must be array or object
    }
}

bool verify_object(const std::string& line)
{
    throw todo("TODO: bool verify_object(const std::string&)");
}

bool verify_array(const std::string& line)
{
    throw todo("TODO: bool verify_array(const std::string&)");
}

bool verify_number(const std::string& line)
{
    throw todo("TODO: bool verify_number(const std::string&)");
}

bool verify_string(const std::string& line)
{
    throw todo("TODO: bool verify_string(const std::string&)");
}

bool verify_bool(const std::string& line)
{
    throw todo("TODO: bool verify_bool(const std::string&)");
}

bool verify_null(const std::string& line)
{
    throw todo("TODO: bool verify_null(const std::string&)");
}



bool is_object(const std::string& line)
{
    throw todo("TODO: bool is_object(const std::string&)");
}

bool is_array(const std::string& line)
{
    throw todo("TODO: bool is_array(const std::string&)");
}

bool is_number(const std::string& line)
{
    throw todo("TODO: bool is_num(const std::string&)");
}

bool is_string(const std::string& line)
{
    throw todo("TODO: bool is_string(const std::string&)");
}

bool is_bool(const std::string& line)
{
    throw todo("TODO: bool is_bool(const std::string&)");
}

bool is_null(const std::string& line)
{
    throw todo("TODO: bool is_null(const std::string&)");
}
