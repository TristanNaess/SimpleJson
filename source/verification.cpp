#include "verification.hpp"

bool verify_json(const std::string& line)
{
    if (line.size() == 0) return false; // cannot be empty

    switch (line[0])
    {
        case '{':
            return verify_object(line);
            break;
        case '[':
            return verify_array(line);
            break;
        default:
            return false; // must be array or object
    }
}

bool verify_object(const std::string& line)
{
    throw todo("TODO: bool verify_object(const std::string&)");
}

bool verify_array(const std::string& line)
{
    throw todo("TODO: bool verify_array(const std::string&)");
}

bool verify_string(const std::string& line)
{
    throw todo("TODO: bool verify_string(const std::string&)");
}

bool verify_number(const std::string& line)
{
    enum class State { Start, Int, Neg, Zero, Dec, ExpSign, Exp } state = State::Start;

    // works because non-ascii are illegal here
    for (auto c : line)
    {
        switch (state)
        {
            case State::Start:
                switch (c)
                {
                    case '0': state = State::Zero; break;
                    case '-': state = State::Neg; break;
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': state = State::Int; break;
                    default: return false;
                }
                break;
            case State::Int:
                switch (c)
                {
                    case '.': state = State::Dec; break;
                    case 'e':
                    case 'E': state = State::ExpSign; break;
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': /*state = State::Int;*/ break;
                    default: return false;

                }
                break;
            case State::Neg:
                switch (c)
                {
                    case '0': state = State::Zero; break;
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': state = State::Int; break;
                    default: return false;
                }
                break;
            case State::Zero:
                switch (c)
                {
                    case '.': state = State::Dec; break;
                    case 'e':
                    case 'E': state = State::ExpSign; break;
                    default: return false;
                }
                break;
            case State::Dec:
                switch (c)
                {
                    case 'e':
                    case 'E': state = State::ExpSign; break;
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': /*state = State::Dec;*/ break;
                    default: return false;
                }
                break;
            case State::ExpSign:
                switch (c)
                {
                    case '+':
                    case '-':
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': state = State::Exp; break;
                    default: return false;
                }
                break;
            case State::Exp:
                switch (c)
                {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': /*state = State::Exp;*/ break;
                    default: return false;
                }
                break;
        }
    }
    return state == State::Zero || state == State::Int || state == State::Dec || state == State::Exp;
}

bool verify_bool(const std::string& line)
{
    return line == "true" || line == "false";
}

bool verify_null(const std::string& line)
{
    return line == "null";
}


// All is_*() functions assume the data is good, and sans-whitespace

bool is_object(const std::string& line)
{
    return line[0] == '{';
}

bool is_array(const std::string& line)
{
    return line[0] == '[';
}

bool is_string(const std::string& line)
{
    return line[0] == '"';
}

bool is_number(const std::string& line)
{
    return (line[0] == '-' || (line[0] >= '0' && line[0] <= '9'));
}

bool is_bool(const std::string& line)
{
    return (line == "true" || line == "false");
}

bool is_null(const std::string& line)
{
    return line == "null";
}
