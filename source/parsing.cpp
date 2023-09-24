#include "parsing.hpp"
#include "error.hpp"


// When it would affect the parsing, it is assumed that all whitespace not in strings has been removed before parsing

std::size_t match_quote(const std::string& line, std::size_t index)
{
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

std::size_t match_bracket(const std::string& line, std::size_t index)
{
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

std::size_t find_unquoted(const std::string& line, char character, std::size_t index)
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
    return std::string::npos;
}

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

// this is a test function, so no error checking, assumes no whitespace
void identify_fields(const std::string& line)
{
    std::string bracketing;
    bracketing.reserve(line.size());
    
    std::size_t itr = 1; // start inside bracket
    std::size_t b_itr = 0;

    bool loop = true;
    while (loop)
    {
        // find start of key
        itr = find_unquoted(line, '"', itr);
        while (b_itr < itr)
        {
            bracketing += ' ';
            b_itr++;
        }
        bracketing += '^';
        b_itr++;

        // find start of value (colon delim)
        itr = find_unquoted(line, ':', itr);
        while (b_itr < itr - 1)
        {
            bracketing += 'k';
            b_itr++;
        }
        bracketing += "^ ^";
        itr++; // no whitespace between ':' and beginning of value
        b_itr = itr + 1;

        // find end of value (comma delim)
        switch (line[itr])
        {
            case '[':
            case '{':
                itr = match_bracket(line, itr);
                break;
            case '"':
                itr = match_quote(line, itr);
                break;
        }
        itr++;
        itr = find_unquoted(line, ',', itr);
        if (itr == std::string::npos)
        {
            itr = line.size() - 1;
            loop = false;
        }
        while (b_itr < itr - 1)
        {
            bracketing += 'v';
            b_itr++;
        }

        bracketing += '^';
        b_itr++;
    }

    std::cout << line << '\n' << bracketing << '\n';
}
