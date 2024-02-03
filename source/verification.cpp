#include "verification.hpp"
#include "parsing.hpp"
#include <vector>
#include <iostream>

// "error.hpp" included by verification.hpp

// ----------------------------------------------------------------------
// ALL VERIFICATION FUNCTIONS ASSUME THE STRINGS HAVE WHITESPACE REMOVED
// ----------------------------------------------------------------------

// data bounds checks and the like are handled by the calling function

json::Result verify_json(std::string_view line)
{
    if (line.size() == 0) return json::Result{std::string("Error verifying json. Empty string")};
    json::Result res;
    switch (line.front())
    {
        case '{':
            // check bounds characters in verify_object
            res = verify_object(line);
            break;
        case '[':
            res = verify_array(line);
            break;
        default:
            // other types as base data not supported. May add, but probably not
            return json::Result(std::string("Error verifying json. Not object or array data: ") + std::string(line));
    }

    if (!res)
    {
        return json::Result("Error verifying object in json: ", res);
    }
    return res;
}

// TODO: re-implement later with same functions as used to extract fields
json::Result verify_object(std::string_view line)
{
    // initial '{' is verified before call
    if (line.back() != '}') return json::Result{"Error verifying object. No closing '}': " + std::string(line)};
    

    // crop off surrounding {}
    line.remove_prefix(1);
    line.remove_suffix(1);

    if (line.size() == 0) return json::Result();

    std::cout << '\'' << line << "'\n";

    // Figure out the best way to iterate over fields
    // Split at ':' and check key and val

    return json::Result{"Not fully verified"};
}

json::Result verify_array(std::string_view line)
{
    return json::Result{ "verify_array() not implemented" };
}

json::Result verify_number(std::string_view line)
{
    // first char must be [0-9]|-|+ to determine this function is called

    unsigned int part = 0;

    auto itr = line.begin()+1;
    if ((line.front() == '-' || line.front() == '+') && (*itr < '0' || *itr > '9')) return json::Result{std::string("Error verifying number. No digit between '+/-' and '.' in string: ") + std::string(line)};
    for (; itr < line.end(); itr++)
    {
        if (part == 0 && *itr == '.') // end whole number
        {
            part = 1;
            if (itr+1 == line.end()) return json::Result{std::string("Error verifying number. No character after decimal point in string: ") + std::string(line)};
            continue;
        }
        if (part == 1 && (*itr == 'e' || *itr == 'E')) // end decimal
        {
            part = 2;
            itr++;
            if (itr == line.end()) return json::Result{std::string("Error verifying number. No character after 'e' in string: ") + std::string(line)};
            if ((*itr < '0' || *itr > '9') && *itr != '+' && *itr != '-') return json::Result{std::string("Error verifying number. Bad character after 'e' in string: ") + std::string(line)};
            continue;
        }
        // end exponent
        if (*itr < '0' || *itr > '9') return json::Result{std::string("Error verifying number. Non-digit character in digit part of string: ") + std::string(line)};
    }

    return json::Result{};
}

json::Result verify_string(std::string_view line)
{
    return json::Result{ "verify_string() not implemented" };
}

json::Result verify_bool(std::string_view line)
{
    return json::Result{ "verify_bool() not implemented" };
}

json::Result verify_null(std::string_view line)
{
    return json::Result{ "verify_null() not implemented" };
}
