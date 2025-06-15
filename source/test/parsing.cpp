#include <gtest/gtest.h>
#include <vector>
#include "parsing.hpp"

// not included in header
std::string::iterator skip_quotes(std::string::iterator opening_quote) noexcept;
std::string::iterator skip_brackets(std::string::iterator opening_bracket) noexcept;
std::string::iterator seek(mut_view data, char c, std::string::iterator start) noexcept;

// internal utilities
TEST(Internals, SkipQuotes)
{
    std::string data = "\"fdsabsdfkl;ja\"dsfdsa";
    EXPECT_EQ(skip_quotes(data.begin()), data.begin() + 14) << "Failed to locate quote at data[14]";

    data = "\"fdfds\\\"dfbdfshfg\"gfdshs";
    EXPECT_EQ(skip_quotes(data.begin()), data.begin() + 17) << "Failed to locate quote at data[17] skipping internal quote";

    data = "\"jkfl;dsafbiuodsfjsakl;fdsba";
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

TEST(Public, Seek)
{
    std::string line = "fabei;\"gsafkl;vdsak\"fdsl;fbio";
    mut_view data(line, line.begin(), line.end());
    EXPECT_EQ(seek(data, 'l', data.begin), line.begin() + 23) << "Failed to find 'l' after quotes, skipping one in quotes";
    EXPECT_EQ(seek(data, 'f', data.begin + 3), line.begin() + 20) << "Failed to find 'f' after quotes, skipping one before start";

    line = "fds;ds{dafs{gfds[afdjksal;]fd{sha}jkl;fdsab}fsjakl;f}fdgsa";
    data = mut_view(line, line.begin(), line.end());
    EXPECT_EQ(seek(data, 'g', data.begin), line.begin() + 55) << "Failed to find 'g' after skipping nested brackets";
    EXPECT_EQ(seek(data, ';', data.begin + 8), line.begin() + 50) << "Failed to find ';' after skipping brackets, starting after first ';'";
    
    line = "ask{fj;l{ab\"ga{sgo\"i;a}jkfd}lfdsa";
    data = mut_view(line, line.begin(), line.end());
    EXPECT_EQ(seek(data, 'd', data.begin), line.begin() + 30) << "Failed to find 'd' after skipping brackets, including quoted bracket";
}


TEST(Public, NextField)
{
    std::string line = R"({"name":["John","","Smith"],"id":7928113,"department":"mensware","availability":{"sunday":[8,1730],"monday":[null,null],"tuesday":[null,null],"wednesday":[8,1730],"thursday":[8,1730],"friday":[8,1200],"saturday":[1200,1730]}})";

    mut_view data(line, line.begin(), line.end());
    mut_view field(line, data.begin, data.begin);
    EXPECT_EQ(field = next_field(data, field.end), R"("name":["John","","Smith"])") << "Failed to extract field";
    EXPECT_EQ(field = next_field(data, field.end), R"("id":7928113)") << "Failed to extract field";
    EXPECT_EQ(field = next_field(data, field.end), R"("department":"mensware")") << "Failed to extract field";
    EXPECT_EQ(field = next_field(data, field.end), R"("availability":{"sunday":[8,1730],"monday":[null,null],"tuesday":[null,null],"wednesday":[8,1730],"thursday":[8,1730],"friday":[8,1200],"saturday":[1200,1730]})") << "Failed to extract field";

    EXPECT_EQ(data = next_field(data, field.end), mut_view(line, line.end(), line.end())) << "Failed to return npos equivalent";

    // empty object case
    std::string empty = "{}";
    data = mut_view(empty, empty.begin(), empty.end());
    field = mut_view(empty, data.begin, data.begin);
    EXPECT_EQ(field = next_field(data, field.end), mut_view(empty, empty.end(), empty.end())) << "Failed to return npos equivalent when called on empty object";
}

TEST(Public, GetKey)
{

}

TEST(Public, GetVal)
{

}

TEST(Public, DeleteField)
{

}

TEST(Public, ChangeField)
{

}
