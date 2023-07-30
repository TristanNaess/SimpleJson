#include <gtest/gtest.h>

#include "error.hpp"

TEST(ErrorTesting, ErrorIsJsonParsingError)
{
    try
    {
        throw json::parsing_error("This is a parsing error");
    }
    catch (const json::parsing_error& pe)
    {
        SUCCEED();
        return;
    }
    catch(...)
    {
        FAIL() << "Incorrect type thrown";
        return;
    }
    FAIL() << "Did not throw";

}

TEST(ErrorTesting, MessageMatchesExpected)
{
    try
    {
        throw json::parsing_error("This is a parsing error");
    }
    catch (const json::parsing_error& pe)
    {
        EXPECT_STREQ(pe.what(), "This is a parsing error") << "Error message did not match expected";
    }
}
