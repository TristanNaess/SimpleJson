#include "verification.hpp"
#include "parsing.hpp"
#include <vector>
#include <iostream>

// "error.hpp" included by verification.hpp

// ----------------------------------------------------------------------
// ALL VERIFICATION FUNCTIONS ASSUME THE STRINGS HAVE WHITESPACE REMOVED
// ----------------------------------------------------------------------

// data bounds checks and the like are handled by the calling function

json::result verify_json(std::string_view line)
{
    if (line.size() == 0) return json::result{"Error verifying json. Empty string"};
    json::result res;
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
            return json::result("Error verifying json. Not object or array data: " + line);
    }

    if (!res)
    {
        return json::result("Error verifying object in json: ", res);
    }
    return res;
}

// TODO: re-implement later with same functions as used to extract fields
json::result verify_object(std::string_view line)
{
    if (line.front() != '{') return json::result{"Error verifying object. No initial '{': " + line};
    if (line.back() != '}') return json::result{"Error verifying object. No closing '}': " + line};
    

    // crop off surrounding {}
    line.remove_prefix(1);
    line.remove_suffix(1);

    // empty object is valid
    if (line.size() == 0) return json::result();

    std::cout << '\'' << line << "'\n";

    // this code may be reused for finding the next field, but may be better to keep separate for iterator-based behavior elsewhere
    std::size_t start = 0;
    std::size_t end = 0;
    std::vector<std::string_view> fields;
    while (end < line.size())
    {
        switch (line[end])
        {
            case '"':
                try
                {
                    end = match_quote(line, end);
                }
                catch (const json::parsing_error& pe)
                {
                    return json::result{ std::string("Error verifying object: ") + std::string(pe.what()) };
                }
                break;
            case '{':
            case '[':
                try
                {
                    end = match_bracket(line, end);
                }
                catch (const json::parsing_error& pe)
                {
                    return json::result{std::string("Error verifying object: ") + pe.what()};
                }
                break;
            case ',':
                fields.push_back(line.substr(start, end-start));
                start = end+1;
                break;
        }
        end++;
    }

    fields.push_back(line.substr(start));

    json::result res;
    std::size_t delim;
    for (auto f : fields)
    {
        try
        {
            delim = find_unquoted(f, ':', 0);
        }
        catch (const json::parsing_error& pe)
        {
            return json::result{ std::string("Error verifying object: ") + pe.what() };
        }

        res = verify_string(f.substr(0, delim-1));
        if (!res) return json::result{ "Error verifying object: " + res.message() };

        delim++;
        switch (f[delim])
        {
            case '{':
                res = verify_object(f.substr(delim));
                break;
            case '[':
                res = verify_array(f.substr(delim));
                break;
            case '"':
                res = verify_string(f.substr(delim));
                break;
            case 't':
            case 'f':
                res = verify_bool(f.substr(delim));
                break;
            case 'n':
                res = verify_null(f.substr(delim));
                break;
            default:
                if (f[delim] == '+' || f[delim] == '-' || (f[delim] >= '0' && f[delim] <= '9'))
                {
                    res = verify_number(f.substr(delim));
                    break;
                }
                res = json::result{ "Error verifying object: Could not determine data type: " + f + "in: " + line };
        }

        if (!res) return res;
        
    }

    return json::result{};
}

json::result verify_array(std::string_view line)
{
    return json::result{ "verify_array() not implemented" };
}

json::result verify_number(std::string_view line)
{
    if (line.front() != '+' && line.front() != '-' && (line.front() < '0' || line.front() > '9')) return json::result{ "Error verifying number. Initial character is not +/- [0-9] in string: " + line };

    unsigned int part = 0;

    auto itr = line.begin()+1;
    if ((line.front() == '-' || line.front() == '+') && (*itr < '0' || *itr > '9')) return json::result{"Error verifying number. No digit between '+/-' and '.' in string: " + line};
    for (; itr < line.end(); itr++)
    {
        if (part == 0 && *itr == '.') // end whole number
        {
            part = 1;
            if (itr+1 == line.end()) return json::result{"Error verifying number. No character after decimal point in string: " + line};
            continue;
        }
        if (part == 1 && (*itr == 'e' || *itr == 'E')) // end decimal
        {
            part = 2;
            itr++;
            if (itr == line.end()) return json::result{"Error verifying number. No character after 'e' in string: " + line};
            if ((*itr < '0' || *itr > '9') && *itr != '+' && *itr != '-') return json::result{"Error verifying number. Bad character after 'e' in string: " + line};
            continue;
        }
        // end exponent
        if (*itr < '0' || *itr > '9') return json::result{"Error verifying number. Non-digit character in digit part of string: " + line};
    }

    return json::result{};
}

json::result verify_string(std::string_view line)
{
    return json::result{ "verify_string() not implemented" };
}

json::result verify_bool(std::string_view line)
{
    return json::result{ "verify_bool() not implemented" };
}

json::result verify_null(std::string_view line)
{
    return json::result{ "verify_null() not implemented" };
}
