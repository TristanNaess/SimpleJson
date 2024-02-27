#include <gtest/gtest.h>
#include <verification.hpp>
// error.hpp included by verification.hpp

// All calls pass through verify_json(), so no direct test for that function

TEST(VerifyJson, GoodObject)
{
    // check function implemented / check empty case
    std::string str = "{}";
    json::result res = verify_json(str);
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
    json::result res = verify_json(str);
    ASSERT_TRUE(res) << "Json Object verification function not implemented";

    // will not parse as object without initial brace
    
    // no closing brace
    str = R"({"Object":{"Key 1":"Value 1","Key 2":true,"Null":null})";
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
    // check function is implemented / check empty case
    std::string str = "[]";
    json::result res = verify_json(str);
    ASSERT_TRUE(res) << "Json Array verification not implemented";

    // simple data
    str = "[1,2,3,4,5]";
    res = verify_json(str);
    EXPECT_TRUE(res) << "Failed to verify array of simple data: '" << str << "' message: " << res.message();

    // complex data
    str = R"([{"Key 1":123,"Key 2":"foobar"},["a","b","c","d"],[1,2,3,4,5]])";
    res = verify_json(str);
    EXPECT_TRUE(res) << "Failed to verify array of compound data types: '" << str << "' message: " << res.message();
}

TEST(VerifyJson, BadArray)
{
    std::string str = "[]";
    json::result res = verify_json(str);
    ASSERT_TRUE(res) << "Json Array verification not implemented";

    // will not parse as array if missing initial bracket

    // missing final bracket
    str = "[[1,2,3,4,5,true,false]";
    res = verify_json(str);
    EXPECT_FALSE(res) << "Accepted bad json array with missing final ']': '" << str << "' message: " << res.message();

    // missing ',' between fields
    str = "[1,2,3[true,false,null],{\"key\":\"val\"}]";
    res = verify_json(str);
    EXPECT_FALSE(res) << "Accepted bad json array with missing ',': '" << str << "' message: " << res.message();
}

TEST(VerifyJson, GoodString)
{
    std::string str = "\"Foobar\"";
    json::result res = verify_string(str);
    ASSERT_TRUE(res) << "Json String verification not implemented";

    str = R"("foobar")";
    res = verify_string(str);
    EXPECT_TRUE(res) << "Failed to verify string with simple data: '" << str << "' message: " << res.message();

    str = R"("iosa\"fhdjsafl;s\\fdsa\/\bfdsaf\fhfdsgf\nfdsa\r\tfdafda\u00D0fdsafdshfd")";
    res = verify_string(str);
    EXPECT_TRUE(res) << "Failed to verify string with escaped characters: '" << str << "' message: " << res.message();
}

TEST(VerifyJson, BadString)
{
    std::string str = "\"Foobar\"";
    json::result res = verify_string(str);
    ASSERT_TRUE(res) << "Json String verification not implemented";

    // will not parse as string if missing initial quote

    // missing final quote
    str = R"("foobar)";
    res = verify_json(str);
    EXPECT_FALSE(res) << "Accepted bad json string with missing final '\"': '" << str << "' message: " << res.message();

    // unescaped quote
    str = R"("fsffsasa"fdsafdsafd")";
    res = verify_json(str);
    EXPECT_FALSE(res) << "Accepted bad json string with unescaped '\"': '" << str << "' message: " << res.message();

    // bad escape character
    str = R"("fsffsasa\fdsafdsafd")";
    res = verify_json(str);
    EXPECT_FALSE(res) << "Accepted bad json string with non-escape character after '\\': '" << str << "' message: " << res.message();

    // bad UTF-16
    str = R"("fdjksl\u00G3fdsa")";
    res = verify_json(str);
    EXPECT_FALSE(res) << "Accepted bad json string with non-hex character in UTF-16 sequence: '" << str << "' message: " << res.message();
}

