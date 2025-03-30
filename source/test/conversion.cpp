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

// Technically I don't need to do more than check that the functions work with basic data, and on the smaller data types that the values aren't too big, because I will be calling
// std::sto[iuld] or std::to_string
// but I already wrote the tests, so I'm leaving them here. Also, if I write my own implementation of the functions I'll need them

TEST(Conversion, LongDouble)
{
    long double epsilon = std::numeric_limits<long double>::epsilon();
    // To long double passing
    EXPECT_NEAR(conversion::to_ldouble("0"), 0L, epsilon) << "Failed to convert 0 from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("-0"), 0L, epsilon) << "Failed to convert -0 from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("12345"), 12345L, epsilon) << "Failed to convert positive integer from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("-9876"), -9876L, epsilon) << "Failed to convert negative integer from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble(".341"), .341L, epsilon) << "Failed to convert positive decimal < 1 from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("-.9476"), -.9476L, epsilon) << "Failed to convert negative decimal > -1 from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("3.14159"), 3.14159L, epsilon) << "Failed to convert positive decimal > 1 from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("-54.3543"), -54.3543L, epsilon) << "Failed to convert negative decimal < -1 from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("123e89"), 123e89L, epsilon) << "Failed to convert positive integer and positive exponent from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("652e-12"), 652e-12L, epsilon) << "Failed to convert positive integer and negative exponent from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("-54e763"), -54e763L, epsilon) << "Failed to convert negative integer and positive exponent from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("-890e-5"), -890e-5L, epsilon) << "Failed to convert negative integer and negative exponent from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("6.0221408e+23"), 6.0221408e+23L, epsilon) << "Failed to convert positive decimal and positive exponent from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("9.1093837e-31"), 9.1093837e-31L, epsilon) << "Failed to convert positive decimal and negative exponent from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("-147.142e43"), -147.142e43L, epsilon) << "Failed to convert negative decimal and positive exponent from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble("-432.42e-5"), -432.42e-5L, epsilon) << "Failed to convert negative decimal and negative exponent from internal representation to long double";
    
    long double max = std::numeric_limits<long double>::max();
    std::string max_str = std::to_string(max);
    long double min = std::numeric_limits<long double>::lowest();
    std::string min_str = std::to_string(min);
    EXPECT_NEAR(conversion::to_ldouble(max_str), max, epsilon) << "Failed to convert max long double from internal representation to long double";
    EXPECT_NEAR(conversion::to_ldouble(min_str), min, epsilon) << "Failed to convert min long double from internal representation to long double";
    
    // From long double passing
    EXPECT_EQ(conversion::to_data(0L), "0") << "Failed to convert 0 from long double to internal representation";
    EXPECT_EQ(conversion::to_data(0L), "-0") << "Failed to convert -0 from long double to internal representation";
    EXPECT_EQ(conversion::to_data(12345L), "12345") << "Failed to convert positive integer from long double to internal representation";
    EXPECT_EQ(conversion::to_data(-9876L), "-9876") << "Failed to convert negative integer from long double to internal representation";
    EXPECT_EQ(conversion::to_data(.341L), ".341") << "Failed to convert positive decimal < 1 from long double to internal representation";
    EXPECT_EQ(conversion::to_data(-.9476L), "-.9476") << "Failed to convert negative decimal > -1 from long double to internal representation";
    EXPECT_EQ(conversion::to_data(3.14159L), "3.14159") << "Failed to convert positive decimal > 1 from long double to internal representation";
    EXPECT_EQ(conversion::to_data(-54.3543L), "-54.3543") << "Failed to convert negative decimal < -1 from long double to internal representation";
    EXPECT_EQ(conversion::to_data(123e89L), "123e89") << "Failed to convert positive integer and positive exponent from long double to internal representation";
    EXPECT_EQ(conversion::to_data(652e-12L), "652e-12") << "Failed to convert positive integer and negative exponent from long double to internal representation";
    EXPECT_EQ(conversion::to_data(-54e763L), "-54e763") << "Failed to convert negative integer and positive exponent from long double to internal representation";
    EXPECT_EQ(conversion::to_data(-890e-5L), "-890e-5") << "Failed to convert negative integer and negative exponent from long double to internal representation";
    EXPECT_EQ(conversion::to_data(6.0221408e+23L), "6.0221408e+23") << "Failed to convert positive decimal and positive exponent from long double to internal representation";
    EXPECT_EQ(conversion::to_data(9.1093837e-31L), "9.1093837e-31") << "Failed to convert positive decimal and negative exponent from long double to internal representation";
    EXPECT_EQ(conversion::to_data(-147.142e43L), "-147.142e43") << "Failed to convert negative decimal and positive exponent from long double to internal representation";
    EXPECT_EQ(conversion::to_data(-432.42e-5L), "-432.42e-5") << "Failed to convert negative decimal and negative exponent from long double to internal representation";
    
    EXPECT_EQ(conversion::to_data(max), max_str) << "Failed to convert max long double from long double to internal representation";
    EXPECT_EQ(conversion::to_data(min), min_str) << "Failed to convert min long double from long double to internal representation";
}

