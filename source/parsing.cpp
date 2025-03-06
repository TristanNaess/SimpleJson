#include "parsing.hpp"

// ------------------------------------
//  mut_view
// ------------------------------------
mut_view::mut_view(std::string& line, std::string::iterator begin, std::string::iterator end) : data(line), begin(begin), end(end) {  }

mut_view& mut_view::operator=(const mut_view& other)
{
    data = other.data;
    begin = other.begin;
    end = other.end;
    return *this;
}

std::size_t mut_view::size() const
{
    return end - begin;
}

mut_view::operator std::string() const
{
    return std::string(begin, end);
}

bool operator==(const mut_view& lhs, const mut_view& rhs)
{
    return lhs.data == rhs.data && lhs.begin == rhs.begin && lhs.end == rhs.end;
}

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

std::string::iterator seek(mut_view data, char c, std::string::iterator start) noexcept
{
    // could add an assert here for start within data
    while (start != data.end)
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
        start++;
    }
    return data.end;
}

mut_view next_field(mut_view data, std::string::iterator start) noexcept
{
    if (start == data.end) return mut_view(data.data, data.end, data.end);
    start++;
    std::string::iterator end = seek(data, ',', start);
    return mut_view(data.data, start, end);
}

mut_view get_key(mut_view field)
{
    std::string::iterator delim = seek(field, ':', field.begin);
    return mut_view(field.data, field.begin, delim);
}

mut_view get_val(mut_view field)
{
    std::string::iterator delim = seek(field, ':', field.begin);
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
std::string remove_whitespace(std::string_view s) noexcept
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
