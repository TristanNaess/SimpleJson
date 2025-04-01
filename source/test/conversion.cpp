#include <gtest/gtest.h>
#include "conversion.hpp"
#include "error.hpp"

TEST(Conversion, String)
{
    // To string passing
    EXPECT_EQ(conversion::to_string("\"This is a basic string\""), "This is a basic string") << "Failed to convert basic string from internal representation to std::string";
    EXPECT_EQ(conversion::to_string("\"This \\\"string\\\" contains \\\"internal quotes\\\"\""), "This \"string\" contains \"internal quotes\"") << "Failed to convert string containing quotes from internal representation to std::string";
    EXPECT_EQ(conversion::to_string("\\\"This string is quote surrounded\\\""), "\"This string is quote surrounded\"") << "Failed to convert quote surrounded string from internal representation to std::string";
    EXPECT_EQ(conversion::to_string("\"This s\\\"tring \\rcontain\\\\s al\\tl esc\\/aped\\f charac\\nters\""), "This s\"tring \rcontain\\s al\tl esc/aped\f charac\nters") << "Failed to convert string containing other escape characters from internal representation to std::string";
    EXPECT_EQ(conversion::to_string("\"This line contains unicode characters: Ü 𐦆 \\u0076 \\u00c8 \\u0aaa\""), "This line contains unicode characters: Ü 𐦆 \u0076 \u00c8 \u0aaa") << "Failed to convert string containing unicode from internal representation to std::string";

    // From string passing
    EXPECT_EQ(conversion::to_data("This is basic ASCII string"), std::string("\"This is a basic ASCII string\"")) << "Failed to convert from basic string to internal representation";
    EXPECT_EQ(conversion::to_data("This \"string\" is more complex\n\tIt\b cont\fains the other escape \rchara\\cters"), std::string("This \\\"string\\\" is more complex\\n\\tIt\\b cont\\fains the other escape \\rchara\\\\cters")) << "Failed to convert a string containing escaped characters to internal representation";
    EXPECT_EQ(conversion::to_data("This line contains unicode characters: \u00ab \u01da \uad2f"), std::string("This line contains unicode characters: \u00ab \u01da \uad2f")) << "Failed to convert string containing unicode characters to internal representation";
}

// Because the functions are only wrappers around std::stold/stoll/stoull and some size checks, I won't be testing basic functionality past that the path works

TEST(Conversion, LongDouble)
{
    EXPECT_NEAR(conversion::to_ldouble("1836.67e-43"), 1836.67e-43l, std::numeric_limits<long double>::epsilon()) << "Failed to convert internal string to long double";
    EXPECT_THROW(conversion::to_ldouble("\"Foobar\""), json::wrong_type) << "Failed to throw json::wrong_type when called with string data";
    EXPECT_THROW(conversion::to_ldouble("true"), json::wrong_type) << "Failed to throw json::wrong_type when called with ";
    // not checking for object or array, since they shouldn't even be considered for conversion

    // no checks for conversion to data, since arg is bound checked by data type
}

TEST(Conversion, Double)
{
    EXPECT_NEAR(conversion::to_double("617965.3743e13"), 617965.3743e13, std::numeric_limits<double>::epsilon()) << "Failed to convert internal string to double";
    EXPECT_THROW(conversion::to_double(std::to_string(static_cast<long double>(std::numeric_limits<double>::max()) + 10000)), json::out_of_range) << "Failed to throw json::out_of_range for long double size data in double conversion";

    EXPECT_THROW(conversion::to_double("\"Foobar\""), json::wrong_type) << "Failed to throw json::wrong_type when called with string data";
    EXPECT_THROW(conversion::to_double("true"), json::wrong_type) << "Failed to throw json::wrong_type when called with boolean data";
} 

