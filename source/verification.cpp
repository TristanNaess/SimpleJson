#include "verification.hpp"

bool verify_json(std::string_view line)
{
    if (line.size() == 0) return false; // cannot be empty
    std::string_view::iterator start = line.begin();
    switch (line[0])
    {
        case '{':
            return verify_object(start, line.end());
            break;
        case '[':
            return verify_array(start, line.end());
            break;
        default:
            return false; // must be array or object
    }
}

bool verify_object(std::string_view::iterator& start, std::string_view::iterator end)
{
    throw todo("TODO: bool verify_object(std::string_view::iterator&, std::string_view::iterator)");
}

bool verify_array(std::string_view::iterator& start, std::string_view::iterator end)
{
    enum class State { Start, End, Field, EndOfField } state = State::Start;

    auto itr = start;
    for (; itr != end; itr++)
    {
        auto& c = *itr;
        switch (state)
        {
            case State::Start:
                if (c != '[') return false;
                state = State::Field;
                break;
            case State::End:
                start = itr;
                return true;
                break;
            case State::Field:
                switch (c)
                {
                    case '{':
                        if (verify_object(itr, end) == false) return false;
                        break;
                    case '[':
                        if (verify_array(itr, end) == false) return false;
                        break;
                    case '"':
                        if (verify_string(itr, end) == false) return false;
                        break;
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
                    case '9':
                        if (verify_number(itr, end) == false) return false;
                        break;
                    case 't':
                    case 'f':
                        if (verify_bool(itr, end) == false) return false;
                        break;
                    case 'n':
                        if (verify_null(itr, end) == false) return false;
                        break;
                    case ']':
                        break;
                    default:
                        return false;
                }
                itr--; // decrement so the itr++ pushes back to the following character
                state = State::EndOfField;
                break;
            case State::EndOfField:
                switch (c)
                {
                    case ',':
                        state = State::Field;
                        break;
                    case ']':
                        state = State::End;
                        break;
                    default:
                        return false;
                }
                break;
        }

    }
    if (state == State::End)
    {
        start = itr;
        return true;
    }
    return false;
}

bool verify_string(std::string_view::iterator& start, std::string_view::iterator end)
{
    enum class State { Start, CloseQuote, Codepoint, Esc, UnicodeEsc, X, XX, XXX } state = State::Start;

    for (auto itr = start; itr != end; itr++)
    {
        auto& c = *itr;
        switch (state)
        {
            case State::Start:
                switch (c)
                {
                    case '"': state = State::Codepoint; break;
                    default: return false;
                }
                break;
            case State::CloseQuote: // technically this may be invalid, but we reached the first valid string ending. The calling function can handle if the next char is wrong
                start = itr;
                return true;
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
    return false;
}

bool verify_number(std::string_view::iterator& start, std::string_view::iterator end)
{
    enum class State { Start, Int, Neg, Zero, Dec, ExpSign, Exp } state = State::Start;

    // works because non-ascii are illegal here
    for (auto itr = start; itr != end; itr++)
    {
        auto& c = *itr;
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
                    case ',':
                    case '}':
                    case ']':
                        start = itr;
                        return true;
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
            case State::Zero: // legal end state
                switch (c)
                {
                    case '.': state = State::Dec; break;
                    case 'e':
                    case 'E': state = State::ExpSign; break;
                    case ',':
                    case '}':
                    case ']':
                        start = itr;
                        return true;
                    default:
                        return false;
                }
                break;
            case State::Dec: // legal end state
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
                    case ',':
                    case '}':
                    case ']':
                        start = itr;
                        return true;
                    default:
                        return false;
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
            case State::Exp: // legal end state
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
                    case ',':
                    case '}':
                    case ']':
                        start = itr;
                        return true;
                    default:
                        return false;
                }
                break;
        }
    }
    return false;
}

bool verify_bool(std::string_view::iterator& start, std::string_view::iterator end)
{
    if (end - start < 4) return false;
    if (std::string_view(start, start+4) == "true")
    {
        start += 4;
        return true;
    }
    if (end - start < 5) return false;
    if (std::string_view(start, start+5) == "false")
    {
        start += 5;
        return true;
    }
    return false;
}

bool verify_null(std::string_view::iterator& start, std::string_view::iterator end)
{
    if (end - start < 4) return false;
    if (std::string_view(start, start+4) == "null")
    {
        start += 4;
        return true;
    }
    return false;
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
