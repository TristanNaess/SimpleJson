#include <gtest/gtest.h>
#include "json.hpp"
#include <vector>
#include <limits>

// --------------------------
//  json::json
// --------------------------
TEST(Json, Constructor)
{
    json::json empty_object{ json::Type::Object };
    EXPECT_EQ(empty_object.type(), json::Type::Object) << "Returned incorrect type for empty json object";

    json::json empty_array{ json::Type::Array };
    EXPECT_EQ(empty_array.type(), json::Type::Array) << "Returned incorrect type for empty json array";
   
    EXPECT_THROW(json::json bad{ json::Type::String }, json::wrong_type) << "Failed to throw when type is not object or array";

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
    EXPECT_FALSE(data.contains("Bad key")) << "Returned true for non-existant key";
}

TEST(Json, ArrayQueries)
{
    json::json data{ "[1, 2, 3, 4, 5]" };
    json::json object{ "{\"Foo\": \"Bar\"}" };

    EXPECT_EQ(data.size(), 5) << "Returned incorrect size from json array";

    EXPECT_THROW(object.size(), json::wrong_type) << "Failed to throw when querying size from object";
}

// json::operator[] checked by accessor tests

// json::begin() and json::end() not yet added

// --------------------------
//  json::accessor
// --------------------------

TEST(Accessor, ReadQueries)
{
    // Top level Object data
    json::json data{ "{\"String Data\": \"Foobar\", \"Integer Data\": -123, \"Float Data\": 5.794e-12, \"Boolean Data\": true, \"Object Data\": {\"Key 1\": \"Foo\", \"Key 2\": \"Bar\", \"Key 3\": \"Baz\"}, \"Array Data\": [1, 2, 3, 4, 5], \"Null Data\": null}" };

    // String
    EXPECT_EQ(data["String Data"].type(), json::Type::String) << "Failed to recognize string data";
    EXPECT_EQ(data["String Data"], std::string("Foobar")) << "Failed to extract string data";

    // Numeric
    EXPECT_EQ(data["Integer Data"].type(), json::Type::Number) << "Failed to recognize integer as numeric data";
    EXPECT_EQ(data["Integer Data"], -123) << "Failed to extract integer data"; // this may not compile, since it may not be able to choose a numeric type.
    //int val = data["Integer Data"];
    //EXPECT_EQ(val, -123) << "Failed to extract numeric data"; // alternative version if the above fails

    EXPECT_EQ(data["Float Data"].type(), json::Type::Number) << "Failed to recognize floating point as numeric data";
    EXPECT_DOUBLE_EQ(data["Float Data"], 5.794e-12) << "Failed to extract floating point data";
    //double d_val = data["Float Data"];
    //EXPECT_DOUBLE_EQ(d_val, 5.794e-12) << "Failed to extract floating point data";

    // I should probably test the other numeric types

    // Boolean
    EXPECT_EQ(data["Boolean Data"].type(), json::Type::Boolean) << "Failed to recognize boolean data";
    EXPECT_EQ(data["Boolean Data"], true) << "Failed to extract boolean data";

    // Null
    EXPECT_EQ(data["Null Data"].type(), json::Type::Null) << "Failed to recognize null data";

    // Sub-object
    EXPECT_EQ(data["Object Data"].type(), json::Type::Object) << "Failed to recognize object data";
    EXPECT_EQ(data["Object Data"]["Key 2"].type(), json::Type::String) << "Failed to recognize string type in sub-object";
    EXPECT_EQ(data["Object Data"]["Key 2"], std::string("Bar")) << "Failed to extract string from sub-object";

    // Sub-array
    EXPECT_EQ(data["Array Data"].type(), json::Type::Array) << "Failed to recognize array data";
    EXPECT_EQ(data["Array Data"][3].type(), json::Type::Number) << "Failed to recognize number type in sub-array";
    EXPECT_EQ(data["Array Data"][3], 4) << "Failed to extract integer from sub-array"; // again, if this doesn't work
    //val = data["Array Data"][3];
    //EXPECT_EQ(val, 4) << "Failed to extract integer from sub-array";

    // Top level Array data
    json::json array_data{ "[\"This\", \"was\", \"a\", \"triumph\", \"I'm\", \"making\", \"a\", \"note\", \"here:\", \"Huge\", \"Success\"]" };
    EXPECT_EQ(array_data[9].type(), json::Type::String) << "Failed to recognize string in array";
    EXPECT_EQ(array_data[9], std::string("Huge")) << "Failed to extract string from array";
}

TEST(Accessor, WriteQueries)
{
    // Top level Object
    json::json data{ "{\"String\": \"Foobar\", \"Number\": 0, \"Boolean\": false, \"Object\": {\"Key 1\": \"Text\", \"Key 2\": 3}, \"Array\": [true, false, true, true]}" };

    // String
    data["String"] = "Hello there";
    EXPECT_EQ(data["String"], std::string("Hello there")) << "Failed to update string field";

    // Numeric
    data["Number"] = 42;
    EXPECT_EQ(data["Number"], 42) << "Failed to update number as integer";
    data["Number"] = 6.022e23;
    EXPECT_DOUBLE_EQ(data["Number"], 6.022e23) << "Failed to update number as floating point";

    // Boolean
    data["Boolean"] = true;
    EXPECT_EQ(data["Boolean"], true) << "Failed to update boolean field";

    // no Null, since it requires a type change and thus has to be changed through delete then insert

    // Sub-object
    data["Object"]["Key 2"] = -9.81;
    EXPECT_DOUBLE_EQ(data["Object"]["Key 2"], -9.81) << "Failed to update number field in sub-object";

    // Sub-array
    data["Array"][2] = false;
    EXPECT_EQ(data["Array"][2], false) << "Failed to update boolean field in sub-array";

    // Top level Array
    json::json array_data{ "[\"This\", \"was\", \"a\", \"triumph\", \"I'm\", \"making\", \"a\", \"note\", \"here:\", \"Huge\", \"Success\"]" };

    array_data[3] = "triumph.";
    EXPECT_EQ(array_data[3], std::string("triumph.")) << "Failed to update string field in array";

    EXPECT_THROW(data["Object"] = "foobar", json::wrong_type) << "Failed to throw correct error when trying to change type while updating";
}

/* -- TO BE ADDED ONCE READ AND WRITE ARE FINISHED --

TEST(Accessor, InsertQueries)
{

}

TEST(Accessor, DeleteQueries)
{

}
*/
