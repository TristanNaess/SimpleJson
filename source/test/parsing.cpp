#include <gtest/gtest.h>
#include "parsing.hpp"
#include "error.hpp"

// --------------------------------------
// Tests for match_quote()
// --------------------------------------

TEST(MatchQuote, WorksOnGoodString)
{
    std::string simple_string{R"(This "String" contains quotes)"};
    std::string nested_quotes{R"("This \"String\" contains escaped quotes inside the quoted delimited section")"};

    EXPECT_EQ(match_quote(simple_string, 5), 12) << "Failed to locate matching quote in simple string";
    EXPECT_EQ(match_quote(nested_quotes, 0), 76) << "Failed to locate matching quote with internal escaped quotes";
}

TEST(MatchQuote, FailsIfIndexNotQuote)
{
    std::string s{"There are no quotes in this string"};
    try
    {
        match_quote(s, 5);
        FAIL() << "match_quote() failed to throw when passed non-quote index as argument";
    }
    catch (const json::parsing_error& pe)
    {
        EXPECT_STREQ(pe.what(), "No quote at index provided in argument") << "match_quote() failed to throw with expected message: \"No quote at index provided in argument\"";
    }
    catch (...)
    {
        FAIL() << "Incorrect error type thrown by match_quote()";
    }
}

TEST(MatchQuote, FailsIfIndexOutsideString)
{
    std::string s{"Short String"};
    try
    {
        match_quote(s, 12);
        FAIL() << "match_quote() failed to throw when passed index outside string";
    }
    catch (const json::parsing_error& pe)
    {
        EXPECT_STREQ(pe.what(), "Index argument provided is outside the string") << "match_quote() failed to throw with expected message: \"Index argument provided is outside the string\"";
    }
    catch (...)
    {
        FAIL() << "Incorrect error type thrown by match_quote()";
    }
}

TEST(MatchQuote, FailsIfNoMatchingQuote)
{
    std::string simple_string{R"(This "String is missing the closing quote)"};
    std::string escaped_quotes{R"("This \"String\" contains escaped quotes and is missing the closing quote)"};
    try
    {
        match_quote(simple_string, 5);
        FAIL() << "match_quote(simple_string) failed to throw when passed a simple string missing the closing quote";
    }
    catch (const json::parsing_error& pe)
    {
        EXPECT_STREQ(pe.what(), "No matching quote located") << "match_quote(simple_string) failed to throw with expected message: \"No matching quote located\"";
    }
    catch (...)
    {
        FAIL() << "Incorrect error type thrown by match_quote(simple_string)";
    }
    try
    {
        match_quote(escaped_quotes, 0);
        FAIL() << "match_quote(escaped_quotes) failed to throw when passed a string missing the closing quote containing escaped quotes";
    }
    catch (const json::parsing_error& pe)
    {
        EXPECT_STREQ(pe.what(), "No matching quote located") << "match_quote(escaped_quotes) failed to throw with expected message: \"No matching quote located\"";
    }
    catch (...)
    {
        FAIL() << "Incorrect error type thrown by match_quote(escaped_quotes)";
    }
}


// --------------------------------------
// Tests for match_bracket()
// --------------------------------------

TEST(MatchBracket, WorksOnGoodString)
{
    std::string simple_string{"fdsafdjsakl;[fdajsfkdsal]dfjdsaklf;d"};
    std::string nested_brackets{"fds{fdsagdebdabdf{fdsag}{fd{}gsdg{gf{gfdsgsd}}sa}}gfds{gfds}gfd"};

    EXPECT_EQ(match_bracket(simple_string, 12), 24) << "Failed to locate matching bracket in simple string";
    EXPECT_EQ(match_bracket(nested_brackets, 3), 49) << "Failed to locate matching bracket with nested brackets";
}

TEST(MatchBracket, FailsIfIndexNotBracket)
{
    std::string s{"This string contains no brackets"};
    try
    {
        match_bracket(s, 5);
        FAIL() << "match_bracket() failed to throw when passed non-bracket index argument";
    }
    catch (const json::parsing_error& pe)
    {
        EXPECT_STREQ(pe.what(), "Character at index is not a bracket") << "match_quote() failed to throw with expected message: \"Character at index is not a bracket\"";
    }
    catch (...)
    {
        FAIL() << "Incorrect error type thrown by match_bracket()";
    }
}

