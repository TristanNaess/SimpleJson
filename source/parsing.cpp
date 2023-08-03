#include "parsing.hpp"
#include "error.hpp"

std::size_t match_quote(const std::string& line, std::size_t index) const
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

std::size_t match_bracket(const std::string& line, std::size_t index) const
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
