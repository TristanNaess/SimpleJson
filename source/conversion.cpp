#include "conversion.hpp"
#include "error.hpp"

namespace conversion
{
    // conversion from json to type

    std::string to_string(std::string_view data)
    {
        std::string line;
        line.reserve(data.size());

        for (auto i = data.begin() + 1; i < data.end() - 1; i++)
        {
            if (*i == '\\')
            {
                i++;
                switch (*i)
                {
                    case '"':
                    case '\\':
                    case '/':
                        line.push_back(*i);
                        break;
                    case 'b':
                        line.push_back('\b');
                        break;
                    case 'f':
                        line.push_back('\f');
                        break;
                    case 'n':
                        line.push_back('\n');
                        break;
                    case 'r':
                        line.push_back('\r');
                        break;
                    case 't':
                        line.push_back('\t');
                        break;
                    case 'u':
                        unsigned int val = 0;
                        for (auto c = 0; c < 4; c++)
                        {
                            i++;
                            unsigned int d = 0;
                            if (*i <= '9') d = *i - '0';      // make digits into their value
                            else d = (*i & 0xdf) - 'A' + 10;  // (zero out lowercase bit) set to A = 0, add so A = 10
                            val = (val << 4) + d;
                        }
                        // max three bytes in utf-8
                        if (val < 0x80) // one byte
                        {
                            line.push_back(val & 0x7f);
                        }
                        else if (val < 0x800) // two bytes
                        {
                            line.push_back(0xc0 + (val >> 6)); // header code + top 5 bits
                            line.push_back(0x80 + (val & 0x3f)); // header + bottom 6 bits
                        }
                        else // three bytes
                        {
                            line.push_back(0xe0 + (val >> 12));
                            line.push_back(0x80 + ((val >> 6) & 0x3f));
                            line.push_back(0x80 + (val& 0x3f));
                        }
                        break;
                }
            }
            else line.push_back(*i);
        }
        line.shrink_to_fit();
        return line;
    }

    long double to_ldouble(std::string_view data)
    {
        throw todo{"TODO: to_ldouble(std::string_view data)"};
    }

    double to_double(std::string_view data)
    {
        throw todo{"TODO: to_double(std::string_view data)"};
    }

    float to_float(std::string_view data)
    {
        throw todo{"TODO: to_float(std::string_view data)"};
    }

    unsigned long long int to_ullint(std::string_view data)
    {
        throw todo{"TODO: to_ullint(std::string_view data)"};
    }

    unsigned long intto_ulint(std::string_view data)
    {
        throw todo{"TODO: to_ulint(std::string_view data)"};
    }

    unsigned int to_uint(std::string_view data)
    {
        throw todo{"TODO: to_uint(std::string_view data)"};
    }

    unsigned char to_uchar(std::string_view data)
    {
        throw todo{"TODO: to_uchar(std::string_view data)"};
    }

    long long int to_llint(std::string_view data)
    {
        throw todo{"TODO: to_llint(std::string_view data)"};
    }

    long int to_lint(std::string_view data)
    {
        throw todo{"TODO: to_lint(std::string_view data)"};
    }

    int to_int(std::string_view data)
    {
        throw todo{"TODO: to_int(std::string_view data)"};
    }

    char to_char(std::string_view data)
    {
        throw todo{"TODO: to_char(std::string_view data)"};
    }

    bool to_bool(std::string_view data)
    {
        return data == "true";
    }

    // conversion from type to json

    std::string to_data(std::string_view line)
    {
        throw todo{"TODO: to_data(std::string_view line)"};
    }

    // because the compiler seems to think this should use the bool version...
    std::string to_data(const char* line)
    {
        return to_data(std::string_view(line));
    }

    std::string to_data(long double d)
    {
        throw todo{"TODO: to_data(long double d)"};
    }

    std::string to_data(double d)
    {
        throw todo{"TODO: to_data(double d)"};
    }

    std::string to_data(float f)
    {
        throw todo{"TODO: to_data(float f)"};
    }

    std::string to_data(unsigned long long int i)
    {
        throw todo{"TODO: to_data(unsigned long long int i)"};
    }

    std::string to_data(unsigned long int i)
    {
        throw todo{"TODO: to_data(unsigned long int i)"};
    }

    std::string to_data(unsigned int i)
    {
        throw todo{"TODO: to_data(unsigned int i)"};
    }

    std::string to_data(unsigned char c)
    {
        throw todo{"TODO: to_data(unsigned char c)"};
    }

    std::string to_data(long long int i)
    {
        throw todo{"TODO: to_data(long long int i)"};
    }

    std::string to_data(long int i)
    {
        throw todo{"TODO: to_data(long int i)"};
    }

    std::string to_data(int i)
    {
        throw todo{"TODO: to_data(int i)"};
    }

    std::string to_data(char c)
    {
        throw todo{"TODO: to_data(char c)"};
    }

    std::string to_data(bool b)
    {
        return b == true ? "true" : "false";
    }
}
