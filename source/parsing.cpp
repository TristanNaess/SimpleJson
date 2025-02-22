#include "parsing.hpp"

// ------------------------------------
//  Data Extraction
// ------------------------------------


// starting at opening character, skip to matching closing character, returns iter to closing character
// only works if data is verified
// non-const
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

// const
std::string_view::iterator skip_quotes(std::string_view::iterator opening_quote) noexcept
{
    while (1)
    {
        opening_quote++;
        switch (*opening_quote)
        {
            case '\\':
                opening_quote++;
                continue;
                break;
            case '"':
                return opening_quote;
            default:
                // no action
                break;
        }
    }
}

// only works if data is verified
// non-const
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

// const
std::string_view::iterator skip_brackets(std::string_view::iterator opening_bracket) noexcept
{
    while (1)
    {
        char open = *opening_bracket;
        char close = open == '{' ? '}' : ']';

        std::size_t depth = 1;
        while (depth > 0)
        {
            opening_bracket++;
            // can't use switch, because case has to be a literal
            if (*opening_bracket == open) { depth++; continue; }
            if (*opening_bracket == close) { depth--; continue; }
            if (*opening_bracket == '"') { opening_bracket = skip_quotes(opening_bracket); continue; }
        }

        return opening_bracket;
    }
}



std::string_view::iterator seek(std::string_view line, char c, std::string_view::iterator start) noexcept
{
    for (; start != line.end(); start++)
    {
        switch (*start)
        {
            case '"':
                start = skip_quotes(start);
                break;
            case '{':
            case '[':
                start = skip_brackets(start);
                break;
            default:
                if (*start == c) return start;
        }
    }
    return line.end();
}

std::string::iterator seek(std::string& line, char c, std::string::iterator start) noexcept
{
    for (; start != line.end(); start++)
    {
        switch (*start)
        {
            case '"':
                start = skip_quotes(start);
                break;
            case '{':
            case '[':
                start = skip_brackets(start);
                break;
            default:
                if (*start == c) return start;
        }
    }
    return line.end();
}

std::string_view next_field(std::string_view line, std::string_view::iterator start) noexcept
{
    if (line.size() == 2) return std::string_view(line.end(), line.end());
    start++;
    if (start == line.end()) return std::string_view(start, start);
    std::string_view::iterator end = seek(line, ',', start);
    if (end == line.end()) end--;
    return std::string_view(start, end);
}

std::string_view get_key(std::string_view field)
{
    std::string_view::iterator delim = seek(field, ':', field.begin());
    return std::string_view(field.begin(), delim);
}

mut_view get_key(mut_view field)
{
    std::string::iterator delim = seek(field.data, ':', field.begin);
    return mut_view(field.data, field.begin, delim);
}

std::string_view get_val(std::string_view field)
{
    std::string_view::iterator delim = seek(field, ':', field.begin());
    return std::string_view(delim, field.end());
}

mut_view get_val(mut_view field)
{
    std::string::iterator delim = seek(field.data, ':', field.begin);
    return mut_view(field.data, delim, field.begin);
}

void delete_field(mut_view field)
{
    field.data.erase(field.begin, field.end);
}

void change_field(mut_view field)
{
    mut_view val = get_val(field);
    field.data.replace(val.begin, val.end, field.data.begin(), field.data.end());
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
