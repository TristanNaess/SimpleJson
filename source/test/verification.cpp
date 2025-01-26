#include <gtest/gtest.h>
#include "verification.hpp"

TEST(Verification, VerifyObject)
{
    EXPECT_TRUE(verify_object("{}")) << "Failed to recognize an empty object";
    EXPECT_TRUE(verify_object(R"({"Key 1": "Value 1", "Key 2": "Value 2"})")) << "Failed to recognize a basic object";
    EXPECT_TRUE(verify_object(R"({"Key 1": 3.14159, "Key 2": {"Key 2.1": "Value 2.1", "Key 2.2": true}, "Key 3": null})")) << "Failed to recognize an object containing another object";
    EXPECT_TRUE(verify_object(R"({"Key 1": 3.14159, "Key 2": ["Val 2.1", 5.5423e-23, false], "Key 3": null})")) << "Failed to recognize an object containing an array";
    EXPECT_TRUE(verify_object(R"({"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{}}}}}}}})")) << "Failed to recognize deeply nested objects";
    EXPECT_TRUE(verify_object("{ \n\r\t\"Key 1\" \n\r\t: \n\r\t\"Val 1\" \n\r\t, \n\r\t, \n\r\t\"Key 2\" \n\r\t: \n\r\t12345 \n\r\t}")) << "Failed to recognize object containing all whitespace characters";

    EXPECT_FALSE(verify_object(R"("Key 1": "Val 1", "Key 2": 12345, "Key 3": true})")) << "Accepted object with missing initial bracket";
    EXPECT_FALSE(verify_object(R"({"Key 1": "Val 1", "Key 2": 12345, "Key 3": true)")) << "Accepted object with missing final bracket";
    EXPECT_FALSE(verify_object(R"({"Key 1": 12345 "Key 2": "Val 2", "Key 3": 78})")) << "Accepted object with missing comma between fields";
    EXPECT_FALSE(verify_object(R"({"Key 1": "Value 1", "Key 2" 12345, "Key 3": false})")) << "Accepted object with missing colon between key and value";
    EXPECT_FALSE(verify_object(R"({"Key 1": "Value 1", 12345, "Key 3": true})")) << "Accepted object with missing key";
    EXPECT_FALSE(verify_object(R"({"Key 1": "Value 1", "Key 1": , "Key 3": false})")) << "Accepted object with missing value";
}

TEST(Verification, VerifyArray)
{
    EXPECT_TRUE(verify_array("[]")) << "Failed to recognize an empty array";
    EXPECT_TRUE(verify_array("[1, 2, 3, 4, 5]")) << "Failed to recognize a basic array of numbers";
    EXPECT_TRUE(verify_array("[\"String\", [\"fds\", \"dsafasg\"], true, null, 6.022e23]")) << "Failed to recognize an array of elements including another array";
    EXPECT_TRUE(verify_array("[1, 2, 3, {\"Key 1\": \"Value 1\", \"Key 2\": 12.345}, 5, 6, 7, 8]")) << "Failed to recognize an array containing an object";
    EXPECT_TRUE(verify_array("[[[[[[]]]]]]")) << "Failed to recognize deeply nested arrays";
    EXPECT_TRUE(verify_array("[ \n\r\t12345 \n\r\t, \n\r\t\"String\" \n\r\t, \n\r\tfalse \n\r\t]")) << "Failed to recognize array containing all whitespace characters";

    EXPECT_FALSE(verify_array("1, 2, 3, 4, 5]")) << "Accepted array missing initial brace";
    EXPECT_FALSE(verify_array("[1, 2, 3, 4, 5")) << "Accepted array missing final brace";
    EXPECT_FALSE(verify_array("[1, 2, 3, , 4, 5]")) << "Accepted array missing field";
    EXPECT_FALSE(verify_array("[1, 2, \"Hello\" 4, 5]")) << "Accepted array missing comma";
}

