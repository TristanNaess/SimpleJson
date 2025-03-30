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

TEST(Conversion, LongLongUnsigned)
{
    // To long long unsigned int passing
    EXPECT_EQ(conversion::to_ullint("17378905"), 17378905ULL) << "Failed to convert unsigned long long int to internal representation";
    
    unsigned long long max = std::numeric_limits<unsigned long long>::max();
    std::string max_string = std::to_string(max);
    EXPECT_EQ(conversion::to_ullint(max_string), max) << "Failed to convert max unsigned long long from internal representation to ullint";

    // From long long unsigned int passing
    EXPECT_EQ(conversion::to_data(7894132ULL), "7894132") << "Failed to convert internal representation to unsigned long long int";
    // not checking min and max because I'm just calling std::to_string, which I use to make the test
}

TEST(Conversion, LongUnsigned)
{
    // To long unsigned int passing
    EXPECT_EQ(conversion::to_ulint("17378905"), 17378905UL) << "Failed to convert unsigned long int to internal representation";
    
    unsigned long max = std::numeric_limits<unsigned long>::max();
    std::string max_string = std::to_string(max);
    EXPECT_EQ(conversion::to_ulint(max_string), max) << "Failed to convert max unsigned long from internal representation to ulint";
    EXPECT_EQ(conversion::to_ulint("0"), 0) << "Failed to convert min unsigned long from internal representation to ulint";

    // From long unsigned int passing
    EXPECT_EQ(conversion::to_data(7894132UL), "7894132") << "Failed to convert internal representation to unsigned long int";
}

TEST(Conversion, Unsigned)
{
    // To unsigned int passing
    EXPECT_EQ(conversion::to_uint("17378905"), 17378905U) << "Failed to convert unsigned  int to internal representation";
    
    unsigned max = std::numeric_limits<unsigned>::max();
    std::string max_string = std::to_string(max);
    EXPECT_EQ(conversion::to_uint(max_string), max) << "Failed to convert max unsigned  from internal representation to uint";
    EXPECT_EQ(conversion::to_uint("0"), 0) << "Failed to convert min unsigned  from internal representation to uint";

    // From  unsigned int passing
    EXPECT_EQ(conversion::to_data(7894132U), "7894132") << "Failed to convert internal representation to unsigned int";
}

TEST(Conversion, UnsignedChar)
{
    // To long unsigned int passing
    EXPECT_EQ(conversion::to_uchar("123"), 123U) << "Failed to convert unsigned long int to internal representation";
    
    unsigned char max = std::numeric_limits<unsigned char>::max();
    std::string max_string = std::to_string(max);
    EXPECT_EQ(conversion::to_uchar(max_string), max) << "Failed to convert max unsigned long from internal representation to ulint";
    EXPECT_EQ(conversion::to_uchar("0"), 0) << "Failed to convert min unsigned long from internal representation to ulint";

    // From long unsigned int passing
    EXPECT_EQ(conversion::to_data(123U), "123") << "Failed to convert internal representation to unsigned long int";
}

TEST(Conversion, LongLongInt)
{
    // To long long int passing
    EXPECT_EQ(conversion::to_ullint("-17378905"), -17378905LL) << "Failed to convert  long long int to internal representation";
    
    long long max = std::numeric_limits<long long>::max();
    std::string max_string = std::to_string(max);
    long long min = std::numeric_limits<long long>::min();
    std::string min_string = std::to_string(min);
    EXPECT_EQ(conversion::to_llint(max_string), max) << "Failed to convert max  long long from internal representation to llint";
    EXPECT_EQ(conversion::to_llint(min_string), min) << "Failed to convert min  long long from internal representation to llint";

    // From long long int passing
    EXPECT_EQ(conversion::to_data(-7894132LL), "-7894132") << "Failed to convert internal representation to long long int";
    // not checking min and max because I'm just calling std::to_string, which I use to make the test
}

TEST(Conversion, LongInt)
{
    // To long int passing
    EXPECT_EQ(conversion::to_lint("-17378905"), -17378905L) << "Failed to convert  long int to internal representation";
    
    long max = std::numeric_limits<long>::max();
    std::string max_string = std::to_string(max);
    long min = std::numeric_limits<long>::min();
    std::string min_string = std::to_string(min);
    EXPECT_EQ(conversion::to_lint(max_string), max) << "Failed to convert max  long from internal representation to lint";
    EXPECT_EQ(conversion::to_lint(min_string), min) << "Failed to convert min  long from internal representation to lint";

    // From long int passing
    EXPECT_EQ(conversion::to_data(-7894132L), "-7894132") << "Failed to convert internal representation to long int";
    // not checking min and max because I'm just calling std::to_string, which I use to make the test
}

TEST(Conversion, Int)
{
    // To int passing
    EXPECT_EQ(conversion::to_int("-17378905"), -17378905) << "Failed to convert  int to internal representation";
    
    int max = std::numeric_limits<int>::max();
    std::string max_string = std::to_string(max);
    int min = std::numeric_limits<int>::min();
    std::string min_string = std::to_string(min);
    EXPECT_EQ(conversion::to_int(max_string), max) << "Failed to convert max  from internal representation to int";
    EXPECT_EQ(conversion::to_int(min_string), min) << "Failed to convert min  from internal representation to int";

    // From int passing
    EXPECT_EQ(conversion::to_data(-7894132), "-7894132") << "Failed to convert internal representation to int";
    // not checking min and max because I'm just calling std::to_string, which I use to make the test
}

TEST(Conversion, Char)
{
    // To int passing
    EXPECT_EQ(conversion::to_char("-41"), -41) << "Failed to convert  int to internal representation";
    
    char max = std::numeric_limits<char>::max();
    std::string max_string = std::to_string(max);
    char min = std::numeric_limits<char>::min();
    std::string min_string = std::to_string(min);
    EXPECT_EQ(conversion::to_char(max_string), max) << "Failed to convert max  from internal representation to char";
    EXPECT_EQ(conversion::to_char(min_string), min) << "Failed to convert min  from internal representation to char";

    // From int passing
    EXPECT_EQ(conversion::to_data(-78), "-78") << "Failed to convert internal representation to char";
    // not checking min and max because I'm just calling std::to_string, which I use to make the test
}

TEST(Conversion, Bool)
{
    EXPECT_EQ(conversion::to_bool("true"), true) << "Failed to convert boolean true from internal representation to bool";
    EXPECT_EQ(conversion::to_bool("false"), false) << "Failed to convert boolean false from internal representation to bool";

    EXPECT_EQ(conversion::to_data(true), "true") << "Failed to convert true from bool to internal representation";
    EXPECT_EQ(conversion::to_data(false), "false") << "Failed to convert false from bool to internal representation";
}
