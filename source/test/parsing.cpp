#include <gtest/gtest.h>
#include "parsing.hpp"

// not included in header
std::string::iterator skip_quotes(std::string::iterator opening_quote) noexcept;
std::string::iterator skip_brackets(std::string::iterator opening_bracket) noexcept;

// internal utilities
TEST(Internals, SkipQuotes)
{
    std::string data = "\"fdsabsdfkl;ja\"dsfdsa";
    EXPECT_EQ(skip_quotes(data.begin()), data.begin() + 14) << "Failed to locate quote at data[14]";

    data = "\"fdfds\\\"dfbdfshfg\"gfdshs";
    EXPECT_EQ(skip_quotes(data.begin()), data.begin() + 17) << "Failed to locate quote at data[17] skipping internal quote";
}

TEST(Internals, SkipBrackets)
{
    std::string data = "fdsa{fdafdshg}fdkjalb";
    EXPECT_EQ(skip_brackets(data.begin() + 4), data.begin() + 13) << "Failed to locate closing } at data[13]";

    data = "jdska[l;fbdsajkfldlajioavpdj]sai";
    EXPECT_EQ(skip_brackets(data.begin() + 5), data.begin() + 28) << "Failed to locate closing ] at data[28]";

    data = "sajkld{dsa{fdsafdsa}dfs{fdsa{fdsa}f{fdsa}dsag}gdsaad}dsajklf";
    EXPECT_EQ(skip_brackets(data.begin() + 6), data.begin() + 52) << "Failed to locate closing } at data[52] with complex nesting";

    data = "fds{wieuusab\"jkfa}l;\"jkdafsl;bf}jkdlsa;y";
    EXPECT_EQ(skip_brackets(data.begin() + 3), data.begin() + 31) << "Failed to locate closing } at data[31] skipping quote surrounded }";
}

// public facing functions
TEST(Public, RemoveWhitespace)
{
    std::string data = "ThisStringContainsNoWhitespace";
    EXPECT_EQ(remove_whitespace(data), data) << "Failed to leave whitespace-less string unchanged";

    data = "Th\fis String\tCo\nntains\rWhite\vspace";
    EXPECT_EQ(remove_whitespace(data), std::string("ThisStringContainsWhitespace")) << "Failed to remove whitespace";

    data = "Th\nis \"String\" Co\rntain\fs Quo\ttes";
    EXPECT_EQ(remove_whitespace(data), std::string("This\"String\"ContainsQuotes")) << "Failed to remove whitespace outside quotes";

    data = "This String \"Cont\vains \tQuot\ne Su\frrou\rnded\" Whitespace";
    EXPECT_EQ(remove_whitespace(data), std::string("ThisString\"Cont\vains \tQuot\ne Su\frrou\rnded\"Whitespace")) << "Failed to ignore whitespace in quotes";

    data = "\"This string starts with a quote\" And Must Account For That";
    EXPECT_EQ(remove_whitespace(data), std::string("\"This string starts with a quote\"AndMustAccountForThat")) << "Failed to ignore whitespace after quote as first character";
}
