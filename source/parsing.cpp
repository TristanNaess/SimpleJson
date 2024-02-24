#include "parsing.hpp"
#include "error.hpp"
#include <iostream> // for debugging

// When it would affect the parsing, it is assumed that all whitespace not in strings has been removed before parsing

// --------------------------------------------------------
// add concatination of string like types in other orders
// --------------------------------------------------------
std::string operator+(const char* line1, const std::string& line2)
{
    std::string res;
    res.reserve(strlen(line1) + line2.size());
    res = line1;
    res += line2;
    return res;
}

std::string operator+(const char* line1, std::string_view line2)
{
    std::string res;
    res.reserve(strlen(line1) + line2.size());
    res = line1;
    res += line2;
    return res;
}

std::string operator+(const std::string& line1, std::string_view line2)
{
    std::string res;
    res.reserve(line1.size() + line2.size());
    res = line1;
    res += line2;
    return res;
}

// --------------------------------------------------------
// find matching character considering syntax of json
// --------------------------------------------------------
std::size_t match_quote(std::string_view line, std::size_t index)
{
    // The guards shouldn't be necessary, since use is purely internal. Move checks to outside function later
    if (index >= line.size()) throw json::parsing_error("Index argument provided is outside the string");
    if (line[index] != '"') throw json::parsing_error("No quote at index provided in argument");
    index++;
    while (index < line.size())
    {
        switch (line[index])
        {
            case '"': // quote located
                return index;
                break;
            case '\\': // skip next escaped character
                index++;
                break;
        }
        index++;
    }
    throw json::parsing_error("No matching quote located");
}

std::size_t match_bracket(std::string_view line, std::size_t index)
{
    // Move check outside function, internal use only
    if (index >= line.size()) throw json::parsing_error("Index argument provided is outside the string");
    char open_bracket = line[index];
    char close_bracket;
    switch (open_bracket)
    {
        case '{':
            close_bracket = '}';
            break;
        case '[':
            close_bracket = ']';
            break;
        default:
            throw json::parsing_error("Character at index is not a bracket");
    }

    index++;
    std::size_t depth = 1;
    while (index < line.size())
    {
        // can't use switch for non-literal comparison
        if (line[index] == open_bracket) { depth++; index++; continue; }
        if (line[index] == close_bracket)
        {
            depth--;
            if (depth == 0) return index;
            index++;
            continue;
        }
        // I could rewrite this with find_unquoted(), but ehh...
        if (line[index] == '"') // skip internal strings to avoid non-syntax brackets
        {
            // try catch for no matching quote error. Other conditions shouldn't occur
            try
            {
                index = match_quote(line, index);
            }
            catch (const json::parsing_error& e)
            {
                throw json::parsing_error(std::string("Error while matching bracket: ") + e.what());
            }
        }
        index++;
    }
    throw json::parsing_error("No matching bracket located");
}

std::size_t find_unquoted(std::string_view line, char character, std::size_t index)
{
    while (index < line.size())
    {
        if (line[index] == character) return index;
        if (line[index] == '"') // if looking for " will return location rather than skipping
        {
            try
            {
                index = match_quote(line, index);
            }
            catch (const json::parsing_error& e)
            {
                throw json::parsing_error(std::string("Error finding character in string: ") + e.what());
            }
        }
        index++;
    }
    return std::string_view::npos;
}

std::size_t next_delim(std::string_view line, std::size_t start)
{
    for (; start < line.size(); start++)
    {
        switch (line[start])
        {
            case '"':
                start = match_quote(line, start);
                break;
            case '{':
            case '[':
                start = match_bracket(line, start);
                break;
            case ',':
            case ':':
                return start;
                break;
        }
    }
    return std::string_view::npos;
}

// --------------------------------------------------------
// remove leading, trailing whitespace not in string
// --------------------------------------------------------
std::string remove_whitespace(const std::string& line)
{
    std::string result;
    result.reserve(line.size());
    std::size_t itr = 0;
    std::size_t temp;
    while (itr < line.size())
    {
        switch (line[itr])
        {
            case ' ':
            case '\n':
            case '\r':
            case '\t':
                itr++;
                break;
            case '"':
                temp = itr;
                itr = match_quote(line, itr) + 1;
                result += line.substr(temp, itr-temp);
                break;
            default:
                result += line[itr];
                itr++;
        }
    }
    result.shrink_to_fit();
    return result;
}

// --------------------------------------------------------
// extract substring for requested data
// --------------------------------------------------------
std::string_view extract_field(std::string_view line, std::string_view key)
{
    // This assumes the string is pre-checked to be an object string
    line.remove_prefix(1);
    line.remove_suffix(1);

    std::size_t start = 0;
    std::size_t end;
    std::string_view field;
    std::string_view temp_key;
    while ((end = find_unquoted(line, ',', start)) != std::string_view::npos)
    {
        field = line.substr(start, end-start);
        temp_key = field.substr(start+1, find_unquoted(field, ':')-1); // trim quotes while we're here
        if (temp_key == key) return field;
    }
    
    field = line.substr(start);
    temp_key = field.substr(start+1, find_unquoted(field, ':')-1);
    if (temp_key == key) return field;

    throw json::out_of_range("No field matching key" + key);
}

std::string_view extract_index(std::string_view line, std::size_t index)
{
    throw json::out_of_range("Not implemented");    
}