TEST(Conversion, Float)
{
    EXPECT_NEAR(conversion::to_float("1768435.376e73"), 1768435.376e73, std::numeric_limits<float>::epsilon()) << "Failed to convert internal string to float";
    EXPECT_THROW(conversion::to_float(std::to_string(static_cast<double>(std::numeric_limits<float>::max()) + 10000)), json::out_of_range) << "Failed to throw json::out_of_range for double sized data in float conversion";

    EXPECT_THROW(conversion::to_float("\"Foobar\""), json::wrong_type) << "Failed to throw json::wrong_type when called with string data";
    EXPECT_THROW(conversion::to_float("true"), json::wrong_type) << "Failed to throw json::wrong_type when called with boolean data";

}

TEST(Conversion, LongLongUnsigned)
{
    EXPECT_EQ(conversion::to_ullint("1678956789780"), 1678956789780ll) << "Failed to convert internal string to unsigned long long int";
    EXPECT_THROW(conversion::to_ullint("-123"), json::wrong_type) << "Failed to throw json::wrong_type when called with negative number";
    EXPECT_THROW(conversion::to_ullint("123.45"), json::wrong_type) << "Failed to throw json::wrong_type when called with floating point number";
    EXPECT_THROW(conversion::to_ullint("\"Foobar\""), json::wrong_type) << "Failed to throw json::wrong_type when called with string data";
    EXPECT_THROW(conversion::to_ullint("true"), json::wrong_type) << "Failed to throw json::wrong_type when called with boolean data";
}

TEST(Conversion, LongUnsigned)
{
    EXPECT_EQ(conversion::to_ulint("1678956789780"), 1678956789780l) << "Failed to convert internal string to unsigned long int";
    if constexpr (sizeof(unsigned long long int) > sizeof(unsigned long int)) // both are 64 bits on some compilers
    {
        EXPECT_THROW(conversion::to_ulint(std::to_string(static_cast<unsigned long long int>(std::numeric_limits<unsigned long int>::max()) + 1)), json::out_of_range) << "Failed to throw json::out_of_range when called with number too large";
    }
    EXPECT_THROW(conversion::to_ulint("-123"), json::wrong_type) << "Failed to throw json::wrong_type when called with negative number";
    EXPECT_THROW(conversion::to_ulint("123.45"), json::wrong_type) << "Failed to throw json::wrong_type when called with floating point number";
    EXPECT_THROW(conversion::to_ulint("\"Foobar\""), json::wrong_type) << "Failed to throw json::wrong_type when called with string data";
    EXPECT_THROW(conversion::to_ulint("true"), json::wrong_type) << "Failed to throw json::wrong_type when called with boolean data";
}

TEST(Conversion, Unsigned)
{
    EXPECT_EQ(conversion::to_uint("1678780"), 1678780) << "Failed to convert internal string to unsigned int";
    if constexpr (sizeof(unsigned long int) > sizeof(unsigned int)) // both are 32 bits on some compilers
    {
        EXPECT_THROW(conversion::to_uint(std::to_string(static_cast<unsigned long int>(std::numeric_limits<unsigned int>::max()) + 1)), json::out_of_range) << "Failed to throw json::out_of_range when called with number too large";
    }
    EXPECT_THROW(conversion::to_uint("-123"), json::wrong_type) << "Failed to throw json::wrong_type when called with negative number";
    EXPECT_THROW(conversion::to_uint("123.45"), json::wrong_type) << "Failed to throw json::wrong_type when called with floating point number";
    EXPECT_THROW(conversion::to_uint("\"Foobar\""), json::wrong_type) << "Failed to throw json::wrong_type when called with string data";
    EXPECT_THROW(conversion::to_uint("true"), json::wrong_type) << "Failed to throw json::wrong_type when called with boolean data";
}

TEST(Conversion, UnsignedChar)
{
    EXPECT_EQ(conversion::to_uchar("178"), 178) << "Failed to convert internal string to unsigned char";
    EXPECT_THROW(conversion::to_uchar("256"), json::out_of_range) << "Failed to throw json::out_of_range when called with number too large";
    EXPECT_THROW(conversion::to_uchar("-123"), json::wrong_type) << "Failed to throw json::wrong_type when called with negative number";
    EXPECT_THROW(conversion::to_uchar("123.45"), json::wrong_type) << "Failed to throw json::wrong_type when called with floating point number";
    EXPECT_THROW(conversion::to_uchar("\"Foobar\""), json::wrong_type) << "Failed to throw json::wrong_type when called with string data";
    EXPECT_THROW(conversion::to_uchar("true"), json::wrong_type) << "Failed to throw json::wrong_type when called with boolean data";
}

