#include "parsing.hpp"

// starting at opening character, skip to matching closing character, returns iter to closing character
// only works if data is verified
std::string::iterator skip_quotes(std::string::iterator opening_quote) noexcept
{
    auto itr = opening_quote;
    while (1)
    {
        itr++;
        switch (*itr)
        {
            case '\\':
                itr++;
                continue;
                break;
            case '"':
                return itr;
            default:
                // no action
                break;
        }
    }
}
// because I need a const version for remove_whitespace
std::string::const_iterator skip_quotes(std::string::const_iterator opening_quote) noexcept
{
    auto itr = opening_quote;
    while (1)
    {
        itr++;
        switch (*itr)
        {
            case '\\':
                itr++;
                continue;
                break;
            case '"':
                return itr;
            default:
                // no action
                break;
        }
    }
}

// only works if data is verified
std::string::iterator skip_brackets(std::string::iterator opening_bracket) noexcept
{
    auto itr = opening_bracket;
    while (1)
    {
        char open = *itr;
        char close = open == '{' ? '}' : ']';

        std::size_t depth = 1;
        while (depth > 0)
        {
            itr++;
            // can't use switch, because case has to be a literal
            if (*itr == open) { depth++; continue; }
            if (*itr == close) { depth--; continue; }
            if (*itr == '"') { itr = skip_quotes(itr); continue; }
        }

        return itr;
    }
}

field next_field(std::string::iterator begin) noexcept
{
    field f;
    f.key.begin = begin;
    f.key.end = skip_quotes(f.key.begin) + 1; // colon after key

    f.val.begin = f.key.end+1; // first byte after ':'
    f.val.end = f.val.begin;
    while (*f.val.end != ',' && *f.val.end != '}' && *f.val.end != ']')
    {
        switch (*f.val.end)
        {
            case '"':
                f.val.end = skip_quotes(f.val.end);
                break;
            case '{':
            case '[':
                f.val.end = skip_brackets(f.val.end);
                break;
            default:
                // no action
                break;
        }
        f.val.end++;
    }

    return f;
}

// will remove any non-string whitespace.
// improperly formatted json will be broken, but it is broken already, so that's fine
// call before verification
std::string remove_whitespace(const std::string& s) noexcept
{
    static std::string whitespace = " \t\n\r\v\f";
    
    if (s.size() == 0) return "";

    std::string result;
    result.reserve(s.size());

    result += s[0];
    bool in_quotes = s[0] == '"';

    for (auto c = s.begin()+1; c != s.end(); c++)
    {

        if (in_quotes)
        {
            if (*c == '"' && *c-1 != '\\')
            {
                in_quotes = false;
            }
            result += *c;
            continue;
        }

        if (*c == '"') in_quotes = true;

        if (whitespace.find(*c) != std::string::npos) continue;
        result += *c;
    }

    return result;
}
