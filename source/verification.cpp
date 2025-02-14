#include "verification.hpp"

bool verify_json(std::string_view line)
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

bool verify_object(std::string_view line)
{
    throw todo("TODO: bool verify_object(std::string_view)");
}

bool verify_array(std::string_view line)
{
    throw todo("TODO: bool verify_array(std::string_view)");
}

bool verify_string(std::string_view line)
{
    enum class State { Start, CloseQuote, Codepoint, Esc, UnicodeEsc, X, XX, XXX } state = State::Start;

    for (auto c : line)
    {
        switch (state)
        {
            case State::Start:
                switch (c)
                {
                    case '"': state = State::Codepoint; break;
                    default: return false;
                }
                break;
            case State::CloseQuote:
                return false;
                break;
            case State::Codepoint:
                if (c < 0x0020) return false;
                if (c > 0x10FFFF) return false;
                if (c == '\\') state = State::Esc;
                if (c == '"') state = State::CloseQuote;
                break; // all other codepoints legal and stay in state
            case State::Esc:
                switch (c)
                {
                    case 'u': state = State::UnicodeEsc; break;
                    case '"':
                    case '\\':
                    case '/':
                    case 'b':
                    case 'n':
                    case 'f':
                    case 'r':
                    case 't': state = State::Codepoint; break;
                    default: return false;
                }
                break;
            case State::UnicodeEsc:
                if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) state = State::X;
                else return false;
                break;
            case State::X:
                if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) state = State::XX;
                else return false;
                break;
            case State::XX:
                if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) state = State::XXX;
                else return false;
                break;
            case State::XXX:
                if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) state = State::Codepoint;
                else return false;
                break;
        }
    }
    return state == State::CloseQuote;
}

bool verify_number(std::string_view line)
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

bool verify_bool(std::string_view line)
{
    return line == "true" || line == "false";
}

bool verify_null(std::string_view line)
{
    return line == "null";
}

// -------------------------------------------------------------------------------------------


// All is_*() functions assume the data is at least a character

bool is_object(std::string_view line)
{
    return line[0] == '{';
}

bool is_array(std::string_view line)
{
    return line[0] == '[';
}

bool is_string(std::string_view line)
{
    return line[0] == '"';
}

bool is_number(std::string_view line)
{
    return (line[0] == '-' || (line[0] >= '0' && line[0] <= '9'));
}

bool is_bool(std::string_view line)
{
    return (line[0] == 't'  || line[0] == 'f');
}

bool is_null(std::string_view line)
{
    return line[0] == 'n';
}
