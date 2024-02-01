#include <gtest/gtest.h>
#include <verification.hpp>
// error.hpp included by verification.hpp

// All calls pass through verify_json(), so no direct test for that function

TEST(VerifyJson, GoodObject)
{
    // check function implemented
    std::string str = "{}";
    json::Result res = verify_json(str);
    ASSERT_TRUE(res) << "Json Object verification function not implemented";

    str = R"({"key":"value"})";
    res = verify_json(str);
    EXPECT_TRUE(res) << "Failed to verify simple json object: '" << str << "' message: " << res.message();

    str = R"({"Key1":"Val1","Key2":"Val2","Key3":"Val3"})";
    res = verify_json(str);
    EXPECT_TRUE(res) << "Failed to verify multi-field json object: '" << str << "' message: " << res.message();
}

TEST(VerifyJson, BadObject)
{
    // check function is implemented, negative tests will not fail for unimplemented function
    std::string str = "{}";
    json::Result res = verify_json(str);
    ASSERT_TRUE(res) << "Json Object verification function not implemented";

    // will not parse as object without initial brace
    
    // no closing brace
    str = R"({"Key 1":"Value 1","Key 2":true)";
    res = verify_json(str);
    EXPECT_FALSE(res) << "Accepted bad json object with missing initial brace: '" << str << "' message: " << res.message();

    // no initial quote on key
    str = R"({"key 1":"value 1",key 2":true,"key 3":123.456})";
    res = verify_json(str);
    EXPECT_FALSE(res) << "Accepted bad json object with missing initial quote on key: '" << str << "' message: " << res.message();

    // no final quote on key
    str = R"({"key 1":"value 1","key 2:true,"key 3":123.456})";
    res = verify_json(str);
    EXPECT_FALSE(res) << "Accepted bad json object with missing final quote on key: '" << str << "' message: " << res.message();

    // no : between key and val
    str = R"({"key 1":"value 1","key 2":true,"key 3"123.456})";
    res = verify_json(str);
    EXPECT_FALSE(res) << "Accepted bad json object with missing ':' delimiter: '" << str << "' message: " << res.message();

    // no ',' between fields
    str = R"({"key 1":"value 1""key 2":true,"key 3":123.456})";
    res = verify_json(str);
    EXPECT_FALSE(res) << "Accepted bad json object with missing ',' delimiter: '" << str << "' message: " << res.message();

    // missing key
    str = R"({"key 1":"value 1",:true,"key 3":123.456})";
    res = verify_json(str);
    EXPECT_FALSE(res) << "Accepted bad json object with missing key: '" << str << "' message: " << res.message();
    
    // missing value
    str = R"({"key 1":"value 1","key 2":,"key 3":123.456})";
    res = verify_json(str);
    EXPECT_FALSE(res) << "Accepted bad json object with missing key: '" << str << "' message: " << res.message();
}

TEST(VerifyJson, GoodArray)
{
    std::string str = "[]";
    json::Result res = verify_json(str);
    ASSERT_TRUE(res) << "Json Array verification not implemented";


}

TEST(VerifyJson, BadArray)
{
    std::string str = "[]";
    json::Result res = verify_json(str);
    ASSERT_TRUE(res) << "Json Array verification not implemented";
}

TEST(VerifyJson, GoodString)
{
    std::string str = "[\"Foobar\"]";
    json::Result res = verify_json(str);
    ASSERT_TRUE(res) << "Json String verification not implemented";
}

TEST(VerifyJson, BadString)
{
    std::string str = "[\"Foobar\"]";
    json::Result res = verify_json(str);
    ASSERT_TRUE(res) << "Json String verification not implemented";
}

TEST(VerifyJson, GoodNumber)
{
    std::string str = "[0]";
    json::Result res = verify_json(str);
    ASSERT_TRUE(res) << "Json Number verification not implemented";
}

TEST(VerifyJson, BadNumber)
{
    std::string str = "[0]";
    json::Result res = verify_json(str);
    ASSERT_TRUE(res) << "Json Number verification not implemented";
}

TEST(VerifyJson, GoodBool)
{
    std::string str = "[t]";
    json::Result res = verify_json(str);
    ASSERT_TRUE(res) << "Json Boolean verification not implemented";
}

TEST(VerifyJson, BadBool)
{
    std::string str = "[t]";
    json::Result res = verify_json(str);
    ASSERT_TRUE(res) << "Json Boolean verification not implemented";
}

TEST(VerifyJson, GoodNull)
{
    std::string str = "[n]";
    json::Result res = verify_json(str);
    ASSERT_TRUE(res) << "Json Null verification not implemented";
}

TEST(VerifyJson, BadNull)
{
    std::string str = "[n]";
    json::Result res = verify_json(str);
    ASSERT_TRUE(res) << "Json Null verification not implemented";
}

