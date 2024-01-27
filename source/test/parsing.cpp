#include <gtest/gtest.h>
#include "parsing.hpp"
#include "error.hpp"

TEST(MatchQuote, GoodString)
{
    std::string str = "\"This string is quote surrounded\"";
    EXPECT_EQ(match_quote(str, 0), 32) << "Failed find matching quote at [32] in: '\"This string is quote surrounded\"'";

    str = "\"This \\\"string\\\" contains escaped \\\" chars\"";
    EXPECT_EQ(match_quote(str, 0), 42) << "Failed to find matching quote at [42] in: '\"This \\\"string\\\" contains escaped \\\" chars\"'";

    str = "The starting \"quote\" in this string is not the first char";
    EXPECT_EQ(match_quote(str, 13), 19) << "Failed to find matching quote at [19] in 'The starting \"quote\" in this string is not the first char'";
}

TEST(MatchQuote, BadString)
{
    std::string str = "\"This string is missing the closing quote";
    EXPECT_THROW(match_quote(str, 0), json::parsing_error) << "Failed to throw json::parsing_error for: '\"This string is missin the closing quote'";

    str = "This \"string contains \\\" multiple escaped \\\" quote\\\" characters";
    EXPECT_THROW(match_quote(str, 5), json::parsing_error) << "Failed to throw json::parsing_error for: 'This \"string contains \\\" multiple escaped \\\" quote\\\" characters'";
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
    EXPECT_THROW(match_bracket(str, 5), json::parsing_error) << "Failed to throw json::parsing_error for 'fdsaf[fjdkal{fjkdla;fdbsa[ jkafl{fdsafdsa'";
    EXPECT_THROW(match_bracket(str, 12), json::parsing_error) << "Failed to throw json::parsing_error for 'fdsaf[fjdkal{fjkdla;fdbsa[ jkafl{fdsafdsa'";

    str = "bfsajg;sa{jkdlsa;fdasb{jfkdsa;f}fjkdsl{fjkdsal;}jfkdlsa{fdjksal;{fjdksal}jkfdl}jkdsljdskal{fdsa";
    EXPECT_THROW(match_bracket(str, 9), json::parsing_error) << "Failed to throw json::parsing_error for 'bfsajg;sa{jkdlsa;fdasb{jfkdsa;f}fjkdsl{fjkdsal;}jfkdlsa{fdjksal;{fjdksal}jkfdl}jkdsljdskal{fdsa'";
}
