#include <gtest/gtest.h>
#include "parsing.hpp"
#include "error.hpp"

TEST(StringConversion, ToJson)
{
    std::string_view str("This is a basic string");
    EXPECT_EQ(to_json_string(str), "\"This is a basic string\"") << "Failed to convert 'This is a basic string' to json format (add surrounding quotes)";
    str = std::string_view("\"\\\b\f\n\r\t€");
    EXPECT_EQ(to_json_string(str), R"("\"\\\b\f\n\r\t€")") << "Failed to convert '\"\\\b\f\n\r\t€' to json format (escape characters, keep UTF-8)";
}

TEST(StringConversion, FromJson)
{
    std::string_view str("\"This is a basic string\"");
    EXPECT_EQ(from_json_string(str), "This is a basic string") << "Failed to convert json string \"This is a basic string\" to C++ syntax (remove surrounding quotes)";
    str = std::string_view(R"("\"\\\/\b\f\n\r\t\u20AC")");
    EXPECT_EQ(from_json_string(str), "\"\\/\b\f\n\r\t€") << "Failed to convert json string \"\\\"\\\\\\/\\b\\f\\n\\r\\t\\u20AC\" to C++ string '\"\\/\b\f\n\r\t€'";
    str = std::string_view("no quotes");
    EXPECT_THROW(from_json_string(str), json::parsing_error) << "Failed to throw when json string is not quote surrounded";
    str = std::string_view(R"("\g")");
    EXPECT_THROW(from_json_string(str), json::parsing_error) << "Failed to throw when passed incorrect escape character '\\g'";
}

TEST(MatchQuote, GoodString)
{
    std::string str = "\"This string is quote surrounded\"";
    EXPECT_EQ(match_quote(str, 0), 32) << "Failed find matching quote at [32] in '\"This string is quote surrounded\"'";

    str = "\"This \\\"string\\\" contains escaped \\\" chars\"";
    EXPECT_EQ(match_quote(str, 0), 42) << "Failed to find matching quote at [42] in '\"This \\\"string\\\" contains escaped \\\" chars\"'";

    str = "The starting \"quote\" in this string is not the first char";
    EXPECT_EQ(match_quote(str, 13), 19) << "Failed to find matching quote at [19] in 'The starting \"quote\" in this string is not the first char'";
}

TEST(MatchQuote, BadString)
{
    std::string str = "\"This string is missing the closing quote";
    EXPECT_THROW(match_quote(str, 0), json::parsing_error) << "Failed to throw json::parsing_error matching [0] in '\"This string is missing the closing quote'";

    str = "This \"string contains \\\" multiple escaped \\\" quote\\\" characters";
    EXPECT_THROW(match_quote(str, 5), json::parsing_error) << "Failed to throw json::parsing_error matching [5] in 'This \"string contains \\\" multiple escaped \\\" quote\\\" characters'";
}

TEST(MatchBracket, GoodString)
{
    std::string str = "[{}]";
    EXPECT_EQ(match_bracket(str, 0), 3) << "Failed to find matching ']' at [3] in '[{}]'";
    EXPECT_EQ(match_bracket(str, 1), 2) << "Failed to find matching '}' at [2] in '[{}]'";

    str = "fdsafs{jfkdlsa;{isanfdsaffdsg{}jkfl;safb{jfkldstbg09}jklfd;sabet}fds}jklab";
    EXPECT_EQ(match_bracket(str, 6), 68) << "Failed to find matching '}' at [68] in 'fdsafs{jfkdlsa;{isanfdsaffdsg{}jkfl;safb{jfkldstbg09}jklfd;sabet}fds}jklab'";
}

