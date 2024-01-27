#include <gtest/gtest.h>
#include "parsing.hpp"

TEST(MatchQuote, GoodString)
{
    std::string basic_quoted = "\"This string is quote surrounded\"";
    EXPECT_EQ(match_quote(basic_quoted, 0), 32) << "Failed find matching quote at [32] in: '\"This string is quote surrounded\"'";

    std::string escaped_quotes = "\"This \\\"string\\\" contains escaped \\\" chars\"";
    EXPECT_EQ(match_quote(escaped_quotes, 0), 42) << "Failed to find matching quote at [42] in: '\"This \\\"string\\\" contains escaped \\\" chars\"'";
}
