#include <gtest/gtest.h>
#include "parsing.hpp"
#include "error.hpp"

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



