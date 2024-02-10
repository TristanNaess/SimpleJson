#include "json.hpp"

json::Json::Json() : m_type(Type::Undefined) {  }

json::Json::Json(const std::string& line)
{
    m_contents = remove_whitespace(line);
    switch (m_contents)
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


std::vector<std::string> json::Json::keys() const
{
    std::vector<std::string> keys;

    // iterate through fields, extract keys, copy into vector

    return keys;
}

bool json::Json::contains(std::string_view key) const
{
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
    if (m_type == json::Json::Array)
    {
        // count fields
        return 0;
    }

    if (m_type == json::Json::Object)
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
}