TEST(Conversion, LongLongInt)
{
    EXPECT_EQ(conversion::to_llint("1678956789780"), 1678956789780ll) << "Failed to convert internal string to long long int";
    EXPECT_THROW(conversion::to_llint("123.45"), json::wrong_type) << "Failed to throw json::wrong_type when called with floating point number";
    EXPECT_THROW(conversion::to_llint("\"Foobar\""), json::wrong_type) << "Failed to throw json::wrong_type when called with string data";
    EXPECT_THROW(conversion::to_llint("true"), json::wrong_type) << "Failed to throw json::wrong_type when called with boolean data";
}

TEST(Conversion, LongInt)
{
    EXPECT_EQ(conversion::to_lint("1678956789780"), 1678956789780l) << "Failed to convert internal string to long int";
    if constexpr (sizeof(long long int) > sizeof(long int)) // both can be 64 bits on some compilers
    {
        EXPECT_THROW(conversion::to_lint(std::to_string(static_cast<long long int>(std::numeric_limits<long int>::max()) + 1)), json::out_of_range) << "Failed to throw json::out_of_range when called with number too large";
    }
    EXPECT_THROW(conversion::to_lint("123.45"), json::wrong_type) << "Failed to throw json::wrong_type when called with floating point number";
    EXPECT_THROW(conversion::to_lint("\"Foobar\""), json::wrong_type) << "Failed to throw json::wrong_type when called with string data";
    EXPECT_THROW(conversion::to_lint("true"), json::wrong_type) << "Failed to throw json::wrong_type when called with boolean data";
}

TEST(Conversion, Int)
{
    EXPECT_EQ(conversion::to_int("1678780"), 1678780) << "Failed to convert internal string to int";
    if constexpr (sizeof(long int) > sizeof(int)) // both can be 32 bits on some compilers
    {
        EXPECT_THROW(conversion::to_int(std::to_string(static_cast<long int>(std::numeric_limits<int>::max()) + 1)), json::out_of_range) << "Failed to throw json::out_of_range when called with number too large";
    }
    EXPECT_THROW(conversion::to_int("123.45"), json::wrong_type) << "Failed to throw json::wrong_type when called with floating point number";
    EXPECT_THROW(conversion::to_int("\"Foobar\""), json::wrong_type) << "Failed to throw json::wrong_type when called with string data";
    EXPECT_THROW(conversion::to_int("true"), json::wrong_type) << "Failed to throw json::wrong_type when called with boolean data";
}

TEST(Conversion, Char)
{
    EXPECT_EQ(conversion::to_char("17"), 17) << "Failed to convert internal string to char";
    EXPECT_THROW(conversion::to_char("128"), json::out_of_range) << "Failed to throw json::out_of_range when called with number too large";
    EXPECT_THROW(conversion::to_char("123.45"), json::wrong_type) << "Failed to throw json::wrong_type when called with floating point number";
    EXPECT_THROW(conversion::to_char("\"Foobar\""), json::wrong_type) << "Failed to throw json::wrong_type when called with string data";
    EXPECT_THROW(conversion::to_char("true"), json::wrong_type) << "Failed to throw json::wrong_type when called with boolean data";
}

TEST(Conversion, Bool)
{
    EXPECT_EQ(conversion::to_bool("true"), true) << "Failed to convert boolean true from internal representation to bool";
    EXPECT_EQ(conversion::to_bool("false"), false) << "Failed to convert boolean false from internal representation to bool";

    EXPECT_EQ(conversion::to_data(true), "true") << "Failed to convert true from bool to internal representation";
    EXPECT_EQ(conversion::to_data(false), "false") << "Failed to convert false from bool to internal representation";
}