TEST(MatchBracket, BadString)
{
    std::string str = "fdsaf[fjdkal{fjkdla;fdbsa[ jkafl{fdsafdsa";
    EXPECT_THROW(match_bracket(str, 5), json::parsing_error) << "Failed to throw json::parsing_error matching [5] in 'fdsaf[fjdkal{fjkdla;fdbsa[ jkafl{fdsafdsa'";
    EXPECT_THROW(match_bracket(str, 12), json::parsing_error) << "Failed to throw json::parsing_error matching [12] in 'fdsaf[fjdkal{fjkdla;fdbsa[ jkafl{fdsafdsa'";

    str = "bfsajg;sa{jkdlsa;fdasb{jfkdsa;f}fjkdsl{fjkdsal;}jfkdlsa{fdjksal;{fjdksal}jkfdl}jkdsljdskal{fdsa";
    EXPECT_THROW(match_bracket(str, 9), json::parsing_error) << "Failed to throw json::parsing_error matching [9] in 'bfsajg;sa{jkdlsa;fdasb{jfkdsa;f}fjkdsl{fjkdsal;}jfkdlsa{fdjksal;{fjdksal}jkfdl}jkdsljdskal{fdsa'";
}

TEST(FindUnquoted, GoodString)
{
    std::string str = "djfkla;fbafjklsa;ghsagdafdiuo";
    EXPECT_EQ(find_unquoted(str, ';'), 6) << "Failed to find ';' starting from [0] at [6] in 'djfkla;fbafjklsa;ghsagdafdiuo'";
    EXPECT_EQ(find_unquoted(str, ';', 7), 16) << "Failed to find ';' starting from [7] at [16] in 'djfkla;fbafjklsa;ghsagdafdiuo'";

    str = "jfklabfdj\"saklbvfi:safdsabj\"dbdfgo:padsfbj";
    EXPECT_EQ(find_unquoted(str, ':'), 34) << "Failed to find ':' at [34] in 'jfklabfdj\"saklbvfi:safdsabj\"dbdfgo:padsfbj'";

    str = "jfkla;sdbfdsiauofdsa";
    EXPECT_EQ(find_unquoted(str, '*'), std::string::npos) << "Failed to return std::string::npos for non-existent '*' in 'jfkla;sdbfdsiauofdsa'";

    str = "jfdsabg\"ofdsal;ghfdsa\"bfjgfd";
    EXPECT_EQ(find_unquoted(str, '"'), 7) << "Failed to find '\"' at [7] in 'jfdsabg\"ofdsal;ghfdsa\"bfjgfd'";
}

TEST(FindUnquoted, BadString)
{
    std::string str = "fjd\"sakl;fbsafodsaanl";
    EXPECT_THROW(find_unquoted(str, ';'), json::parsing_error) << "Failed to throw json::parsing_error finding ';' in 'fjd\"sakl;fbsafodsaanl'";
}

TEST(RemoveWhitespace, RemovesCorrectly)
{
    std::string str = "jkfldsaf  jfk\tdsla;bfao\tfdbs   fdsjklb;fai\njofds fjkdsla gfd\rsabnfjd;gj k\tlfd jak\r\nfl;dshl";
    EXPECT_EQ(remove_whitespace(str), "jkfldsafjfkdsla;bfaofdbsfdsjklb;faijofdsfjkdslagfdsabnfjd;gjklfdjakfl;dshl") << "Failed to remove whitespace characters from 'jkfldsaf  jfk\tdsla;bfao\tfdbs   fdsjklb;fai\njofds fjkdsla gfd\rsabnfjd;gj k\tlfd jak\r\nfl;dshl'";

    str = "jf klsda;fb\tdajs\"kglj\rsaiof; dsa\nfd\"sa\rjl";
    EXPECT_EQ(remove_whitespace(str), "jfklsda;fbdajs\"kglj\rsaiof; dsa\nfd\"sajl") << "Failed to remove unquoted whitespace characters from 'jf klsda;fb\tdajs\"kglj\rsaiof; dsa\nfd\"sa\rjl'";
}

