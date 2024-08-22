#include <gtest/gtest.h>
#include "parsing.hpp"

// internal utilities
TEST(Internals, SkipQuotes)
{
    std::string data = "\"fdsabsdfkl;ja\"dsfdsa";
    EXPECT_EQ(skip_quotes(data.begin(), data.end()), data.begin() + 14) << "Failed to locate quote at data[14]";

    data = "\"fdfds\\\"dfbdfshfg\"gfdshs";
    EXPECT_EQ(skip_quotes(data.begin(), data.end()), data.begin() + 17) << "Failed to locate quote at data[17] skipping internal quote";

    data = "\"fasbafgh";
    EXPECT_EQ(skip_quotes(data.begin(), data.end()), data.end()) << "Failed to return data.end() when there is no quote to find";

    data = "\"kjl;adb\\\"djakl;";
    EXPECT_EQ(skip_quotes(data.begin(), data.end()), data.end()) << "Failed to return data.end() when there is no non-internal quote";
}

TEST(Internals, SkipBrackets)
{
    std::string data = "fdsa{fdafdshg}fdkjalb";
    EXPECT_EQ(skip_brackets(data.begin() + 4, data.end()), data.begin() + 13) << "Failed to locate closing } at data[13]";

    data = "jdska[l;fbdsajkfldlajioavpdj]sai";
    EXPECT_EQ(skip_brackets(data.begin() + 5, data.end()), data.begin() + 28) << "Failed to locate closing ] at data[28]";

    data = "sajkld{dsa{fdsafdsa}dfs{fdsa{fdsa}f{fdsa}dsag}gdsaad}dsajklf";
    EXPECT_EQ(skip_brackets(data.begin() + 6, data.end()), data.begin() + 52) << "Failed to locate closing } at data[52] with complex nesting";

    data = "fds{wieuusab\"jkfa}l;\"jkdafsl;bf}jkdlsa;y";
    EXPECT_EQ(skip_brackets(data.begin() + 3, data.end()), data.begin() + 31) << "Failed to locate closing } at data[31] skipping quote surrounded }";
}

TEST(Internals, FindApplicable)
{
    // unquoted
    std::string data = "dabdskls\"fdsaghbagd\"dfsaGfdsakl;f";
    EXPECT_EQ(find_applicable(data.begin(), data.end(), 'G'), data.begin() + 24) << "Failed to locate G at data[24]";

    data = "jklbaop\"fdsabFdadsa\"asbnmsaFfjkdl";
    EXPECT_EQ(find_applicable(data.begin(), data.end(), 'F'), data.begin() + 27) << "Failed to locate F at data[27] skipping internal F";

    data = "klf;biuoban\"dafdsabvfdhgsaf\"fdsabnbjl";
    EXPECT_EQ(find_applicable(data.begin(), data.end(), 'A'), data.end()) << "Failed to return data.end() when there is no A";

    data = "fdjskl;b a\"jiikBasl;fd\"jakaibqjik";
    EXPECT_EQ(find_applicable(data.begin(), data.end(), 'B'), data.end()) << "Failed to return data.end() when there is no non-internal B";

    // unnested
    data = "jkl{dsabfds;jflkai}vosankdl;asfn";
    EXPECT_EQ(find_applicable(data.begin(), data.end(), ';'), data.begin() + 27) << "failed to locate ';' at data[27]";

    data = "kljf{dab\"jkdsal;u}vasHbvdjh\"uHbiapd}neHtbgu";
    EXPECT_EQ(find_applicable(data.begin(), data.end(), 'H'), data.begin() + 38) << "Failed to locate 'H' at data[38] ignoring nested H and quote surrounded termainal }";
}

// public facing functions
TEST(Public, RemoveWhitespace)
{
    std::string data = "ThisStringContainsNoWhitespace";
    EXPECT_EQ(remove_whitespace(data), data) << "Failed to leave whitespace-less string unchanged";

    data = "Th\fis String\tCo\nntains\rWhite\vspace";
    EXPECT_EQ(remove_whitespace(data), std::string("ThisStringContainsWhitespace")) << "Failed to remove whitespace";

    data = "Th\nis \"String\" Co\rntain\fs Quo\ttes\v";
    EXPECT_EQ(remove_whitespace(data), std::string("This\"String\"ContainsQuotes")) << "Failed to remove whitespace outside quotes";

    data = "This String \"Cont\vains \tQuot\ne Su\frrou\rnded\" Whitespace";
    EXPECT_EQ(remove_whitespace(data), std::string("ThisString\"Cont\vains \tQuot\ne Su\frrou\rnded\"Whitespace")) << "Failed to ignore whitespace in quotes";
}

TEST(Public, GetData)
{
    std::string data = R"({"Key 1":"value 1","Key 2":{"Key 2.1":"foobar","Key 2.2":3.14159},"Key 3":["a","b","c","d","e"],"Key 4":-12.345e67})";

    mut_view view = {data, data.begin() + 1, data.end()-1};
    auto field = get_field(view);
    EXPECT_EQ(field.key, "\"Key 1\"") << "Failed to extract first key";
    EXPECT_EQ(std::string(field.val.begin, field.val.end), "\"value 1\"") << "Failed to extract first value";
    
    view.begin = field.val.end + 1;
    field = get_field(view);
    EXPECT_EQ(field.key, "\"Key 2\"") << "Failed to extract second key";
    EXPECT_EQ(std::string(field.val.begin, field.val.end), R"({"Key 2.1":"foobar","Key 2.2":3.14159})") << "Failed to extract second value";

    view.begin = field.val.end + 1;
    field = get_field(view);
    EXPECT_EQ(field.key, "\"Key 3\"") << "Failed to extract third key";
    EXPECT_EQ(std::string(field.val.begin, field.val.end), R"(["a","b","c","d","e"])") << "Failed to extract third value";

    view.begin = field.val.end + 1;
    field = get_field(view);
    EXPECT_EQ(field.key, "\"Key 4\"") << "Failed to extract fourth key";
    EXPECT_EQ(std::string(field.val.begin, field.val.end), "-12.345e67") << "Failed to extract fourth value";
}
