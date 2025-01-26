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


// All is_*() functions assume the data is good, and sans-whitespace

bool is_object(const std::string& line)
{
    return line[0] == '{';
}

bool is_array(const std::string& line)
{
    return line[0] == '[';
}

bool is_number(const std::string& line)
{
    return (line[0] == '-' || (line[0] >= '0' && line[0] <= '9'));
}

bool is_string(const std::string& line)
{
    return line[0] == '"';
}

bool is_bool(const std::string& line)
{
    return (line == "true" || line == "false");
}

bool is_null(const std::string& line)
{
    return line == "null";
}
