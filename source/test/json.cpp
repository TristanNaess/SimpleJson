#include <gtest/gtest.h>
#include "json.hpp"
#include "error.hpp"

TEST(Json, ReadObject)
{
    // basic data
    json::Json data(R"({"String": "foobar", "Number": 3.14159, "Boolean": true, "Null": null})");

    EXPECT_EQ(data.type(), json::Type::Object) << "Wrong type returned for data. Expected json::Type::Object";

    EXPECT_EQ(data.type("String"), json::Type::String) << "Wrong type returned for data[\"String\"]";
    EXPECT_EQ(data.get_string("String"), "foobar") << "Failed to read string field from basic data object";
    EXPECT_EQ(data.type("Number"), json::Type::Number) << "Wrong type returned for data[\"Number\"]";
    EXPECT_EQ(data.get_int("Number"), 3) << "Failed to read number field as int from basic data object";
    EXPECT_FLOAT_EQ(data.get_double("Number"), 3.14159) << "Failed to read number field as float from basic data object";
    EXPECT_EQ(data.type("Boolean"), json::Type::Boolean) << "Wrong type returned for data[\"Boolean\"]";
    EXPECT_EQ(data.get_bool("Boolean"), true) << "Failed to read boolean field from basic object";
    EXPECT_EQ(data.type("Null"), json::Type::Null) << "Wrong type returned for data[\"Null\"]";
    EXPECT_EQ(data.is_null("Null"), true) << "Failed to read null field from basic object";
    EXPECT_EQ(data.is_null("Number"), false) << "Obtained null from non-null field in basic object";
}
