#include "parsing.hpp"
#include "error.hpp"

#include <algorithm>

// -----------------------------
// Parsing internal functions
// -----------------------------

// ----- general utility -----

// unless otherwise noted, ranges are [start, end)

// seek end of quoted field
// assumes at start of quoted string
// returns iterator to final quote
std::string::iterator skip_quotes(std::string::iterator start, std::string::iterator end) noexcept
{
    while (start != end)
    {
        start++;
        if (*start == '\\') { start++; continue; }
        if (*start == '"') { return start; }
    }
    return start;
}
std::string::iterator skip_brackets(std::string::iterator start, std::string::iterator end) noexcept
{
    char open = *start;
    char close;
    if (open == '{') close = '}';
    else close = ']';

    int depth = 1;
    while (start != end && depth > 0)
    {
        start++;
        if (*start == open) { depth++; continue; }
        if (*start == close) { depth--; continue; }
        if (*start == '"') { start = skip_quotes(start, end); continue; }
    }
    return start;
}
// locate char c between start and end, not within a quoted string and not nested within a deeper structure
std::string::iterator find_applicable(std::string::iterator start, std::string::iterator end, char c) noexcept
{
    while (start != end)
    {
        if (*start == c) { return start; }
        if (*start == '"') { start = skip_quotes(start, end); if (start == end) break; }
        if (*start == '{' || *start == '[') { start = skip_brackets(start, end); if (start == end) break; }
        start++;
    }
    return end;
}

// ----- specialized -----

// -----------------------------
// Header Defined
// -----------------------------

field get_field(mut_view data) noexcept
{
    std::string::iterator delim = find_applicable(data.begin, data.end, ':');
    data.end = find_applicable(data.begin, data.end, ',');
    field result;
    result.key = std::string_view(data.begin, delim);
    data.begin = delim+1;
    result.val = data;
    return result;
}

std::string remove_whitespace(std::string& s) noexcept
{
    static std::string whitespace = " \t\n\r\v\f";
    std::string result;
    result.reserve(s.size());

    for (auto c = s.begin(); c != s.end(); c++)
    {
        if (whitespace.find(*c) != std::string::npos) { continue; }

        if (*c == '"')
        {
            auto t = skip_quotes(c, s.end());
            for (; c < t; c++)
            {
                result += *c;
            }

        }
        result += *c;
    }

    return result;
}
