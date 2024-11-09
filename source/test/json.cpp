#include <gtest/gtest.h>
#include "json.hpp"
#include <vector>
#include <limits>

// --------------------------
//  json::json
// --------------------------
TEST(Json, Constructor)
{
    // default constructor is untested, since it just sets m_type to Type::Null
    json::json nullobject;
    EXPECT_EQ(nullobject.type(), json::Type::Null) << "Returned incorrect type for empty json structure (Null)";

    json::json object{ std::string("{\"Foo\": 3.14, \"Bar\": \"Baz Quux\"}") };
    EXPECT_EQ(object.type(), json::Type::Object) << "Returned incorrect type for json object";

    json::json array{ "[\"This was a triumph\", 6.022e23, true]" };
    EXPECT_EQ(array.type(), json::Type::Array) << "Returned incorrect type for json array";


}
TEST(Json, ObjectQueries)
{
    json::json data{ "{\"Field 1\": \"Value 1\", \"Field 2\": -123.45, \"Field 3\": false}" };
    json::json array{ "[1, 2, 3, 4, 5]" };

    std::vector<std::string> expected( { "Field 1", "Field 2", "Field 3" } );

    std::vector<std::string> keys;
    EXPECT_THROW(keys = array.keys(), json::wrong_type) << "Failed to throw when querying keys from array";
    EXPECT_NO_THROW(keys = data.keys()) << "Threw when querying keys from json object";

    EXPECT_EQ(keys, expected) << "Returned keys did not match expected";

    EXPECT_TRUE(data.contains(std::string("Field 1"))) << "Did not return true for known key";
    EXPECT_TRUE(data.contains("Field 2")) << "Did not return true for known key (c-str)";

    // could check std::string version, but it will probably be called by the c-str version anyway
    EXPECT_FALSE(data.contains("Bad key")) << "Returned true for non-existant key;
}

TEST(Json, ArrayQueries)
{
    json::json data{ "[1, 2, 3, 4, 5]" };
    json::json object{ "{\"Foo\": \"Bar\"}" };

    EXPECT_EQ(data.size(), 5) << "Returned incorrect size from json array";
5
    EXPECT_THROW(object.size(), json::wrong_type) << "Failed to throw when querying size from object";
}

// json::operator[] checked by accessor tests

// json::begin() and json::end() not yet added

// --------------------------
//  json::accessor
// --------------------------

TEST(Accessor, ReadQueries)
{
    // ----------------------------
    //  Object access
    // ----------------------------

    json::json data{ "{\"String Data\": \"Foobar\", \"Numeric Data\": -123, \"Boolean Data\": true, \"Object data\": {\"Key 1\": \"Foo\", \"Key 2\": \"Bar\", \"Key 3\": \"Baz\"}, \"Array Data\": [1, 2, 3, 4, 5], \"Null Data\": null}" };

    // String
    EXPECT_EQ(data["String Data"].type(), json::Type::String) << "Failed to recognize string data";
    EXPECT_EQ(data["String Data"], "Foobar") << "Failed to extract string data";

    // Numeric
    EXPECT_EQ(data["Numeric Data"].type(), json::Type::Number) << "Failed to recognize numeric data";
    EXPECT_EQ(data["Numeric Data"], -123) << "Failed to extract numeric data"; // this may not compile, since it may not be able to choose a numeric type.
    // double val = data["Numeric Data"];
    //EXPECT_EQ(val, -123) << "Failed to extract numeric data"; // alternative version if the above fails


}
