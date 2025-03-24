#include <gtest/gtest.h>
#include "conversion.hpp"

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

/*
TEST(Conversion, LongDouble)
{
    // To long double passing
    
    // From long double passing

}

TEST(Conversion, Double)
{
    // To double passing

    // To double failing

    // From double passing

    // From double failing

}

TEST(Conversion, Float)
{
    // To float passing
    
    // To float failing

    // To float passing

    // To float failing

}

TEST(Conversion, LongLongUnsigned)
{
    // To long long unsigned int passing

    // To long long unsigned int failing

    // From long long unsigned int passing

    // From long long unsigned int failing
}

TEST(Conversion, LongUnsigned)
{
    // To long unsigned int passing
    
    // To long unsigned int failing

    // From long unsigned int passing

    // From long unsigned int failing
}

TEST(Conversion, Unsigned)
{
    // To unsigned int passing

    // To unsigned int failing

    // From unsigned int passing

    // From unsigned int failing

}

TEST(Conversion, UnsignedChar)
{
    // To unsigned char passing

    // To unsigned char failing

    // From unsigned char passing

    // From unsigned char failing

}

TEST(Conversion, LongLongInt)
{
    // To long long int passing
    
    // To long long int failing

    // From long long int passing

    // From lon glong int failing

}

TEST(Conversion, LongInt)
{
    // To long int passing

    // To long int failing

    // From long int passing

    // From long int failing

}

TEST(Conversion, Int)
{
    // To int passing

    // To int failing

    // From int passing

    // From int failing

}

TEST(Conversion, Char)
{
    // To char passing

    // To char failing

    // From char passing

    // From char failing

}

TEST(Conversion, Bool)
{
    // To bool passing

    // From bool passing

    // From bool failing

}
*/