TEST(NextDelim, WithinObject)
{
    std::string str = R"({"Key 1":"Value 1","Key: containing,delimiters":{"Key 2.1":1,"Key 2.2":[1,2,3],"Key 2.3":false},"Key 3":[1,2,3,4,5],"Key 4":null})";
    std::size_t start = 1; // skip initial '{'
    start = next_delim(str, start);
    EXPECT_EQ(start, 8) << R"(Failed to locate ':' at index 8 in '{"Key 1":"Value 1","Key: containing,delimiters":{"Key 2.1":1,"Key 2.2":[1,2,3],"Key 2.3":false},"Key 3":null}')";
    start = 9;
    start = next_delim(str, start);
    EXPECT_EQ(start, 18) << R"(Failed to locate ',' at index 18 in '{"Key 1":"Value 1","Key: containing,delimiters":{"Key 2.1":1,"Key 2.2":[1,2,3],"Key 2.3":false},"Key 3":null}')";
    start = 19;
    start = next_delim(str, start);
    EXPECT_EQ(start, 47) << R"(Failed to locate ':' at index 47 in '{"Key 1":"Value 1","Key: containing,delimiters":{"Key 2.1":1,"Key 2.2":[1,2,3],"Key 2.3":false},"Key 3":null}')";
    start = 48;
    start = next_delim(str, start);
    EXPECT_EQ(start, 95) << R"(Failed to locate ',' at index 95 in '{"Key 1":"Value 1","Key: containing,delimiters":{"Key 2.1":1,"Key 2.2":[1,2,3],"Key 2.3":false},"Key 3":null}')";
    start = 96;
    start = next_delim(str, start);
    EXPECT_EQ(start, 103) << R"(Failed to locate ':' at index 103 in '{"Key 1":"Value 1","Key: containing,delimiters":{"Key 2.1":1,"Key 2.2":[1,2,3],"Key 2.3":false},"Key 3":null}')";
    start = 104;
    start = next_delim(str, start);
    EXPECT_EQ(start, 115) << R"(Failed to locate ',' at index 115 in '{"Key 1":"Value 1","Key: containing,delimiters":{"Key 2.1":1,"Key 2.2":[1,2,3],"Key 2.3":false},"Key 3":null}')";
    start = 116;
    start = next_delim(str, start);
    EXPECT_EQ(start, 123) << R"(Failed to locate ':' at index 123 in '{"Key 1":"Value 1","Key: containing,delimiters":{"Key 2.1":1,"Key 2.2":[1,2,3],"Key 2.3":false},"Key 3":null}')";
    start = 124;
    start = next_delim(str, start);
    EXPECT_EQ(start, std::string_view::npos) << R"(Failed to return std::string_view::npos when no more delimiters in '{"Key 1":"Value 1","Key: containing,delimiters":{"Key 2.1":1,"Key 2.2":[1,2,3],"Key 2.3":false},"Key 3":null}')";
}

TEST(NextDelim, WithinArray)
{
    std::string str = R"(["hello world",["this","was","a","triumph"],3.14159,true,{"Key 1":"Value 1","Key 2":42},null])";
    std::size_t start = 1;
    start = next_delim(str, start);
    EXPECT_EQ(start, 14) << R"(Failed to locate ',' at index 14 in '["hello world",["this","was","a","triumph"],3.14159,true,{"Key 1":"Value 1"."Key 2":42},null]')";
    start = 15;
    start = next_delim(str, start);
    EXPECT_EQ(start, 43) << R"(Failed to locate ',' at index 43 in '["hello world",["this","was","a","triumph"],3.14159,true,{"Key 1":"Value 1"."Key 2":42},null]')";
    start = 44;
    start = next_delim(str, start);
    EXPECT_EQ(start, 51) << R"(Failed to locate ',' at index 51 in '["hello world",["this","was","a","triumph"],3.14159,true,{"Key 1":"Value 1"."Key 2":42},null]')";
    start = 52;
    start = next_delim(str, start);
    EXPECT_EQ(start, 56) << R"(Failed to locate ',' at index 56 in '["hello world",["this","was","a","triumph"],3.14159,true,{"Key 1":"Value 1"."Key 2":42},null]')";
    start = 57;
    start = next_delim(str, start);
    EXPECT_EQ(start, 87) << R"(Failed to locate ',' at index 87 in '["hello world",["this","was","a","triumph"],3.14159,true,{"Key 1":"Value 1"."Key 2":42},null]')";
    start = 88;
    start = next_delim(str, start);
    EXPECT_EQ(start, std::string_view::npos) << R"(Failed to return std::string::npos when no more delimiters in '["hello world",["this","was","a","triumph"],3.14159,true,{"Key 1":"Value 1"."Key 2":42},null]')";
}

