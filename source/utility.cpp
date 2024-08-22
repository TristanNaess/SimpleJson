#include "utility.hpp"

std::string operator+(const char* line1, const std::string& line2)
{
    return std::string(line1) + line2;
}
