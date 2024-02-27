#include "json.hpp"
#include "parsing.hpp"
#include "error.hpp"
#include "verification.hpp"

// ------------------------
// Constructors
// ------------------------

json::Json::Json() : m_type(Type::Undefined) {  }

json::Json::Json(const std::string& line)
{
    m_contents = remove_whitespace(line);
    switch (m_contents.front())
    {
        case '{':
            m_type = Type::Object;
            break;
        case '[':
            m_type = Type::Array;
            break;
        default:
            throw json::wrong_type("String passed to Json::Json() does not specify an object or array");
    }

    json::result verified = verify_json(m_contents);

    if (!verified) throw json::parsing_error("Error constructing Json: " + verified.message());
}

json::Json::Json(std::string_view line) : json::Json::Json(std::string(line)) {  }
json::Json::Json(const char* line) : json::Json::Json(std::string(line)) {  }


// ------------------------
// Information
// ------------------------

std::vector<std::string> json::Json::keys() const
{
    throw todo("");

    std::vector<std::string> keys;

    // iterate through fields, extract keys, copy into vector

    return keys;
}

bool json::Json::contains(std::string_view key) const
{
    throw todo("");

    if (m_type != json::Type::Object) throw json::wrong_type("Cannot call contains() on non-object type");

    // iterate through fields, extract keys, if key matches return true

    return false;
}

bool json::Json::contains(const std::string& key) const
{
    return contains(std::string_view(key));
}

json::index json::Json::size() const
{
    throw todo("");
    if (m_type == json::Type::Array)
    {
        // count fields
        return 0;
    }

    if (m_type == json::Type::Object)
    {
        // count fields
        return 0;
    }

    throw json::wrong_type("Cannot call size() on non-array or object type");
}

json::Type json::Json::type() const
{
    return m_type;
}

json::Type json::Json::type(std::string_view key) const
{
    std::string_view field;
    try
    {
        field = extract_field(m_contents, key);
    }
    catch (const json::out_of_range& oor)
    {
        throw oor;
    }

    if (is_object(field)) return json::Type::Object;
    if (is_array(field)) return json::Type::Array;
    if (is_string(field)) return json::Type::String;
    if (is_number(field)) return json::Type::Number;
    if (is_bool(field)) return json::Type::Boolean;
    return json::Type::Null;
    // the data is verified, so there won't be an error option
}

json::Type json::Json::type(index index) const
{
    throw todo("");
    // get type from iterator
    return json::Type::Undefined;
}

// ------------------------
// Accessors
// ------------------------

json::Json json::Json::get_object(std::string_view key) const
{
    throw todo("json::Json::get_object() not implemented");
}

json::Json json::Json::get_object(json::index index) const
{
    throw todo("json::Json::get_object() not implemented");
}


json::Json json::Json::get_array(std::string_view key) const
{
	throw todo("json::Json::get_array() not implemented");
}

json::Json json::Json::get_array(json::index index) const
{
	throw todo("json::Json::get_array() not implemented");
}


std::string json::Json::get_string(std::string_view key) const
{
    std::string_view field;
    try
    {
        field = extract_field(m_contents, key);
    }
    catch (const json::out_of_range& oor)
    {
        throw oor;
    }

    if (field.front() != '"')
    {
        throw json::wrong_type("Type with key '" + key + "' is not a string");
    }

    return from_json_string(field);
}

std::string json::Json::get_string(json::index index) const
{
	throw todo("json::Json::get_string() not implemented");
}


double json::Json::get_double(std::string_view key) const
{
    std::string_view field;
    try
    {
        field = extract_field(m_contents, key);
    }
    catch (const json::out_of_range& oor)
    {
        throw oor;
    }

    if (!is_number(field))
    {
        throw json::wrong_type("Type with key '" + key + "' is not a number");
    }

    return std::stod(std::string(field));
}

double json::Json::get_double(json::index index) const
{
	throw todo("Json::get_double(std() not implemented");
}


int json::Json::get_int(std::string_view key) const
{
    std::string_view field;
    try
    {
        field = extract_field(m_contents, key);
    }
    catch (const json::out_of_range& oor)
    {
        throw oor;
    }

    if (!is_number(field))
    {
        throw json::wrong_type("Type with key '" + key + "' is not a number");
    }

    return std::stoi(std::string(field));
}

int json::Json::get_int(json::index index) const
{
	throw todo("Json::get_int(std() not implemented");
}


bool json::Json::get_bool(std::string_view key) const
{
    std::string_view field;
    try
    {
        field = extract_field(m_contents, key);
    }
    catch (const json::out_of_range& oor)
    {
        throw oor;
    }

    if (!::is_bool(field))
    {
        throw json::wrong_type("Type with key '" + key + "' is not a boolean");
    }

    return (field == "true" || field == "True");
}

bool json::Json::get_bool(json::index index) const
{
	throw todo("Json::get_bool(std() not implemented");
}


bool json::Json::is_null(std::string_view key) const
{
    std::string_view field;
    try
    {
        field = extract_field(m_contents, key);
    }
    catch (const json::out_of_range& oor)
    {
        throw oor;
    }

    return ::is_null(field);

}

bool json::Json::is_null(json::index index) const
{
    throw todo("Json::is_null() not implemented");
}
