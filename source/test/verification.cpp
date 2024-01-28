#include <gtest/gtest.h>
#include <verification.hpp>
// error.hpp included by verification.hpp

// All calls pass through verify_json(), so no direct test for that function

TEST(VerifyJson, GoodObject)
{
    std::string str = R"({"key":"value"})";

    json::Result res = verify_json(str);
    EXPECT_TRUE(res) << "Failed to verify simple json object: '" << str << "' message: " << res.message();

    str = R"({"Key1":"Val1","Key2":"Val2","Key3":"Val3"})";
    res = verify_json(str);
    EXPECT_TRUE(res) << "Failed to verify multi-field json object: '" << str << "' message: " << res.message();
}

TEST(VerifyJson, BadObject)
{
    FAIL() << "Test not implemented";
}

TEST(VerifiyJson, GoodArray)
{
    FAIL() << "Test not implemented";
}

TEST(VerifyJson, BadArray)
{
    FAIL() << "Test not implemented";
}

TEST(VerifyJson, GoodString)
{
    FAIL() << "Test not implemented";
}

TEST(VerifyJson, BadString)
{
    FAIL() << "Test not implemented";
}

TEST(VerifyJson, GoodNumber)
{
    FAIL() << "Test not implemented";
}

TEST(VerifyJson, BadNumber)
{
    FAIL() << "Test not implemented";
}

TEST(VerifyJson, GoodBool)
{
    FAIL() << "Test not implemented";
}

TEST(VerifyJson, BadBool)
{
    FAIL() << "Test not implemented";
}

TEST(VerifyJson, GoodNull)
{
    FAIL() << "Test not implemented";
}

TEST(VerifyJson, BadNull)
{
    FAIL() << "Test not implemented";
}