TEST(VerifyJson, GoodNumber)
{
    std::string str = "0";
    json::result res = verify_number(str);
    ASSERT_TRUE(res) << "Json Number verification not implemented";

    // whole numbers
    str = "12345";
    res = verify_number(str);
    EXPECT_TRUE(res) << "Failed to verify whole positive number: '" << str << "' message: " << res.message();
    str = "+12345";
    res = verify_number(str);
    EXPECT_TRUE(res) << "Failed to verify explicit positive number: '" << str << "' message: " << res.message();
    str = "-42";
    res = verify_number(str);
    EXPECT_TRUE(res) << "Failed to verify whole negative number: '" << str << "' message: " << res.message();

    // decimal number
    str = "0.123"; // must have preceding 0
    res = verify_number(str);
    EXPECT_TRUE(res) << "Failed to verify decimal only number: '" << str << "' message: " << res.message();
    str = "123.45";
    res = verify_number(str);
    EXPECT_TRUE(res) << "Failed to verify whole and decimal number: '" << str << "' message: " << res.message();

    // exponent
    str = "123e45";
    res = verify_number(str);
    EXPECT_TRUE(res) << "Failed to verify whole and exponent number: '" << str << "' message: " << res.message();
    str = "123e+45";
    res = verify_number(str);
    EXPECT_TRUE(res) << "Failed to verify whole and explicit positive exponent number: '" << str << "' message: " << res.message();
    str = "123e-45";
    res = verify_number(str);
    EXPECT_TRUE(res) << "Failed to verify whole and negative exponent number: '" << str << "' message: " << res.message();
    str = "123.45E67";
    res = verify_number(str);
    EXPECT_TRUE(res) << "Failed to verify whole, decimal and exponent number: '" << str << "' message: " << res.message();
    
}

TEST(VerifyJson, BadNumber)
{
    std::string str = "0";
    json::result res = verify_number(str);
    ASSERT_TRUE(res) << "Json Number verification not implemented";

    // will not parse as number with bad initial character

    // missing 0 before '.' after '-'
    str = "-.42";
    res = verify_number(str);
    EXPECT_FALSE(res) << "Accepted number with no digit between '-' and '.': '" << str << "' message: " << res.message();
    // bad character in whole part/wrong decimal character
    str = "123*45";
    res = verify_number(str);
    EXPECT_FALSE(res) << "Accepted number with bad character: '" << str << "' message: " << res.message();
    
    // non-digit in decimal
    str = "123.4f6";
    res = verify_number(str);
    EXPECT_FALSE(res) << "Accepted number with bad character in decimal: '" << str << "' message: " << res.message();

    // non-digit in exponent
    str = "123.4eg6";
    res = verify_number(str);
    EXPECT_FALSE(res) << "Accepted number with bad character in exponent: '" << str << "' message: " << res.message();
}

TEST(VerifyJson, GoodBool)
{
    std::string str = "true";
    json::result res = verify_bool(str);
    ASSERT_TRUE(res) << "Json Boolean verification not implemented";

    // exhaustive test of all options
    str = "true";
    res = verify_bool(str);
    EXPECT_TRUE(res) << "Failed to verify boolean: '" << str << "' message: " << res.message();

    str = "false";
    res = verify_bool(str);
    EXPECT_TRUE(res) << "Failed to verify boolean: '" << str << "' message: " << res.message();
}

TEST(VerifyJson, BadBool)
{
    std::string str = "true";
    json::result res = verify_bool(str);
    ASSERT_TRUE(res) << "Json Boolean verification not implemented";

    str = "teue";
    res = verify_bool(str);
    EXPECT_FALSE(res) << "Accepted bool true with misspelling: '" << str << "' message: " << res.message();

    str = "flase";
    res = verify_bool(str);
    EXPECT_FALSE(res) << "Accepted bool false with misspelling: '" << str << "' message: " << res.message();
}

TEST(VerifyJson, GoodNull)
{
    std::string str = "null";
    json::result res = verify_null(str);
    ASSERT_TRUE(res) << "Json Null verification not implemented";

    // only one possibility
    str = "null";
    res = verify_null(str);
    EXPECT_TRUE(res) << "Failed to verify boolean: '" << str << "' message: " << res.message();
}

TEST(VerifyJson, BadNull)
{
    std::string str = "null";
    json::result res = verify_null(str);
    ASSERT_TRUE(res) << "Json Null verification not implemented";

    str = "nul";
    res = verify_null(str);
    EXPECT_FALSE(res) << "Accepted misspelled 'null': '" << str << "' message: " << res.message();
}

// No tests for is_*() functions, as they are the same as the beginnings of the verify_*() functions