TEST(ExtractData, ExtractField)
{
    std::string_view str{R"({"Object":{"Field 1":"Foobar","Field 2":123.45,"Field 3":true},"Array":[1,2,3,4,5],"String":"Hello there.","Number":3.14159,"Bool":true,"\"Null\"":null})"};

    EXPECT_EQ(extract_field(str, "Object"), R"({"Field 1":"Foobar","Field 2":123.45,"Field 3":true})") << R"(Failed to properly extract 'Object' field from '{"Object":{"Field 1":"Foobar","Field 2":123.45,"Field 3":true},"Array":[1,2,3,4,5],"String":"Hello there.","Number":3.14159,"Bool":true,"\"Null\"":null}')";
    EXPECT_EQ(extract_field(str, "Array"), R"([1,2,3,4,5])") << R"(Failed to properly extract 'Array' field from '{"Object":{"Field 1":"Foobar","Field 2":123.45,"Field 3":true},"Array":[1,2,3,4,5],"String":"Hello there.","Number":3.14159,"Bool":true,"\"Null\"":null}')";
    EXPECT_EQ(extract_field(str, "String"), "\"Hello there.\"") << R"(Failed to properly extract 'String' field from '{"Object":{"Field 1":"Foobar","Field 2":123.45,"Field 3":true},"Array":[1,2,3,4,5],"String":"Hello there.","Number":3.14159,"Bool":true,"\"Null\"":null}')";
    EXPECT_EQ(extract_field(str, "Number"), "3.14159") << R"(Failed to properly extract 'Number' field from '{"Object":{"Field 1":"Foobar","Field 2":123.45,"Field 3":true},"Array":[1,2,3,4,5],"String":"Hello there.","Number":3.14159,"Bool":true,"\"Null\"":null}')";
    EXPECT_EQ(extract_field(str, "Bool"), "true") << R"(Failed to properly extract 'Bool' field from '{"Object":{"Field 1":"Foobar","Field 2":123.45,"Field 3":true},"Array":[1,2,3,4,5],"String":"Hello there.","Number":3.14159,"Bool":true,"\"Null\"":null}')";
    EXPECT_EQ(extract_field(str, "\"Null\""), "null") << R"(Failed to properly extract '"Null"' field from '{"Object":{"Field 1":"Foobar","Field 2":123.45,"Field 3":true},"Array":[1,2,3,4,5],"String":"Hello there.","Number":3.14159,"Bool":true,"\"Null\"":null}')";
    EXPECT_THROW(extract_field(str, "Null"), json::out_of_range) << "Failed to throw json::out_of_range when non-existant field is requested";
}

TEST(ExtractData, ExtractIndex)
{
    std::string_view str{R"([{"Key 1":"foobar","Key 2":true,"Key 3":42},[3],true,"String",6.022e23,null])"};
    EXPECT_EQ(extract_index(str,0),R"({"Key 1":"foobar","Key 2":true,"Key 3":42})") << R"(Failed to properly extract element 0 from '[{"Key 1":"foobar","Key 2":true,"Key 3":42},[3],true,"String",6.022e23,null]')";
    EXPECT_EQ(extract_index(str,1),"[3]") << R"(Failed to properly extract element 1 from '[{"Key 1":"foobar","Key 2":true,"Key 3":42},[3],true,"String",6.022e23,null]')";
    EXPECT_EQ(extract_index(str,2),"true") << R"(Failed to properly extract element 2 from '[{"Key 1":"foobar","Key 2":true,"Key 3":42},[3],true,"String",6.022e23,null]')";
    EXPECT_EQ(extract_index(str,3),"\"String\"") << R"(Failed to properly extract element 3 from '[{"Key 1":"foobar","Key 2":true,"Key 3":42},[3],true,"String",6.022e23,null]')";
    EXPECT_EQ(extract_index(str,4),"6.022e23") << R"(Failed to properly extract element 4 from '[{"Key 1":"foobar","Key 2":true,"Key 3":42},[3],true,"String",6.022e23,null]')";
    EXPECT_EQ(extract_index(str,5),"null") << R"(Failed to properly extract element 5 from '[{"Key 1":"foobar","Key 2":true,"Key 3":42},[3],true,"String",6.022e23,null]')";
    EXPECT_THROW(extract_index(str,6), json::out_of_range) << "Failed to throw json::out_of_range when index greater than size is requested";
}