TEST(Conversion, Double)
{
    double epsilon = std::numeric_limits<double>::epsilon();
    // To double passing
    EXPECT_NEAR(conversion::to_double("0"), 0L, epsilon) << "Failed to convert 0 from internal representation to double";
    EXPECT_NEAR(conversion::to_double("-0"), 0L, epsilon) << "Failed to convert -0 from internal representation to double";
    EXPECT_NEAR(conversion::to_double("12345"), 12345L, epsilon) << "Failed to convert positive integer from internal representation to double";
    EXPECT_NEAR(conversion::to_double("-9876"), -9876L, epsilon) << "Failed to convert negative integer from internal representation to double";
    EXPECT_NEAR(conversion::to_double(".341"), .341L, epsilon) << "Failed to convert positive decimal < 1 from internal representation to double";
    EXPECT_NEAR(conversion::to_double("-.9476"), -.9476L, epsilon) << "Failed to convert negative decimal > -1 from internal representation to double";
    EXPECT_NEAR(conversion::to_double("3.14159"), 3.14159L, epsilon) << "Failed to convert positive decimal > 1 from internal representation to double";
    EXPECT_NEAR(conversion::to_double("-54.3543"), -54.3543L, epsilon) << "Failed to convert negative decimal < -1 from internal representation to double";
    EXPECT_NEAR(conversion::to_double("123e89"), 123e89L, epsilon) << "Failed to convert positive integer and positive exponent from internal representation to double";
    EXPECT_NEAR(conversion::to_double("652e-12"), 652e-12L, epsilon) << "Failed to convert positive integer and negative exponent from internal representation to double";
    EXPECT_NEAR(conversion::to_double("-54e763"), -54e763L, epsilon) << "Failed to convert negative integer and positive exponent from internal representation to double";
    EXPECT_NEAR(conversion::to_double("-890e-5"), -890e-5L, epsilon) << "Failed to convert negative integer and negative exponent from internal representation to double";
    EXPECT_NEAR(conversion::to_double("6.0221408e+23"), 6.0221408e+23L, epsilon) << "Failed to convert positive decimal and positive exponent from internal representation to double";
    EXPECT_NEAR(conversion::to_double("9.1093837e-31"), 9.1093837e-31L, epsilon) << "Failed to convert positive decimal and negative exponent from internal representation to double";
    EXPECT_NEAR(conversion::to_double("-147.142e43"), -147.142e43L, epsilon) << "Failed to convert negative decimal and positive exponent from internal representation to double";
    EXPECT_NEAR(conversion::to_double("-432.42e-5"), -432.42e-5L, epsilon) << "Failed to convert negative decimal and negative exponent from internal representation to double";
    
    double max = std::numeric_limits<double>::max();
    std::string max_str = std::to_string(max);
    double min = std::numeric_limits<double>::lowest();
    std::string min_str = std::to_string(min);
    EXPECT_NEAR(conversion::to_double(max_str), max, epsilon) << "Failed to convert max double from internal representation to double";
    EXPECT_NEAR(conversion::to_double(min_str), min, epsilon) << "Failed to convert min double from internal representation to double";

    // To double failing
    EXPECT_THROW(conversion::to_double(std::to_string(max + 1000)), json::out_of_range) << "Failed to throw json::out_of_range error for value greater than possible to store in double";
    EXPECT_THROW(conversion::to_double(std::to_string(min - 1000)), json::out_of_range) << "Failed to throw json::out_of_range error for negative value of greater magnitude than possible to store in double";

    // From double passing
    EXPECT_EQ(conversion::to_data(0L), "0") << "Failed to convert 0 from double to internal representation";
    EXPECT_EQ(conversion::to_data(0L), "-0") << "Failed to convert -0 from double to internal representation";
    EXPECT_EQ(conversion::to_data(12345L), "12345") << "Failed to convert positive integer from double to internal representation";
    EXPECT_EQ(conversion::to_data(-9876L), "-9876") << "Failed to convert negative integer from double to internal representation";
    EXPECT_EQ(conversion::to_data(.341L), ".341") << "Failed to convert positive decimal < 1 from double to internal representation";
    EXPECT_EQ(conversion::to_data(-.9476L), "-.9476") << "Failed to convert negative decimal > -1 from double to internal representation";
    EXPECT_EQ(conversion::to_data(3.14159L), "3.14159") << "Failed to convert positive decimal > 1 from double to internal representation";
    EXPECT_EQ(conversion::to_data(-54.3543L), "-54.3543") << "Failed to convert negative decimal < -1 from double to internal representation";
    EXPECT_EQ(conversion::to_data(123e89L), "123e89") << "Failed to convert positive integer and positive exponent from double to internal representation";
    EXPECT_EQ(conversion::to_data(652e-12L), "652e-12") << "Failed to convert positive integer and negative exponent from double to internal representation";
    EXPECT_EQ(conversion::to_data(-54e763L), "-54e763") << "Failed to convert negative integer and positive exponent from double to internal representation";
    EXPECT_EQ(conversion::to_data(-890e-5L), "-890e-5") << "Failed to convert negative integer and negative exponent from double to internal representation";
    EXPECT_EQ(conversion::to_data(6.0221408e+23L), "6.0221408e+23") << "Failed to convert positive decimal and positive exponent from double to internal representation";
    EXPECT_EQ(conversion::to_data(9.1093837e-31L), "9.1093837e-31") << "Failed to convert positive decimal and negative exponent from double to internal representation";
    EXPECT_EQ(conversion::to_data(-147.142e43L), "-147.142e43") << "Failed to convert negative decimal and positive exponent from double to internal representation";
    EXPECT_EQ(conversion::to_data(-432.42e-5L), "-432.42e-5") << "Failed to convert negative decimal and negative exponent from double to internal representation";
    
    EXPECT_EQ(conversion::to_data(max), max_str) << "Failed to convert max double from double to internal representation";
    EXPECT_EQ(conversion::to_data(min), min_str) << "Failed to convert min double from double to internal representation";

    // From double can't call conversion::to_data(double) with long double arguement, will call long double version. Error will occur at assignment
} 

