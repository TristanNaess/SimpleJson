#include "parsing.hpp"
#include "error.hpp"

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

std::pair<std::size_t, std::size_t> next_field(const std::string& line, std::size_t index)
{
    if (index > line.size()) throw json::parsing_error("Index provided is outside the string");
    
    std::size_t start = std::string::npos;

    // locate ',' that is not inside quotes or brackets/braces
    while (index < line.size() && start == std::string::npos)
    {
        switch (line[index])
        {
            case ',':
                index++; // first character of field, not the comma
                start = index;
                break;
            case '"':
                try
                {
                    index = match_quote(line, index);
                }
                catch (const json::parsing_error& pe)
                {
                    throw json::parsing_error(std::string("Error while finding field: ") + pe.what());
                }
                break;
            case '[':
            case '{':
                try
                {
                    index = match_bracket(line, index);
                }
                catch (const json::parsing_error& pe)
                {
                    throw json::parsing_error(std::string("Error while finding field: ") + pe.what());
                }
                break;
            default:
        }
        index++;
    }

    // no comma located
    if (start == std::string::npos) return std::make_pair(start, std::string::npos);

    // locate second comma
    while (index < line.size() && end == std::string::npos)
    {
        switch (line[index])
        {
            case ',':
                return std::make_pair(start, index);
                break;
            case '"':
                try
                {
                    index = match_quote(line, index);
                }
                catch (const json::parsing_error& pe)
                {
                    throw json::parsing_error(std::string("Error while finding field: ") + pe.what());
                }
                break;
            case '[':
            case '{':
                try
                {
                    index = match_bracket(line, index);
                }
                catch (const json::parsing_error& pe)
                {
                    throw json::parsing_error(std::string("Error while finding field: ") + pe.what());
                }
                break;
            default:
        }
        index++;
    }
}