TEST(Verification, VerifyString)
{
    EXPECT_TRUE(verify_string("\"This is a basic string\"")) <<  "Failed to recognize a basic string";
    EXPECT_TRUE(verify_string(R"("\"This is a quote surrounded string\"")")) <<  "Failed to recognize a string surrounded by quote characters";
    EXPECT_TRUE(verify_string(R"("This string \" contains \" quotes internal\" to the string")")) <<  "Failed to recognize a string containing quote characters";
    EXPECT_TRUE(verify_string(R"("This \\string c\/ont\u00c6ains  \bthe \fother\n escap\re cha\tract\u00C6ers")")) <<  "Failed to recognize a string containing escape characters";

    EXPECT_FALSE(verify_string("No initial quote\"")) << "Accepted a string with no initial quote";
    EXPECT_FALSE(verify_string("\"No final quote")) << "Accepted a string with no final quote";
    EXPECT_FALSE(verify_string(R"("This string has a bad hex\uAG29 escape sequence")")) << "Accepted a string with a bad escape sequence (\\uAG29)";
    EXPECT_FALSE(verify_string(R"("This string contains a \ncodepoint < 0x0020")")) << "Accepted a string with a codepoint < 0x0020";
}

TEST(Verification, VerifyNumber)
{
    // I could make this more exhaustive, but this is probably good
    EXPECT_TRUE(verify_number("0")) << "Failed to recognize zero as number";
    EXPECT_TRUE(verify_number("-0")) << "Failed to recognize negative zero as number";
    EXPECT_TRUE(verify_number("12345")) << "Failed to recognize positive integer as number";
    EXPECT_TRUE(verify_number("-442297")) << "Failed to recognize negative integer as number";
    EXPECT_TRUE(verify_number("0.453")) << "Failed to recognize decimal < 1 as number";
    EXPECT_TRUE(verify_number("1763.9988")) << "Failed to recognize decimal > 1 as number";
    EXPECT_TRUE(verify_number("0e32")) << "Failed to recognize 0e form exponent as number";
    EXPECT_TRUE(verify_number("32e+462")) << "Failed to recognize positive [e]xponent without decimal as number";
    EXPECT_TRUE(verify_number("32E+462")) << "Failed to recognize positive [E]xponent without decimal as number";
    EXPECT_TRUE(verify_number("32e462")) << "Failed to recognize unmarked positive exponent without decimal as number";
    EXPECT_TRUE(verify_number("32e-462")) << "Failed to recognize negative exponent without decimal as number";
    EXPECT_TRUE(verify_number("789.532e23")) << "Failed to recognize exponent with decimal mantissa";

    // I can't make these exhaustive, I think this covers all edge cases?
    //EXPECT_FALSE(verify_number("")) << "Accepted leading 0(s)"; I'm going to allow this
    EXPECT_FALSE(verify_number("f324")) << "Accepted non-(digit or -) as first character";
    EXPECT_FALSE(verify_number("218v3456")) << "Accepted non-(digit or .) after first digit";
    EXPECT_FALSE(verify_number("32.l467")) << "Accepted non-digit after .";
    EXPECT_FALSE(verify_number("32.678d545")) << "Accepted non-digit in decimal";
    EXPECT_FALSE(verify_number(".12345")) << "Accepted decimal without leading 0";
    EXPECT_FALSE(verify_number("e67")) << "Accepted exponent without leading 0";
    EXPECT_FALSE(verify_number("123.54ej78")) << "Accepted non-(sign or digit) after [eE]";
    EXPECT_FALSE(verify_number("123.45e")) << "Accepted exponent without following digits";
}

TEST(Verification, VerifyBool)
{
    EXPECT_TRUE(verify_bool("true")) << "Failed to verify string 'true' as bool";
    EXPECT_TRUE(verify_bool("false")) << "Failed to verify string 'false' as bool";
    EXPECT_FALSE(verify_bool("null")) << "Accepted 'null' as bool value";
}

TEST(Verification, VerifyNull)
{
    EXPECT_TRUE(verify_null("null")) << "Failed to verify string 'null' as null";
    EXPECT_FALSE(verify_null("other")) << "Accepted 'other' as null";
}

// All is_*() will be passed verified data, so checks are to make sure only the correct type is identified

TEST(TypeChecking, IsObject)
{
    // Objects
    EXPECT_TRUE(is_object("{}")) << "Failed to recognize an empty object";
    EXPECT_TRUE(is_object(R"({"Key 1": "Value 1", "Key 2": "Value 2"})")) << "Failed to recognize a basic object";
    EXPECT_TRUE(is_object(R"({"Key 1": 3.14159, "Key 2": {"Key 2.1": "Value 2.1", "Key 2.2": true}, "Key 3": null})")) << "Failed to recognize an object containing another object";
    EXPECT_TRUE(is_object(R"({"Key 1": 3.14159, "Key 2": ["Val 2.1", 5.5423e-23, false], "Key 3": null})")) << "Failed to recognize an object containing an array";
    EXPECT_TRUE(is_object(R"({"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{}}}}}}}})")) << "Failed to recognize deeply nested objects";
    EXPECT_TRUE(is_object("{ \n\r\t\"Key 1\" \n\r\t: \n\r\t\"Val 1\" \n\r\t, \n\r\t, \n\r\t\"Key 2\" \n\r\t: \n\r\t12345 \n\r\t}")) << "Failed to recognize object containing all whitespace characters";

    // Arrays
    EXPECT_FALSE(is_object("[]")) << "Accepted array as object";
    EXPECT_FALSE(is_object("[1, 2, 3, 4, 5]")) << "Accepted array as object";
    EXPECT_FALSE(is_object("[\"String\", [\"fds\", \"dsafasg\"], true, null, 6.022e23]")) << "Accepted array as object";
    EXPECT_FALSE(is_object("[1, 2, 3, {\"Key 1\": \"Value 1\", \"Key 2\": 12.345}, 5, 6, 7, 8]")) << "Accepted array as object";
    EXPECT_FALSE(is_object("[[[[[[]]]]]]")) << "Accepted array as object";
    EXPECT_FALSE(is_object("[ \n\r\t12345 \n\r\t, \n\r\t\"String\" \n\r\t, \n\r\tfalse \n\r\t]")) << "Accepted array as object";


    // Strings
    EXPECT_FALSE(is_object("\"This is a basic string\"")) <<  "Accepted string as object";
    EXPECT_FALSE(is_object(R"("\"This is a quote surrounded string\"")")) <<  "Accepted string as object";
    EXPECT_FALSE(is_object(R"("This string \" contains \" quotes internal\" to the string")")) <<  "Accepted string as object";
    EXPECT_FALSE(is_object(R"("This \\string c\/ont\u00c6ains  \bthe \fother\n escap\re cha\tract\u00C6ers")")) <<  "Accepted string as object";

    // Numbers
    EXPECT_FALSE(is_object("0")) << "Accepted number as object";
    EXPECT_FALSE(is_object("-0")) << "Accepted number as object";
    EXPECT_FALSE(is_object("12345")) << "Accepted number as object";
    EXPECT_FALSE(is_object("-442297")) << "Accepted number as object";
    EXPECT_FALSE(is_object("0.453")) << "Accepted number as object";
    EXPECT_FALSE(is_object("1763.9988")) << "Accepted number as object";
    EXPECT_FALSE(is_object("0e32")) << "Accepted number as object";
    EXPECT_FALSE(is_object("32e+462")) << "Accepted number as object";
    EXPECT_FALSE(is_object("32E+462")) << "Accepted number as object";
    EXPECT_FALSE(is_object("32e462")) << "Accepted number as object";
    EXPECT_FALSE(is_object("32e-462")) << "Accepted number as object";
    EXPECT_FALSE(is_object("789.532e23")) << "Accepted number as object";

    // Bools
    EXPECT_FALSE(is_object("true")) << "Accepted bool as object";
    EXPECT_FALSE(is_object("false")) << "Accepted bool as object";

    // Null
    EXPECT_FALSE(is_object("null")) << "Accepted null as object";

}

TEST(TypeChecking, IsArray)
{
    // Objects
    EXPECT_FALSE(is_array("{}")) << "Accepted object as array";
    EXPECT_FALSE(is_array(R"({"Key 1": "Value 1", "Key 2": "Value 2"})")) << "Accepted object as array";
    EXPECT_FALSE(is_array(R"({"Key 1": 3.14159, "Key 2": {"Key 2.1": "Value 2.1", "Key 2.2": true}, "Key 3": null})")) << "Accepted object as array";
    EXPECT_FALSE(is_array(R"({"Key 1": 3.14159, "Key 2": ["Val 2.1", 5.5423e-23, false], "Key 3": null})")) << "Accepted object as array";
    EXPECT_FALSE(is_array(R"({"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{}}}}}}}})")) << "Accepted object as array";
    EXPECT_FALSE(is_array("{ \n\r\t\"Key 1\" \n\r\t: \n\r\t\"Val 1\" \n\r\t, \n\r\t, \n\r\t\"Key 2\" \n\r\t: \n\r\t12345 \n\r\t}")) << "Accepted object as array";

    // Arrays
    EXPECT_TRUE(is_array("[]")) << "Failed to recognize an empty array";
    EXPECT_TRUE(is_array("[1, 2, 3, 4, 5]")) << "Failed to recognize a basic array of numbers";
    EXPECT_TRUE(is_array("[\"String\", [\"fds\", \"dsafasg\"], true, null, 6.022e23]")) << "Failed to recognize an array of elements including another array";
    EXPECT_TRUE(is_array("[1, 2, 3, {\"Key 1\": \"Value 1\", \"Key 2\": 12.345}, 5, 6, 7, 8]")) << "Failed to recognize an array containing an object";
    EXPECT_TRUE(is_array("[[[[[[]]]]]]")) << "Failed to recognize deeply nested arrays";
    EXPECT_TRUE(is_array("[ \n\r\t12345 \n\r\t, \n\r\t\"String\" \n\r\t, \n\r\tfalse \n\r\t]")) << "Failed to recognize array containing all whitespace characters";

    // Strings
    EXPECT_FALSE(is_array("\"This is a basic string\"")) <<  "Accepted string as array";
    EXPECT_FALSE(is_array(R"("\"This is a quote surrounded string\"")")) <<  "Accepted string as array";
    EXPECT_FALSE(is_array(R"("This string \" contains \" quotes internal\" to the string")")) <<  "Accepted string as array";
    EXPECT_FALSE(is_array(R"("This \\string c\/ont\u00c6ains  \bthe \fother\n escap\re cha\tract\u00C6ers")")) <<  "Accepted string as array";

    // Numbers
    EXPECT_FALSE(is_array("0")) << "Accepted number as array";
    EXPECT_FALSE(is_array("-0")) << "Accepted number as array";
    EXPECT_FALSE(is_array("12345")) << "Accepted number as array";
    EXPECT_FALSE(is_array("-442297")) << "Accepted number as array";
    EXPECT_FALSE(is_array("0.453")) << "Accepted number as array";
    EXPECT_FALSE(is_array("1763.9988")) << "Accepted number as array";
    EXPECT_FALSE(is_array("0e32")) << "Accepted number as array";
    EXPECT_FALSE(is_array("32e+462")) << "Accepted number as array";
    EXPECT_FALSE(is_array("32E+462")) << "Accepted number as array";
    EXPECT_FALSE(is_array("32e462")) << "Accepted number as array";
    EXPECT_FALSE(is_array("32e-462")) << "Accepted number as array";
    EXPECT_FALSE(is_array("789.532e23")) << "Accepted number as array";

    // Bools
    EXPECT_FALSE(is_array("true")) << "Accepted bool as array";
    EXPECT_FALSE(is_array("false")) << "Accepted bool as array";

    // Null
    EXPECT_FALSE(is_array("null")) << "Accepted null as array";
}

TEST(TypeChecking, IsString)
{
    // Objects
    EXPECT_FALSE(is_string("{}")) << "Accepted object as string";
    EXPECT_FALSE(is_string(R"({"Key 1": "Value 1", "Key 2": "Value 2"})")) << "Accepted object as string";
    EXPECT_FALSE(is_string(R"({"Key 1": 3.14159, "Key 2": {"Key 2.1": "Value 2.1", "Key 2.2": true}, "Key 3": null})")) << "Accepted object as string";
    EXPECT_FALSE(is_string(R"({"Key 1": 3.14159, "Key 2": ["Val 2.1", 5.5423e-23, false], "Key 3": null})")) << "Accepted object as string";
    EXPECT_FALSE(is_string(R"({"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{}}}}}}}})")) << "Accepted object as string";
    EXPECT_FALSE(is_string("{ \n\r\t\"Key 1\" \n\r\t: \n\r\t\"Val 1\" \n\r\t, \n\r\t, \n\r\t\"Key 2\" \n\r\t: \n\r\t12345 \n\r\t}")) << "Accepted object as string";

    // Arrays
    EXPECT_FALSE(is_string("[]")) << "Accepted array as string";
    EXPECT_FALSE(is_string("[1, 2, 3, 4, 5]")) << "Accepted array as string";
    EXPECT_FALSE(is_string("[\"String\", [\"fds\", \"dsafasg\"], true, null, 6.022e23]")) << "Accepted array as string";
    EXPECT_FALSE(is_string("[1, 2, 3, {\"Key 1\": \"Value 1\", \"Key 2\": 12.345}, 5, 6, 7, 8]")) << "Accepted array as string";
    EXPECT_FALSE(is_string("[[[[[[]]]]]]")) << "Accepted array as string";
    EXPECT_FALSE(is_string("[ \n\r\t12345 \n\r\t, \n\r\t\"String\" \n\r\t, \n\r\tfalse \n\r\t]")) << "Accepted array as string";

    // Strings
    EXPECT_TRUE(is_string("\"This is a basic string\"")) <<  "Failed to recognize a basic string";
    EXPECT_TRUE(is_string(R"("\"This is a quote surrounded string\"")")) <<  "Failed to recognize a string surrounded by quote characters";
    EXPECT_TRUE(is_string(R"("This string \" contains \" quotes internal\" to the string")")) <<  "Failed to recognize a string containing quote characters";
    EXPECT_TRUE(is_string(R"("This \\string c\/ont\u00c6ains  \bthe \fother\n escap\re cha\tract\u00C6ers")")) <<  "Failed to recognize a string containing escape characters";

    // Numbers
    EXPECT_FALSE(is_string("0")) << "Accepted number as string";
    EXPECT_FALSE(is_string("-0")) << "Accepted number as string";
    EXPECT_FALSE(is_string("12345")) << "Accepted number as string";
    EXPECT_FALSE(is_string("-442297")) << "Accepted number as string";
    EXPECT_FALSE(is_string("0.453")) << "Accepted number as string";
    EXPECT_FALSE(is_string("1763.9988")) << "Accepted number as string";
    EXPECT_FALSE(is_string("0e32")) << "Accepted number as string";
    EXPECT_FALSE(is_string("32e+462")) << "Accepted number as string";
    EXPECT_FALSE(is_string("32E+462")) << "Accepted number as string";
    EXPECT_FALSE(is_string("32e462")) << "Accepted number as string";
    EXPECT_FALSE(is_string("32e-462")) << "Accepted number as string";
    EXPECT_FALSE(is_string("789.532e23")) << "Accepted number as string";

    // Bools
    EXPECT_FALSE(is_string("true")) << "Accepted bool as string";
    EXPECT_FALSE(is_string("false")) << "Accepted bool as string";

    // Null
    EXPECT_FALSE(is_string("null")) << "Accepted null as string";
}
TEST(TypeChecking, IsNumber)
{
    // Objects
    EXPECT_FALSE(is_number("{}")) << "Accepted object as number";
    EXPECT_FALSE(is_number(R"({"Key 1": "Value 1", "Key 2": "Value 2"})")) << "Accepted object as number";
    EXPECT_FALSE(is_number(R"({"Key 1": 3.14159, "Key 2": {"Key 2.1": "Value 2.1", "Key 2.2": true}, "Key 3": null})")) << "Accepted object as number";
    EXPECT_FALSE(is_number(R"({"Key 1": 3.14159, "Key 2": ["Val 2.1", 5.5423e-23, false], "Key 3": null})")) << "Accepted object as number";
    EXPECT_FALSE(is_number(R"({"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{}}}}}}}})")) << "Accepted object as number";
    EXPECT_FALSE(is_number("{ \n\r\t\"Key 1\" \n\r\t: \n\r\t\"Val 1\" \n\r\t, \n\r\t, \n\r\t\"Key 2\" \n\r\t: \n\r\t12345 \n\r\t}")) << "Accepted object as number";

    // Arrays
    EXPECT_FALSE(is_number("[]")) << "Accepted array as number";
    EXPECT_FALSE(is_number("[1, 2, 3, 4, 5]")) << "Accepted array as number";
    EXPECT_FALSE(is_number("[\"String\", [\"fds\", \"dsafasg\"], true, null, 6.022e23]")) << "Accepted array as number";
    EXPECT_FALSE(is_number("[1, 2, 3, {\"Key 1\": \"Value 1\", \"Key 2\": 12.345}, 5, 6, 7, 8]")) << "Accepted array as number";
    EXPECT_FALSE(is_number("[[[[[[]]]]]]")) << "Accepted array as number";
    EXPECT_FALSE(is_number("[ \n\r\t12345 \n\r\t, \n\r\t\"String\" \n\r\t, \n\r\tfalse \n\r\t]")) << "Accepted array as number";

    // Strings
    EXPECT_FALSE(is_number("\"This is a basic string\"")) <<  "Accepted string as number";
    EXPECT_FALSE(is_number(R"("\"This is a quote surrounded string\"")")) <<  "Accepted string as number";
    EXPECT_FALSE(is_number(R"("This string \" contains \" quotes internal\" to the string")")) <<  "Accepted string as number";
    EXPECT_FALSE(is_number(R"("This \\string c\/ont\u00c6ains  \bthe \fother\n escap\re cha\tract\u00C6ers")")) <<  "Accepted string as number";

    // Numbers
    EXPECT_TRUE(is_number("0")) << "Failed to recognize zero as number";
    EXPECT_TRUE(is_number("-0")) << "Failed to recognize negative zero as number";
    EXPECT_TRUE(is_number("12345")) << "Failed to recognize positive integer as number";
    EXPECT_TRUE(is_number("-442297")) << "Failed to recognize negative integer as number";
    EXPECT_TRUE(is_number("0.453")) << "Failed to recognize decimal < 1 as number";
    EXPECT_TRUE(is_number("1763.9988")) << "Failed to recognize decimal > 1 as number";
    EXPECT_TRUE(is_number("0e32")) << "Failed to recognize 0e form exponent as number";
    EXPECT_TRUE(is_number("32e+462")) << "Failed to recognize positive [e]xponent without decimal as number";
    EXPECT_TRUE(is_number("32E+462")) << "Failed to recognize positive [E]xponent without decimal as number";
    EXPECT_TRUE(is_number("32e462")) << "Failed to recognize unmarked positive exponent without decimal as number";
    EXPECT_TRUE(is_number("32e-462")) << "Failed to recognize negative exponent without decimal as number";
    EXPECT_TRUE(is_number("789.532e23")) << "Failed to recognize exponent with decimal mantissa";

    // Bools
    EXPECT_FALSE(is_number("true")) << "Accepted bool as number";
    EXPECT_FALSE(is_number("false")) << "Accepted bool as number";

    // Null
    EXPECT_FALSE(is_number("null")) << "Accepted null as number";
}

TEST(TypeChecking, IsBool)
{
    // Objects
    EXPECT_FALSE(is_bool("{}")) << "Accepted object as bool";
    EXPECT_FALSE(is_bool(R"({"Key 1": "Value 1", "Key 2": "Value 2"})")) << "Accepted object as bool";
    EXPECT_FALSE(is_bool(R"({"Key 1": 3.14159, "Key 2": {"Key 2.1": "Value 2.1", "Key 2.2": true}, "Key 3": null})")) << "Accepted object as bool";
    EXPECT_FALSE(is_bool(R"({"Key 1": 3.14159, "Key 2": ["Val 2.1", 5.5423e-23, false], "Key 3": null})")) << "Accepted object as bool";
    EXPECT_FALSE(is_bool(R"({"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{}}}}}}}})")) << "Accepted object as bool";
    EXPECT_FALSE(is_bool("{ \n\r\t\"Key 1\" \n\r\t: \n\r\t\"Val 1\" \n\r\t, \n\r\t, \n\r\t\"Key 2\" \n\r\t: \n\r\t12345 \n\r\t}")) << "Accepted object as bool";

    // Arrays
    EXPECT_FALSE(is_bool("[]")) << "Accepted array as bool";
    EXPECT_FALSE(is_bool("[1, 2, 3, 4, 5]")) << "Accepted array as bool";
    EXPECT_FALSE(is_bool("[\"String\", [\"fds\", \"dsafasg\"], true, null, 6.022e23]")) << "Accepted array as bool";
    EXPECT_FALSE(is_bool("[1, 2, 3, {\"Key 1\": \"Value 1\", \"Key 2\": 12.345}, 5, 6, 7, 8]")) << "Accepted array as bool";
    EXPECT_FALSE(is_bool("[[[[[[]]]]]]")) << "Accepted array as bool";
    EXPECT_FALSE(is_bool("[ \n\r\t12345 \n\r\t, \n\r\t\"String\" \n\r\t, \n\r\tfalse \n\r\t]")) << "Accepted array as bool";

    // Strings
    EXPECT_FALSE(is_bool("\"This is a basic string\"")) <<  "Accepted string as bool";
    EXPECT_FALSE(is_bool(R"("\"This is a quote surrounded string\"")")) <<  "Accepted string as bool";
    EXPECT_FALSE(is_bool(R"("This string \" contains \" quotes internal\" to the string")")) <<  "Accepted string as bool";
    EXPECT_FALSE(is_bool(R"("This \\string c\/ont\u00c6ains  \bthe \fother\n escap\re cha\tract\u00C6ers")")) <<  "Accepted string as bool";

    // Numbers
    EXPECT_FALSE(is_bool("0")) << "Accepted number as bool";
    EXPECT_FALSE(is_bool("-0")) << "Accepted number as bool";
    EXPECT_FALSE(is_bool("12345")) << "Accepted number as bool";
    EXPECT_FALSE(is_bool("-442297")) << "Accepted number as bool";
    EXPECT_FALSE(is_bool("0.453")) << "Accepted number as bool";
    EXPECT_FALSE(is_bool("1763.9988")) << "Accepted number as bool";
    EXPECT_FALSE(is_bool("0e32")) << "Accepted number as bool";
    EXPECT_FALSE(is_bool("32e+462")) << "Accepted number as bool";
    EXPECT_FALSE(is_bool("32E+462")) << "Accepted number as bool";
    EXPECT_FALSE(is_bool("32e462")) << "Accepted number as bool";
    EXPECT_FALSE(is_bool("32e-462")) << "Accepted number as bool";
    EXPECT_FALSE(is_bool("789.532e23")) << "Accepted number as bool";

    // Bools
    EXPECT_TRUE(is_bool("true")) << "Failed to is string 'true' as bool";
    EXPECT_TRUE(is_bool("false")) << "Failed to is string 'false' as bool";

    // Null
    EXPECT_FALSE(is_bool("null")) << "Accepted null as bool";
}

TEST(TypeChecking, IsNull)
{
    // Objects
    EXPECT_FALSE(is_null("{}")) << "Accepted object as null";
    EXPECT_FALSE(is_null(R"({"Key 1": "Value 1", "Key 2": "Value 2"})")) << "Accepted object as null";
    EXPECT_FALSE(is_null(R"({"Key 1": 3.14159, "Key 2": {"Key 2.1": "Value 2.1", "Key 2.2": true}, "Key 3": null})")) << "Accepted object as null";
    EXPECT_FALSE(is_null(R"({"Key 1": 3.14159, "Key 2": ["Val 2.1", 5.5423e-23, false], "Key 3": null})")) << "Accepted object as null";
    EXPECT_FALSE(is_null(R"({"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{"Key":{}}}}}}}})")) << "Accepted object as null";
    EXPECT_FALSE(is_null("{ \n\r\t\"Key 1\" \n\r\t: \n\r\t\"Val 1\" \n\r\t, \n\r\t, \n\r\t\"Key 2\" \n\r\t: \n\r\t12345 \n\r\t}")) << "Accepted object as null";

    // Arrays
    EXPECT_FALSE(is_null("[]")) << "Accepted array as null";
    EXPECT_FALSE(is_null("[1, 2, 3, 4, 5]")) << "Accepted array as null";
    EXPECT_FALSE(is_null("[\"String\", [\"fds\", \"dsafasg\"], true, null, 6.022e23]")) << "Accepted array as null";
    EXPECT_FALSE(is_null("[1, 2, 3, {\"Key 1\": \"Value 1\", \"Key 2\": 12.345}, 5, 6, 7, 8]")) << "Accepted array as null";
    EXPECT_FALSE(is_null("[[[[[[]]]]]]")) << "Accepted array as null";
    EXPECT_FALSE(is_null("[ \n\r\t12345 \n\r\t, \n\r\t\"String\" \n\r\t, \n\r\tfalse \n\r\t]")) << "Accepted array as null";

    // Strings
    EXPECT_FALSE(is_null("\"This is a basic string\"")) <<  "Accepted string as null";
    EXPECT_FALSE(is_null(R"("\"This is a quote surrounded string\"")")) <<  "Accepted string as null";
    EXPECT_FALSE(is_null(R"("This string \" contains \" quotes internal\" to the string")")) <<  "Accepted string as null";
    EXPECT_FALSE(is_null(R"("This \\string c\/ont\u00c6ains  \bthe \fother\n escap\re cha\tract\u00C6ers")")) <<  "Accepted string as null";

    // Numbers
    EXPECT_FALSE(is_null("0")) << "Accepted number as null";
    EXPECT_FALSE(is_null("-0")) << "Accepted number as null";
    EXPECT_FALSE(is_null("12345")) << "Accepted number as null";
    EXPECT_FALSE(is_null("-442297")) << "Accepted number as null";
    EXPECT_FALSE(is_null("0.453")) << "Accepted number as null";
    EXPECT_FALSE(is_null("1763.9988")) << "Accepted number as null";
    EXPECT_FALSE(is_null("0e32")) << "Accepted number as null";
    EXPECT_FALSE(is_null("32e+462")) << "Accepted number as null";
    EXPECT_FALSE(is_null("32E+462")) << "Accepted number as null";
    EXPECT_FALSE(is_null("32e462")) << "Accepted number as null";
    EXPECT_FALSE(is_null("32e-462")) << "Accepted number as null";
    EXPECT_FALSE(is_null("789.532e23")) << "Accepted number as null";

    // Bools
    EXPECT_FALSE(is_null("true")) << "Accepted bool as null";
    EXPECT_FALSE(is_null("false")) << "Accepted bool as null";

    // Null
    EXPECT_TRUE(is_null("null")) << "Failed to is string 'null' as null";
}
