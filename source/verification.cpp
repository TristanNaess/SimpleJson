#include "parsing.hpp"
#include "error.hpp"
#include <iostream> // temporary for lazy debugging


// -------------------------------------------------------------------------
// ALL VERIFICATION FUNCTIONS ASSUME THE STRINGS HAVE WHITE SPACE REMOVED
// THE FUNCTIONS WILL BREAK IF PASSED A STRING WITH WHITESPACE
// -------------------------------------------------------------------------

// Due to being non-public apart from verify_json(), tests for verify_*() functions will only be done through verify_json() tests

bool verify_object(const std::string_view line)
{
    // bounds and bracket check
    if (line.size() < 2) { return false; }
    if (line.front() != '{' || line.back() != '}') { return false; }

    // iteratively extract fields and verify

    return false;
}

bool verify_array(const std::string_view line)
{
    // TODO implement
    return false;   
}

bool verify_string(const std::string_view line)
{
    if (line.size() < 2) return false;
    if (line.front() != '"' || line.back() != '"') return false;

    for (auto itr = line.begin(); itr != line.end(); itr++)
    {
        // not a control character
        if (*itr < 0x20 || (*itr > 0x7e && *itr < 0xa0)) return false;
        // errant quote
        if (*itr == '"' && itr != line.end() - 1) return false;
        if (*itr == '\\')
        {
            // cannot be penultimate character
            if (itr == line.end() - 2) return false;
            itr++;

            if (*itr == '"' || 
                *itr == '\\' ||
                *itr == '/' ||
                *itr == 'b' ||
                *itr == 'f' ||
                *itr == 'n' ||
                *itr == 'r' ||
                *itr == 't') continue;
            if (*itr == 'u')
            {
                // four characters and terminal quote must fit before back()
                if (itr > line.end() - 6) return false;
                itr++;

                for (size_t i = 0; i < 4; i++)
                {
                    if (!((*itr >= '0' && *itr <= '9') || (*itr >= 'a' && *itr <= 'f') || (*itr >= 'A' && *itr <= 'F'))) return false;
                    itr++;
                }
            }
        }
    }
    return true;
}

bool verify_number(const std::string_view line)
{
    auto itr = line.begin();
    if (itr == line.end()) return false; // number cannot be empty string

    if (*itr != '-' && (*itr < '0' || *itr > '9')) return false; // incorrect start character (special case to handle '-'
    // technically allows non-standard leading 0's, but I prefer being able to put these
    itr++;

    bool has_decimal = false;
    bool has_exponent = false;

    for (;itr != line.end(); itr++)
    {
        if (*itr == '.') { has_decimal = true; itr++; break; }
        if (*itr == 'e' || *itr == 'E') { has_exponent = true; itr++; break; }
        if (*itr < '0' || *itr > '9') return false; // non-digit char
    }

    // decimal part
    if (has_decimal)
    {
        for (; itr != line.end(); itr++)
        {
            if (*itr == 'e' || *itr == 'E') { has_exponent = true; itr++; break; }
            if (*itr < '0' || *itr > '9') return false; // non-digit char
        }
    }

    // exponent part
    if (has_exponent)
    {
        if (*itr != '-' && *itr != '+' && (*itr < '0' || *itr > '9')) return false; // not legal first char of exponent
        for (; itr != line.end(); itr++)
        {
            if (*itr < '0' || *itr > '9') return false; // non-digit char
        }
    }
    return true;
}

// covers null as well
bool verify_bool(const std::string_view line)
{
    return (line == "true" || line == "false" || line == "null");
}

// TODO:Write Tests
bool verify_json(const std::string_view line)
{
    // do not need to determine what is wrong, just whether there is an error
    // May extend to allow other top level types, but currently only accepts objects
    return verify_object(line);
}