TEST(MatchBracket, FailsIfIndexOutsideString)
{
    std::string s{"short string"};
    try
    {
        match_bracket(s, 12);
        FAIL() << "match_bracket() failed to throw when passed index outside string";
    }
    catch (const json::parsing_error& pe)
    {
        EXPECT_STREQ(pe.what(), "Index argument provided is outside the string");
    }
    catch (...)
    {
        FAIL() << "Incorrect error type thrown by match_bracket()";
    }
}

TEST(MatchBracket, FailsIfNoMatchingBracket)
{
    std::string simple_string{"fds{fdafdsa"};
    std::string bracket_in_string{R"(fsdajfkl{ fdsag"fjdkl f;ds}"fdsa)"};
    std::string nested_brackets{"fas{hsv vmkl ; {fdsa f} dfjk{ fds{ fds} djkfld;a} fdjkl fa"};
    try
    {
        match_bracket(simple_string, 3);
        FAIL() << "match_bracket(simple_string) failed to throw when passed a simple string missing closing bracket";
    }
    catch (const json::parsing_error& pe)
    {
        EXPECT_STREQ(pe.what(), "No matching bracket located") << "match_bracket(simple_string) failed to throw with expected message: \"No matching bracket located\"";
    }
    catch (...)
    {
        FAIL() << "Incorrect error type thrown by match_bracket(simple_string)";
    }
    try
    {
        match_bracket(bracket_in_string, 8);
        FAIL() << "match_bracket(bracket_in_string) failed to throw when passed a string with no matching bracket outside escaped string";
    }
    catch (const json::parsing_error& pe)
    {
        EXPECT_STREQ(pe.what(), "No matching bracket located") << "match_bracket(bracket_in_string) failed to throw with expected message: \"no matching bracket located\"";
    }
    catch (...)
    {
        FAIL() << "Incorrect error type thrown by match_bracket(bracket_in_string)";
    }
    try
    {
        match_bracket(nested_brackets, 3);
        FAIL() << "match_bracket(nested_brackets) failed to throw when passed a string with no matching bracket";
    }
    catch (const json::parsing_error& pe)
    {
        EXPECT_STREQ(pe.what(), "No matching bracket located") << "match_bracket(nested_brackets) failed to throw with expected message: \"no matching bracket located\"";
    }
    catch (...)
    {
        FAIL() << "Incorrect error type thrown by match_bracket(nested_brackets)";
    }
}


// --------------------------------------
// Tests for match_bracket()
// --------------------------------------

TEST(FindUnquoted, WorksOnGoodString)
{
    std::string simple_string = "This string does not contain quotes.";
    std::string quoted_string = "This \"String\" contains quotes";

    EXPECT_EQ(find_unquoted(simple_string, 'n'), 9) << "Failed to locate 'n' at index 9";
    EXPECT_EQ(find_unquoted(simple_string, 'n', 10), 17) << "Failed to locate 'n' at index 17, starting from index 10";
    EXPECT_EQ(find_unquoted(simple_string, 'z'), std::string::npos) << "Failed to return std::string::npos when looking for non-existent 'z'";

    EXPECT_EQ(find_unquoted(quoted_string, 't'), 17) << "Failed to locate 't' at index 17, skipping 't' in quotes at 7";
    EXPECT_EQ(find_unquoted(quoted_string, 'i', 3), 19) << "Failed to locate 'i' at index 19, starting at index 3, skipping 'i' in quotes at 8";
    EXPECT_EQ(find_unquoted(quoted_string, 'S'), std::string::npos) << "Failed to return std::string::npos when looking for 'S', which only occurs in quotes";
}

TEST(FindUnquoted, FailsWhenQuotesUnmatched)
{
    std::string unmatched_quote = "This \"String contains unmatched quotes.";

    try
    {
        find_unquoted(unmatched_quote, 'a');
        FAIL() << "find_unquoted() failed to throw when passed a string with unmatched quotes";
    }
    catch (const json::parsing_error& pe)
    {
        EXPECT_STREQ(pe.what(), "Error finding character in string: No matching quote located") << "find_unquoted(unmatched_quote) failed to throw with expected message: \"Error finding character in string: No matching quote located\"";
    }
    catch (...)
    {
        FAIL() << "Incorrect error type thrown by find_unquoted(unmatched_quotes)";
    }
}
