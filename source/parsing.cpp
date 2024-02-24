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

//---------------------------------------------------------
// Conversion between C++ and json string representations
//---------------------------------------------------------

std::string to_json_string(std::string_view line)
{
    std::string str;
    str.reserve(line.size() + 2); // space for surrounding quotes, assume no escape characters most of the time
    str.push_back('"');

    for (auto c : line)
    {
        switch (c)
        {
            case '"':
                str.append("\\\"");
                break;
            case '\\':
                str.append("\\\\");
                break;
            case '\b':
                str.append("\\b");
                break;
            case '\f':
                str.append("\\f");
                break;
            case '\n':
                str.append("\\n");
                break;
            case '\r':
                str.append("\\r");
                break;
            case '\t':
                str.append("\\t");
                break;
            default:
                str.push_back(c);
        }
    }

    str.push_back('"');
    return str;
}

std::string from_json_string(std::string_view line)
{
    std::string str;
    str.reserve(line.size());
    if (line.front() != '"' || line.back() != '"') throw json::parsing_error("Missing inital or final quote in json string: '" + line + '\'');

    line.remove_prefix(1);
    line.remove_suffix(1);

    bool escaped = false;
    unsigned int codepoint = 0;
    unsigned int nibble;
    for (auto itr = line.begin(); itr != line.end(); itr++)
    {
        if (escaped)
        {
            escaped = false;
            switch (*itr)
            {
                case '"':
                    str.push_back('"');
                    break;
                case '\\':
                    str.push_back('\\');
                    break;
                case '/':
                    str.push_back('/');
                    break;
                case 'b':
                    str.push_back('\b');
                    break;
                case 'f':
                    str.push_back('\f');
                    break;
                case 'n':
                    str.push_back('\n');
                    break;
                case 'r':
                    str.push_back('\r');
                    break;
                case 't':
                    str.push_back('\t');
                    break;
                case 'u':
                    // convert 4 char codepoint to uint
                    // This was exhaustively checked with a separate program
                    for (std::size_t i = 0; i < 4; i++)
                    {
                        itr++;
                        if (!((*itr >= '0' && *itr <= '9') || (*itr >= 'a' && *itr <= 'f') || (*itr >= 'A' && *itr <= 'F')))
                        {
                            throw json::parsing_error("Bad character in UTF codepoint in: '" + line + '\'');
                        }
                        nibble = *itr - '0';
                        if (nibble > 9)
                        {
                            nibble &= 0xdf; // clear shifted capital bit
                            nibble -= 0x07;
                        }
                        codepoint = (codepoint << 4) | nibble;
                    }

                    // convert uint to UTF-8
                    // This hasn't been fully checked, but I ran a decent number of lookups and it seems to work
                    if (codepoint < 0x0080)
                    {
                        str.push_back(codepoint);
                        break;
                    }
                    if (codepoint < 0x0800)
                    {
                        str.push_back((codepoint >> 6) | 0xc0);
                        str.push_back((codepoint & 0x3f) | 0x80);
                        break;
                    }
                    // we're ignoring 0x10000 and higher, since JSON only specifies up to 4 hex chars in a codepoint
                    str.push_back((codepoint >> 12) | 0xe0);
                    str.push_back(((codepoint >> 6) & 0x3f) | 0x80);
                    str.push_back((codepoint & 0x3f) | 0x80);
                    break;
                default:
                    throw json::parsing_error("Bad escape character in json string: '" + line + '\'');
            }
            continue;
        }
        if (*itr == '\\')
        {
            escaped = true;
            continue;
        }
        str.push_back(*itr);
    }

    return str;
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
