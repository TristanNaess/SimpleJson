#include "verification.hpp"
#include "parsing.hpp"

// "error.hpp" included by verification.hpp

// ----------------------------------------------------------------------
// ALL VERIFICATION FUNCTIONS ASSUME THE STRINGS HAVE WHITESPACE REMOVED
// ----------------------------------------------------------------------

// verify_*() functions are only called through verify_json() and so are not tested directly
// data bounds checks and the like are handled by the calling function, either verify_object, array, or json

json::Result verify_json(const std::string_view line)
{
    if (line.size() == 0) return json::Result{std::string("Error verifying json. Empty string") + line};
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
            return json::Result(std::string("Error verifying json. Not object or array data: ") + line);
    }

    if (!res)
    {
        return json::Result("Error verifying object in json: ", res);
    }
    return res;
}

json::Result verify_object(const std::string_view line);

json::Result verify_array(const std::string_view line);

json::Result verify_number(const std::string_view line)
{
    // first char must be [0-9]|-|+ to determine this function is called

    unsigned int part = 0;

    for (auto itr = line.begin()+1; itr < line.end(); itr++)
    {
        if (part == 0 && *itr == '.')
        {
            part = 1;
            if (itr+1 == line.end()) return json::Result{std::string("Error verifying number. No character after decimal point in string: ") + line};
            continue;
        }
        if (part == 1 && (*itr == 'e' || *itr == 'E'))
        {
            part = 2;
            itr++;
            if (itr == line.end()) return json::Result{std::string("Error verifying number. No character after 'e' in string: ") + line};
            if ((*itr < '0' || *itr > '9') && *itr != '+' && *itr != '-') return json::Result{std::string("Error verifying number. Bad character after 'e' in string: ") + line};
            continue;
        }
        if (*itr < '0' || *itr > '9') return json::Result{std::string("Error verifying number. Non-digit character in digit part of string: ") + line};
    }

    return json::Result;
}

json::Result verify_string(const std::string_view line);

json::Result verify_bool(const std::string_view line);

json::Result verify_null(const std::string_view line);