TEST(Conversion, Float)
{
    float epsilon = std::numeric_limits<float>::epsilon();
    // To float passing
    EXPECT_NEAR(conversion::to_float("0"), 0L, epsilon) << "Failed to convert 0 from internal representation to float";
    EXPECT_NEAR(conversion::to_float("-0"), 0L, epsilon) << "Failed to convert -0 from internal representation to float";
    EXPECT_NEAR(conversion::to_float("12345"), 12345L, epsilon) << "Failed to convert positive integer from internal representation to float";
    EXPECT_NEAR(conversion::to_float("-9876"), -9876L, epsilon) << "Failed to convert negative integer from internal representation to float";
    EXPECT_NEAR(conversion::to_float(".341"), .341L, epsilon) << "Failed to convert positive decimal < 1 from internal representation to float";
    EXPECT_NEAR(conversion::to_float("-.9476"), -.9476L, epsilon) << "Failed to convert negative decimal > -1 from internal representation to float";
    EXPECT_NEAR(conversion::to_float("3.14159"), 3.14159L, epsilon) << "Failed to convert positive decimal > 1 from internal representation to float";
    EXPECT_NEAR(conversion::to_float("-54.3543"), -54.3543L, epsilon) << "Failed to convert negative decimal < -1 from internal representation to float";
    EXPECT_NEAR(conversion::to_float("123e89"), 123e89L, epsilon) << "Failed to convert positive integer and positive exponent from internal representation to float";
    EXPECT_NEAR(conversion::to_float("652e-12"), 652e-12L, epsilon) << "Failed to convert positive integer and negative exponent from internal representation to float";
    EXPECT_NEAR(conversion::to_float("-54e763"), -54e763L, epsilon) << "Failed to convert negative integer and positive exponent from internal representation to float";
    EXPECT_NEAR(conversion::to_float("-890e-5"), -890e-5L, epsilon) << "Failed to convert negative integer and negative exponent from internal representation to float";
    EXPECT_NEAR(conversion::to_float("6.0221408e+23"), 6.0221408e+23L, epsilon) << "Failed to convert positive decimal and positive exponent from internal representation to float";
    EXPECT_NEAR(conversion::to_float("9.1093837e-31"), 9.1093837e-31L, epsilon) << "Failed to convert positive decimal and negative exponent from internal representation to float";
    EXPECT_NEAR(conversion::to_float("-147.142e43"), -147.142e43L, epsilon) << "Failed to convert negative decimal and positive exponent from internal representation to float";
    EXPECT_NEAR(conversion::to_float("-432.42e-5"), -432.42e-5L, epsilon) << "Failed to convert negative decimal and negative exponent from internal representation to float";
    
    float max = std::numeric_limits<float>::max();
    std::string max_str = std::to_string(max);
    float min = std::numeric_limits<float>::lowest();
    std::string min_str = std::to_string(min);
    EXPECT_NEAR(conversion::to_float(max_str), max, epsilon) << "Failed to convert max float from internal representation to float";
    EXPECT_NEAR(conversion::to_float(min_str), min, epsilon) << "Failed to convert min float from internal representation to float";
    
    // To float failing
    EXPECT_THROW(conversion::to_float(std::to_string(max + 1000)), json::out_of_range) << "Failed to throw json::out_of_range error for value greater than possible to store in float";
    EXPECT_THROW(conversion::to_float(std::to_string(min - 1000)), json::out_of_range) << "Failed to throw json::out_of_range error for negative value of greater magnitude than possible to store in float";

    // From float passing
    EXPECT_EQ(conversion::to_data(0L), "0") << "Failed to convert 0 from float to internal representation";
    EXPECT_EQ(conversion::to_data(0L), "-0") << "Failed to convert -0 from float to internal representation";
    EXPECT_EQ(conversion::to_data(12345L), "12345") << "Failed to convert positive integer from float to internal representation";
    EXPECT_EQ(conversion::to_data(-9876L), "-9876") << "Failed to convert negative integer from float to internal representation";
    EXPECT_EQ(conversion::to_data(.341L), ".341") << "Failed to convert positive decimal < 1 from float to internal representation";
    EXPECT_EQ(conversion::to_data(-.9476L), "-.9476") << "Failed to convert negative decimal > -1 from float to internal representation";
    EXPECT_EQ(conversion::to_data(3.14159L), "3.14159") << "Failed to convert positive decimal > 1 from float to internal representation";
    EXPECT_EQ(conversion::to_data(-54.3543L), "-54.3543") << "Failed to convert negative decimal < -1 from float to internal representation";
    EXPECT_EQ(conversion::to_data(123e89L), "123e89") << "Failed to convert positive integer and positive exponent from float to internal representation";
    EXPECT_EQ(conversion::to_data(652e-12L), "652e-12") << "Failed to convert positive integer and negative exponent from float to internal representation";
    EXPECT_EQ(conversion::to_data(-54e763L), "-54e763") << "Failed to convert negative integer and positive exponent from float to internal representation";
    EXPECT_EQ(conversion::to_data(-890e-5L), "-890e-5") << "Failed to convert negative integer and negative exponent from float to internal representation";
    EXPECT_EQ(conversion::to_data(6.0221408e+23L), "6.0221408e+23") << "Failed to convert positive decimal and positive exponent from float to internal representation";
    EXPECT_EQ(conversion::to_data(9.1093837e-31L), "9.1093837e-31") << "Failed to convert positive decimal and negative exponent from float to internal representation";
    EXPECT_EQ(conversion::to_data(-147.142e43L), "-147.142e43") << "Failed to convert negative decimal and positive exponent from float to internal representation";
    EXPECT_EQ(conversion::to_data(-432.42e-5L), "-432.42e-5") << "Failed to convert negative decimal and negative exponent from float to internal representation";
    
    EXPECT_EQ(conversion::to_data(max), max_str) << "Failed to convert max float from float to internal representation";
    EXPECT_EQ(conversion::to_data(min), min_str) << "Failed to convert min float from float to internal representation";

    // From float can't call conversion::to_data(float) with double arguement, will call double version. Error will occur at assignment

}

/*
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
